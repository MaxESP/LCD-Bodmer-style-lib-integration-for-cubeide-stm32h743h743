#ifndef __ARIAL42_H__
#define __ARIAL42_H__

#include <stdint.h>
#include "font_renderer.h"   // for Font_t and Glyph_t definitions

// Bitmap data for Arial 26 (A8 alpha mask)
extern const uint8_t Arial42Bitmaps[];

// Glyph metadata (width, height, offsets, advance, bitmapOffset)
extern const Glyph_t Arial42Glyphs[];

// Font structure tying it all together
extern  Font_t Arial42;

#endif /* __ARIAL26_H__ */
