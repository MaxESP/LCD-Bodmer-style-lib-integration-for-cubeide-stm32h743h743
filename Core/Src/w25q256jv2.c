

#include "w25q256jv2.h"
#include "quadspi.h"
#include "string.h"
#include "core_cm7.h"
#include "stdio.h"
#include "stdbool.h"



extern QSPI_HandleTypeDef hqspi;

//volatile uint8_t qspi_mdma_complete = 0;
//volatile uint8_t qspi_mdma_error = 0;
static bool qspi_memory_mapped = false;
static uint32_t qspi_ccr_backup = 0;

/* Private helper functions */
static uint8_t WriteEnable(void);
static uint8_t WriteDisable(void);
static uint8_t WaitForReady(uint32_t timeout);
static void CleanCache(void *addr, uint32_t size);
static void InvalidateCache(void *addr, uint32_t size);

/* Cache helper - STM32H7 requires 32-byte alignment */
static void CleanCache(void *addr, uint32_t size)
{
    if (size == 0 || addr == NULL) return;
    const uint32_t CACHE_LINE = 32U;
    uint32_t start = (uint32_t)addr & ~(CACHE_LINE - 1U);
    uint32_t end = (((uint32_t)addr + size + CACHE_LINE - 1U) & ~(CACHE_LINE - 1U));
    SCB_CleanDCache_by_Addr((uint32_t *)start, (int32_t)(end - start));
}

static void InvalidateCache(void *addr, uint32_t size)
{
    if (size == 0 || addr == NULL) return;
    const uint32_t CACHE_LINE = 32U;
    uint32_t start = (uint32_t)addr & ~(CACHE_LINE - 1U);
    uint32_t end = (((uint32_t)addr + size + CACHE_LINE - 1U) & ~(CACHE_LINE - 1U));
    SCB_InvalidateDCache_by_Addr((uint32_t *)start, (int32_t)(end - start));
}

/* ===================== PUBLIC FUNCTIONS ===================== */

uint8_t W25Q256JV_Init(void)
{
    printf("[W25Q256JV] Initializing (QE=1 permanent)\r\n");

    /* Reset flash */
    QSPI_CommandTypeDef cmd = {0};
    cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    cmd.Instruction = W25Q256JV_CMD_RESET_ENABLE;
    cmd.AddressMode = QSPI_ADDRESS_NONE;
    cmd.DataMode = QSPI_DATA_NONE;
    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    HAL_QSPI_Command(&hqspi, &cmd, 100);

    cmd.Instruction = W25Q256JV_CMD_RESET_DEVICE;
    HAL_QSPI_Command(&hqspi, &cmd, 100);

    HAL_Delay(10);

    /* Verify QE=1 (should always be true on your chip) */
    uint8_t status2 = W25Q256JV_ReadStatus2();
    printf("[W25Q256JV] Status2: 0x%02X (QE=%s)\r\n",
           status2, (status2 & W25Q256JV_STATUS_QE) ? "PERMANENT" : "ERROR");

    /* Read ID to verify communication */
    uint32_t id = W25Q256JV_ReadID();
    printf("[W25Q256JV] JEDEC ID: 0x%06lX\r\n", id);

    if ((id >> 16) != W25Q256JV_MANUFACTURER_ID) {
        printf("[W25Q256JV] ERROR: Wrong manufacturer (expected Winbond)\r\n");
        return HAL_ERROR;
    }

    printf("[W25Q256JV] Ready - always in Quad mode\r\n");
    return HAL_OK;
}

/**
  * @brief  Enable memory-mapped mode for direct read access
  * @param  base_addr: Memory-mapped base address (usually 0x90000000)
  * @param  size: Flash size in bytes (0x2000000 for 32MB)
  * @retval HAL_OK or HAL_ERROR
  */
/* ===================== MEMORY-MAPPED MODE ===================== */

/**
 * @brief  Enter memory-mapped mode - READ ONLY!
 * @param  base_addr: Memory-mapped base address (0x90000000)
 * @param  size: Flash size in bytes (0x2000000 for 32MB)
 * @retval HAL_OK or HAL_ERROR
 */
