/*
if (val < 0){
		TEXT_SetTextColor(COLOR_BLUE, COLOR_BLACK);
		//TEXT_LAYER_ClearArea(200, 200, 200, 48, COLOR_RED);
		//TEXT_LAYER_Clear(TEXT_LAYER_GetBufferAddress(), 0x0000); // black
	}



	else {
		TEXT_SetTextColor(COLOR_GREEN, COLOR_BLACK);
		//TEXT_LAYER_Clear(TEXT_LAYER_GetBufferAddress(), 0x0000); // black
	}*/

/*============================================================================
 * Alpha Blending Operations - FIXED FOR CLEAN BACKGROUND!
 *============================================================================*/

/**
 * @brief Alpha blend A8 glyph mask onto RGB888 framebuffer
 * @param glyphMaskAddr: Address of A8 alpha mask (8-bit per pixel)
 * @param destAddr: Base address of framebuffer
 * @param x,y: Position to draw glyph
 * @param w,h: Glyph dimensions
 * @param textColor: RGB888 text color
 * @note  Uses DMA2D_M2M_BLEND mode - HARDWARE ACCELERATED
 */


/*



void DMA2D_AlphaBlendGlyph(uint32_t glyphMaskAddr,
                           uint32_t destAddr,
                           uint16_t x, uint16_t y,
                           uint16_t w, uint16_t h,
                           uint32_t textColor)
{
     Calculate destination address
    uint32_t dst = destAddr + ((uint32_t)y * LCD_PIXEL_WIDTH + x) * LCD_BYTES_PER_PIXEL;
    uint32_t stride = LCD_PIXEL_WIDTH - w;

     Switch to M2M_BLEND mode
    DMA2D_SetMode(DMA2D_M2M_BLEND);

    ----------------------------------------------------------------------
     * Foreground: A8 alpha mask - USING HAL REGISTER MACROS
     *----------------------------------------------------------------------
    hdma2d.Instance->FGMAR = glyphMaskAddr;
    hdma2d.Instance->FGOR = 0;
    hdma2d.Instance->FGPFCCR = DMA2D_INPUT_A8;

     USE HAL MACROS - These are guaranteed to work
    MODIFY_REG(hdma2d.Instance->FGPFCCR,
    		DMA2D_BGPFCCR_ALPHA_MODE,
               (0x01 << DMA2D_FGPFCCR_ALPHA_MODE_Pos));   ALPHA_MODE = 01

    MODIFY_REG(hdma2d.Instance->FGPFCCR,
               DMA2D_FGPFCCR_ALPHA,
               (0xFF << DMA2D_FGPFCCR_ALPHA_Pos));        ALPHA = 0xFF

    CLEAR_BIT(hdma2d.Instance->FGPFCCR,
              DMA2D_FGPFCCR_AM);                         ALPHA_INVERTED = 0

    ----------------------------------------------------------------------
     * Background: RGB888 framebuffer
     *----------------------------------------------------------------------
    hdma2d.Instance->BGMAR = dst;
    hdma2d.Instance->BGOR = stride;
    hdma2d.Instance->BGPFCCR = DMA2D_INPUT_RGB888;

    CLEAR_BIT(hdma2d.Instance->BGPFCCR,
              DMA2D_BGPFCCR_ALPHA_MODE);                ALPHA_MODE = 0

    ----------------------------------------------------------------------
     * Output: RGB888
     *----------------------------------------------------------------------
    hdma2d.Instance->OMAR = dst;
    hdma2d.Instance->OOR = stride;
    hdma2d.Instance->OPFCCR = DMA2D_OUTPUT_RGB888;

    ----------------------------------------------------------------------
     *
     *
     *
     *
     *
     * Foreground color - RGB888
     *----------------------------------------------------------------------
     ---- FIX: Convert RGB888 to ARGB8888 with alpha ----
    uint32_t argbColor = 0xFF000000 | (textColor & 0xFFFFFF);
    hdma2d.Instance->FGCOLR = argbColor;



    //hdma2d.Instance->FGCOLR = textColor;

    ----------------------------------------------------------------------
     * Size
     *----------------------------------------------------------------------
    hdma2d.Instance->NLR = (w << 16) | h;

     Start transfer
    __HAL_DMA2D_ENABLE(&hdma2d);
    DMA2D_WaitForTransfer();
}

*/




