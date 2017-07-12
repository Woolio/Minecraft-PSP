/*
 * pgeFont.h: Header for bitmap fonts
 *
 * This file is part of "Phoenix Game Engine".
 *
 * Copyright (C) 2008 Phoenix Game Engine
 * Copyright (C) 2008 InsertWittyName <tias_dp@hotmail.com>
 * Copyright (C) 2008 MK2k <pge@mk2k.net>
 *
 * Phoenix Game Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 
 * Phoenix Game Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 
 * You should have received a copy of the GNU General Public License
 * along with Phoenix Game Engine.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __PGEFONT_H__
#define __PGEFONT_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup pgeFont Font Library
 *  @{
 */

#include <ft2build.h>
#include FT_FREETYPE_H

#include "pgeTexture.h"

/**
 * A Glyph struct
 *
 * @note This is used internally by ::pgeFont and has no other relevance.
 */
typedef struct Glyph
{
	unsigned short x;
	unsigned short y;
	unsigned char width;
	unsigned char height;
	char left;
	char top;
	char advance;
	unsigned char unused;
	
} Glyph;

/**
 * A Font struct
 */
typedef struct
{
	unsigned int texSize;				/**<  Texture size (power2) */
	unsigned int texHeight;				/**<  Texture height (power2) */
	unsigned char *texture;				/**<  The bitmap data */
	int texturememid;					/**<  The uid of the bitmap data */
	enum pgeMemoryLocation location;	/**<  Either in RAM or VRAM */
	unsigned int map[256];				/**<  Character map */
	Glyph glyph[256];					/**<  Character glyphs */
	int fontHeight;
	int memid;
	
} pgeFont;

enum pgeFontSizeType
{
	PGE_FONT_SIZE_PIXELS = 0,
	PGE_FONT_SIZE_POINTS
};

/**
 * Initialise the Font library
 *
 * @returns 1 on success.
 */
int pgeFontInit(void);

/**
 * Shutdown the Font library
 */
void pgeFontShutdown(void);

/**
 * Load a TrueType font.
 *
 * @param filename - Path to the font
 *
 * @param fontsize - Size to set the font to
 *
 * @param fontsizetype - One of ::pgeFontSizeType
 *
 * @param location - One of ::pgeTextureLocation
 *
 * @returns A ::pgeFont struct
 */
pgeFont* pgeFontLoad(const char *filename, unsigned int fontsize, enum pgeFontSizeType fontsizetype, enum pgeMemoryLocation location);

/**
 * Load a TrueType font from memory.
 *
 * @param data - Pointer to the start of the memory buffer.
 *
 * @param datasize - Size of the buffer
 *
 * @param fontsize - Size to set the font to
 *
 * @param fontsizetype - One of ::pgeFontSizeType
 *
 * @param location - One of ::pgeTextureLocation
 *
 * @returns A ::pgeFont struct
 */
pgeFont* pgeFontLoadMemory(unsigned char *data, int datasize, unsigned int fontsize, enum pgeFontSizeType fontsizetype, enum pgeMemoryLocation location);

/**
 * Free the specified font.
 *
 * @param font - A valid ::pgeFont
 */
void pgeFontDestroy(pgeFont *font);

/**
 * Activate the specified font.
 *
 * @param font - A valid ::pgeFont
 */
void pgeFontActivate(pgeFont *font);

/**
 * Draw text along the baseline starting at x, y (with formatting).
 *
 * @param font - A valid ::pgeFont
 *
 * @param x - X position on screen
 *
 * @param y - Y position on screen
 *
 * @param color - Text color
 *
 * @param text - Text to draw
 *
 * @returns The total width of the text drawn.
 */
int pgeFontPrintf(pgeFont *font, short x, short y, unsigned int color, const char *text, ...);

/**
 * Draw centred text along the baseline (with formatting).
 *
 * @param font - A valid ::pgeFont
 *
 * @param x - X position on screen
 *
 * @param y - Y position on screen
 *
 * @param color - Text color
 *
 * @param text - Text to draw
 *
 * @returns The total width of the text drawn.
 */
int pgeFontPrintfCenter(pgeFont *font, short y, unsigned int color, const char *text, ...);

/**
 * Set the charset to be loaded from TTF files.
 *
 * @param charset - The characters to be loaded.
 *
 * @returns 1 on success, otherwise 0.
 */
int pgeFontSetCharset(const char *charset);

/**
 * Reset the charset to be loaded from TTF files.
 */
void pgeFontResetCharset(void);

/**
 * Measure a length of text if it were to be drawn
 *
 * @param font - A valid ::pgeFont
 *
 * @param text - Text to measure
 *
 * @returns The total width of the text.
 */
int pgeFontMeasureText(pgeFont *font, const char *text);

/**
 * Measure a length of formatted text if it were to be drawn
 *
 * @param font - A valid ::pgeFont
 *
 * @param text - Text to measure
 *
 * @returns The total width of the text.
 */
int pgeFontMeasureFormattedText(pgeFont *font, const char *text, ...);

/**
 * Get the maximum height of any glyph
 *
 * @param font - A valid ::pgeFont
 *
 * @returns The maximum height of any glyph in the font.
 */
int pgeFontGetHeight(pgeFont *font);

/** @} */

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __PGEFONT_H__
