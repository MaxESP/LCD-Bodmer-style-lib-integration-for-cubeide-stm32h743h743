

#include "dma2d_driver.h"
#include "lcd_driver.h"

/* External handles from HAL */
extern DMA2D_HandleTypeDef hdma2d;

/*============================================================================
 * Private Definitions - NO DEPENDENCY ON LTDC LAYER CFG!
 *============================================================================*/
#ifndef LCD_PIXEL_WIDTH
#define LCD_PIXEL_WIDTH  1024  /* Must match your LCD width */
#endif

#ifndef LCD_PIXEL_HEIGHT
#define LCD_PIXEL_HEIGHT 600   /* Must match your LCD height */
#endif

#ifndef LCD_BYTES_PER_PIXEL
#define LCD_BYTES_PER_PIXEL 3   /* 3 for RGB888, 2 for RGB565 */
#endif




#ifndef DMA2D_FGPFCCR_ALPHA_MODE_Pos
#define DMA2D_FGPFCCR_ALPHA_MODE_Pos 16
#endif

#ifndef DMA2D_FGPFCCR_ALPHA_Pos
#define DMA2D_FGPFCCR_ALPHA_Pos 8
#endif

#ifndef DMA2D_FGPFCCR_AM
#define DMA2D_FGPFCCR_AM (1 << 14)
#endif

#ifndef DMA2D_BGPFCCR_ALPHA_MODE
#define DMA2D_BGPFCCR_ALPHA_MODE (3 << 16)
#endif

#ifndef LCD_PIXEL_WIDTH
#define LCD_PIXEL_WIDTH  1024
#endif

#ifndef LCD_PIXEL_HEIGHT
#define LCD_PIXEL_HEIGHT 600
#endif

#ifndef LCD_BYTES_PER_PIXEL
#define LCD_BYTES_PER_PIXEL 3
#endif


/*============================================================================
 * Private Functions
 *============================================================================*/

/**
 * @brief Wait for DMA2D transfer complete
 * @note  Polls TC flag, then clears it
 */
static void DMA2D_WaitForTransfer(void)
{
    while (__HAL_DMA2D_GET_FLAG(&hdma2d, DMA2D_FLAG_TC) == RESET) {}
    __HAL_DMA2D_CLEAR_FLAG(&hdma2d, DMA2D_FLAG_TC);
}

/**
 * @brief Switch DMA2D mode
 * @param mode: DMA2D_R2M, DMA2D_M2M, or DMA2D_M2M_BLEND
 */
static void DMA2D_SetMode(uint32_t mode)
{
    if (hdma2d.Init.Mode != mode) {
        hdma2d.Init.Mode = mode;
        HAL_DMA2D_Init(&hdma2d);
    }
}

/*============================================================================
 * Initialization
 *============================================================================*/

/**
 * @brief Initialize DMA2D driver
 * @note  Sets default mode to R2M for fill operations
 */
void DMA2D_Driver_Init(void)
{
    __HAL_RCC_DMA2D_CLK_ENABLE();

    hdma2d.Instance = DMA2D;
    hdma2d.Init.Mode = DMA2D_R2M;
    hdma2d.Init.ColorMode = DMA2D_OUTPUT_RGB888;
    hdma2d.Init.OutputOffset = 0;

    /* No layer configuration needed for R2M mode */
    if (HAL_DMA2D_Init(&hdma2d) != HAL_OK) {
        while (1);  /* Init failed */
    }
}

/*============================================================================
 * Basic Drawing Operations
 *============================================================================*/

/**
 * @brief Fill rectangle with solid RGB888 color
 * @param destAddr: Base address of framebuffer
 * @param x,y: Top-left corner
 * @param w,h: Width and height in pixels
 * @param color: RGB888 color (0xRRGGBB)
 * @note  Uses DMA2D_R2M mode - FASTEST for solid fills
 */
void DMA2D_FillRect(uint32_t destAddr,
                    uint16_t x, uint16_t y,
                    uint16_t w, uint16_t h,
                    uint32_t color)
{
    /* Calculate stride (pixels between lines) - USE CONSTANT! */
    uint32_t stride = LCD_PIXEL_WIDTH - w;

    /* Calculate start address: 3 bytes per pixel - USE CONSTANT! */
    uint32_t addr = destAddr + ((uint32_t)y * LCD_PIXEL_WIDTH + x) * LCD_BYTES_PER_PIXEL;

    /* Ensure R2M mode */
    DMA2D_SetMode(DMA2D_R2M);

    /* Configure R2M operation */
    hdma2d.Instance->OPFCCR = DMA2D_OUTPUT_RGB888;
    hdma2d.Instance->OCOLR = color;
    hdma2d.Instance->OMAR = addr;
    hdma2d.Instance->OOR = stride;
    hdma2d.Instance->NLR = (w << 16) | h;

    /* Start transfer */
    __HAL_DMA2D_ENABLE(&hdma2d);
    DMA2D_WaitForTransfer();
}

/**
 * @brief Draw single pixel - CPU write
 * @param destAddr: Base address of framebuffer
 * @param x,y: Pixel coordinates
 * @param color: RGB888 color (0xRRGGBB)
 * @note  DMA2D overhead not worth it for 1 pixel
 */
void DMA2D_DrawPixel(uint32_t destAddr,
                     uint16_t x, uint16_t y,
                     uint32_t color)
{
    /* Calculate address: 3 bytes per pixel - USE CONSTANT! */
    uint32_t addr = destAddr + ((uint32_t)y * LCD_PIXEL_WIDTH + x) * LCD_BYTES_PER_PIXEL;

    /* Write RGB888 components */
    *(volatile uint8_t *)(addr)     = (color >> 16) & 0xFF;  /* R */
    *(volatile uint8_t *)(addr + 1) = (color >> 8)  & 0xFF;  /* G */
    *(volatile uint8_t *)(addr + 2) = color         & 0xFF;  /* B */
}

