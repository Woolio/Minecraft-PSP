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
#include <pspgum.h>
#include <pspdisplay.h>

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "pge.h"
#include "pgeFont.h"
#include "pgeVram.h"

#define PGE_FONT_TEXTURE_MIN_SIZE	64
#define PGE_FONT_TEXTURE_MAX_SIZE	512

#define isUTF8(c) (((c) & 0xC0) != 0x80)

static unsigned int pgeFontCharsetSize = 95;

static unsigned int PGE_FONT_CHARSET[95];
static unsigned int *PGE_FONT_CUSTOM_CHARSET = NULL;
static unsigned int *PGE_FONT_CURRENT_CHARSET = PGE_FONT_CHARSET;

static unsigned int __attribute__((aligned(16))) clut[16];

typedef struct GlyphInfo
{
	struct GlyphInfo *next, *prev;
	unsigned int c;
	Glyph glyph;
} GlyphInfo;

GlyphInfo glyphList =
{
	.next = &glyphList,
	.prev = &glyphList,
};

static int nextPow2(unsigned int w)
{
	if (w == 0)
		return 0;

	unsigned int n = 2;

	while (w > n)
		n <<= 1;

	return (n);
}

static const unsigned int offsetsFromUTF8[6] =
{
    0x00000000UL, 0x00003080UL, 0x000E2080UL,
    0x03C82080UL, 0xFA082080UL, 0x82082080UL
};

static const char trailingBytesForUTF8[256] =
{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
};

static int UTF8toUCS(unsigned int *dest, int sz, char *src, int srcsz)
{
	unsigned int ch;
    char *srcend = src + srcsz;
    int nb;
    int i = 0;

    while(i < sz)
	{
        nb = trailingBytesForUTF8[(unsigned char)*src];
        if(srcsz == -1)
		{
            if(*src == 0)
            {
                return 0;
			}
        }
        else
		{
            if(src + nb >= srcend)
            {
                return 0;
             }
        }
		
        ch = 0;
		
        switch(nb)
		{
            /* these fall through deliberately */
			case 3: ch += (unsigned char)*src++; ch <<= 6;
			case 2: ch += (unsigned char)*src++; ch <<= 6;
			case 1: ch += (unsigned char)*src++; ch <<= 6;
			case 0: ch += (unsigned char)*src++;
        }
		
        ch -= offsetsFromUTF8[nb];
        dest[i++] = ch;
    }
    
    return 0;
}

unsigned int UTF8NextChar(char *s, int *i)
{
    unsigned int ch = 0;
    int sz = 0;

    do
	{
        ch <<= 6;
        ch += (unsigned char)s[(*i)++];
        sz++;
		
    } while (s[*i] && !isUTF8(s[*i]));
	
    ch -= offsetsFromUTF8[sz - 1];

    return ch;
}

int UTF8len(char *s)
{
    int count = 0;
    int i = 0;

    while(UTF8NextChar(s, &i) != 0)
        count++;

    return count;
}

static int pgeFontPrint(pgeFont *font, short x, short y, unsigned int color, const char *text)
{
	if(!font)
		return 0;
	
	//pgeGfxStartDrawing();

	int i, length;
	
	typedef struct
	{
		unsigned short u, v;
		short x, y, z;
		
	} fontVertex;

	fontVertex *v, *v0, *v1;

	length = UTF8len((char *)text);
	
	int strlength = strlen(text);
	
	unsigned int *utf8text = (unsigned int *)pgeMalloc(length * sizeof(unsigned int));
	
	if(utf8text == NULL)
		return 0;
	
	UTF8toUCS(utf8text, length, (char *)text, strlength);
		
	v = sceGuGetMemory(sizeof(fontVertex) * 2 * length);
	v0 = v;
	
	short xstart = x;

	sceGuColor(color);

	int j;

	for(i = 0; i < length; i++)
	{
		if(utf8text[i] == 0)
			continue;
		
		for(j = 0; j < 256;j++)
		{
			if(font->map[j] == utf8text[i])
				break;
		}
		
		Glyph *glyph = font->glyph + j;

		v1 = v0 + 1;
		
		v0->u = glyph->x;
		v0->v = glyph->y;
		v0->x = x + glyph->left;
		v0->y = y - glyph->top;
		v0->z = 0;

		v1->u = glyph->x + glyph->width;
		v1->v = glyph->y + glyph->height;
		v1->x = v0->x + glyph->width;
		v1->y = v0->y + glyph->height;
		v1->z = 0;

		v0 += 2;
		x += glyph->advance;
		
		if(utf8text[i] == '\n')
		{
			x = xstart;
			y += font->fontHeight + 3;
		}
	}

	sceGuDrawArray(GU_SPRITES, GU_TEXTURE_16BIT|GU_VERTEX_16BIT|GU_TRANSFORM_2D, length * 2, 0, v);
	
	pgeFree(utf8text);

	return x;
}

