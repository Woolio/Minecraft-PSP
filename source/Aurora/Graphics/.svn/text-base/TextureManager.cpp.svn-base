#include <Aurora/Graphics/TextureManager.h>
#include <Aurora/Graphics/vram.h>
#include <Aurora/Utils/Logger.h>

namespace Aurora
{
	namespace Graphics
	{
		TextureManager TextureManager::m_TextureManager;

		TextureManager* TextureManager::Instance()
		{
			return &m_TextureManager;
		}

		void TextureManager::Init()
		{
			currentTexture = -1;
			//by default it will load default texture default.png
			//LoadTexture("default.png"); //<-- something like this
		}

		bool TextureManager::TextureExist(std::string name)
		{
			for (unsigned int i=0;i < Names.size();i++)
			{
				if (strcmp(name.c_str(),Names[i].c_str())==0)
				{
					return true;
				}
			}
			return false;
		}

		int TextureManager::GetTextureNumber(std::string name)
		{
			for (unsigned int i=0;i < Names.size();i++)
			{
				if (strcmp(name.c_str(),Names[i].c_str())==0)
				{
					return i;
				}
			}
			return 0;
		}

		void TextureManager::RemoveTexture(int number)
		{
			//name
			Names.erase(Names.begin() + number);
			//image
			delete Images[number];
			Images.erase(Images.begin() + number);

		}

		void TextureManager::SetTexture(int number,int minFilter,int maxFilter)
		{
			if(currentTexture != number)
			{
				Image *image = Images[number];

				sceGuTexMode(image->ColorMode,0,0,1);
				sceGuTexFunc(GU_TFX_REPLACE,GU_TCC_RGBA);
				sceGuTexFilter(minFilter,maxFilter);
				sceGuTexOffset( 0.0f, 0.0f );
				sceGuTexWrap(GU_REPEAT,GU_REPEAT);
				sceGuTexImage(0,image->power2Width,image->power2Height,image->power2Width,image->ImageData);
				//sceGuTexImage(0,image->Width,image->Height,image->Width,image->ImageData);

				currentTexture = number;
			}
		}

		void TextureManager::SetTextureModeulate(int number)
		{
			if(currentTexture != number)
			{
				Image *image = Images[number];

				sceGuTexMode(image->ColorMode,0,0,image->Swizzle);
				sceGuTexFunc(GU_TFX_MODULATE,GU_TCC_RGBA);
				sceGuTexFilter(GU_NEAREST,GU_NEAREST);
				sceGuTexOffset( 0.0f, 0.0f );
				sceGuTexWrap(GU_CLAMP,GU_CLAMP);
				sceGuTexImage(0,image->power2Width,image->power2Height,image->power2Width,image->ImageData);

				currentTexture = number;
			}
		}

		void TextureManager::SetMipMapsTextures(int texture,int mipmap1,int mipmap2,int mipmap3)
		{
			if(currentTexture != texture)
			{
				Image *image = Images[texture];
				Image *mip1 = Images[mipmap1];
				Image *mip2 = Images[mipmap2];
				Image *mip3 = Images[mipmap3];

				sceGuTexMode(image->ColorMode,3,0,1);
				sceGuTexFunc(GU_TFX_REPLACE,GU_TCC_RGBA);
				sceGuTexOffset( 0.0f, 0.0f );
				sceGuTexWrap(GU_REPEAT,GU_REPEAT);
				sceGuTexLevelMode(GU_TEXTURE_AUTO,0.0f);
				sceGuTexFilter(GU_LINEAR_MIPMAP_LINEAR,GU_LINEAR);

				sceGuTexImage(0,image->power2Width,image->power2Height,image->power2Width,image->ImageData);
				sceGuTexImage(1,mip1->power2Width,mip1->power2Height,mip1->power2Width,mip1->ImageData);
				sceGuTexImage(2,mip2->power2Width,mip2->power2Height,mip2->power2Width,mip2->ImageData);
				sceGuTexImage(3,mip3->power2Width,mip3->power2Height,mip3->power2Width,mip3->ImageData);

				currentTexture = texture;
			}
		}

