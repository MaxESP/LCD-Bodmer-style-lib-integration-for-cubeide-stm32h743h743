/*
#ifndef __DMA2D_DRIVER_H__
#define __DMA2D_DRIVER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"
#include "ltdc.h"
#include "lcd_driver.h"

extern DMA2D_HandleTypeDef hdma2d;

 Initialize DMA2D for RGB888 operations
void DMA2D_Driver_Init(void);

 Fill rectangle with solid RGB888 color
void DMA2D_FillRect(uint32_t destAddr, uint16_t x, uint16_t y,
                    uint16_t w, uint16_t h, uint32_t color);

 Draw single pixel in RGB888 format
void DMA2D_DrawPixel(uint32_t destAddr, uint16_t x, uint16_t y, uint32_t color);

 Copy buffer to buffer (M2M)
void DMA2D_CopyBuffer(uint32_t srcAddr, uint32_t dstAddr,
                      uint16_t width, uint16_t height);

 Alpha blend glyph (A8 mask) onto RGB888 framebuffer
void DMA2D_AlphaBlendGlyph(uint32_t glyphMaskAddr, uint32_t destAddr,
                          uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                          uint32_t textColor);   ← Change to uint32_t

 Draw circle outline
void DMA2D_DrawCircle(uint32_t destAddr, uint16_t x0, uint16_t y0,
                      uint16_t r, uint32_t color);

 Fill solid circle
void DMA2D_FillCircle(uint32_t destAddr, uint16_t x0, uint16_t y0,
                      uint16_t r, uint32_t color);






#ifdef __cplusplus
}
#endif

#endif  __DMA2D_DRIVER_H__

*/


//****************************************************************************



/**
 * @file dma2d_driver.h
 * @brief DMA2D hardware acceleration for STM32H7
 */

#ifndef __DMA2D_DRIVER_H__
#define __DMA2D_DRIVER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*============================================================================
 * Initialization
 *============================================================================*/
void DMA2D_Driver_Init(void);

/*============================================================================
 * Basic Drawing Operations
 *============================================================================*/
void DMA2D_FillRect(uint32_t destAddr,
                    uint16_t x, uint16_t y,
                    uint16_t w, uint16_t h,
                    uint32_t color);

void DMA2D_DrawPixel(uint32_t destAddr,
                     uint16_t x, uint16_t y,
                     uint32_t color);

void DMA2D_CopyBuffer(uint32_t srcAddr,
                      uint32_t dstAddr,
                      uint16_t width,
                      uint16_t height);

/*============================================================================
 * Advanced Drawing Operations
 *============================================================================*/
void DMA2D_AlphaBlendGlyph(uint32_t glyphMaskAddr,
                           uint32_t destAddr,
                           uint16_t x, uint16_t y,
                           uint16_t w, uint16_t h,
                           uint32_t textColor);

void DMA2D_DrawCircle(uint32_t destAddr,
                      uint16_t x0, uint16_t y0,
                      uint16_t r,
                      uint32_t color);

void DMA2D_FillCircle(uint32_t destAddr,
                      uint16_t x0, uint16_t y0,
                      uint16_t r,
                      uint32_t color);

/*============================================================================
 * Utility Functions
 *============================================================================*/
uint32_t DMA2D_IsBusy(void);
void DMA2D_Abort(void);

#ifdef __cplusplus
}
#endif

#endif /* __DMA2D_DRIVER_H__ */















































/*
#ifndef __DMA2D_DRIVER_H__
#define __DMA2D_DRIVER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"
#include "ltdc.h"
#include "lcd_driver.h"



extern DMA2D_HandleTypeDef hdma2d;

void DMA2D_Driver_Init(void);

 Fill a rectangle in the framebuffer with a solid RGB888 color
void DMA2D_FillRect(uint32_t destAddr,uint16_t x, uint16_t y, uint16_t w, uint16_t h,uint16_t color);

 Draw a single pixel (RGB565)
void DMA2D_DrawPixel(uint32_t destAddr,uint16_t x, uint16_t y,uint32_t color);

void DMA2D_AlphaBlendGlyph(uint32_t glyphMaskAddr, uint32_t destAddr, uint16_t destWidth, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t textColor);

void DMA2D_DrawCircle(uint32_t destAddr, uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);

void DMA2D_FillCircle(uint32_t destAddr, uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);





#ifdef __cplusplus
}
#endif

#endif  __DMA2D_DRIVER_H__
*/
