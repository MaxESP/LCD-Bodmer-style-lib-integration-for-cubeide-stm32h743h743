


#ifndef FONT_RENDERER_H
#define FONT_RENDERER_H

#include <stdint.h>

typedef struct {
	uint32_t bitmapOffset;
    uint8_t  width;
    uint8_t  height;
    int8_t   xOffset;
    int8_t   yOffset;
    int8_t   xAdvance;
} Glyph_t;

typedef struct {
    const uint8_t *bitmap;
    const Glyph_t *glyphs;
    uint8_t first;
    uint8_t last;
    uint8_t cellWidth;   // fixed 32
    uint8_t cellHeight;  // fixed 32
    int8_t   yAdvance;
} Font_t;

void FONT_RenderGlyph(const Font_t *font,
                      char c,
                      uint32_t destAddr,
                      int16_t cursorX,
                      int16_t cursorY,
                      uint32_t textColor,
                      uint32_t backColor);


void FONT_ComputeCellSize(Font_t *font);



#endif /* FONT_RENDERER_H */