		bool TextureManager::FileExist(std::string fileName)
		{
			FILE *fp;

			if ((fp = fopen(fileName.c_str(), "r")) == NULL)
			{
				Utils::Logger::Instance()->LogMessage("Can't loaded texture: %s\n",fileName.c_str());
				return false;
			}

			fclose(fp);

			return true;
		}


		void TextureManager::LoadTexture(std::string fileName)
		{
			//check if texture is loaded
			if(TextureExist(fileName))
				return;

			//load texture
			if(fileName.substr(fileName.find_last_of(".") + 1) == "png")
			{
				//load png image
				Image *image = LoadPng(fileName);
				if (image != NULL)
				{
					Names.push_back(fileName);
					Images.push_back(image);
					Utils::Logger::Instance()->LogMessage("Loaded texture: %s\n",fileName.c_str());
				}else
					Utils::Logger::Instance()->LogMessage("Can't open file %s\n",fileName.c_str());
			}
		}

		void TextureManager::LoadTexureVFS(std::string fileName)
		{
			if(fileName.substr(fileName.find_last_of(".") + 1) == "png")
			{
				//load png image
				Image *image = LoadPngFromMemory(fileName);
				if (image != NULL)
				{
					Names.push_back(fileName);
					Images.push_back(image);
					Utils::Logger::Instance()->LogMessage("Loaded texture: %s\n",fileName.c_str());
				}else
					Utils::Logger::Instance()->LogMessage("Can't open file %s\n",fileName.c_str());
			}
		}


		Image* TextureManager::LoadPng(std::string fileName)
		{
			return LoadPng(fileName.c_str(),GU_PSM_8888,1,0);
		}

		Image* TextureManager::LoadPngFromMemory(std::string fileName)
		{
			return NULL;
		}

		Image* TextureManager::LoadPng(const char* filename,int ColorMode,int Swizzle,int Vram)
		{
			unsigned short *Buffer;
			unsigned short *swizzled_pixels = NULL;

			int OutBytesPerPixel;
			int Power2Width = 0;
			int Power2Height = 0;

			png_structp png_ptr;
			png_infop info_ptr;
			unsigned int sig_read = 0;
			png_uint_32 width, height,x, y;
			int bit_depth, color_type, interlace_type;
			unsigned int* line;
			FILE *fp;

			if(ColorMode == GU_PSM_4444 || ColorMode == GU_PSM_5650 || ColorMode == GU_PSM_5551)
				OutBytesPerPixel = 2;
			else
				OutBytesPerPixel = 4;

			if ((fp = fopen(filename, "rb")) == NULL)
			{
				printf("Can't open file %s\n",filename);
				return NULL;
			}

			png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			if (png_ptr == NULL)
			{
				fclose(fp);
				return NULL;
			}

			info_ptr = png_create_info_struct(png_ptr);
			if (info_ptr == NULL)
			{
				fclose(fp);
				png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
				return NULL;
			}

			png_init_io(png_ptr, fp);
			png_set_sig_bytes(png_ptr, sig_read);
			png_read_info(png_ptr, info_ptr);
			png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, int_p_NULL, int_p_NULL);
			png_set_strip_16(png_ptr);
			png_set_packing(png_ptr);

			if (color_type == PNG_COLOR_TYPE_PALETTE)
				png_set_palette_to_rgb(png_ptr);
			if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
				png_set_expand_gray_1_2_4_to_8(png_ptr);
			if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
				png_set_tRNS_to_alpha(png_ptr);

			png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);

			line = (unsigned int*) malloc(width * 4);
			if (!line)
			{
				fclose(fp);
				png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
				return NULL;
			}

