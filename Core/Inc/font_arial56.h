#ifndef __ARIAL56_H__
#define __ARIAL56_H__

#include <stdint.h>
#include "font_renderer.h"   // for Font_t and Glyph_t definitions

// Bitmap data for Arial 26 (A8 alpha mask)
extern const uint8_t Arial56Bitmaps[];

// Glyph metadata (width, height, offsets, advance, bitmapOffset)
extern const Glyph_t Arial56Glyphs[];

// Font structure tying it all together
extern  Font_t Arial56;

#endif /* __ARIAL26_H__ */