uint8_t W25Q256JV_EnterMemoryMappedMode(uint32_t base_addr, uint32_t size)
{
    if (qspi_memory_mapped) return HAL_OK;

    QSPI_CommandTypeDef sCommand = {0};
    QSPI_MemoryMappedTypeDef sMemMappedCfg = {0};

    /* Abort any ongoing operation */
    HAL_QSPI_Abort(&hqspi);

    /*-----------------------------------------------------------------------
     * Configure Quad I/O Fast Read (0xEB) - FASTEST for memory-mapped
     *-----------------------------------------------------------------------*/
    sCommand.InstructionMode    = QSPI_INSTRUCTION_1_LINE;
    sCommand.Instruction       = W25Q256JV_CMD_QUAD_IO_FAST_READ;  /* 0xEB */
    sCommand.AddressMode       = QSPI_ADDRESS_4_LINES;            /* Quad address */
    sCommand.AddressSize      = QSPI_ADDRESS_24_BITS;            /* 24-bit address */
    sCommand.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode         = QSPI_DATA_4_LINES;               /* Quad data */
    sCommand.DummyCycles      = 6;                               /* Required for 0xEB */
    sCommand.DdrMode          = QSPI_DDR_MODE_DISABLE;
    sCommand.SIOOMode         = QSPI_SIOO_INST_EVERY_CMD;

    /* Backup CCR for later restore */
    qspi_ccr_backup = QUADSPI->CCR;

    /* Memory-mapped configuration */
    sMemMappedCfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
    sMemMappedCfg.TimeOutPeriod = 0;

    if (HAL_QSPI_MemoryMapped(&hqspi, &sCommand, &sMemMappedCfg) == HAL_OK) {
        qspi_memory_mapped = true;
        printf("[W25Q256JV] Memory-mapped mode enabled at 0x%08lX (READ ONLY)\r\n", base_addr);
        return HAL_OK;
    }

    return HAL_ERROR;
}

/**
 * @brief  Exit memory-mapped mode to enable writes
 * @retval HAL_OK or HAL_ERROR
 */
uint8_t W25Q256JV_ExitMemoryMappedMode(void)
{
    if (!qspi_memory_mapped) return HAL_OK;

    printf("[W25Q256JV] Exiting memory-mapped mode...\r\n");

    /* Disable interrupts during mode switch */
    __disable_irq();

    /* Disable caches temporarily */
    #if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    SCB_DisableDCache();
    #endif
    #if defined(__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
    SCB_DisableICache();
    #endif

    /* Abort memory-mapped mode */
    HAL_QSPI_DeInit(&hqspi);
    HAL_Delay(1);
    MX_QUADSPI_Init();  /* Re-initialize with your CubeMX config */

    /* Restore caches */
    #if defined(__ICACHE_PRESENT) && (__ICACHE_PRESENT == 1U)
    SCB_EnableICache();
    #endif
    #if defined(__DCACHE_PRESENT) && (__DCACHE_PRESENT == 1U)
    SCB_EnableDCache();
    #endif

    __enable_irq();

    qspi_memory_mapped = false;
    printf("[W25Q256JV] Memory-mapped mode disabled - writes enabled\r\n");

    return HAL_OK;
}

uint32_t W25Q256JV_ReadID(void)
{
    QSPI_CommandTypeDef cmd = {0};
    uint8_t id[3];

    cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    cmd.Instruction = W25Q256JV_CMD_READ_JEDEC_ID;
    cmd.AddressMode = QSPI_ADDRESS_NONE;
    cmd.DataMode = QSPI_DATA_1_LINE;
    cmd.NbData = 3;
    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &cmd, W25Q256JV_CMD_TIMEOUT) != HAL_OK) return 0;
    if (HAL_QSPI_Receive(&hqspi, id, W25Q256JV_CMD_TIMEOUT) != HAL_OK) return 0;

    return ((uint32_t)id[0] << 16) | ((uint32_t)id[1] << 8) | id[2];
}

