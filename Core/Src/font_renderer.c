#include "font_renderer.h"
#include "dma2d_driver.h"
#include "lcd_driver.h"



//*****************************good**************************


void FONT_RenderGlyph(const Font_t *font,
                      char c,
                      uint32_t destAddr,
                      int16_t cursorX,
                      int16_t cursorY,
                      uint32_t textColor,
                      uint32_t backColor)
{
    if (!font) return;
    if (c < font->first || c > font->last) c = '?';

    const Glyph_t *g = &font->glyphs[c - font->first];
    if (g->width == 0 || g->height == 0) return;

    int16_t drawX = cursorX + g->xOffset;
    int16_t drawY = cursorY + g->yOffset;

    // 1. Fill background rectangle (Bodmer style)
    DMA2D_FillRect(destAddr,
                   drawX,
                   drawY,
                   g->width,
                   g->height,
                   backColor);

    // 2. Blend glyph alpha mask on top
    uint32_t glyphAddr = (uint32_t)(font->bitmap + g->bitmapOffset);

    DMA2D_AlphaBlendGlyph(glyphAddr,
                          destAddr,
                          drawX,
                          drawY,
                          g->width,
                          g->height,
                          textColor);
}

void FONT_ComputeCellSize(Font_t *font)
{
	uint8_t maxWidth = 0; uint8_t maxHeight = 0;
	for (uint8_t c = font->first; c <= font->last; c++) { const Glyph_t *g = &font->glyphs[c - font->first];
	if (g->width > maxWidth) maxWidth = g->width;
	if (g->height > maxHeight) maxHeight = g->height; }
	font->cellWidth = maxWidth + 2;
  font->cellHeight = maxHeight + 2; // margin }
}