static int pgeFontSwizzle(pgeFont *font)
{
	int byteWidth = font->texSize/2;
	int textureSize = font->texSize*font->texHeight/2;
	int height = textureSize / byteWidth;

	// Swizzle the texture
	int rowBlocks = (byteWidth/16);
	int rowBlocksAdd = (rowBlocks - 1)*128;
	unsigned int blockAddress = 0;
	unsigned int *src = (unsigned int*) font->texture;
	static unsigned char *tData = 0;

	if(font->location == PGE_VRAM)
		tData = (unsigned char*) pgeVramAlloc(textureSize);
	else
		tData = (unsigned char*) pgeMalloc(textureSize);

	if(!tData)
		return 0;

	int j;

	for(j = 0; j < height; j++, blockAddress += 16)
	{
		unsigned int *block;

		if (font->location == PGE_VRAM)
			block = (unsigned int*)((unsigned int)&tData[blockAddress]|0x40000000);
		else
			block = (unsigned int*)&tData[blockAddress];

		int i;

		for(i=0; i < rowBlocks; i++)
		{
			*block++ = *src++;
			*block++ = *src++;
			*block++ = *src++;
			*block++ = *src++;
			block += 28;
		}

		if((j & 0x7) == 0x7)
			blockAddress += rowBlocksAdd;
	}
	
	sceKernelDcacheWritebackAll();

	pgeFree(font->texture);

	font->texture = tData;
	
	return 1;
}

static unsigned long pgeFontIoFunc(FT_Stream stream, unsigned long offset, unsigned char *buffer, unsigned long count)
{
	unsigned long result = 0;
	
	sceIoLseek(stream->descriptor.value, offset, PSP_SEEK_SET);
	
	if(count > 0)
	{
		result = sceIoRead(stream->descriptor.value, buffer, count);
	}
	
	return result;
}

static void pgeFontIoClose(FT_Stream stream)
{
	sceIoClose(stream->descriptor.value);
}