/*
void DMA2D_AlphaBlendGlyph(uint32_t glyphMaskAddr,
                           uint32_t destAddr,
                           uint16_t x, uint16_t y,
                           uint16_t w, uint16_t h,
                           uint32_t textColor)
{
     Calculate destination address
    uint32_t dst = destAddr + ((uint32_t)y * LCD_PIXEL_WIDTH + x) * LCD_BYTES_PER_PIXEL;
    uint32_t stride = LCD_PIXEL_WIDTH - w;

     Switch to M2M_BLEND mode
    DMA2D_SetMode(DMA2D_M2M_BLEND);

    ----------------------------------------------------------------------
     * Foreground: A8 alpha mask - CRITICAL FIX FOR CLEAN BACKGROUND
     *----------------------------------------------------------------------
    hdma2d.Instance->FGMAR = glyphMaskAddr;
    hdma2d.Instance->FGOR = 0;
    hdma2d.Instance->FGPFCCR = DMA2D_INPUT_A8;

     CRITICAL: Set ALPHA_MODE = 0x01 (Replace with FGCOLR * alpha/255)
       This ensures text color is SOLID, not accumulated
    hdma2d.Instance->FGPFCCR &= ~(3 << 16);         Clear bits 16-17
    hdma2d.Instance->FGPFCCR |= (0x01 << 16);       Set ALPHA_MODE = 0x01

     Set constant alpha to 0xFF for full opacity
    hdma2d.Instance->FGPFCCR &= ~(0xFF << 8);       Clear bits 8-15
    hdma2d.Instance->FGPFCCR |= (0xFF << 8);        Set ALPHA = 0xFF

    ----------------------------------------------------------------------
     * Background: RGB888 framebuffer
     *----------------------------------------------------------------------
    hdma2d.Instance->BGMAR = dst;
    hdma2d.Instance->BGOR = stride;
    hdma2d.Instance->BGPFCCR = DMA2D_INPUT_RGB888;

     Background: No alpha modification
    hdma2d.Instance->BGPFCCR &= ~(3 << 16);         Clear bits 16-17
     ALPHA_MODE = 0x00 is default

    ----------------------------------------------------------------------
     * Output: RGB888
     *----------------------------------------------------------------------
    hdma2d.Instance->OMAR = dst;
    hdma2d.Instance->OOR = stride;
    hdma2d.Instance->OPFCCR = DMA2D_OUTPUT_RGB888;

    ----------------------------------------------------------------------
     * Foreground color - RGB888
     *----------------------------------------------------------------------
    hdma2d.Instance->FGCOLR = textColor;

    ----------------------------------------------------------------------
     * Size
     *----------------------------------------------------------------------
    hdma2d.Instance->NLR = (w << 16) | h;

     Start transfer
    __HAL_DMA2D_ENABLE(&hdma2d);
    DMA2D_WaitForTransfer();
}
*/