uint8_t W25Q256JV_ReadStatus1(void)
{
    QSPI_CommandTypeDef cmd = {0};
    uint8_t status;

    cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    cmd.Instruction = W25Q256JV_CMD_READ_STATUS1;
    cmd.AddressMode = QSPI_ADDRESS_NONE;
    cmd.DataMode = QSPI_DATA_1_LINE;
    cmd.NbData = 1;
    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    HAL_QSPI_Command(&hqspi, &cmd, W25Q256JV_CMD_TIMEOUT);
    HAL_QSPI_Receive(&hqspi, &status, W25Q256JV_CMD_TIMEOUT);

    return status;
}

uint8_t W25Q256JV_ReadStatus2(void)
{
    QSPI_CommandTypeDef cmd = {0};
    uint8_t status;

    cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    cmd.Instruction = W25Q256JV_CMD_READ_STATUS2;
    cmd.AddressMode = QSPI_ADDRESS_NONE;
    cmd.DataMode = QSPI_DATA_1_LINE;
    cmd.NbData = 1;
    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    HAL_QSPI_Command(&hqspi, &cmd, W25Q256JV_CMD_TIMEOUT);
    HAL_QSPI_Receive(&hqspi, &status, W25Q256JV_CMD_TIMEOUT);

    return status;
}

uint8_t W25Q256JV_WaitForReady(void)
{
    uint32_t timeout = W25Q256JV_CMD_TIMEOUT;

    while (timeout--) {
        if (W25Q256JV_IsReady()) {
            return HAL_OK;
        }
        HAL_Delay(1);
    }

    printf("[W25Q256JV] Timeout waiting for ready\r\n");
    return HAL_ERROR;
}

uint8_t W25Q256JV_EraseChip(void)
{
    if (WriteEnable() != HAL_OK) return HAL_ERROR;

    QSPI_CommandTypeDef cmd = {0};
    cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    cmd.Instruction = W25Q256JV_CMD_CHIP_ERASE;
    cmd.AddressMode = QSPI_ADDRESS_NONE;
    cmd.DataMode = QSPI_DATA_NONE;
    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &cmd, W25Q256JV_CMD_TIMEOUT) != HAL_OK) return HAL_ERROR;

    printf("[W25Q256JV] Chip erase started...\r\n");
    return WaitForReady(W25Q256JV_ERASE_TIMEOUT);
}

uint8_t W25Q256JV_EraseBlock(uint32_t address)
{
    if (WriteEnable() != HAL_OK) return HAL_ERROR;

    QSPI_CommandTypeDef cmd = {0};
    cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    cmd.Instruction = W25Q256JV_CMD_BLOCK_ERASE;
    cmd.AddressMode = QSPI_ADDRESS_1_LINE;
    cmd.AddressSize = QSPI_ADDRESS_24_BITS;
    cmd.Address = address;
    cmd.DataMode = QSPI_DATA_NONE;
    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &cmd, W25Q256JV_CMD_TIMEOUT) != HAL_OK) return HAL_ERROR;

    return WaitForReady(W25Q256JV_ERASE_TIMEOUT);
}

uint8_t W25Q256JV_EraseSector(uint32_t address)
{
    if (WriteEnable() != HAL_OK) return HAL_ERROR;

    QSPI_CommandTypeDef cmd = {0};
    cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    cmd.Instruction = W25Q256JV_CMD_SECTOR_ERASE;
    cmd.AddressMode = QSPI_ADDRESS_1_LINE;
    cmd.AddressSize = QSPI_ADDRESS_24_BITS;
    cmd.Address = address;
    cmd.DataMode = QSPI_DATA_NONE;
    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &cmd, W25Q256JV_CMD_TIMEOUT) != HAL_OK) return HAL_ERROR;

    return WaitForReady(W25Q256JV_ERASE_TIMEOUT);
}