pgeFont* pgeFontLoad(const char *filename, unsigned int fontsize, enum pgeFontSizeType fontsizetype, enum pgeMemoryLocation location)
{
	FT_Library library;
	FT_Face face;
	FT_Stream stream;
	FT_Open_Args args;
	
	stream = (FT_Stream)pgeMalloc(sizeof(*stream));
	
	if(!stream)
		return NULL;
		
	memset(stream, 0, sizeof(*stream));
	
	int fd = sceIoOpen(filename, PSP_O_RDONLY, 0777);
		
	if(fd < 0)
	{
		pgeFree(stream);
		return NULL;
	}
		
	unsigned int fsize = sceIoLseek(fd, 0, PSP_SEEK_END);
	
	sceIoLseek(fd, 0, PSP_SEEK_SET);
		
	stream->base = NULL;
	stream->size = fsize;
	stream->pos = 0;
	stream->descriptor.value = fd;
	stream->read = pgeFontIoFunc;
	stream->close = pgeFontIoClose;
	
	memset(&args, 0, sizeof(args));
	
	args.flags = FT_OPEN_STREAM;
	args.stream = stream;
		
	if(FT_Init_FreeType(&library))
		return NULL;

	if(FT_Open_Face(library, &args, 0, &face))
	{
		return NULL;
	}
	
	FT_GlyphSlot slot;
	GlyphInfo *gp;
	GlyphInfo *gi = pgeMalloc(sizeof(GlyphInfo) * pgeFontCharsetSize);
	
	if(gi == NULL)
	{
		pgeFree(stream);
		return NULL;
	}
	
	int n, count, charCount;
	int xx, yy;

	// FIX!
	charCount = pgeFontCharsetSize;
	count = charCount;

	pgeFont* font = (pgeFont*) pgeMalloc(sizeof(pgeFont));
	
	if(!font)
		return NULL;

	font->location = location;

	if(fontsizetype == PGE_FONT_SIZE_PIXELS)
	{
		if(FT_Set_Pixel_Sizes(face, fontsize, 0))
		{
			pgeFree(font);
			return NULL;
		}
	}
	else
	{
		if(FT_Set_Char_Size(face, fontsize*64, 0, 100, 0))
		{
			pgeFree(font);
			return NULL;
		}
	}

	slot = face->glyph;
	
	char minLeft = 127, maxTop = -127;

	// Assemble metrics and sort by size
	for(n = 0; n < count; n++)
	{
		if(FT_Load_Char(face, PGE_FONT_CURRENT_CHARSET[n], FT_LOAD_RENDER|FT_LOAD_NO_BITMAP))
		{
			pgeFree(font);
			return NULL;
		}

		gi[n].c = PGE_FONT_CURRENT_CHARSET[n];
		gi[n].glyph.x = 0;
		gi[n].glyph.y = 0;
		gi[n].glyph.width = slot->bitmap.width;
		gi[n].glyph.height = slot->bitmap.rows;
		gi[n].glyph.top = slot->bitmap_top;
		gi[n].glyph.left = slot->bitmap_left;
		gi[n].glyph.advance = slot->advance.x/64;
		gi[n].glyph.unused = 0;
		
		if(minLeft > slot->bitmap_left)
			minLeft = slot->bitmap_left;
			
		if(maxTop < slot->bitmap_top)
			maxTop = slot->bitmap_top;

		// Find a good fit
		gp = glyphList.next;

		while((gp != &glyphList) && (gp->glyph.height > gi[n].glyph.height))
			gp = gp->next;

		gi[n].next = gp;
		gi[n].prev = gp->prev;
		gi[n].next->prev = gi;
		gi[n].prev->next = gi;
	}
	
	int size = maxTop;
	int maxWidth = 0;
	int maxHeight = 0;
	
	for(n = 0; n < count; n++)
	{
		if(FT_Load_Char(face, PGE_FONT_CURRENT_CHARSET[n], FT_LOAD_RENDER|FT_LOAD_NO_BITMAP))
		{
			pgeFree(font);
			return NULL;
		}
		
		if((size - slot->bitmap_top) + slot->bitmap.rows > maxHeight)
		{
			maxHeight = (size - slot->bitmap_top) + slot->bitmap.rows;
		}
		
		if(slot->bitmap.width > maxWidth)
		{
			maxWidth = slot->bitmap.width;
		}
	}
	
	int texColumns = 1;
	
	while((count / texColumns) * (maxHeight + 8) > PGE_FONT_TEXTURE_MAX_SIZE)
	{
		texColumns++;
	}
	
	int textureHeight = PGE_FONT_TEXTURE_MAX_SIZE;
	int textureWidth = nextPow2(texColumns * (maxWidth + 8));
	
	if(textureWidth < PGE_FONT_TEXTURE_MIN_SIZE)
		textureWidth = PGE_FONT_TEXTURE_MIN_SIZE;
		
	if(textureWidth > PGE_FONT_TEXTURE_MAX_SIZE)
	{
		pgeFree(font);
		return NULL;
	}
	
	font->texSize = textureWidth;
	
	int x = 0;
	int y = 0;
	int ynext = 0;
	int used = 0;

	count = 0;
	memset(font->map, 255, pgeFontCharsetSize);

	font->texture = (unsigned char*) pgeMalloc(textureWidth * textureHeight / 2);
    	
	if(!font->texture)
	{
		pgeFree(font);
		return NULL;
	}
	
	memset(font->texture, 0, textureWidth * textureHeight / 2);
	
	font->fontHeight = 0;
	
	for(n = 0; n < charCount; n++)
	{
		if(FT_Load_Char(face, gi[n].c, FT_LOAD_RENDER))
		{
			pgeFontDestroy(font);
			return NULL;
		}
	
		if((x + gi[n].glyph.width) > textureWidth)
		{
			y += ynext;
			x = 0;
		}
		
		if(gi[n].glyph.height > ynext)
			ynext = gi[n].glyph.height;

		if((y + ynext) > textureHeight)
		{
			pgeFontDestroy(font);
			return NULL;
		}
		
		font->map[n] = gi[n].c;
		gi[n].glyph.x = x;
		gi[n].glyph.y = y;
		gi[n].glyph.left -= minLeft;
		gi[n].glyph.top -= maxTop;
	
		if((int)(gi[n].glyph.height + gi[n].glyph.top) > font->fontHeight)
			font->fontHeight = (gi[n].glyph.height+gi[n].glyph.top);
			
		unsigned int slotoffs = 0;
		unsigned int texoffs = (y * textureWidth)/2;
		unsigned char* slotbuf = slot->bitmap.buffer;

		for(yy = 0; yy < gi[n].glyph.height; yy++)
		{
			xx = 0;
		
			if (x & 1)
			{
				font->texture[(x>>1) + texoffs] |= (slotbuf[slotoffs] & 0xF0);
				xx++;
			}
			
			for(; xx < gi[n].glyph.width; xx += 2)
			{
				if (xx + 1 < gi[n].glyph.width)
					font->texture[((x+xx)>>1) + texoffs] = (slotbuf[slotoffs + xx] >> 4) | (slotbuf[slotoffs + xx + 1] & 0xF0);
				else
					font->texture[((x+xx)>>1) + texoffs] = (slotbuf[slotoffs + xx] >> 4);
			}
			
			texoffs += textureWidth>>1;
			slotoffs += slot->bitmap.width;
		}

		x += gi[n].glyph.width;

		used += (gi[n].glyph.width * gi[n].glyph.height);
	}

	font->texHeight = (y + ynext + 7)&~7;

	if(font->texHeight > font->texSize)
		font->texHeight = font->texSize;

	/* redirect any undefined characters to ' ' */
	for(n = 0; n < (int)pgeFontCharsetSize; n++)
	{
		if(font->map[n] == 255)
			font->map[n] = font->map[0];
	}

	for(n = 0; n < charCount; n++)
		memcpy(&font->glyph[n], &gi[n].glyph, sizeof(gi[n].glyph));

	FT_Done_Face(face);
	FT_Done_FreeType(library);
	
	#ifdef __PSP__
	if(!pgeFontSwizzle(font))
	{
		pgeFontDestroy(font);
		return NULL;
	}
	#endif
	
	pgeFree(stream);
	
	pgeFree(gi);

	return font;
}