/*
#include "dma2d_driver.h"
#include "lcd_driver.h"

 External handles from HAL
extern DMA2D_HandleTypeDef hdma2d;
extern LTDC_HandleTypeDef hltdc;

============================================================================
 * Private Functions
 *============================================================================

*
 * @brief Wait for DMA2D transfer complete
 * @note  Polls TC flag, then clears it

static void DMA2D_WaitForTransfer(void)
{
    while (__HAL_DMA2D_GET_FLAG(&hdma2d, DMA2D_FLAG_TC) == RESET) {}
    __HAL_DMA2D_CLEAR_FLAG(&hdma2d, DMA2D_FLAG_TC);
}

*
 * @brief Switch DMA2D mode
 * @param mode: DMA2D_R2M, DMA2D_M2M, or DMA2D_M2M_BLEND

static void DMA2D_SetMode(uint32_t mode)
{
    if (hdma2d.Init.Mode != mode) {
        hdma2d.Init.Mode = mode;
        HAL_DMA2D_Init(&hdma2d);
    }
}

============================================================================
 * Initialization
 *============================================================================

*
 * @brief Initialize DMA2D driver
 * @note  Sets default mode to R2M for fill operations

void DMA2D_Driver_Init(void)
{
    __HAL_RCC_DMA2D_CLK_ENABLE();

    hdma2d.Instance = DMA2D;
    hdma2d.Init.Mode = DMA2D_R2M;
    hdma2d.Init.ColorMode = DMA2D_OUTPUT_RGB888;
    hdma2d.Init.OutputOffset = 0;

     No layer configuration needed for R2M mode

    if (HAL_DMA2D_Init(&hdma2d) != HAL_OK) {
        while (1);   Init failed
    }
}

============================================================================
 * Basic Drawing Operations
 *============================================================================

*
 * @brief Fill rectangle with solid RGB888 color
 * @param destAddr: Base address of framebuffer
 * @param x,y: Top-left corner
 * @param w,h: Width and height in pixels
 * @param color: RGB888 color (0xRRGGBB)
 * @note  Uses DMA2D_R2M mode - FASTEST for solid fills

void DMA2D_FillRect(uint32_t destAddr,
                    uint16_t x, uint16_t y,
                    uint16_t w, uint16_t h,
                    uint32_t color)
{
     Calculate stride (pixels between lines)
    uint32_t stride = hltdc.LayerCfg[0].ImageWidth - w;

     Calculate start address: 3 bytes per pixel
    uint32_t addr = destAddr + ((uint32_t)y * hltdc.LayerCfg[0].ImageWidth + x) * 3U;

     Ensure R2M mode
    DMA2D_SetMode(DMA2D_R2M);

     Configure R2M operation
    hdma2d.Instance->OPFCCR = DMA2D_OUTPUT_RGB888;
    hdma2d.Instance->OCOLR = color;
    hdma2d.Instance->OMAR = addr;
    hdma2d.Instance->OOR = stride;
    hdma2d.Instance->NLR = (w << 16) | h;

     Start transfer
    __HAL_DMA2D_ENABLE(&hdma2d);
    DMA2D_WaitForTransfer();
}

*
 * @brief Draw single pixel - CPU write
 * @param destAddr: Base address of framebuffer
 * @param x,y: Pixel coordinates
 * @param color: RGB888 color (0xRRGGBB)
 * @note  DMA2D overhead not worth it for 1 pixel

void DMA2D_DrawPixel(uint32_t destAddr,
                     uint16_t x, uint16_t y,
                     uint32_t color)
{
     Calculate address: 3 bytes per pixel
    uint32_t addr = destAddr + ((uint32_t)y * LCD_GetWidth() + x) * 3U;

     Write RGB888 components
    *(volatile uint8_t *)(addr)     = (color >> 16) & 0xFF;   R
    *(volatile uint8_t *)(addr + 1) = (color >> 8)  & 0xFF;   G
    *(volatile uint8_t *)(addr + 2) = color         & 0xFF;   B
}

*
 * @brief Copy buffer to buffer
 * @param srcAddr: Source address
 * @param dstAddr: Destination address
 * @param width,height: Dimensions in pixels
 * @note  Uses DMA2D_M2M mode

void DMA2D_CopyBuffer(uint32_t srcAddr,
                      uint32_t dstAddr,
                      uint16_t width,
                      uint16_t height)
{
     Calculate stride
    uint32_t stride = hltdc.LayerCfg[0].ImageWidth - width;

     Switch to M2M mode
    DMA2D_SetMode(DMA2D_M2M);

     Configure M2M operation
    hdma2d.Instance->FGMAR = srcAddr;
    hdma2d.Instance->OMAR = dstAddr;
    hdma2d.Instance->FGOR = 0;
    hdma2d.Instance->OOR = stride;
    hdma2d.Instance->FGPFCCR = DMA2D_INPUT_RGB888;
    hdma2d.Instance->OPFCCR = DMA2D_OUTPUT_RGB888;
    hdma2d.Instance->NLR = (width << 16) | height;

     Start transfer
    __HAL_DMA2D_ENABLE(&hdma2d);
    DMA2D_WaitForTransfer();
}

============================================================================
 * Alpha Blending Operations
 *============================================================================

*
 * @brief Alpha blend A8 glyph mask onto RGB888 framebuffer
 * @param glyphMaskAddr: Address of A8 alpha mask (8-bit per pixel)
 * @param destAddr: Base address of framebuffer
 * @param x,y: Position to draw glyph
 * @param w,h: Glyph dimensions
 * @param textColor: RGB888 text color
 * @note  Uses DMA2D_M2M_BLEND mode - HARDWARE ACCELERATED

void DMA2D_AlphaBlendGlyph(uint32_t glyphMaskAddr,
                           uint32_t destAddr,
                           uint16_t x, uint16_t y,
                           uint16_t w, uint16_t h,
                           uint32_t textColor)
{
     Calculate destination address: 3 bytes per pixel
    uint32_t dst = destAddr + ((uint32_t)y * hltdc.LayerCfg[0].ImageWidth + x) * 3U;
    uint32_t stride = hltdc.LayerCfg[0].ImageWidth - w;

     Switch to M2M_BLEND mode
    DMA2D_SetMode(DMA2D_M2M_BLEND);

    ----------------------------------------------------------------------
     * Foreground: A8 alpha mask
     *----------------------------------------------------------------------
    hdma2d.Instance->FGMAR = glyphMaskAddr;
    hdma2d.Instance->FGOR = 0;
    hdma2d.Instance->FGPFCCR = DMA2D_INPUT_A8;       0x09

    ----------------------------------------------------------------------
     * Background: RGB888 framebuffer
     *----------------------------------------------------------------------
    hdma2d.Instance->BGMAR = dst;
    hdma2d.Instance->BGOR = stride;
    hdma2d.Instance->BGPFCCR = DMA2D_INPUT_RGB888;   0x0C

    ----------------------------------------------------------------------
     * Output: RGB888 (in-place)
     *----------------------------------------------------------------------
    hdma2d.Instance->OMAR = dst;
    hdma2d.Instance->OOR = stride;
    hdma2d.Instance->OPFCCR = DMA2D_OUTPUT_RGB888;   0x0C

    ----------------------------------------------------------------------
     * Foreground color (RGB888)
     *----------------------------------------------------------------------
    hdma2d.Instance->FGCOLR = textColor;

    ----------------------------------------------------------------------
     * Size: width | (height << 16)
     *----------------------------------------------------------------------
    hdma2d.Instance->NLR = (w << 16) | h;

     Start transfer
    __HAL_DMA2D_ENABLE(&hdma2d);
    DMA2D_WaitForTransfer();
}

============================================================================
 * Shape Drawing
 *============================================================================

*
 * @brief Draw circle outline using Bresenham algorithm
 * @param destAddr: Base address of framebuffer
 * @param x0,y0: Center coordinates
 * @param r: Radius
 * @param color: RGB888 color

void DMA2D_DrawCircle(uint32_t destAddr,
                      uint16_t x0, uint16_t y0,
                      uint16_t r,
                      uint32_t color)
{
    int16_t x = 0;
    int16_t y = r;
    int16_t d = 3 - 2 * r;

    while (y >= x)
    {
         Draw 8 octants
        DMA2D_DrawPixel(destAddr, x0 + x, y0 + y, color);
        DMA2D_DrawPixel(destAddr, x0 - x, y0 + y, color);
        DMA2D_DrawPixel(destAddr, x0 + x, y0 - y, color);
        DMA2D_DrawPixel(destAddr, x0 - x, y0 - y, color);
        DMA2D_DrawPixel(destAddr, x0 + y, y0 + x, color);
        DMA2D_DrawPixel(destAddr, x0 - y, y0 + x, color);
        DMA2D_DrawPixel(destAddr, x0 + y, y0 - x, color);
        DMA2D_DrawPixel(destAddr, x0 - y, y0 - x, color);

        x++;

        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
    }
}

*
 * @brief Fill solid circle using horizontal spans
 * @param destAddr: Base address of framebuffer
 * @param x0,y0: Center coordinates
 * @param r: Radius
 * @param color: RGB888 color
 * @note  Uses DMA2D_FillRect for horizontal lines - FAST!

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
         Draw horizontal spans using DMA2D_FillRect
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

*/


