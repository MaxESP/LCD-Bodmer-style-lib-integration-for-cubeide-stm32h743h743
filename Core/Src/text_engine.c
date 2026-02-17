
#include "text_engine.h"
#include "text_layer.h"
#include "dma2d_driver.h"
#include "lcd_driver.h"
#include "font_arialbd36.h"
#include "font_renderer.h"
#include <stdio.h>
//#include "font_arial56.h"



static uint32_t textLayerAddr = 0;
static uint16_t cursorX = 0;
static uint16_t cursorY = 0;
static uint32_t textColor = COLOR_WHITE;   // Now uint32_t for RGB888
static uint32_t backColor = COLOR_BLACK;   // Now uint32_t for RGB888

void TEXT_Init(void)
{
    textLayerAddr = TEXT_LAYER_GetBufferAddress();
   // FONT_ComputeCellSize(&Arialbd36);
    //FONT_ComputeCellSize(&Arial56);
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
//*******************************************************************

void TEXT_PutChar(Font_t *font, char c)
{
    if (c < font->first || c > font->last)
        c = '?';

    const Glyph_t *g = &font->glyphs[c - font->first];

    FONT_RenderGlyph(font,
                     c,
                     textLayerAddr,
                     cursorX,
                     cursorY,
                     textColor,
                     backColor);

    cursorX += g->xAdvance;   // proportional spacing
}

void TEXT_Print( Font_t *font, const char *s)
{
    while (*s)
        TEXT_PutChar(font, *s++);
}

void TEXT_PrintNumber( Font_t *font, int32_t num)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%ld", (long)num);
    TEXT_Print(font, buf);
}
































































/*
#include "text_engine.h"
#include "text_layer.h"
#include "dma2d_driver.h"
#include "lcd_driver.h"
#include "font_arial26.h"
#include "font_arial36.h"


static uint32_t textLayerAddr = 0;
static uint16_t cursorX = 0;
static uint16_t cursorY = 0;

static uint16_t textColor = 0xFFFF;   // white
static uint16_t backColor = 0x0000;   // black

//extern const Font_t Arial36;

static const Font_t *activeFont=&Arial26;



void TEXT_SetFont(const Font_t *font){


	activeFont = font;
}




void TEXT_Init(void)
{
    textLayerAddr = TEXT_LAYER_GetBufferAddress();
}

void TEXT_SetCursor(uint16_t x, uint16_t y)
{
    cursorX = x;
    cursorY = y;
}

void TEXT_PutChar(char c)
{
	if (c < activeFont->first || c > activeFont->last)
		c = '?';
	FONT_RenderGlyph(activeFont,
			c,
			textLayerAddr,
			LCD_GetWidth(),
			cursorX,
			cursorY,
			textColor,
			backColor);
	const Glyph_t *g = &activeFont->glyphs[c - activeFont->first];
	cursorX += g->xAdvance;
}


void TEXT_Print(const Font_t *font, const char *s)
{
    while (*s)
        TEXT_PutChar(font, *s++);
}
*/