/**
 * @brief Copy buffer to buffer
 * @param srcAddr: Source address
 * @param dstAddr: Destination address
 * @param width,height: Dimensions in pixels
 * @note  Uses DMA2D_M2M mode
 */
void DMA2D_CopyBuffer(uint32_t srcAddr,
                      uint32_t dstAddr,
                      uint16_t width,
                      uint16_t height)
{
    /* Calculate stride - USE CONSTANT! */
    uint32_t stride = LCD_PIXEL_WIDTH - width;

    /* Switch to M2M mode */
    DMA2D_SetMode(DMA2D_M2M);

    /* Configure M2M operation */
    hdma2d.Instance->FGMAR = srcAddr;
    hdma2d.Instance->OMAR = dstAddr;
    hdma2d.Instance->FGOR = 0;
    hdma2d.Instance->OOR = stride;
    hdma2d.Instance->FGPFCCR = DMA2D_INPUT_RGB888;
    hdma2d.Instance->OPFCCR = DMA2D_OUTPUT_RGB888;
    hdma2d.Instance->NLR = (width << 16) | height;

    /* Start transfer */
    __HAL_DMA2D_ENABLE(&hdma2d);
    DMA2D_WaitForTransfer();
}







void DMA2D_AlphaBlendGlyph(uint32_t glyphMaskAddr,
                           uint32_t destAddr,
                           uint16_t x, uint16_t y,
                           uint16_t w, uint16_t h,
                           uint32_t textColor)
{
    /* Calculate destination address */
    uint32_t dst = destAddr + ((uint32_t)y * LCD_PIXEL_WIDTH + x) * LCD_BYTES_PER_PIXEL;
    uint32_t stride = LCD_PIXEL_WIDTH - w;

    /* Switch to M2M_BLEND mode */
    DMA2D_SetMode(DMA2D_M2M_BLEND);

    /*----------------------------------------------------------------------
     * Foreground: A8 alpha mask
     *----------------------------------------------------------------------*/
    hdma2d.Instance->FGMAR = glyphMaskAddr;
    hdma2d.Instance->FGOR = 0;
    hdma2d.Instance->FGPFCCR = DMA2D_INPUT_A8;

    /* Configure alpha mode: use alpha from A8 mask */
    hdma2d.Instance->FGPFCCR &= ~(0x3U << DMA2D_FGPFCCR_ALPHA_MODE_Pos);   // clear ALPHA_MODE bits
    hdma2d.Instance->FGPFCCR |=  (0x1U << DMA2D_FGPFCCR_ALPHA_MODE_Pos);   // ALPHA_MODE = 01 (use A8 mask)

    hdma2d.Instance->FGPFCCR &= ~(0xFFU << DMA2D_FGPFCCR_ALPHA_Pos);       // clear ALPHA bits
    hdma2d.Instance->FGPFCCR |=  (0xFFU << DMA2D_FGPFCCR_ALPHA_Pos);       // ALPHA = 0xFF (full)

    CLEAR_BIT(hdma2d.Instance->FGPFCCR, DMA2D_FGPFCCR_AM);                 // ALPHA_INVERTED = 0

    /*----------------------------------------------------------------------
     * Background: RGB888 framebuffer
     *----------------------------------------------------------------------*/
    hdma2d.Instance->BGMAR = dst;
    hdma2d.Instance->BGOR = stride;
    hdma2d.Instance->BGPFCCR = DMA2D_INPUT_RGB888;
    CLEAR_BIT(hdma2d.Instance->BGPFCCR, DMA2D_BGPFCCR_ALPHA_MODE);

    /*----------------------------------------------------------------------
     * Output: RGB888
     *----------------------------------------------------------------------*/
    hdma2d.Instance->OMAR = dst;
    hdma2d.Instance->OOR = stride;
    hdma2d.Instance->OPFCCR = DMA2D_OUTPUT_RGB888;

    /*----------------------------------------------------------------------
     * Foreground color: convert RGB888 to ARGB8888
     *----------------------------------------------------------------------*/

    uint32_t argbColor = 0xFF000000 | (textColor & 0xFFFFFF);
    hdma2d.Instance->FGCOLR = 0xFF000000 | (textColor & 0xFFFFFF);
    hdma2d.Instance->FGCOLR = argbColor;

    /*----------------------------------------------------------------------
     * Size
     *----------------------------------------------------------------------*/
    hdma2d.Instance->NLR = (w << 16) | h;

    /* Start transfer */
    __HAL_DMA2D_ENABLE(&hdma2d);
    DMA2D_WaitForTransfer();
}

void DMA2D_FillCircle(uint32_t destAddr,
                      uint16_t x0, uint16_t y0,
                      uint16_t r,
                      uint32_t color)
{
    int16_t x = 0;
    int16_t y = r;
    int16_t d = 3 - 2 * r;

    while (y >= x)
    {
        /* Draw horizontal spans using DMA2D_FillRect */
        DMA2D_FillRect(destAddr, x0 - x, y0 - y, 2 * x + 1, 1, color);
        DMA2D_FillRect(destAddr, x0 - x, y0 + y, 2 * x + 1, 1, color);
        DMA2D_FillRect(destAddr, x0 - y, y0 - x, 2 * y + 1, 1, color);
        DMA2D_FillRect(destAddr, x0 - y, y0 + x, 2 * y + 1, 1, color);

        x++;

        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
    }
}













































