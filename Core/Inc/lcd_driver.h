#ifndef __LCD_DRIVER_H__
#define __LCD_DRIVER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"

/* -------------------------------------------------------------------------- */
/* Panel Resolution                                                            */
/* -------------------------------------------------------------------------- */
#define LCD_PANEL_WIDTH     1024U
#define LCD_PANEL_HEIGHT    600U

/* -------------------------------------------------------------------------- */
/* Panel Timing (adjust if your datasheet differs)                             */
/* -------------------------------------------------------------------------- */
#define LCD_HSYNC           20U
#define LCD_HBP             140U
#define LCD_HFP             160U

#define LCD_VSYNC           3U
#define LCD_VBP             20U
#define LCD_VFP             12U

/* -------------------------------------------------------------------------- */
/* Backlight + Reset Pins                                                      */
/* -------------------------------------------------------------------------- */
#define LCD_RESET_GPIO_PORT     GPIOH
#define LCD_RESET_PIN           GPIO_PIN_3

#define LCD_BACKLIGHT_GPIO_PORT GPIOH
#define LCD_BACKLIGHT_PIN       GPIO_PIN_4

/* Reset control */
#define LCD_RESET_HIGH()   HAL_GPIO_WritePin(LCD_RESET_GPIO_PORT, LCD_RESET_PIN, GPIO_PIN_SET)
#define LCD_RESET_LOW()    HAL_GPIO_WritePin(LCD_RESET_GPIO_PORT, LCD_RESET_PIN, GPIO_PIN_RESET)

/* Backlight control */
#define LCD_BACKLIGHT_ON()  HAL_GPIO_WritePin(LCD_BACKLIGHT_GPIO_PORT, LCD_BACKLIGHT_PIN, GPIO_PIN_SET)
#define LCD_BACKLIGHT_OFF() HAL_GPIO_WritePin(LCD_BACKLIGHT_GPIO_PORT, LCD_BACKLIGHT_PIN, GPIO_PIN_RESET)

/* -------------------------------------------------------------------------- */
/* Orientation                                                                 */
/* -------------------------------------------------------------------------- */
typedef enum
{
    LCD_ORIENTATION_PORTRAIT  = 0,
    LCD_ORIENTATION_LANDSCAPE = 1
} LCD_Orientation_t;

/* -------------------------------------------------------------------------- */
/* Public API                                                                  */
/* -------------------------------------------------------------------------- */

/* Initialize GPIO + reset + backlight */
void LCD_Driver_Init(void);

/* Set display orientation */
void LCD_SetOrientation(LCD_Orientation_t orientation);

/* Get current width/height depending on orientation */
uint16_t LCD_GetWidth(void);
uint16_t LCD_GetHeight(void);

#ifdef __cplusplus
}
#endif

#endif /* __LCD_DRIVER_H__ */