pgeFont* pgeFontLoadMemory(unsigned char *data, int datasize, unsigned int fontsize, enum pgeFontSizeType fontsizetype, enum pgeMemoryLocation location)
{	
	if(data == NULL || datasize < 1)
		return NULL;
	
	FT_Library library;
	FT_Face face;
	
	if(FT_Init_FreeType(&library))
		return NULL;

	if(FT_New_Memory_Face(library, data, datasize, 0, &face))
		return NULL;
	
	FT_GlyphSlot slot;
	GlyphInfo *gp;
	GlyphInfo gi[pgeFontCharsetSize];
	int n, count, charCount;
	int xx, yy;

	// FIX!
	charCount = pgeFontCharsetSize;
	count = charCount;

	pgeFont* font = (pgeFont*) pgeMalloc(sizeof(pgeFont));
	
	if(!font)
		return NULL;

	font->location = location;

	if(fontsizetype == PGE_FONT_SIZE_PIXELS)
	{
		if(FT_Set_Pixel_Sizes(face, fontsize, 0))
		{
			pgeFree(font);
			return NULL;
		}
	}
	else
	{
		if(FT_Set_Char_Size(face, fontsize*64, 0, 100, 0))
		{
			pgeFree(font);
			return NULL;
		}
	}

	slot = face->glyph;
	
	char minLeft = 127, maxTop = -127;

	// Assemble metrics and sort by size
	for(n = 0; n < count; n++)
	{
		if(FT_Load_Char(face, PGE_FONT_CURRENT_CHARSET[n], FT_LOAD_RENDER|FT_LOAD_NO_BITMAP))
		{
			pgeFree(font);
			return NULL;
		}

		gi[n].c = PGE_FONT_CURRENT_CHARSET[n];
		gi[n].glyph.x = 0;
		gi[n].glyph.y = 0;
		gi[n].glyph.width = slot->bitmap.width;
		gi[n].glyph.height = slot->bitmap.rows;
		gi[n].glyph.top = slot->bitmap_top;
		gi[n].glyph.left = slot->bitmap_left;
		gi[n].glyph.advance = slot->advance.x/64;
		gi[n].glyph.unused = 0;
		
		if(minLeft > slot->bitmap_left)
			minLeft = slot->bitmap_left;
			
		if(maxTop < slot->bitmap_top)
			maxTop = slot->bitmap_top;

		// Find a good fit
		gp = glyphList.next;

		while((gp != &glyphList) && (gp->glyph.height > gi[n].glyph.height))
			gp = gp->next;

		gi[n].next = gp;
		gi[n].prev = gp->prev;
		gi[n].next->prev = gi;
		gi[n].prev->next = gi;
	}
	
	int size = maxTop;
	int maxWidth = 0;
	int maxHeight = 0;
	
	for(n = 0; n < count; n++)
	{
		if(FT_Load_Char(face, PGE_FONT_CURRENT_CHARSET[n], FT_LOAD_RENDER|FT_LOAD_NO_BITMAP))
		{
			pgeFree(font);
			return NULL;
		}
		
		if((size - slot->bitmap_top) + slot->bitmap.rows > maxHeight)
		{
			maxHeight = (size - slot->bitmap_top) + slot->bitmap.rows;
		}
		
		if(slot->bitmap.width > maxWidth)
		{
			maxWidth = slot->bitmap.width;
		}
	}
	
	int texColumns = 1;
	
	while((count / texColumns) * (maxHeight + 8) > PGE_FONT_TEXTURE_MAX_SIZE)
	{
		texColumns++;
	}
	
	int textureHeight = PGE_FONT_TEXTURE_MAX_SIZE;
	int textureWidth = nextPow2(texColumns * (maxWidth + 8));
	
	if(textureWidth < PGE_FONT_TEXTURE_MIN_SIZE)
		textureWidth = PGE_FONT_TEXTURE_MIN_SIZE;
		
	if(textureWidth > PGE_FONT_TEXTURE_MAX_SIZE)
	{
		pgeFree(font);
		return NULL;
	}
	
	font->texSize = textureWidth;
	
	int x = 0;
	int y = 0;
	int ynext = 0;
	int used = 0;

	count = 0;
	memset(font->map, 255, pgeFontCharsetSize);

	font->texture = (unsigned char*) pgeMalloc(textureWidth * textureHeight / 2);
    	
	if(!font->texture)
	{
		pgeFree(font);
		return NULL;
	}
	
	memset(font->texture, 0, textureWidth * textureHeight / 2);
	
	font->fontHeight = 0;
	
	for(n = 0; n < charCount; n++)
	{		
		if(FT_Load_Char(face, gi[n].c, FT_LOAD_RENDER))
		{
			pgeFontDestroy(font);
			return NULL;
		}
	
		if((x + gi[n].glyph.width) > textureWidth)
		{
			y += ynext;
			x = 0;
		}
		
		if(gi[n].glyph.height > ynext)
			ynext = gi[n].glyph.height;

		if((y + ynext) > textureHeight)
		{
			pgeFontDestroy(font);
			return NULL;
		}
		
		font->map[gi[n].c] = count++;
		gi[n].glyph.x = x;
		gi[n].glyph.y = y;
		gi[n].glyph.left -= minLeft;
		gi[n].glyph.top -= maxTop;
	
		if((int)(gi[n].glyph.height + gi[n].glyph.top) > font->fontHeight)
			font->fontHeight = (gi[n].glyph.height+gi[n].glyph.top);
			
		unsigned int slotoffs = 0;
		unsigned int texoffs = (y * textureWidth)/2;
		unsigned char* slotbuf = slot->bitmap.buffer;

		for(yy = 0; yy < gi[n].glyph.height; yy++)
		{
			xx = 0;
		
			if (x & 1)
			{
				font->texture[(x>>1) + texoffs] |= (slotbuf[slotoffs] & 0xF0);
				xx++;
			}
			
			for(; xx < gi[n].glyph.width; xx += 2)
			{
				if (xx + 1 < gi[n].glyph.width)
					font->texture[((x+xx)>>1) + texoffs] = (slotbuf[slotoffs + xx] >> 4) | (slotbuf[slotoffs + xx + 1] & 0xF0);
				else
					font->texture[((x+xx)>>1) + texoffs] = (slotbuf[slotoffs + xx] >> 4);
			}
			
			texoffs += textureWidth>>1;
			slotoffs += slot->bitmap.width;
		}

		x += gi[n].glyph.width;

		used += (gi[n].glyph.width * gi[n].glyph.height);
	}

	font->texHeight = (y + ynext + 7)&~7;

	if(font->texHeight > font->texSize)
		font->texHeight = font->texSize;

	for(n = 0; n < (int)pgeFontCharsetSize; n++)
	{
		if(font->map[n] == 255)
			font->map[n] = font->map[0];
	}

	for(n = 0; n < charCount; n++)
		memcpy(&font->glyph[n], &gi[n].glyph, sizeof(gi[n].glyph));

	FT_Done_Face(face);
	FT_Done_FreeType(library);
	
	#ifdef __PSP__
	if(!pgeFontSwizzle(font))
	{
		pgeFontDestroy(font);
		return NULL;
	}
	#endif

	return font;
}

