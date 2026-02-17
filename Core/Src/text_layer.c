


#include "text_layer.h"
#include "dma2d_driver.h"
#include "lcd_driver.h"
#include "text_engine.h"
/*============================================================================
 * Private Variables
 *============================================================================*/
#define SDRAM_BANK_ADDR     ((uint32_t)0xC0000000)
#define LCD_FRAMEBUFFER     (SDRAM_BANK_ADDR)                    /* Layer 0 */
#define TEXT_LAYER_ADDR     (SDRAM_BANK_ADDR + 0x00200000)      /* 2MB offset */

static uint32_t textLayerAddr = TEXT_LAYER_ADDR;
static uint16_t layerWidth = 0;
static uint16_t layerHeight = 0;
static uint32_t backgroundColor = COLOR_BLACK;

/*============================================================================
 * Initialization
 *============================================================================*/
void TEXT_LAYER_Init(uint16_t width, uint16_t height)
{
    layerWidth = width;
    layerHeight = height;

    /* Clear entire layer at startup */
    TEXT_LAYER_Clear(textLayerAddr, backgroundColor);
}

/*============================================================================
 * Buffer Management
 *============================================================================*/
void TEXT_LAYER_Clear(uint32_t destAddr, uint32_t color)  /* RGB888 */
{
    backgroundColor = color;

    /* DMA2D accelerated fill */
    DMA2D_FillRect(destAddr,
                   0, 0,
                   layerWidth,
                   layerHeight,
                   color);
}

uint32_t TEXT_LAYER_GetBufferAddress(void)
{
    return textLayerAddr;
}



void TEXT_LAYER_ClearArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color)
{
    DMA2D_FillRect(textLayerAddr, x, y, w, h, color);
}








