uint8_t W25Q256JV_Write(uint8_t *data, uint32_t address, uint32_t size)
{
    if (data == NULL || size == 0) return HAL_ERROR;

    QSPI_CommandTypeDef cmd = {0};
    uint32_t end_addr = address + size;
    uint32_t current_addr = address;
    uint8_t *buffer = data;

    /* Configure command for Quad Page Program (0x32) */
    cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    cmd.Instruction = W25Q256JV_CMD_QUAD_PAGE_PROGRAM;
    cmd.AddressMode = QSPI_ADDRESS_1_LINE;      /* Address on 1 line */
    cmd.AddressSize = QSPI_ADDRESS_24_BITS;
    cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    cmd.DataMode = QSPI_DATA_4_LINES;           /* Data on 4 lines (Quad mode) */
    cmd.DummyCycles = 0;
    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    while (current_addr < end_addr) {
        uint32_t page_offset = current_addr % W25Q256JV_PAGE_SIZE;
        uint32_t chunk_size = W25Q256JV_PAGE_SIZE - page_offset;

        if (chunk_size > (end_addr - current_addr)) {
            chunk_size = end_addr - current_addr;
        }

        cmd.Address = current_addr;
        cmd.NbData = chunk_size;

        /* Enable write */
        if (WriteEnable() != HAL_OK) {
            printf("[W25Q256JV] Write enable failed\r\n");
            return HAL_ERROR;
        }

        /* Clean cache for DMA */
        CleanCache(buffer, chunk_size);

        /* Send command */
        if (HAL_QSPI_Command(&hqspi, &cmd, W25Q256JV_CMD_TIMEOUT) != HAL_OK) {
            printf("[W25Q256JV] Write command failed\r\n");
            return HAL_ERROR;
        }

        /* Send data */
        if (HAL_QSPI_Transmit(&hqspi, buffer, W25Q256JV_CMD_TIMEOUT) != HAL_OK) {
            printf("[W25Q256JV] Write transmit failed\r\n");
            return HAL_ERROR;
        }

        /* Wait for completion */
        if (WaitForReady(100) != HAL_OK) {
            printf("[W25Q256JV] Write timeout\r\n");
            return HAL_ERROR;
        }

        current_addr += chunk_size;
        buffer += chunk_size;
    }

    return HAL_OK;
}

uint8_t W25Q256JV_Read(uint8_t *data, uint32_t address, uint32_t size)
{
    if (data == NULL || size == 0) return HAL_ERROR;

    /* Configure Quad I/O Fast Read (0xEB) - 4 lines for address and data */
    QSPI_CommandTypeDef cmd = {0};
    cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    cmd.Instruction = W25Q256JV_CMD_QUAD_IO_FAST_READ;  /* 0xEB - Quad I/O Fast Read */
    cmd.AddressMode = QSPI_ADDRESS_4_LINES;             /* Address on 4 lines */
    cmd.AddressSize = QSPI_ADDRESS_24_BITS;
    cmd.Address = address;
    cmd.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
    cmd.DataMode = QSPI_DATA_4_LINES;                   /* Data on 4 lines */
    cmd.DummyCycles = 6;                                /* Standard for 0xEB */
    cmd.NbData = size;
    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    /* Invalidate cache for DMA read */
    InvalidateCache(data, size);

    /* Send read command */
    if (HAL_QSPI_Command(&hqspi, &cmd, W25Q256JV_CMD_TIMEOUT) != HAL_OK) {
        printf("[W25Q256JV] Read command failed\r\n");
        return HAL_ERROR;
    }

    /* Receive data */
    if (HAL_QSPI_Receive(&hqspi, data, W25Q256JV_CMD_TIMEOUT) != HAL_OK) {
        printf("[W25Q256JV] Read receive failed\r\n");
        return HAL_ERROR;
    }

    return HAL_OK;
}

/**
  * @brief  Test memory-mapped mode by comparing with indirect read
  * @retval HAL_OK or HAL_ERROR
  */








/* ===================== PRIVATE FUNCTIONS ===================== */






static uint8_t WriteEnable(void)
{
    QSPI_CommandTypeDef cmd = {0};

    cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    cmd.Instruction = W25Q256JV_CMD_WRITE_ENABLE;
    cmd.AddressMode = QSPI_ADDRESS_NONE;
    cmd.DataMode = QSPI_DATA_NONE;
    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    return HAL_QSPI_Command(&hqspi, &cmd, W25Q256JV_CMD_TIMEOUT);
}

