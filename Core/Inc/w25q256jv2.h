#ifndef W25Q256JV1_H
#define W25Q256JV1_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/* Memory Size Definitions */
#define W25Q256JV_FLASH_SIZE          0x2000000UL  /* 256 Mbits / 32 MBytes */
#define W25Q256JV_BLOCK_SIZE          0x10000UL    /* 64 KBytes per block */
#define W25Q256JV_SECTOR_SIZE         0x1000UL     /* 4 KBytes per sector */
#define W25Q256JV_PAGE_SIZE           0x100UL      /* 256 bytes per page */

/* Manufacturer and Device ID */
#define W25Q256JV_MANUFACTURER_ID     0xEF
#define W25Q256JV_DEVICE_ID           0x4019

/* Commands (Quad mode only) */
#define W25Q256JV_CMD_WRITE_ENABLE    0x06
#define W25Q256JV_CMD_WRITE_DISABLE   0x04
#define W25Q256JV_CMD_READ_STATUS1    0x05
#define W25Q256JV_CMD_READ_STATUS2    0x35
#define W25Q256JV_CMD_QUAD_PAGE_PROGRAM 0x32
#define W25Q256JV_CMD_QUAD_IO_FAST_READ 0xEB
#define W25Q256JV_CMD_SECTOR_ERASE    0x20
#define W25Q256JV_CMD_BLOCK_ERASE     0xD8
#define W25Q256JV_CMD_CHIP_ERASE      0xC7
#define W25Q256JV_CMD_RESET_ENABLE    0x66
#define W25Q256JV_CMD_RESET_DEVICE    0x99
#define W25Q256JV_CMD_READ_JEDEC_ID   0x9F

/* Status Register Bits */
#define W25Q256JV_STATUS_BUSY         (1 << 0)
#define W25Q256JV_STATUS_WEL          (1 << 1)
#define W25Q256JV_STATUS_QE           (1 << 1)  /* Always 1 on your chip! */

/* Timeouts */
#define W25Q256JV_CMD_TIMEOUT         1000
#define W25Q256JV_ERASE_TIMEOUT       5000

/* Public Function Prototypes */
uint8_t W25Q256JV_Init(void);
uint32_t W25Q256JV_ReadID(void);
uint8_t W25Q256JV_ReadStatus1(void);
uint8_t W25Q256JV_ReadStatus2(void);
uint8_t W25Q256JV_EraseChip(void);
uint8_t W25Q256JV_EraseBlock(uint32_t address);
uint8_t W25Q256JV_EraseSector(uint32_t address);
uint8_t W25Q256JV_Write(uint8_t *data, uint32_t address, uint32_t size);
uint8_t W25Q256JV_Read(uint8_t *data, uint32_t address, uint32_t size);
uint8_t W25Q256JV_WaitForReady(void);
uint8_t W25Q256JV_EnableMemoryMappedMode(uint32_t base_addr, uint32_t size);
uint8_t W25Q256JV_EnterMemoryMappedMode(uint32_t base_addr, uint32_t size);
uint8_t W25Q256JV_ExitMemoryMappedMode(void);
uint8_t W25Q256JV_TestMemoryMapped(void);

/* Safe Operations */
uint8_t W25Q256JV_SafeWrite(uint8_t *data, uint32_t address, uint32_t size);
uint8_t W25Q256JV_SafeRead(uint8_t *data, uint32_t address, uint32_t size);

/* Helper functions */
static inline uint8_t W25Q256JV_IsReady(void) {
    return (W25Q256JV_ReadStatus1() & W25Q256JV_STATUS_BUSY) == 0;
}

static inline uint8_t W25Q256JV_IsQuadEnabled(void) {
    return (W25Q256JV_ReadStatus2() & W25Q256JV_STATUS_QE) != 0;
}

#ifdef __cplusplus
}
#endif

#endif /* W25Q256JV_H */






































