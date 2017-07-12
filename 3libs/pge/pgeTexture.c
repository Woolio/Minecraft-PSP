/*
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

#include <pspkernel.h>
#include <pspgu.h>
#include <stdlib.h>
#include <stdio.h>
#include <png.h>

#include "pgeTexture.h"
#include "pgeVram.h"
#include "pgeGfx.h"
#include "pge.h"

#define RGBA5650(col,r,g,b,a)	{ \
		a=0xFF;\
		b=(((col>>11)&0x1F)*255/31);\
		g=(((col>>5)&0x3F)*255/63);\
		r=((col&0x1F)*255/31);\
		}
#define RGBA5551(col,r,g,b,a)  { \
		a=((col>>15)==0?0:0xFF);\
		b=(((col>>10)&0x1F)*255/31);\
		g=(((col>>5)&0x1F)*255/31);\
		r=((col&0x1F)*255/31);\
		}
#define RGBA4444(col,r,g,b,a)	{ \
		a=(((col>>12)&0xF)*255/15);\
		b=(((col>>8)&0xF)*255/15);\
		g=(((col>>4)&0xF)*255/15);\
		r=((col&0xF)*255/15);\
		}
#define RGBA8888(col,r,g,b,a)	{ \
		a=(col>>24)&0xFF;\
		b=(col>>16)&0xFF;\
		g=(col>>8)&0xFF;\
		r=(col&0xFF);\
		}
		
#define MIN(a,b) ((a)<(b)?(a):(b))
		
static int pgeTextureMode = 3;

static void debugOut(const char *text)
{
	int fd = sceIoOpen("debug.txt", PSP_O_WRONLY|PSP_O_CREAT|PSP_O_APPEND, 0777);
	
	sceIoWrite(fd, text, strlen(text));
	
	sceIoClose(fd);
}


// Return next power of 2
static int pgeTextureNextPow2(unsigned int w)
{
	if(w == 0)
		return 0;

	unsigned int n = 2;

	while(w > n)
		n <<= 1;

	return n;
}

// Return next multiple of 8 (needed for swizzling)
static int pgeTextureNextMul8(unsigned int w)
{
	return((w+7)&~0x7);
}

void pgeTextureDestroy(pgeTexture *texture)
{	
	if(texture == 0)
		return;

	if(texture->data != 0)
	{
		if(texture->location == PGE_VRAM)
			pgeVramFree(texture->data);
		else
			pgeFree(texture->data);
	}

	if(texture->palette != 0)
		pgeFree(texture->palette);

	pgeFree(texture);
}

pgeTexture* pgeTextureCreate(unsigned int width, unsigned int height, enum pgePixelFormat format, enum pgeMemoryLocation location)
{
	pgeTexture* texture = (pgeTexture*) pgeMalloc(sizeof(pgeTexture));

	if(!texture)
		return NULL;

	texture->swizzled = 0;
	texture->width = width;
	texture->height = height;
	texture->textureHeight = pgeTextureNextPow2(height);
	texture->textureWidth = pgeTextureNextPow2(width);
	texture->format = format;
	texture->location = location;
	texture->palette = 0;
	texture->palFormat = 0;

	switch(format)
	{
		case PGE_PIXEL_FORMAT_5650:
		case PGE_PIXEL_FORMAT_5551:
		case PGE_PIXEL_FORMAT_4444:
		case PGE_PIXEL_FORMAT_T16:
			texture->bits = 16;
			break;
			
		case PGE_PIXEL_FORMAT_8888:
		case PGE_PIXEL_FORMAT_T32:
			texture->bits = 32;
			break;
		
		case PGE_PIXEL_FORMAT_T8:
			texture->bits = 8;
			break;

		case PGE_PIXEL_FORMAT_T4:
			texture->bits = 4;
			break;
			
		default:
			return NULL;
	}

	texture->size = texture->textureWidth*pgeTextureNextMul8(texture->height)*(texture->bits>>3);

	if(location == PGE_RAM)
	{
		texture->data = pgeMalloc(texture->size);
		
		if(!texture->data)
		{
			pgeFree(texture);
			return NULL;
		}
	}
	else
	{
		texture->data = pgeVramAlloc(texture->size);
		
		if(!texture->data)
		{
			pgeFree(texture);
			return NULL;
		}
	}

	memset(texture->data, 0xFF, texture->size);

	return texture;
}

static void user_warning_fn(png_structp png_ptr, png_const_charp warning_msg)
{
	(void)png_ptr;
	(void)warning_msg;
}

static int pgeTextureLoadPngInternal(png_structp png_ptr, pgeTexture* texture)
{
	png_infop info_ptr;
	unsigned int sig_read = 0;
	png_uint_32 width, height;
	int bit_depth, color_type, interlace_type;
	unsigned int y;

	png_set_error_fn(png_ptr, (png_voidp) NULL, (png_error_ptr) NULL, user_warning_fn);
	info_ptr = png_create_info_struct(png_ptr);

	if(info_ptr == NULL)
	{
		debugOut("1\n");
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		
		return 0;
	}

	png_set_sig_bytes(png_ptr, sig_read);
	png_read_info(png_ptr, info_ptr);
	png_set_strip_16(png_ptr);
	png_set_packing(png_ptr);
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, int_p_NULL, int_p_NULL);
	
	if(width > 512 || height > 512)
	{
		debugOut("2\n");
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		
		return 0;
	}

	texture->palette = 0;

	int i;

	switch(color_type)
	{
		case PNG_COLOR_TYPE_GRAY:
			texture->format = PGE_PIXEL_FORMAT_T8;
			texture->bits = 8;

			if(bit_depth < 8)
				png_set_gray_1_2_4_to_8(png_ptr);

			texture->palette = pgeMalloc(1024);

			texture->palFormat = PGE_PIXEL_FORMAT_8888;

			if(texture->palette == 0)
			{
				debugOut("3\n");
				png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
				
				return 0;
			}

			for(i = 0;i < 256;i++)
				((int*)texture->palette)[i] = ((0xFF << 24) | (i << 16) | (i << 8) | i);

			break;

		case PNG_COLOR_TYPE_PALETTE:
			texture->format = PGE_PIXEL_FORMAT_T8;
			texture->bits = 8;
			texture->palette = pgeMalloc(1024); //256 * 4
			texture->palFormat = PGE_PIXEL_FORMAT_8888;

			if(texture->palette == 0)
			{
				debugOut("4\n");
				png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
				return 0;
			}
			
			png_colorp palette = NULL;
			png_bytep trans = NULL;
			int num_palette = 0;
			int num_trans = 0;
			png_color_16p trans_values;
			png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette);
			png_get_tRNS(png_ptr, info_ptr, &trans, &num_trans, &trans_values);
			int col;

			for(i = 0; i < num_palette; i++)
			{
				col = (palette[i].red) | (palette[i].green << 8) | (palette[i].blue << 16);

				if(trans != NULL && i < num_trans)
				{
					col |= trans[i] << 24;
				}
				else
				{
					col |= 0xff000000;
				}

				((int*)texture->palette)[i] = col;
			}
			break;
		
		case PNG_COLOR_TYPE_RGB:
			if(!png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
				png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);
			else
				png_set_tRNS_to_alpha(png_ptr);

		case PNG_COLOR_TYPE_RGB_ALPHA:
			texture->format = PGE_PIXEL_FORMAT_8888;
			texture->bits = 32;
			break;
	
		default:
			debugOut("5\n");
			png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
			
			return 0;
	}

	texture->swizzled = 0;
	texture->width = width;
	texture->height = height;
	texture->textureHeight = pgeTextureNextPow2(height);
	texture->textureWidth = pgeTextureNextPow2(width);
	
	if(texture->textureHeight < 8)
		texture->textureHeight = 8;
		
	if(texture->textureWidth < 16)
		texture->textureWidth = 16;
		
	texture->size = texture->textureWidth*pgeTextureNextMul8(texture->height)*(texture->bits>>3);
	texture->data = pgeMalloc(texture->size);
	
	memset(texture->data, 0xFF, texture->size);

	if(texture->data == 0)
	{
		debugOut("6\n");
		png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
		return 0;
	}
	
	unsigned char* dst = (unsigned char*)texture->data;

	for(y = 0; y < height; y++)
	{
		png_read_row(png_ptr, (unsigned char*)dst, png_bytep_NULL);
		dst += (texture->textureWidth)*(texture->bits>>3);
	}

	png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);

	return 1;
}

typedef struct
{
	const unsigned char *data;
	png_size_t size;
	png_size_t seek;
	
} pgeTexturePngData;
	
static void pgeTextureReadPngData(png_structp png_ptr, png_bytep data, png_size_t length)
{
	pgeTexturePngData *pngData = (pgeTexturePngData*) png_get_io_ptr(png_ptr);
	
	png_size_t i;

	if(pngData)
	{
		for(i = 0; i < length; i++)
		{
			if(pngData->seek >= pngData->size)
				break;

			data[i] = pngData->data[pngData->seek++];
		}
	}
}

static void pgeTextureReadPngFile(png_structp png_ptr, png_bytep data, png_size_t length)
{
	int *fd = (int*)png_get_io_ptr(png_ptr);
	sceIoRead(*fd, data, length);
}

static int pgeTextureLoadPngMemoryInternal(unsigned char* buffer, int size, pgeTexture* texture)
{
	png_structp png_ptr;
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if(png_ptr == NULL)
		return 0;
	
	pgeTexturePngData pgeTexturePngData;
	pgeTexturePngData.data = buffer;
	pgeTexturePngData.size = size;
	pgeTexturePngData.seek = 0;

	png_set_read_fn(png_ptr, (void *) &pgeTexturePngData, pgeTextureReadPngData);
	
	return pgeTextureLoadPngInternal(png_ptr, texture);
}

static int pgeTextureLoadPngFile(const char* filename, pgeTexture* texture)
{
	png_structp png_ptr;

	int fd;
		
	fd = sceIoOpen(filename, PSP_O_RDONLY, 0777);
	
	if(fd < 0)
	{
		debugOut("7\n");
		return 0;
	}
		
	unsigned char sig[4];
	
	sceIoRead(fd, &sig, 4);
	
	sceIoLseek(fd, 0, PSP_SEEK_SET);
	
	if(png_sig_cmp(sig, 0, 4))
	{
		debugOut("8\n");
		return 0;
	}

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

	if(png_ptr == NULL)
	{
		debugOut("9\n");
		sceIoClose(fd);
		
		return 0;
	}

	png_set_read_fn(png_ptr, &fd, (png_rw_ptr)pgeTextureReadPngFile);

	int result = pgeTextureLoadPngInternal(png_ptr, texture);

	sceIoClose(fd);

	return result;
}

int pgeTextureSwizzle(pgeTexture *texture)
{
	if(texture->swizzled)
		return 1;
	
	int bytewidth = texture->textureWidth*(texture->bits>>3);
	int height = texture->size / bytewidth;

	int rowblocks = (bytewidth>>4);
	int rowblocksadd = (rowblocks-1)<<7;
	unsigned int blockaddress = 0;
	unsigned int *src = (unsigned int*) texture->data;

	unsigned char *data = NULL;
	
	if(texture->location == PGE_VRAM)
		data = (void*) pgeVramAlloc(texture->size);
	else
		data = pgeMalloc(texture->size);

	if(!data)
	{
		debugOut("10\n");
		return 0;
	}

	int j;

	for(j = 0; j < height; j++, blockaddress += 16)
	{
		unsigned int *block;

		if(texture->location == PGE_VRAM)
			block = (unsigned int*)((unsigned int)&data[blockaddress]|0x40000000);
		else
			block = (unsigned int*)&data[blockaddress];

		int i;

		for(i = 0; i < rowblocks; i++)
		{
			*block++ = *src++;
			*block++ = *src++;
			*block++ = *src++;
			*block++ = *src++;
			block += 28;
		}

		if((j & 0x7) == 0x7)
			blockaddress += rowblocksadd;
	}

	if(texture->location == PGE_VRAM)
		pgeVramFree(texture->data);
	else
		pgeFree(texture->data);
	
	texture->data = data;

	texture->swizzled = 1;

	return 1;
}

int pgeTextureUnswizzle(pgeTexture *texture)
{
	if(!texture->swizzled)
		return 1;
		
	int blockx, blocky;
	
	int bytewidth = texture->textureWidth*(texture->bits>>3);
	int height = texture->size / bytewidth;
	
	int widthblocks = bytewidth/16;
	int heightblocks = height/8;
	
	int dstpitch = (bytewidth - 16)/4;
	int dstrow = bytewidth * 8;

	unsigned int *src = (unsigned int*) texture->data;

	unsigned char *data = NULL;
	
	if(texture->location == PGE_VRAM)
		data = (void*) pgeVramAlloc(texture->size);
	else
		data = pgeMalloc(texture->size);

	if(!data)
		return 0;
		
	sceKernelDcacheWritebackAll();

	int j;
	
	unsigned char *ydst = (unsigned char *)data;

	for(blocky = 0; blocky < heightblocks; ++blocky)
	{
		unsigned char *xdst = ydst;
		
		for(blockx = 0; blockx < widthblocks; ++blockx)
		{
			unsigned int *block;

			if(texture->location == PGE_VRAM)
				block = (unsigned int*)((unsigned int)xdst|0x40000000);
			else
				block = (unsigned int*)xdst;

			for(j = 0; j < 8; ++j)
			{
				*(block++) = *(src++);
				*(block++) = *(src++);
				*(block++) = *(src++);
				*(block++) = *(src++);
				block += dstpitch;
			}

			xdst += 16;
		}
		
		ydst += dstrow;
	}

	if(texture->location == PGE_VRAM)
		pgeVramFree(texture->data);
	else
		pgeFree(texture->data);
	
	texture->data = data;

	texture->swizzled = 0;

	return 1;
}

unsigned int pgeTextureGetPixel(pgeTexture *texture, unsigned int x, unsigned int y)
{
	if(x < texture->width && y < texture->height)
	{
		unsigned int *data = (unsigned int*)texture->data;
		return data[x + y * texture->textureWidth];
	}
		
	return 0;
}

void pgeTextureSetPixel(pgeTexture *texture, unsigned int color, unsigned int x, unsigned int y)
{
	if(x < texture->width && y < texture->height)
	{
		unsigned int *data = (unsigned int*)texture->data;
		data[x + y * texture->textureWidth] = color;
	}
}

pgeTexture* pgeTextureLoadMemory(unsigned char *buffer, int size, enum pgeMemoryLocation location, unsigned char swizzle)
{
	pgeTexture *texture = (pgeTexture*) pgeMalloc(sizeof(pgeTexture));
	
	if(!texture)
		return NULL;

	texture->location = location;
	
	if(!pgeTextureLoadPngMemoryInternal(buffer, size, texture))
	{
		pgeFree(texture);
		return NULL;
	}
	
	sceKernelDcacheWritebackAll();
	
	if(swizzle)
	{
		#ifdef __PSP__
		if(!pgeTextureSwizzle(texture))
		{
			pgeFree(texture);
			return NULL;
		}
		#endif
	}

	sceKernelDcacheWritebackAll();
	
	return texture;
}

pgeTexture* pgeTextureLoad(const char *filename, enum pgeMemoryLocation location, unsigned char swizzle)
{
	pgeTexture *texture = (pgeTexture*) pgeMalloc(sizeof(pgeTexture));

	if(!texture)
	{
		debugOut("12\n");
		return NULL;
	}

	texture->location = location;
	
	if(!pgeTextureLoadPngFile(filename, texture))
	{
		debugOut("13\n");
		pgeFree(texture);
		return NULL;
	}

	sceKernelDcacheWritebackAll();
	
	if(swizzle)
	{
		#ifdef __PSP__
		if(!pgeTextureSwizzle(texture))
		{
			debugOut("14\n");
			pgeFree(texture);
			return NULL;
		}
		#endif
	}

	sceKernelDcacheWritebackAll();
	
	return texture;
}

void pgeTextureActivate(pgeTexture *texture)
{		
	if(texture->format == PGE_PIXEL_FORMAT_T4)
	{
		sceGuClutMode(texture->palFormat, 0, 0xff, 0);
		sceGuClutLoad(2, texture->palette);
	}
	else if(texture->format == PGE_PIXEL_FORMAT_T8)
	{
		sceGuClutMode(texture->palFormat, 0, 0xff, 0);
		sceGuClutLoad(32, texture->palette);
	}

	sceGuEnable(GU_TEXTURE_2D);
	sceGuTexWrap(GU_REPEAT, GU_REPEAT);
	sceGuTexFilter(GU_NEAREST, GU_NEAREST);
	sceGuTexFunc(pgeTextureMode, GU_TCC_RGBA);
	sceGuTexEnvColor(0xFFFFFFFF);
	sceGuColor(0xFFFFFFFF);
	sceGuAmbientColor(0xFFFFFFFF);
	sceGuTexOffset(0.0f, 0.0f);
	sceGuTexScale(1.0f/(float)texture->textureWidth, 1.0f/(float)texture->textureHeight);

	sceGuTexMode(texture->format, 0, 0, texture->swizzled);
	sceGuTexImage(0, texture->textureWidth, texture->textureHeight, texture->textureWidth, texture->data);
}

static void pgeTexturePaletteGet(pgeTexture *texture, unsigned int col, unsigned int *r, unsigned int *g, unsigned int *b, unsigned int *a)
{
	if(texture == 0)
		return;
		
	if(texture->palette == 0)
		return;
	
	unsigned short col16;
	
	unsigned char* pal = texture->palette;
	
	switch(texture->palFormat)
	{
		case PGE_PIXEL_FORMAT_5650:
			col16 = (pal[col*2] << 8) | (pal[col*2+1]);
			RGBA5650(col16, *r, *g, *b, *a);
			break;
			
		case PGE_PIXEL_FORMAT_5551:
			col16 = (pal[col*2] << 8) | (pal[col*2+1]);
			RGBA5551(col16, *r, *g, *b, *a);
			break;
			
		case PGE_PIXEL_FORMAT_4444:
			col16 = (pal[col*2] << 8) | (pal[col*2+1]);
			RGBA4444(col16, *r, *g, *b, *a);
			break;
			
		case PGE_PIXEL_FORMAT_8888:
			*r = pal[col*4];
			*g = pal[col*4+1];
			*b = pal[col*4+2];
			*a = pal[col*4+3];
			break;
			
		case PGE_PIXEL_FORMAT_T4:
		case PGE_PIXEL_FORMAT_T8:
		case PGE_PIXEL_FORMAT_T16:
		case PGE_PIXEL_FORMAT_T32:
		default:
			break;
	}
}

int pgeTextureToRam(pgeTexture *texture)
{
	if(texture->location == PGE_RAM)
		return 0;
		
	unsigned int *destination = (unsigned int *)pgeMalloc(texture->size);
	
	if(destination == NULL)
		return 0;
		
	memcpy(destination, texture->data, texture->size);
	
	pgeVramFree(texture->data);
	
	texture->data = destination;
	
	texture->location = PGE_RAM;
	
	sceKernelDcacheWritebackAll();
	
	return 1;
}

int pgeTextureToVram(pgeTexture *texture)
{
	if(texture->location == PGE_VRAM)
		return 0;
		
	unsigned int *destination = (unsigned int *)pgeVramAlloc(texture->size);
	
	if(destination == NULL)
		return 0;
		
	memcpy(destination, texture->data, texture->size);
	
	pgeFree(texture->data);
	
	texture->data = destination;
	
	texture->location = PGE_VRAM;
	
	sceKernelDcacheWritebackAll();
	
	return 1;
}

void pgeTextureSave(pgeTexture *texture, const char* filename)
{
	if(texture == 0)
		return;
		
	if(texture->data == 0 || texture->width == 0 || texture->height == 0)
		return;
		
	int swizzled = texture->swizzled;
	
	if(swizzled)
		pgeTextureUnswizzle(texture);
	
	png_structp png_ptr;
	png_infop info_ptr;
	FILE* fp;
	unsigned char* line;

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	
	if(!png_ptr)
		return;
	
	info_ptr = png_create_info_struct(png_ptr);
	
	if(!info_ptr)
	{
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		
		return;
	}
	
	unsigned int col_type = PNG_COLOR_TYPE_RGBA;

	fp = fopen(filename, "wb");
	if (!fp) return;
	
	png_init_io(png_ptr, fp);
	png_set_IHDR(png_ptr, info_ptr, texture->width, texture->height, 8, col_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png_ptr, info_ptr);
	
	line = (unsigned char*) pgeMalloc(texture->width * 4);
	
	if(line == 0)
	{
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		fclose(fp);
		
		return;
	}
	
	unsigned char* src = (unsigned char*)texture->data;
	unsigned char* src8 = (unsigned char*)src;
	unsigned short* src16 = (unsigned short*)src;
	unsigned int* src32 = (unsigned int*)src;

	unsigned int i, x, y;
	
	for(y = 0; y < texture->height; y++)
	{
		unsigned int swap = 0;
		src8 = (unsigned char*)src;
		src16 = (unsigned short*)src;
		src32 = (unsigned int*)src;
		
		for(i = 0, x = 0; x < texture->width; x++)
		{
			unsigned int r = 0, g = 0, b = 0, a = 0;
			unsigned short col16;
			unsigned int col32;
			unsigned char col8;
			
			switch(texture->format)
			{
				case PGE_PIXEL_FORMAT_5650:
					col16 = *src16++;
					RGBA5650(col16,r,g,b,a);
					break;
					
				case PGE_PIXEL_FORMAT_5551:
					col16 = *src16++;
					RGBA5551(col16,r,g,b,a);
					break;
					
				case PGE_PIXEL_FORMAT_4444:
					col16 = *src16++;
					RGBA4444(col16,r,g,b,a);
					break;
					
				case PGE_PIXEL_FORMAT_8888:
					col32 = *src32++;
					RGBA8888(col32,r,g,b,a);
					break;
					
				case PGE_PIXEL_FORMAT_T4:
					col8 = *src8;
					
					if(swap == 0)
						col8 &= 0xF;
					else
					{
						col8 >>= 4;
						src8++;
					}
					
					swap ^= 1;
					pgeTexturePaletteGet(texture, col8, &r, &g, &b, &a);
					break;
					
				case PGE_PIXEL_FORMAT_T8:
					col8 = *src8++;
					pgeTexturePaletteGet(texture, col8, &r, &g, &b, &a);
					break;
					
				case PGE_PIXEL_FORMAT_T16:
				case PGE_PIXEL_FORMAT_T32:
				default:
					break;
			}
			
			line[i++] = r;
			line[i++] = g;
			line[i++] = b;
			line[i++] = a;
		}
		
		png_write_row(png_ptr, line);
		src += ((texture->textureWidth*texture->bits) >> 3);
	}
	
	pgeFree(line);
	png_write_end(png_ptr, info_ptr);
	png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	fclose(fp);
	
	if(swizzled)
		pgeTextureSwizzle(texture);
}

int pgeTextureModeSet(enum pgeTextureMode mode)
{
	int oldmode = pgeTextureMode;
	
	pgeTextureMode = mode;
	
	sceGuTexFunc(pgeTextureMode, GU_TCC_RGBA);
	
	return oldmode;
}

void pgeTextureScreenshot(const char* filename)
{
	png_structp png_ptr;
	png_infop info_ptr;
	FILE* fp;
	unsigned char* line;

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	
	if(!png_ptr)
		return;
	
	info_ptr = png_create_info_struct(png_ptr);
	
	if(!info_ptr)
	{
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		return;
	}
	
	unsigned int col_type = PNG_COLOR_TYPE_RGBA;

	fp = fopen(filename, "wb");
	if (!fp) return;
	
	png_init_io(png_ptr, fp);
	png_set_IHDR(png_ptr, info_ptr, 480, 272, 8, col_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
	png_write_info(png_ptr, info_ptr);
	
	line = (unsigned char*) pgeMalloc(480 * 4);
	
	if(line == 0)
	{
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
		fclose(fp);
		return;
	}
	
	unsigned char* src = (unsigned char*)pgeGfxGetFramebuffer();
	unsigned char* src8 = (unsigned char*)src;
	unsigned short* src16 = (unsigned short*)src;
	unsigned int* src32 = (unsigned int*)src;

	int x, y, i;
	
	for(y = 0; y < 272; y++)
	{
		src8 = (unsigned char*)src;
		src16 = (unsigned short*)src;
		src32 = (unsigned int*)src;
		
		for (i = 0, x = 0; x < 480; x++)
		{
			unsigned int r = 0, g = 0, b = 0, a = 0;
			unsigned int col32;
			
			col32 = *src32++;
			RGBA8888(col32, r, g, b, a);
			
			line[i++] = r;
			line[i++] = g;
			line[i++] = b;
			line[i++] = 255;
		}
		
		png_write_row(png_ptr, line);
		src += ((512*32) >> 3);
	}
	
	pgeFree(line);
	png_write_end(png_ptr, info_ptr);
	png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	fclose(fp);	
}