/*
#include "dma2d_driver.h"
#include "stm32h7xx_hal.h"
#include "lcd_driver.h"


extern DMA2D_HandleTypeDef hdma2d;
extern LTDC_HandleTypeDef hltdc;

static void DMA2D_Clock_Init(void)
{
    __HAL_RCC_DMA2D_CLK_ENABLE();
}

void DMA2D_Driver_Init(void)
{
    DMA2D_Clock_Init();

    hdma2d.Instance         = DMA2D;
    hdma2d.Init.Mode        = DMA2D_R2M;
    hdma2d.Init.ColorMode   = DMA2D_OUTPUT_RGB888;
    hdma2d.Init.OutputOffset= 0;

    if (HAL_DMA2D_Init(&hdma2d) != HAL_OK)
    {
        while (1) {}
    }
}
*/



//hdma2d.Instance->OOR = lineOffset;
//*************************************************************************************
/*void DMA2D_FillRect(uint32_t destAddr,
                    uint16_t x, uint16_t y,
                    uint16_t w, uint16_t h,
                    uint16_t color)
{
    uint32_t stride = hltdc.LayerCfg[0].ImageWidth - w;
    uint32_t addr   = destAddr + ((uint32_t)y * hltdc.LayerCfg[0].ImageWidth + x) * 2U;

    hdma2d.Instance->CR    = DMA2D_R2M;
    hdma2d.Instance->OCOLR = color;
    hdma2d.Instance->OMAR  = addr;
    hdma2d.Instance->OOR   = stride;
    hdma2d.Instance->NLR   = ((uint32_t)w << 16) | h;

    __HAL_DMA2D_ENABLE(&hdma2d);
    while (__HAL_DMA2D_GET_FLAG(&hdma2d, DMA2D_FLAG_TC) == RESET) {}
    __HAL_DMA2D_CLEAR_FLAG(&hdma2d, DMA2D_FLAG_TC);
}*/
//*****************************************************************************
//* Fill a rectangle with a solid color */
/*void DMA2D_FillRect(uint32_t destAddr,
                    uint16_t x, uint16_t y,
                    uint16_t w, uint16_t h,
                    uint16_t color)
{
    uint32_t stride = hltdc.LayerCfg[0].ImageWidth - w;
    uint32_t addr   = destAddr + ((uint32_t)y * hltdc.LayerCfg[0].ImageWidth + x) * 2U;

    hdma2d.Instance->CR    = DMA2D_R2M;
    hdma2d.Instance->OCOLR = color;
    hdma2d.Instance->OMAR  = addr;
    hdma2d.Instance->OOR   = stride;
    hdma2d.Instance->NLR   = ((uint32_t)w << 16) | h;

    __HAL_DMA2D_ENABLE(&hdma2d);
    while (__HAL_DMA2D_GET_FLAG(&hdma2d, DMA2D_FLAG_TC) == RESET) {}
    __HAL_DMA2D_CLEAR_FLAG(&hdma2d, DMA2D_FLAG_TC);
}*/