/*



*
  ******************************************************************************
  * @file    w25q256jv.h
  * @brief   W25Q256JV Flash Memory Driver Header
  ******************************************************************************
  * @attention
  *
  * Created specifically for W25Q256JV 256Mb Quad-SPI Flash Memory
  *
  ******************************************************************************


#ifndef W25Q256JV2_H
#define W25Q256JV2_H

#ifdef __cplusplus
extern "C" {
#endif

 Includes ------------------------------------------------------------------
#include "main.h"

 Memory Size Definitions ---------------------------------------------------
#define W25Q256JV_FLASH_SIZE          0x2000000UL   256 Mbits / 32 MBytes
#define W25Q256JV_BLOCK_SIZE          0x10000UL     64 KBytes per block
#define W25Q256JV_SECTOR_SIZE         0x1000UL      4 KBytes per sector
#define W25Q256JV_PAGE_SIZE           0x100UL       256 bytes per page
#define W25Q256JV_NUM_BLOCKS          512           512 blocks of 64KB
#define W25Q256JV_NUM_SECTORS         8192          8192 sectors of 4KB
#define W25Q256JV_NUM_PAGES           131072        131072 pages of 256B

 Manufacturer and Device ID ------------------------------------------------
#define W25Q256JV_MANUFACTURER_ID     0xEF
#define W25Q256JV_DEVICE_ID           0x4019        W25Q256JV
#define W25Q256JV_JEDEC_ID            ((W25Q256JV_MANUFACTURER_ID << 16) | W25Q256JV_DEVICE_ID)

 Command Set --------------------------------------------------------------
#define W25Q256JV_CMD_WRITE_ENABLE                0x06
#define W25Q256JV_CMD_VOLATILE_SR_WRITE_ENABLE    0x50
#define W25Q256JV_CMD_WRITE_DISABLE               0x04
#define W25Q256JV_CMD_READ_STATUS_REG1            0x05
#define W25Q256JV_CMD_READ_STATUS_REG2            0x35
#define W25Q256JV_CMD_READ_STATUS_REG3            0x15
#define W25Q256JV_CMD_WRITE_STATUS_REG1           0x01
#define W25Q256JV_CMD_WRITE_STATUS_REG2           0x31
#define W25Q256JV_CMD_WRITE_STATUS_REG3           0x11
#define W25Q256JV_CMD_READ_DATA                   0x03
#define W25Q256JV_CMD_FAST_READ                   0x0B
#define W25Q256JV_CMD_QUAD_FAST_READ              0x6B
#define W25Q256JV_CMD_QUAD_IO_FAST_READ           0xEB
#define W25Q256JV_CMD_PAGE_PROGRAM                0x02
#define W25Q256JV_CMD_QUAD_PAGE_PROGRAM           0x32
#define W25Q256JV_CMD_SECTOR_ERASE_4KB            0x20
#define W25Q256JV_CMD_BLOCK_ERASE_32KB            0x52
#define W25Q256JV_CMD_BLOCK_ERASE_64KB            0xD8
#define W25Q256JV_CMD_CHIP_ERASE                  0xC7
#define W25Q256JV_CMD_ENABLE_RESET                0x66
#define W25Q256JV_CMD_RESET_DEVICE                0x99
#define W25Q256JV_CMD_READ_JEDEC_ID               0x9F
#define W25Q256JV_CMD_READ_UNIQUE_ID              0x4B
#define W25Q256JV_CMD_READ_SFDP                   0x5A
#define W25Q256JV_CMD_RELEASE_POWERDOWN           0xAB

 Status Register Bits ------------------------------------------------------
#define W25Q256JV_STATUS_BUSY         (1 << 0)      BUSY bit
#define W25Q256JV_STATUS_WEL          (1 << 1)      Write Enable Latch
#define W25Q256JV_STATUS_BP0          (1 << 2)      Block Protect 0
#define W25Q256JV_STATUS_BP1          (1 << 3)      Block Protect 1
#define W25Q256JV_STATUS_BP2          (1 << 4)      Block Protect 2
#define W25Q256JV_STATUS_BP3          (1 << 5)      Block Protect 3
#define W25Q256JV_STATUS_QE           (1 << 1)      Quad Enable (Status Reg2)
#define W25Q256JV_STATUS_SRWD         (1 << 7)      Status Register Write Disable

 Public Function Prototypes -----------------------------------------------

*
  * @brief  Initialize W25Q256JV flash memory
  * @retval HAL_OK if successful, HAL_ERROR otherwise

uint8_t W25Q256JV_Init(void);

*
  * @brief  Read the JEDEC ID of the flash memory
  * @retval JEDEC ID (Manufacturer ID << 16 | Memory Type << 8 | Capacity)

uint32_t W25Q256JV_ReadID(void);

*
  * @brief  Erase the entire flash chip
  * @retval HAL_OK if successful, HAL_ERROR otherwise

uint8_t W25Q256JV_EraseChip(void);

*
  * @brief  Erase a 64KB block
  * @param  BlockAddress: Address within the block to erase
  * @retval HAL_OK if successful, HAL_ERROR otherwise

uint8_t W25Q256JV_EraseBlock(uint32_t BlockAddress);

*
  * @brief  Erase a 4KB sector
  * @param  SectorAddress: Address within the sector to erase
  * @retval HAL_OK if successful, HAL_ERROR otherwise

uint8_t W25Q256JV_EraseSector(uint32_t SectorAddress);

*
  * @brief  Erase multiple sectors
  * @param  StartAddress: Starting address
  * @param  EndAddress: Ending address
  * @retval HAL_OK if successful, HAL_ERROR otherwise

uint8_t W25Q256JV_EraseSectors(uint32_t StartAddress, uint32_t EndAddress);

*
  * @brief  Write data to flash memory
  * @param  pData: Pointer to data buffer
  * @param  WriteAddr: Write address
  * @param  Size: Size of data to write
  * @retval HAL_OK if successful, HAL_ERROR otherwise

uint8_t W25Q256JV_Write(uint8_t *pData, uint32_t WriteAddr, uint32_t Size);

*
  * @brief  Read data from flash memory
  * @param  pData: Pointer to data buffer
  * @param  ReadAddr: Read address
  * @param  Size: Size of data to read
  * @retval HAL_OK if successful, HAL_ERROR otherwise

uint8_t W25Q256JV_Read(uint8_t *pData, uint32_t ReadAddr, uint32_t Size);

*
  * @brief  Enable memory mapped mode for XIP
  * @retval HAL_OK if successful, HAL_ERROR otherwise

uint8_t W25Q256JV_EnableMemoryMappedMode(void);

*
  * @brief  Read status register 1
  * @retval Status register value

uint8_t W25Q256JV_ReadStatusRegister1(void);

*
  * @brief  Check if the flash is ready
  * @retval 1 if ready, 0 if busy

static inline uint8_t W25Q256JV_IsReady(void)
{
    return (W25Q256JV_ReadStatusRegister1() & W25Q256JV_STATUS_BUSY) == 0;
}

*
  * @brief  Check if write is enabled
  * @retval 1 if write enabled, 0 if not

static inline uint8_t W25Q256JV_IsWriteEnabled(void)
{
    return (W25Q256JV_ReadStatusRegister1() & W25Q256JV_STATUS_WEL) != 0;
}

#ifdef __cplusplus
}
#endif

#endif  W25Q256JV_H



























































*/











