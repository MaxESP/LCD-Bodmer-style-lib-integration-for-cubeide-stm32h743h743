#ifndef __ARIALBD36_H__
#define __ARIALBD36_H__

#include <stdint.h>
#include "font_renderer.h"   // for Font_t and Glyph_t definitions

// Bitmap data for Arial 26 (A8 alpha mask)
extern const uint8_t Arial36Bitmaps[];

// Glyph metadata (width, height, offsets, advance, bitmapOffset)
extern const Glyph_t Arial36Glyphs[];

// Font structure tying it all together
extern  Font_t Arialbd36;

#endif /* __ARIAL26_H__ */