/*void DMA2D_DrawPixel(uint32_t destAddr,
                     uint16_t x, uint16_t y,
                     uint16_t color)
{
    uint32_t addr = destAddr + ((uint32_t)y * LCD_GetWidth() + x) * 2U;
    *(volatile uint16_t *)addr = color;
}*/
//****************************************************************************
/*void DMA2D_DrawPixel(uint32_t destAddr,
                     uint16_t x, uint16_t y,
                     uint32_t color)   // now uint32_t
{
    uint32_t addr = destAddr + ((uint32_t)y * LCD_GetWidth() + x) * 2U;

    uint8_t *p = (uint8_t *)addr;
    p[0] = (color >> 16) & 0xFF; // R
    p[1] = (color >> 8)  & 0xFF; // G
    p[2] =  color        & 0xFF; // B



    //*(volatile uint32_t *)(addr & ~0x3) = color; // careful: 3-byte pixels, align to 32-bit
}*/


//*****************************************************************************
/*void DMA2D_CopyBuffer(uint32_t srcAddr,
                      uint32_t dstAddr,
                      uint16_t width,
                      uint16_t height)
{
    hdma2d.Instance->CR    = DMA2D_M2M;
    hdma2d.Instance->FGMAR = srcAddr;
    hdma2d.Instance->OMAR  = dstAddr;

    hdma2d.Instance->FGOR  = 0;

    // stride = LTDC layer width - copy width
    uint32_t stride = hltdc.LayerCfg[0].ImageWidth - width;
    hdma2d.Instance->OOR   = stride;

    hdma2d.Instance->NLR   = ((uint32_t)width << 16) | height;

    __HAL_DMA2D_ENABLE(&hdma2d);
    while (__HAL_DMA2D_GET_FLAG(&hdma2d, DMA2D_FLAG_TC) == RESET) {}
    __HAL_DMA2D_CLEAR_FLAG(&hdma2d, DMA2D_FLAG_TC);
}*/




