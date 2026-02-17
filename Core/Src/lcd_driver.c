#include "ltdc.h"
#include "lcd_driver.h"   // only if you want dynamic width/height


/* Current orientation */
static LCD_Orientation_t lcd_orientation = LCD_ORIENTATION_LANDSCAPE;

/* -------------------------------------------------------------------------- */
/* Local helpers                                                               */
/* -------------------------------------------------------------------------- */
static void LCD_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable GPIO clocks */
    __HAL_RCC_GPIOH_CLK_ENABLE();

    /* Reset pin */
    GPIO_InitStruct.Pin = LCD_RESET_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LCD_RESET_GPIO_PORT, &GPIO_InitStruct);

    /* Backlight pin */
    GPIO_InitStruct.Pin = LCD_BACKLIGHT_PIN;
    HAL_GPIO_Init(LCD_BACKLIGHT_GPIO_PORT, &GPIO_InitStruct);
}

/* -------------------------------------------------------------------------- */
/* Public API                                                                  */
/* -------------------------------------------------------------------------- */
void LCD_Driver_Init(void)
{
    LCD_GPIO_Init();

    /* Hardware reset sequence */
    LCD_RESET_LOW();
    HAL_Delay(20);
    LCD_RESET_HIGH();
    HAL_Delay(20);

    /* Turn on backlight */
    LCD_BACKLIGHT_ON();

    /* If your panel requires an init sequence (SPI/I2C), add it here */
}

void LCD_SetOrientation(LCD_Orientation_t orientation)
{
    lcd_orientation = orientation;
}

uint16_t LCD_GetWidth(void)
{
    return (lcd_orientation == LCD_ORIENTATION_LANDSCAPE) ?
           LCD_PANEL_WIDTH : LCD_PANEL_HEIGHT;
}

uint16_t LCD_GetHeight(void)
{
    return (lcd_orientation == LCD_ORIENTATION_LANDSCAPE) ?
           LCD_PANEL_HEIGHT : LCD_PANEL_WIDTH;
}