			Power2Width = PowerOfTwo(width);
			Power2Height = PowerOfTwo(height);
			Buffer = (unsigned short*)memalign(16,Power2Width*Power2Height*OutBytesPerPixel);
			//Buffer = (unsigned short*)malloc(sizeof(unsigned short) * (Power2Width*Power2Height*OutBytesPerPixel));

			for (y = 0; y < height; y++)
			{
				png_read_row(png_ptr, (unsigned char*) line, NULL);

				for (x = 0; x < width; x++)
				{
					unsigned int *Buffer32 = (unsigned int*)Buffer;
					unsigned int color32 = line[x];
					unsigned short color16;

					if(ColorMode == GU_PSM_5551)
					{
						color16 = Color8888To5551(color32);
						Buffer[y*Power2Width+x] = color16;
					}
					else if(ColorMode == GU_PSM_4444)
					{
						color16 = Color8888To4444(color32);
						Buffer[y*Power2Width+x] = color16;
					}
					else if(ColorMode == GU_PSM_5650)
					{
						color16 = Color8888To5650(color32);
						Buffer[y*Power2Width+x] = color16;
					}
					else
					{
						Buffer32[y*Power2Width+x] = color32;
					}
				}
			}

			free(line);
			png_read_end(png_ptr, info_ptr);
			png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
			fclose(fp);

			Image *Image1 = new Image();

			Image1->Width = width;
			Image1->Height = height;
			Image1->power2Width = Power2Width;
			Image1->power2Height = Power2Height;
			Image1->vRam = Vram;
			Image1->ColorMode = ColorMode;
			Image1->Swizzle = Swizzle;

			if (Vram == 1)
			{
				swizzled_pixels = (unsigned short*)getStaticVramTexture(Power2Width,Power2Height,ColorMode);//valloc(Image1->power2Height*Image1->power2Width*OutBytesPerPixel);

			}else
			{
				swizzled_pixels = (unsigned short*)memalign(16,Image1->power2Height*Image1->power2Width*OutBytesPerPixel);
				//swizzled_pixels = (unsigned short*)malloc(sizeof(unsigned short) * (Power2Width*Power2Height*OutBytesPerPixel));
			}

			swizzle_fast((u8*)swizzled_pixels,(const u8*)Buffer,Image1->power2Width*OutBytesPerPixel,Image1->power2Height);

			// 512*2 because swizzle operates in bytes, and each pixel in a 16-bit texture is 2 bytes
			//sceKernelDcacheWritebackAll();

			Image1->ImageData = swizzled_pixels;
			free(Buffer);

			//clear the cache or there will be some errors
			sceKernelDcacheWritebackInvalidateAll();