//********************************************
//Blend a glyph alpha mask into the framebuffer
/*
void DMA2D_AlphaBlendGlyph(uint32_t glyphMaskAddr,
                           uint32_t destAddr,
                           uint16_t destWidth,
                           uint16_t x, uint16_t y,
                           uint16_t w, uint16_t h,
                           uint16_t textColor)
{
    uint32_t dst    = destAddr + ((uint32_t)y * hltdc.LayerCfg[0].ImageWidth + x) * 2U;
    uint32_t stride = hltdc.LayerCfg[0].ImageWidth - w;

    hdma2d.Instance->CR = DMA2D_M2M_BLEND;

     Foreground: A8 alpha mask
    hdma2d.Instance->FGMAR   = glyphMaskAddr;
    hdma2d.Instance->FGOR    = 0;
    hdma2d.Instance->FGPFCCR = (0x09 << 0);  // A8

     Background: RGB565 framebuffer
    hdma2d.Instance->BGMAR   = dst;
    hdma2d.Instance->BGOR    = stride;
    hdma2d.Instance->BGPFCCR = (0x02 << 0);  // RGB565

     Output: in-place RGB565
    hdma2d.Instance->OMAR    = dst;
    hdma2d.Instance->OOR     = stride;
    hdma2d.Instance->OPFCCR  = (0x02 << 0);  // RGB565

     Text color as RGB888
    hdma2d.Instance->FGCOLR =
        (((textColor >> 11) & 0x1F) << 19) |   R
        (((textColor >> 5)  & 0x3F) << 10) |   G
        (( textColor        & 0x1F) << 3);     B

    hdma2d.Instance->NLR = ((uint32_t)w << 16) | h;

    __HAL_DMA2D_ENABLE(&hdma2d);
    while (__HAL_DMA2D_GET_FLAG(&hdma2d, DMA2D_FLAG_TC) == RESET) {}
    __HAL_DMA2D_CLEAR_FLAG(&hdma2d, DMA2D_FLAG_TC);
}



//************************************************************


void DMA2D_DrawCircle(uint32_t destAddr,
                      uint16_t x0, uint16_t y0,
                      uint16_t r,
                      uint16_t color)
{
    int16_t x = 0;
    int16_t y = r;
    int16_t d = 3 - 2 * r;

    while (y >= x)
    {
        DMA2D_DrawPixel(destAddr, x0 + x, y0 + y, color);
        DMA2D_DrawPixel(destAddr, x0 - x, y0 + y, color);
        DMA2D_DrawPixel(destAddr, x0 + x, y0 - y, color);
        DMA2D_DrawPixel(destAddr, x0 - x, y0 - y, color);
        DMA2D_DrawPixel(destAddr, x0 + y, y0 + x, color);
        DMA2D_DrawPixel(destAddr, x0 - y, y0 + x, color);
        DMA2D_DrawPixel(destAddr, x0 + y, y0 - x, color);
        DMA2D_DrawPixel(destAddr, x0 - y, y0 - x, color);

        x++;

        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
        {
            d = d + 4 * x + 6;
        }
    }
}

void DMA2D_FillCircle(uint32_t destAddr,
                      uint16_t x0, uint16_t y0,
                      uint16_t r,
                      uint16_t color)
{
    int16_t x = 0;
    int16_t y = r;
    int16_t d = 3 - 2 * r;

    while (y >= x)
    {
        // Draw horizontal spans using DMA2D_FillRect
        DMA2D_FillRect(destAddr, x0 - x, y0 - y, 2 * x + 1, 1, color);
        DMA2D_FillRect(destAddr, x0 - x, y0 + y, 2 * x + 1, 1, color);
        DMA2D_FillRect(destAddr, x0 - y, y0 - x, 2 * y + 1, 1, color);
        DMA2D_FillRect(destAddr, x0 - y, y0 + x, 2 * y + 1, 1, color);

        x++;

        if (d > 0)
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else
        {
            d = d + 4 * x + 6;
        }
    }
}


*/