void pgeFontDestroy(pgeFont *font)
{
	if(font->texture)
	{
		if(font->location == PGE_VRAM)
			pgeVramFree(font->texture);
		else
			pgeFree(font->texture);
	}		

	if(font)
		pgeFree(font);
}

void pgeFontActivate(pgeFont *font)
{
	if(!font)
		return;

	sceGuClutMode(GU_PSM_8888, 0, 255, 0);
	sceGuClutLoad(2, clut);
	
	sceGuEnable(GU_TEXTURE_2D);
	sceGuTexMode(GU_PSM_T4, 0, 0, 1);
	sceGuTexImage(0, font->texSize, font->texSize, font->texSize, font->texture);
	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
	sceGuTexOffset(0.0f, 0.0f);
	sceGuTexWrap(GU_REPEAT, GU_REPEAT);
	sceGuTexFilter(GU_NEAREST, GU_NEAREST);
}

int pgeFontInit(void)
{
	int n;

	for(n = 0; n < 16; n++)
		clut[n] = ((n | n << 4) << 24) | 0x00FFFFFF;
		
	int i;
		
	for(n = 32, i = 0; n < 127; n++, i++)
		PGE_FONT_CHARSET[i] = n;
	
	return 1;
}

void pgeFontShutdown(void)
{
	if(PGE_FONT_CUSTOM_CHARSET != NULL)
		free(PGE_FONT_CUSTOM_CHARSET);
}