			return Image1;
			/*unsigned short *Buffer;
			unsigned short *swizzled_pixels = NULL;

			int OutBytesPerPixel;
			int Power2Width = 0;
			int Power2Height = 0;

			png_structp png_ptr;
			png_infop info_ptr;
			unsigned int sig_read = 0;
			png_uint_32 width, height,x, y;
			int bit_depth, color_type, interlace_type;
			unsigned int* line;
			FILE *fp;

			if(ColorMode == GU_PSM_4444 || ColorMode == GU_PSM_5650 || ColorMode == GU_PSM_5551)
				OutBytesPerPixel = 2;
			else
				OutBytesPerPixel = 4;

			if ((fp = fopen(filename, "rb")) == NULL)
			{
				printf("Can't open file %s\n",filename);
				return NULL;
			}

			png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			if (png_ptr == NULL)
			{
				fclose(fp);
				return NULL;
			}

			info_ptr = png_create_info_struct(png_ptr);
			if (info_ptr == NULL)
			{
				fclose(fp);
				png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
				return NULL;
			}

			png_init_io(png_ptr, fp);
			png_set_sig_bytes(png_ptr, sig_read);
			png_read_info(png_ptr, info_ptr);
			png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_type, int_p_NULL, int_p_NULL);
			png_set_strip_16(png_ptr);
			png_set_packing(png_ptr);

			if (color_type == PNG_COLOR_TYPE_PALETTE)
				png_set_palette_to_rgb(png_ptr);
			if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
				png_set_expand_gray_1_2_4_to_8(png_ptr);
			if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
				png_set_tRNS_to_alpha(png_ptr);

			png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);

			line = (unsigned int*) malloc(width * 4);
			if (!line)
			{
				fclose(fp);
				png_destroy_read_struct(&png_ptr, png_infopp_NULL, png_infopp_NULL);
				return NULL;
			}

			Power2Width = PowerOfTwo(width);
			Power2Height = PowerOfTwo(height);
			Buffer = (unsigned short*)memalign(16,width*height*OutBytesPerPixel);

			for (y = 0; y < height; y++)
			{
				png_read_row(png_ptr, (unsigned char*) line, NULL);

				for (x = 0; x < width; x++)
				{
					unsigned int *Buffer32 = (unsigned int*)Buffer;
					unsigned int color32 = line[x];
					unsigned short color16;

					if(ColorMode == GU_PSM_5551)
					{
						color16 = Color8888To5551(color32);
						Buffer[y*width+x] = color16;
					}
					else if(ColorMode == GU_PSM_4444)
					{
						color16 = Color8888To4444(color32);
						Buffer[y*width+x] = color16;
					}
					else if(ColorMode == GU_PSM_5650)
					{
						color16 = Color8888To5650(color32);
						Buffer[y*width+x] = color16;
					}
					else
					{
						Buffer32[y*width+x] = color32;
					}
				}
			}

			free(line);
			png_read_end(png_ptr, info_ptr);
			png_destroy_read_struct(&png_ptr, &info_ptr, png_infopp_NULL);
			fclose(fp);

			Image *Image1 = (Image*)malloc(sizeof(Image));

			Image1->Width = width;
			Image1->Height = height;
			Image1->power2Width = Power2Width;
			Image1->power2Height = Power2Height;
			Image1->vRam = Vram;
			Image1->ColorMode = ColorMode;
			Image1->Swizzle = Swizzle;

			if (Vram == 1)
			{
				swizzled_pixels = (unsigned short*)getStaticVramTexture(Power2Width,Power2Height,ColorMode);//valloc(Image1->power2Height*Image1->power2Width*OutBytesPerPixel);

			}else
			{
				swizzled_pixels = (unsigned short*)memalign(16,Image1->power2Height*Image1->power2Width*OutBytesPerPixel);
			}

			if(Swizzle == 1)
			{
				swizzle_fast((u8*)swizzled_pixels,(const u8*)Buffer,Image1->power2Width*OutBytesPerPixel,Image1->power2Height);

				// 512*2 because swizzle operates in bytes, and each pixel in a 16-bit texture is 2 bytes
				sceKernelDcacheWritebackAll();

				Image1->ImageData = swizzled_pixels;
				free(Buffer);
			}else
			{
				Image1->ImageData = Buffer;
			}

			return Image1;
		}

		Image* TextureManager::CreateImage(int width,int height,int ColorMode,int Vram)
		{
			Image *Image1 = (Image*)malloc(sizeof(Image));

			Image1->Width = width;
			Image1->Height = height;
			Image1->power2Width = width;
			Image1->power2Height = height;
			Image1->vRam = Vram;
			Image1->ColorMode = ColorMode;

			int OutBytesPerPixel = 0;

			if(ColorMode == GU_PSM_4444 || ColorMode == GU_PSM_5650 || ColorMode == GU_PSM_5551)
				OutBytesPerPixel = 2;
			else
				OutBytesPerPixel = 4;

			if (Vram == 1)
			{
				Image1->ImageData = (unsigned short*)getStaticVramTexture(width,height,ColorMode);

			}else
			{
				Image1->ImageData = (unsigned short*)memalign(16,Image1->power2Height*Image1->power2Width*OutBytesPerPixel);
			}

			return Image1;*/
		}
	}
}