/*void DMA2D_CopyBuffer(uint32_t srcAddr,
                      uint32_t dstAddr,
                      uint16_t width,
                      uint16_t height)
{
    hdma2d.Instance->CR    = DMA2D_M2M;
    hdma2d.Instance->FGMAR = srcAddr;
    hdma2d.Instance->OMAR  = dstAddr;
    hdma2d.Instance->FGOR  = 0;
    hdma2d.Instance->OOR   = 0;
    hdma2d.Instance->NLR   = ((uint32_t)width << 16) | height;

    __HAL_DMA2D_ENABLE(&hdma2d);
    while (__HAL_DMA2D_GET_FLAG(&hdma2d, DMA2D_FLAG_TC) == RESET) {}
    __HAL_DMA2D_CLEAR_FLAG(&hdma2d, DMA2D_FLAG_TC);
}*/
//****************************************************************






/*
void DMA2D_AlphaBlendGlyph(uint32_t glyphMaskAddr,
                           uint32_t destAddr,
						   uint16_t destWidth,
                           uint16_t x, uint16_t y,
                           uint16_t w, uint16_t h,
                           uint16_t textColor)   // RGB888
{
    uint32_t dst    = destAddr + ((uint32_t)y * hltdc.LayerCfg[0].ImageWidth + x) * 3U;
    uint32_t stride = hltdc.LayerCfg[0].ImageWidth - w;

    hdma2d.Instance->CR = DMA2D_M2M_BLEND;

    // Foreground: A8 alpha mask
    hdma2d.Instance->FGMAR   = glyphMaskAddr;
    hdma2d.Instance->FGOR    = 0;
    hdma2d.Instance->FGPFCCR = DMA2D_INPUT_A8;

    // Background: RGB888 framebuffer
    hdma2d.Instance->BGMAR   = dst;
    hdma2d.Instance->BGOR    = stride;
    hdma2d.Instance->BGPFCCR = DMA2D_INPUT_RGB888;

    // Output: in-place RGB888
    hdma2d.Instance->OMAR    = dst;
    hdma2d.Instance->OOR     = stride;
    hdma2d.Instance->OPFCCR  = DMA2D_OUTPUT_RGB888;

    // Text color in RGB888
    hdma2d.Instance->FGCOLR = textColor;

    hdma2d.Instance->NLR = ((uint32_t)w << 16) | h;

    __HAL_DMA2D_ENABLE(&hdma2d);
    while (__HAL_DMA2D_GET_FLAG(&hdma2d, DMA2D_FLAG_TC) == RESET) {}
    __HAL_DMA2D_CLEAR_FLAG(&hdma2d, DMA2D_FLAG_TC);
}

*/






























