import freetype
import textwrap

# ---------------------------------------------------------
# CONFIGURATION
# ---------------------------------------------------------

FONT_PATH = "C:/Windows/Fonts/arialbd.ttf"   # Path to Arial.ttf
FONT_SIZE = 42                             # Output font size
FIRST_CHAR = 32                            # space
LAST_CHAR  = 126                           # '~'
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
# DETERMINE TABULAR WIDTH
# ---------------------------------------------------------
# Measure widest digit (0–9) to use as tabular width
max_digit_width = 0
for d in range(ord('0'), ord('9') + 1):
    face.load_char(chr(d), freetype.FT_LOAD_RENDER | freetype.FT_LOAD_TARGET_NORMAL)
    max_digit_width = max(max_digit_width, face.glyph.bitmap.width)

tabular_width = max_digit_width

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

    buf = bitmap.buffer

    # -----------------------------------------------------
    # Apply tabular width for digits only
    # -----------------------------------------------------
    if '0' <= ch <= '9':
        # Pad each row to tabular_width
        padded = bytearray()
        for row in range(height):
            start = row * width
            padded.extend(buf[start:start+width])       # copy original pixels
            padded.extend(b'\x00' * (tabular_width - width))  # pad with zeros
        bitmaps.extend(padded)

        glyphs.append({
            "width": tabular_width,
            "height": height,
            "xOffset": 0,
            "yOffset": y_offset,
            "xAdvance": tabular_width,
            "bitmapOffset": bitmap_offset
        })
        bitmap_offset += tabular_width * height

    else:
        # Non-digit glyphs unchanged
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
