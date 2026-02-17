
#ifndef __TEXT_ENGINE_H__
#define __TEXT_ENGINE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "font_renderer.h"   // for Font_t

/*============================================================================
 * RGB888 Color Definitions - 24-bit (0xRRGGBB)
 *============================================================================*/

/*============================================================================
 * RGB888 Color Definitions - 24-bit (0xRRGGBB)
 *============================================================================*/
#define COLOR_BLACK        0x000000
#define COLOR_WHITE        0xFFFFFF
#define COLOR_RED          0xFF0000
#define COLOR_GREEN        0x00FF00
#define COLOR_BLUE         0x0000FF
#define COLOR_YELLOW       0xFFFF00
#define COLOR_CYAN         0x00FFFF
#define COLOR_MAGENTA      0xFF00FF
#define COLOR_ORANGE       0xFFA500
#define COLOR_GRAY         0x808080
#define COLOR_LIGHTGRAY    0xD3D3D3
#define COLOR_DARKGRAY     0x404040
#define COLOR_NAVY         0x000080
#define COLOR_DARKGREEN    0x006400
#define COLOR_TEAL         0x008080
#define COLOR_PURPLE       0x800080
#define COLOR_PINK         0xFF69B4
#define COLOR_BROWN        0xA52A2A
#define COLOR_GOLD         0xFFD700
#define COLOR_SILVER       0xC0C0C0
#define COLOR_SKYBLUE      0x87CEEB
#define COLOR_VIOLET       0xEE82EE
#define COLOR_BEIGE        0xF5F5DC
#define COLOR_TURQUOISE    0x40E0D0


/*============================================================================
 * Initialization
 *============================================================================*/
void TEXT_Init(void);
void TEXT_SetCursor(uint16_t x, uint16_t y);
void TEXT_SetTextColor(uint32_t fg, uint16_t bg);
void TEXT_PutChar( Font_t *font, char c);
void TEXT_Print( Font_t *font, const char *s);
void TEXT_PrintNumber( Font_t *font, int32_t num);

#ifdef __cplusplus
}
#endif

#endif /* __TEXT_ENGINE_H__ */






























































/*
#ifndef __TEXT_ENGINE_H__
#define __TEXT_ENGINE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "font_renderer.h"   // for Font_t


typedef enum { COLOR_BLACK = 0x0000,
	COLOR_WHITE = 0xFFFF,
	COLOR_RED = 0xF800,
	COLOR_GREEN = 0x07E0,
	COLOR_BLUE = 0x001F,
	COLOR_YELLOW = 0xFFE0,
	COLOR_CYAN = 0x07FF,
	COLOR_MAGENTA = 0xF81F,
	COLOR_ORANGE = 0xFD20,
	COLOR_GRAY = 0x8410,
	COLOR_LIGHTGRAY = 0xC618,
	COLOR_DARKGRAY = 0x4208,
	COLOR_NAVY = 0x000F,
	COLOR_DARKGREEN = 0x03E0,
	COLOR_TEAL = 0x0410,
	COLOR_PURPLE = 0x780F,
	COLOR_PINK = 0xF81F,
	COLOR_BROWN = 0xA145,
	COLOR_GOLD = 0xFEA0,
	COLOR_SILVER = 0xC618,
	COLOR_SKYBLUE = 0x867D,
	COLOR_VIOLET = 0xEC1D,
	COLOR_BEIGE = 0xF7BB,
	COLOR_TURQUOISE = 0x471A
} TextColor_t;







 Initialization
void TEXT_Init(void);

 Cursor and color
void TEXT_SetCursor(uint16_t x, uint16_t y);
void TEXT_SetTextColor(uint32_t fg, uint16_t bg);

 Print functions with font parameter
void TEXT_PutChar(const Font_t *font, char c);
void TEXT_Print(const Font_t *font, const char *s);

 Optional helper for numbers
void TEXT_PrintNumber(const Font_t *font, int32_t num);

#ifdef __cplusplus
}
#endif

#endif  __TEXT_ENGINE_H__

*/














































#ifndef __TEXT_ENGINE_H__
#define __TEXT_ENGINE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "dma2d_driver.h"

/* Simple 8x16 font (monospace) */
#define FONT_WIDTH   8
#define FONT_HEIGHT  16




void TEXT_Init(void);

void TEXT_SetCursor(uint16_t x, uint16_t y);

void TEXT_SetTextColor(uint16_t fg, uint16_t bg);

void TEXT_SetFont(const Font_t *font);

void TEXT_PutChar(char c);

void TEXT_Print(const char *s);

#ifdef __cplusplus
}
#endif

#endif








