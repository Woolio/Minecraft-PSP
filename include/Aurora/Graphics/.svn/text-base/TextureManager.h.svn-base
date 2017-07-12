#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <stdio.h>
#include <vector>
#include <string>
#include <string.h>

#define png_infopp_NULL (png_infopp)NULL
#define int_p_NULL (int*)NULL


#include <stdlib.h>
#include <malloc.h>
#include <png.h>
#include <pspdebug.h>
#include <pspgu.h>
#include <pspgum.h>
#include <psputils.h>
#include <pspdisplay.h>

namespace Aurora
{
	namespace Graphics
	{
		class Image
		{
		public:

			int Width;
			int Height;
			int power2Width;
			int power2Height;
			int vRam;
			int ColorMode;
			int Swizzle;

			unsigned int ID;

			std::string FileName;

			//more like for psp
			unsigned short *ImageData;

			~Image()
			{
				if(ImageData != NULL)
				free(ImageData);
			}
		};

		class TextureManager
		{
		private:

			static TextureManager m_TextureManager;


			int PowerOfTwo(int value)
			{
				int poweroftwo = 1;
				while (poweroftwo < value ){
					poweroftwo <<= 1;
				}
				return poweroftwo;
			}

			unsigned short Color8888To5551(unsigned int Color32)
			{
				unsigned char Red = (Color32);
				unsigned short Red16 = Red>>3;
				unsigned char Green = (Color32>>8);
				unsigned short Green16 = (Green>>3)<<5;
				unsigned char Blue = (Color32>>16);
				unsigned short Blue16 = (Blue>>3)<<10;
				unsigned char Alpha = (Color32>>24);
				unsigned short Alpha16 = (Alpha>>3)<<15;
				unsigned short Color16 = Red16 | Green16 | Blue16 | Alpha16;
				return Color16;
			}

			unsigned short Color8888To4444(unsigned int Color32)
			{
				unsigned char Red = (Color32);
				unsigned short Red16 = Red>>4;
				unsigned char Green = (Color32>>8);
				unsigned short Green16 = (Green>>4)<<4;
				unsigned char Blue = (Color32>>16);
				unsigned short Blue16 = (Blue>>4)<<8;
				unsigned char Alpha = (Color32>>24);
				unsigned short Alpha16 = (Alpha>>4)<<12;
				unsigned short Color16 = Red16 | Green16 | Blue16 | Alpha16;
				return Color16;
			}

			unsigned short Color8888To5650(unsigned int Color32)
			{
				unsigned char Red = (Color32);
				unsigned short Red16 = Red>>3;
				unsigned char Green = (Color32>>8);
				unsigned short Green16 = (Green>>2)<<5;
				unsigned char Blue = (Color32>>16);
				unsigned short Blue16 = (Blue>>3)<<11;
				//unsigned char Alpha = (Color32>>24);
				unsigned short Color16 = Red16 | Green16 | Blue16;
				return Color16;
			}


			void swizzle_fast(u8* out, const u8* in, unsigned int width, unsigned int height)
			{
				unsigned int blockx, blocky;
				unsigned int j;

				unsigned int width_blocks = (width / 16);
				unsigned int height_blocks = (height / 8);

				unsigned int src_pitch = (width-16)/4;
				unsigned int src_row = width * 8;

				const u8* ysrc = in;
				u32* dst = (u32*)out;

				for (blocky = 0; blocky < height_blocks; ++blocky)
				{
					const u8* xsrc = ysrc;
					for (blockx = 0; blockx < width_blocks; ++blockx)
					{
						const u32* src = (u32*)xsrc;
						for (j = 0; j < 8; ++j)
						{
							*(dst++) = *(src++);
							*(dst++) = *(src++);
							*(dst++) = *(src++);
							*(dst++) = *(src++);
							src += src_pitch;
						}
						xsrc += 16;
					}
					ysrc += src_row;
				}
			}
		public:

			Image* LoadPng(const char* filename,int ColorMode,int Swizzle,int Vram);

			std::vector<std::string> Names;
			std::vector<Image*> Images;

			int currentTexture;

			static TextureManager* Instance();

			void Init();

			bool FileExist(std::string fileName);

			bool TextureExist(std::string name);
			int GetTextureNumber(std::string name);

			void SetTexture(int number,int minFilter = GU_LINEAR,int maxFilter = GU_LINEAR);
			void SetTextureModeulate(int number);
			void SetMipMapsTextures(int texture,int mipmap1,int mipmap2,int mipmap3);

			void LoadTexture(std::string fileName);
			void LoadTexureVFS(std::string fileName);

			void RemoveTexture(int number);

			Image* LoadPng(std::string fileName);
			Image* LoadPngFromMemory(std::string fileName);

			Image* CreateImage(int width,int height,int ColorMode,int Vram);
		};

	}
}

#endif