/*#ifndef W25Q256JV2_H
#define W25Q256JV2_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

 Memory Size Definitions
#define W25Q256JV_FLASH_SIZE          0x2000000UL   256 Mbits / 32 MBytes
#define W25Q256JV_BLOCK_SIZE          0x10000UL     64 KBytes per block
#define W25Q256JV_SECTOR_SIZE         0x1000UL      4 KBytes per sector
#define W25Q256JV_PAGE_SIZE           0x100UL       256 bytes per page

 Manufacturer and Device ID
#define W25Q256JV_MANUFACTURER_ID     0xEF
#define W25Q256JV_DEVICE_ID           0x4019

 Commands (Quad mode only)
#define W25Q256JV_CMD_WRITE_ENABLE    0x06
#define W25Q256JV_CMD_WRITE_DISABLE   0x04
#define W25Q256JV_CMD_READ_STATUS1    0x05
#define W25Q256JV_CMD_READ_STATUS2    0x35
#define W25Q256JV_CMD_QUAD_PAGE_PROGRAM 0x32
#define W25Q256JV_CMD_QUAD_IO_FAST_READ 0xEB
#define W25Q256JV_CMD_SECTOR_ERASE    0x20
#define W25Q256JV_CMD_BLOCK_ERASE     0xD8
#define W25Q256JV_CMD_CHIP_ERASE      0xC7
#define W25Q256JV_CMD_RESET_ENABLE    0x66
#define W25Q256JV_CMD_RESET_DEVICE    0x99
#define W25Q256JV_CMD_READ_JEDEC_ID   0x9F

 Status Register Bits
#define W25Q256JV_STATUS_BUSY         (1 << 0)
#define W25Q256JV_STATUS_WEL          (1 << 1)
#define W25Q256JV_STATUS_QE           (1 << 1)   Always 1 on your chip!

 Timeouts
#define W25Q256JV_CMD_TIMEOUT         1000
#define W25Q256JV_ERASE_TIMEOUT       5000

 Public Function Prototypes
uint8_t W25Q256JV_Init(void);
uint32_t W25Q256JV_ReadID(void);
uint8_t W25Q256JV_ReadStatus1(void);
uint8_t W25Q256JV_ReadStatus2(void);
uint8_t W25Q256JV_EraseChip(void);
uint8_t W25Q256JV_EraseBlock(uint32_t address);
uint8_t W25Q256JV_EraseSector(uint32_t address);
uint8_t W25Q256JV_Write(uint8_t *data, uint32_t address, uint32_t size);
uint8_t W25Q256JV_Read(uint8_t *data, uint32_t address, uint32_t size);
uint8_t W25Q256JV_WaitForReady(void);

uint8_t W25Q256JV_Read_MDMA(uint8_t *data, uint32_t address, uint32_t size);
void HAL_QSPI_RxCpltCallback(QSPI_HandleTypeDef *hqspi);






 Helper functions
static inline uint8_t W25Q256JV_IsReady(void) {
    return (W25Q256JV_ReadStatus1() & W25Q256JV_STATUS_BUSY) == 0;
}

static inline uint8_t W25Q256JV_IsQuadEnabled(void) {
    return (W25Q256JV_ReadStatus2() & W25Q256JV_STATUS_QE) != 0;
}

#ifdef __cplusplus
}
#endif

#endif  W25Q256JV_H */
