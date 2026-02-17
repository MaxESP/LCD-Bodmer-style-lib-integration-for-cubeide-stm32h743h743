

#ifndef __TEXT_LAYER_H__
#define __TEXT_LAYER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/*============================================================================
 * Initialization
 *============================================================================*/
void TEXT_LAYER_Init(uint16_t width, uint16_t height);
void TEXT_LAYER_Clear(uint32_t destAddr, uint32_t color);  /* RGB888 color */
void TEXT_LAYER_ClearArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color);
/*============================================================================
 * Buffer Management
 *============================================================================*/
uint32_t TEXT_LAYER_GetBufferAddress(void);

#ifdef __cplusplus
}
#endif

#endif /* __TEXT_LAYER_H__ */
























/*
#ifndef __TEXT_LAYER_H__
#define __TEXT_LAYER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

 Initialize the text layer (allocate SDRAM region, set width/height)
void TEXT_LAYER_Init(uint16_t width, uint16_t height);

 Clear the entire text layer to a background color
void TEXT_LAYER_Clear(uint32_t destAddr, uint16_t color);

 Return the base address of the text layer buffer in SDRAM
uint32_t TEXT_LAYER_GetBufferAddress(void);

 Copy the text layer into the main LTDC framebuffer
void TEXT_LAYER_BlitToScreen(void);

 Change background color (does not clear immediately)
void TEXT_LAYER_SetBackgroundColor(uint16_t color);

#ifdef __cplusplus
}
#endif

#endif  __TEXT_LAYER_H__
*/