static uint8_t WriteDisable(void)
{
    QSPI_CommandTypeDef cmd = {0};

    cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    cmd.Instruction = W25Q256JV_CMD_WRITE_DISABLE;
    cmd.AddressMode = QSPI_ADDRESS_NONE;
    cmd.DataMode = QSPI_DATA_NONE;
    cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
    cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

    return HAL_QSPI_Command(&hqspi, &cmd, W25Q256JV_CMD_TIMEOUT);
}

static uint8_t WaitForReady(uint32_t timeout)
{
    uint32_t start = HAL_GetTick();

    while (!W25Q256JV_IsReady()) {
        if (HAL_GetTick() - start > timeout) {
            return HAL_ERROR;
        }
    }

    return HAL_OK;
}



/* ===================== SAFE OPERATIONS ===================== */

/**
 * @brief  Safe write - automatically exits memory-mapped mode
 */
uint8_t W25Q256JV_SafeWrite(uint8_t *data, uint32_t address, uint32_t size)
{
    uint8_t result;

    /* Exit memory-mapped mode if active */
    if (qspi_memory_mapped) {
        W25Q256JV_ExitMemoryMappedMode();
    }

    /* Perform write */
    result = W25Q256JV_Write(data, address, size);

    /* Invalidate cache for the written region */
    SCB_InvalidateDCache_by_Addr((uint32_t *)(0x90000000 + address), size);

    return result;
}

/**
 * @brief  Safe read - uses memory-mapped mode if available
 */
uint8_t W25Q256JV_SafeRead(uint8_t *data, uint32_t address, uint32_t size)
{
    /* If in memory-mapped mode, use direct pointer access */
    if (qspi_memory_mapped) {
        uint8_t *src = (uint8_t *)(0x90000000 + address);
        memcpy(data, src, size);
        return HAL_OK;
    }

    /* Otherwise use indirect mode */
    return W25Q256JV_Read(data, address, size);
}

/**
 * @brief  Test memory-mapped mode by comparing with indirect read
 */
uint8_t W25Q256JV_TestMemoryMapped(void)
{
    uint8_t indirect_buf[16];
    uint8_t mmapped_buf[16];

    if (!qspi_memory_mapped) {
        printf("[W25Q256JV] Not in memory-mapped mode\r\n");
        return HAL_ERROR;
    }

    /* Read via indirect mode */
    uint8_t was_mapped = qspi_memory_mapped;
    if (was_mapped) W25Q256JV_ExitMemoryMappedMode();

    if (W25Q256JV_Read(indirect_buf, 0, 16) != HAL_OK) {
        printf("[W25Q256JV] Indirect read failed\r\n");
        return HAL_ERROR;
    }

    /* Re-enter memory-mapped mode */
    if (was_mapped) W25Q256JV_EnterMemoryMappedMode(0x90000000, W25Q256JV_FLASH_SIZE);

    /* Read via memory-mapped mode */
    uint8_t *flash_ptr = (uint8_t *)0x90000000;
    for (int i = 0; i < 16; i++) {
        mmapped_buf[i] = flash_ptr[i];
    }

    /* Compare results */
    printf("[W25Q256JV] Memory-mapped test:\r\n");
    printf("  Indirect: ");
    for (int i = 0; i < 16; i++) printf("0x%02X ", indirect_buf[i]);
    printf("\r\n");
    printf("  M-Mapped: ");
    for (int i = 0; i < 16; i++) printf("0x%02X ", mmapped_buf[i]);
    printf("\r\n");

    if (memcmp(indirect_buf, mmapped_buf, 16) == 0) {
        printf("[W25Q256JV] Memory-mapped mode OK\r\n");
        return HAL_OK;
    } else {
        printf("[W25Q256JV] Memory-mapped mode FAILED\r\n");
        return HAL_ERROR;
    }
}




