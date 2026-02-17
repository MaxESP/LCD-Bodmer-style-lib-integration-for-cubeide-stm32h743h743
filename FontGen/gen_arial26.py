
import freetype
import textwrap

# ---------------------------------------------------------
# CONFIGURATION
# ---------------------------------------------------------

# Path to Arial.ttf on your system
FONT_PATH = "C:/Windows/Fonts/arial.ttf"

# Change this to generate any size you want
FONT_SIZE = 26

FIRST_CHAR = 32      # space
LAST_CHAR  = 126     # '~'

OUTPUT_FILE = f"font_arial{FONT_SIZE}.c"

# ---------------------------------------------------------
# INITIALIZE FREETYPE
# ---------------------------------------------------------

face = freetype.Face(FONT_PATH)
face.set_char_size(FONT_SIZE * 64)

bitmaps = bytearray()
glyphs  = []

bitmap_offset = 0

# ---------------------------------------------------------
# PROCESS EACH CHARACTER
# ---------------------------------------------------------

for char_code in range(FIRST_CHAR, LAST_CHAR + 1):
    ch = chr(char_code)

    face.load_char(ch, freetype.FT_LOAD_RENDER | freetype.FT_LOAD_TARGET_NORMAL)
    glyph = face.glyph
    bitmap = glyph.bitmap

    width  = bitmap.width
    height = bitmap.rows

    # Handle empty glyphs (space, etc.)
    if width == 0 or height == 0:
        glyphs.append({
            "width": 0,
            "height": 0,
            "xOffset": 0,
            "yOffset": 0,
            "xAdvance": glyph.advance.x // 64,
            "bitmapOffset": bitmap_offset
        })
        continue

    # Metrics
    x_offset   = glyph.bitmap_left
    y_offset   = -glyph.bitmap_top
    x_advance  = glyph.advance.x // 64

    # Copy L8 bitmap
    buf = bitmap.buffer
    bitmaps.extend(buf)

    glyphs.append({
        "width": width,
        "height": height,
        "xOffset": x_offset,
        "yOffset": y_offset,
        "xAdvance": x_advance,
        "bitmapOffset": bitmap_offset
    })

    bitmap_offset += width * height

# ---------------------------------------------------------
# LINE HEIGHT
# ---------------------------------------------------------

y_advance = (face.size.ascender - face.size.descender) // 64

# ---------------------------------------------------------
# FORMAT C OUTPUT
# ---------------------------------------------------------

def format_bytes(data, indent="    ", per_line=16):
    lines = []
    for i in range(0, len(data), per_line):
        chunk = data[i:i+per_line]
        line = ", ".join(f"0x{b:02X}" for b in chunk)
        lines.append(indent + line + ",")
    return "\n".join(lines)

def format_glyphs(glyphs, indent="    "):
    lines = []
    for g in glyphs:
        lines.append(indent + "{")
        lines.append(indent + f"    .width        = {g['width']},")
        lines.append(indent + f"    .height       = {g['height']},")
        lines.append(indent + f"    .xOffset      = {g['xOffset']},")
        lines.append(indent + f"    .yOffset      = {g['yOffset']},")
        lines.append(indent + f"    .xAdvance     = {g['xAdvance']},")
        lines.append(indent + f"    .bitmapOffset = {g['bitmapOffset']}")
        lines.append(indent + "},")
    return "\n".join(lines)

c_bitmap = format_bytes(bitmaps)
c_glyphs = format_glyphs(glyphs)

c_source = f"""
#include "font_renderer.h"

static const uint8_t Arial{FONT_SIZE}Bitmaps[] =
{{
{c_bitmap}
}};

static const Glyph_t Arial{FONT_SIZE}Glyphs[] =
{{
{c_glyphs}
}};

const Font_t Arial{FONT_SIZE} =
{{
    .bitmap   = Arial{FONT_SIZE}Bitmaps,
    .glyphs   = Arial{FONT_SIZE}Glyphs,
    .first    = {FIRST_CHAR},
    .last     = {LAST_CHAR},
    .yAdvance = {y_advance}
}};
"""

with open(OUTPUT_FILE, "w", encoding="utf-8") as f:
    f.write(textwrap.dedent(c_source))

print(f"Generated {OUTPUT_FILE} successfully.")










































































# #include "text_engine.h"
# #include "text_layer.h"
# #include "dma2d_driver.h"

# static uint32_t textLayerAddr = 0;

# static uint16_t cursorX = 0;
# static uint16_t cursorY = 0;

# static uint16_t textColor = 0xFFFF;   // white
# static uint16_t backColor = 0x0000;   // black

# void TEXT_Init(void)
# {
    # textLayerAddr = TEXT_LAYER_GetBufferAddress();
# }

# void TEXT_SetCursor(uint16_t x, uint16_t y)
# {
    # cursorX = x;
    # cursorY = y;
# }

# void TEXT_SetTextColor(uint16_t fg, uint16_t bg)
# {
    # textColor = fg;
    # backColor = bg;
# }

# void TEXT_PutChar(char c)
# {
    # if (c < 32 || c > 126)
        # c = '?';

    # uint16_t w = FONT_WIDTH;
    # uint16_t h = FONT_HEIGHT;

    # const uint8_t *glyph = font8x16[c - 32];

    # for (uint16_t row = 0; row < h; row++)
    # {
        # uint8_t bits = glyph[row];

        # for (uint16_t col = 0; col < w; col++)
        # {
            # uint16_t color = (bits & (1 << (7 - col))) ? textColor : backColor;

            # DMA2D_DrawPixel(textLayerAddr,
                            # cursorX + col,
                            # cursorY + row,
                            # color);
        # }
    # }

    # cursorX += w;
# }

# void TEXT_Print(const char *s)
# {
    # while (*s)
        # TEXT_PutChar(*s++);
# }
