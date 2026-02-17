/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma2d.h"
#include "i2c.h"
#include "jpeg.h"
#include "ltdc.h"
#include "quadspi.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"
#include "w9825g6kh.h"
#include "w25q256jv2.h"
#include "retarget.h"
#include <string.h>
#include "stdio.h"
#include <stdlib.h> // for rand()
#include <time.h>

#include "lcd_driver.h"  // Reset/backlight + LCD_GetWidth/Height
#include "test_lcd.h"
#include "dma2d_driver.h"  // FillRect, CopyBuffer, AlphaBlendGlyph
#include "text_layer.h"    // Layer buffer management in SDRAM
#include "text_engine.h"   // Cursor, Print, PutChar
#include "font_renderer.h" // Glyph rendering

#include "font_arialbd36.h"
#include "font_arial36.h"
#include "font_arial42.h"
#include "font_arial46.h"
#include "font_arial52.h"
#include "font_arial56.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define LCD_FRAMEBUFFER ((uint32_t)0xC0000000)
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern SDRAM_HandleTypeDef hsdram1;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* Enable the CPU Cache */

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_LTDC_Init();
  MX_QUADSPI_Init();
  MX_FMC_Init();
  MX_I2C2_Init();
  MX_SPI1_Init();
  MX_UART4_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  MX_DMA2D_Init();
  MX_JPEG_Init();
  MX_TIM2_Init();
  MX_I2C4_Init();
  /* USER CODE BEGIN 2 */
  if (W25Q256JV_Init() == HAL_OK) {
            printf("[MAIN] Flash initialized\r\n");

            /* Read and verify JEDEC ID */
            uint32_t id = W25Q256JV_ReadID();
            printf("[MAIN] Flash JEDEC ID: 0x%06lX\r\n", id);

            /*-------------------------------------------------------------------
             * STEP 4: Enable memory-mapped mode - THIS IS THE KEY!
             *-------------------------------------------------------------------*/
            if (W25Q256JV_EnterMemoryMappedMode(0x90000000, W25Q256JV_FLASH_SIZE) == HAL_OK) {
                printf("[MAIN] Memory-mapped mode enabled at 0x90000000\r\n");

                /* Test memory-mapped mode */
                //W25Q256JV_TestMemoryMapped();
            }
        }

  DMA2D_Driver_Init();

    LCD_Driver_Init();   // Reset + backlight
        TEXT_LAYER_Init(LCD_GetWidth(), LCD_GetHeight());
        TEXT_Init();


       TEXT_ColorBarTest();

       TEST_LCD_ColorNames();

       void LCD_RandomPitchDemo();

       TEXT_SetCursor(100, 500);
       TEXT_SetTextColor(COLOR_WHITE, COLOR_BLACK);
       TEXT_Print(&Arial42, "HELLO°abcdefghijklmnopqrstuvw");


//*****************************************************************
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

	  LCD_RandomPitchDemo();


    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 3;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /*-----------------------------------------------------------------------
      * SDRAM Region (0xC0000000) - Write-through, non-cacheable for LTDC
      *-----------------------------------------------------------------------*/

     MPU_InitStruct.Enable = MPU_REGION_ENABLE;
     MPU_InitStruct.Number = MPU_REGION_NUMBER0;
     MPU_InitStruct.BaseAddress = 0xC0000000;
     MPU_InitStruct.Size = MPU_REGION_SIZE_32MB;
     MPU_InitStruct.SubRegionDisable = 0;
     MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
     MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
     MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
     MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
     MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
     MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;  /* Write-through */
     HAL_MPU_ConfigRegion(&MPU_InitStruct);

     /*-----------------------------------------------------------------------
      * QSPI Flash Region (0x90000000) - Add this!
      *-----------------------------------------------------------------------*/
     MPU_Config_QSPI();

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}


void MPU_Config_QSPI(void)
{
    MPU_Region_InitTypeDef MPU_InitStruct = {0};

    /* QSPI Flash Region - READ ONLY, EXECUTION NEVER */
    MPU_InitStruct.Enable = MPU_REGION_ENABLE;
    MPU_InitStruct.Number = MPU_REGION_NUMBER1;
    MPU_InitStruct.BaseAddress = 0x90000000;
    MPU_InitStruct.Size = MPU_REGION_SIZE_32MB;  /* 2^25 = 32MB */
    MPU_InitStruct.SubRegionDisable = 0;
    MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
    MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
    MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;  /* XN = Prevent code execution */
    MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
    MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;           /* Cache for speed */
    MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

    HAL_MPU_ConfigRegion(&MPU_InitStruct);
    printf("[MPU] QSPI region: 0x90000000, 32MB, cacheable, XN\r\n");
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