/*
#include "text_engine.h"
#include "text_layer.h"
#include "dma2d_driver.h"
#include "lcd_driver.h"
#include <stdio.h>

static uint32_t textLayerAddr = 0;
static uint16_t cursorX = 0;
static uint16_t cursorY = 0;
static uint16_t textColor = 0xFFFF;   // white
static uint16_t backColor = 0x0000;   // black

void TEXT_Init(void)
{
    textLayerAddr = TEXT_LAYER_GetBufferAddress();
}

void TEXT_SetCursor(uint16_t x, uint16_t y)
{
    cursorX = x;
    cursorY = y;
}

void TEXT_SetTextColor(uint32_t fg, uint16_t bg)
{
    textColor = fg;
    backColor = bg;
}

void TEXT_PutChar(const Font_t *font, char c)
{
    if (c < font->first || c > font->last)
        c = '?';

    const Glyph_t *g = &font->glyphs[c - font->first];

     IMPORTANT: Clear the ENTIRE character cell, not just glyph area
    DMA2D_FillRect(textLayerAddr,
                   cursorX,                     Start at cursor
                   cursorY,                    Start at cursor Y
                   g->xAdvance,               Full character width
                   font->yAdvance,            Full line height
                   backColor);                Background color

     Render the glyph
    FONT_RenderGlyph(font,
                     c,
                     textLayerAddr,
					 LCD_GetWidth(),  // Add this back!
                     cursorX,
                     cursorY,
                     textColor,
                     backColor);

     Advance cursor
    cursorX += g->xAdvance;
}

void TEXT_Print(const Font_t *font, const char *s)
{
    while (*s)
        TEXT_PutChar(font, *s++);
}

void TEXT_PrintNumber(const Font_t *font, int32_t num)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%ld", (long)num);
    TEXT_Print(font, buf);
}
*/


/*


          void LCD_RandomPitchDemo(void)
              {
                  int pitch;
                  char buf[16];
                  int absPitch;

                  srand(HAL_GetTick());

                  while (1)
                  {
                      pitch = (rand() % 71) - 35;
                      absPitch = abs(pitch);

                      // Clear area
                      TEXT_LAYER_ClearArea(365, 135, 165, 83, COLOR_BLACK);
                      //for (volatile int i = 0; i < 100; i++);
                      // Test with explicit sign positioning
                      if (pitch < 0)
                      {
                          // Minus sign in RED at position 390,160
                          TEXT_SetTextColor(COLOR_RED, COLOR_BLACK);
                          TEXT_SetCursor(395, 160);
                          TEXT_Print(&Arial56, "-");

                          // Number in RED at position 435,160
                          snprintf(buf, sizeof(buf), "%02d", absPitch);
                          TEXT_SetCursor(435, 160);
                          TEXT_Print(&Arial56, buf);
                      }
                      else
                      {
                          // Positive number in GREEN at position 435,160
                          TEXT_SetTextColor(COLOR_GREEN, COLOR_BLACK);
                          snprintf(buf, sizeof(buf), "%02d", pitch);
                          TEXT_SetCursor(435, 160);
                          TEXT_Print(&Arial56, buf);
                      }

                      HAL_Delay(500);
                  }
              }


*/