int pgeFontSetCharset(const char *charset)
{
	int length = strlen(charset);
	int utflength = UTF8len((char *)charset);
	
	if(PGE_FONT_CUSTOM_CHARSET != NULL)
		free(PGE_FONT_CUSTOM_CHARSET);
		
	PGE_FONT_CUSTOM_CHARSET = (unsigned int *)pgeMalloc(utflength * sizeof(unsigned int));
	
	if(PGE_FONT_CUSTOM_CHARSET == NULL)
		return 0;
		
	UTF8toUCS(PGE_FONT_CUSTOM_CHARSET, utflength, (char *)charset, length);
	
	PGE_FONT_CURRENT_CHARSET = PGE_FONT_CUSTOM_CHARSET;
	
	pgeFontCharsetSize = utflength;
	
	return 1;
}

void pgeFontResetCharset(void)
{
	PGE_FONT_CURRENT_CHARSET = PGE_FONT_CHARSET;
	
	pgeFontCharsetSize = 95;
}	

int pgeFontPrintf(pgeFont *font, short x, short y, unsigned int color, const char *text, ...)
{
	if(!font)
		return 0;

	char buffer[256];
	va_list ap;
	
	va_start(ap, text);
	vsnprintf(buffer, 256, text, ap);
	va_end(ap);
	buffer[255] = 0;
	
	return pgeFontPrint(font, x, y, color, buffer);
}

int pgeFontPrintfCenter(pgeFont *font, short y, unsigned int color, const char *text, ...)
{
	if(!font)
		return 0;
	
	char buffer[256];
	va_list ap;
	
	va_start(ap, text);
	vsnprintf(buffer, 256, text, ap);
	va_end(ap);
	buffer[255] = 0;
	
	short x = 240 - (pgeFontMeasureText(font, buffer) / 2);
	
	return pgeFontPrint(font, x, y, color, buffer);
}

int pgeFontMeasureText(pgeFont *font, const char *text)
{
	if(!font)
		return 0;

	int x = 0;
	
	while(*text)
	{
		Glyph *glyph = font->glyph + font->map[*text++ & 0xff];
		x += glyph->advance;
	}
	
	return x;
}

int pgeFontMeasureFormattedText(pgeFont *font, const char *text, ...)
{
	if(!font)
		return 0;
	
	char buffer[256];
	va_list ap;
	
	va_start(ap, text);
	vsnprintf(buffer, 256, text, ap);
	va_end(ap);
	buffer[255] = 0;
	
	return pgeFontMeasureText(font, buffer);
}

int pgeFontGetHeight(pgeFont *font)
{
	return font->fontHeight;
}
