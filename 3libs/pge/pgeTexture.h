/*
 * pgeTexture.h: Header for handling textures
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

#ifndef __PGETEXTURE_H__
#define __PGETEXTURE_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup pgeTexture Texture Library
 *  @{
 */

enum pgePixelFormat
{
	PGE_PIXEL_FORMAT_5650,	/**< Pixelformat R5:G6:B5:A0. */
	PGE_PIXEL_FORMAT_5551,	/**< Pixelformat R5:G5:B5:A1. */
	PGE_PIXEL_FORMAT_4444,	/**< Pixelformat R4:G4:B4:A4. */
	PGE_PIXEL_FORMAT_8888,	/**< Pixelformat R8:G8:B8:A8. */
	PGE_PIXEL_FORMAT_T4,	/**< Pixelformat 4bit indexed. */
	PGE_PIXEL_FORMAT_T8,	/**< Pixelformat 8bit indexed. */
	PGE_PIXEL_FORMAT_T16,	/**< Pixelformat 16bit indexed. */
	PGE_PIXEL_FORMAT_T32	/**< Pixelformat 32bit indexed. */
};

enum pgeTextureMode
{
	PGE_TEX_MODE_MODULATE,
	PGE_TEX_MODE_DECAL,
	PGE_TEX_MODE_BLEND,
	PGE_TEX_MODE_REPLACE,
	PGE_TEX_MODE_ADD
};

enum pgeMemoryLocation
{
	PGE_RAM,
	PGE_VRAM
};

/**
 * A texture struct
 */
typedef struct pgeTexture
{
	void					*palette;		/**< Image palette. */
	enum pgePixelFormat		palFormat;		/**< Palette format - one of PGE_PIXEL_FORMAT_5650, PGE_PIXEL_FORMAT_5551, PGE_PIXEL_FORMAT_4444, PGE_PIXEL_FORMAT_8888. */
	void					*data;			/**< Image data. */
	unsigned int			size;			/**< Size of data in bytes. */
	unsigned int			width;			/**< Image width. */
	unsigned int			height;			/**< Image height. */
	unsigned int			textureWidth;	/**< Texture width (power of two). */
	unsigned int			textureHeight;	/**< Texture height (power of two). */
	unsigned int			bits;			/**< Image bits per pixel. */
	enum pgePixelFormat		format;			/**< Image format - one of ::pgePixelFormat. */
	char					swizzled;		/**< Is image swizzled. */
	enum pgeMemoryLocation	location;		/**< One of ::pgeMemoryLocation. */
	
} pgeTexture;

/**
 * Load a texture (PNG)
 *
 * @param filename - Path of the file to load.
 *
 * @param location - One of ::pgeMemoryLocation
 *
 * @param swizzle - Set to 1 to swizzle, 0 to not
 *
 * @returns A pointer to a ::pgeTexture struct or NULL on error.
 */
pgeTexture* pgeTextureLoad(const char *filename, enum pgeMemoryLocation location, unsigned char swizzle);

/**
 * Load a PNG texture from memory
 *
 * @param buffer - A pointer to the image data
 *
 * @param size - Size of the image data
 *
 * @param location - One of ::pgeMemoryLocation
 *
 * @param swizzle - Set to 1 to swizzle, 0 to not
 *
 * @returns A pointer to a ::pgeTexture struct or NULL on error.
 */
pgeTexture* pgeTextureLoadMemory(unsigned char *buffer, int size, enum pgeMemoryLocation location, unsigned char swizzle);

/**
 * Destroy a previously created texture
 *
 * @param texture - A valid ::pgeTexture
 */
void pgeTextureDestroy(pgeTexture *texture);

/**
 * Create a texture
 *
 * @param width - Width of the texture
 *
 * @param height - Height of the texture
 *
 * @param format - One of pgePixelFormat
 *
 * @param location - One of ::pgeMemoryLocation
 *
 * @returns A pointer to a ::pgeTexture struct or NULL on error.
 */
pgeTexture* pgeTextureCreate(unsigned int width, unsigned int height, enum pgePixelFormat format, enum pgeMemoryLocation location);

/**
 * Swizzle a texture
 *
 * @param texture - A valid ::pgeTexture
 */
int pgeTextureSwizzle(pgeTexture *texture);

/**
 * Unswizzle a texture
 *
 * @param texture - A valid ::pgeTexture
 */
int pgeTextureUnswizzle(pgeTexture *texture);

/**
 * Get a color value from a texture
 *
 * @param texture - A valid ::pgeTexture
 *
 * @param x - X position
 *
 * @param y - Y position
 */
unsigned int pgeTextureGetPixel(pgeTexture *texture, unsigned int x, unsigned int y);

/**
 * Set a color value in a texture
 *
 * @param texture - A valid ::pgeTexture
 *
 * @param color - The color value to set
 *
 * @param x - X position
 *
 * @param y - Y position
 */
void pgeTextureSetPixel(pgeTexture *texture, unsigned int color, unsigned int x, unsigned int y);

/**
 * Activate a texture, ready for drawing
 *
 * @param texture - A valid ::pgeTexture
 */
void pgeTextureActivate(pgeTexture *texture);

/**
 * Move a texture from VRAM to RAM
 *
 * @param texture - A valid ::pgeTexture
 */
int pgeTextureToRam(pgeTexture *texture);

/**
 * Move a texture from RAM to VRAM
 *
 * @param texture - A valid ::pgeTexture
 */
int pgeTextureToVram(pgeTexture *texture);

/**
 * Set the texture mode
 *
 * @param mode - One of ::pgeTextureMode
 *
 * @returns The previous mode.
 */
int pgeTextureModeSet(enum pgeTextureMode mode);

/**
 * Save a texture to a PNG file
 *
 * @param texture - Texture to convert to PNG
 *
 * @param filename - File path of the PNG
 */
void pgeTextureSave(pgeTexture *texture, const char* filename);

/**
 * Take a screenshot.
 *
 * @param filename - File name to save the screenshot as.
 */
void pgeTextureScreenshot(const char* filename);

/** @} */

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __PGETEXTURE_H__
