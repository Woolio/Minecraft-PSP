#include <Aurora/Graphics/Image.h>
#include <Aurora/System/FileManager.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace Aurora
{
	namespace Graphics
	{
		Image::Image()
		{
			_width = 0;
			_height = 0;
			_textureHeight = 0;
			_textureWidth = 0;
			_id = 0;
			_pixels = 0;

			_fileName = "";
		}

		Image::~Image()
		{
			if(_pixels != 0)
			{
				delete [] _pixels;
			}
		}

		void Image::getColour(int x,int y,unsigned char &r,unsigned char &g,unsigned char &b,unsigned char &a)
		{
			r = _pixels[(x * 4) + ((y * (_width*4)))];
			g = _pixels[(1+(x * 4)) + ((y * (_width*4)))];
			b = _pixels[(2+(x * 4)) + ((y * (_width*4)))];
			a = _pixels[(3+(x * 4)) + ((y * (_width*4)))];
		}

		unsigned int Image::getColour(int x,int y)
		{
			unsigned char r = _pixels[(x * 4) + ((y * (_width*4)))];
			unsigned char g = _pixels[(1+(x * 4)) + ((y * (_width*4)))];
			unsigned char b = _pixels[(2+(x * 4)) + ((y * (_width*4)))];
			unsigned char a = _pixels[(3+(x * 4)) + ((y * (_width*4)))];

			return (r) | (g << 8) | (b << 16) | (a << 24);
		}
		
		void Image::setColour(int x,int y, unsigned int color)
		{
			_pixels[(x * 4) + ((y * (_width*4)))] = color;
			_pixels[(1+(x * 4)) + ((y * (_width*4)))] = color >> 8;
			_pixels[(2+(x * 4)) + ((y * (_width*4)))] = color >> 16;
			_pixels[(3+(x * 4)) + ((y * (_width*4)))] = color >> 24;
		}

		bool Image::loadImageFromFile(std::string fileName)
		{
			_fileName = fileName;

			if(_pixels != 0)
			{
				delete [] _pixels;
			}
			
			System::File* file = System::FileManager::Instance()->GetFile(_fileName);

			if(file == 0)
				return false;

			file->Open();

			int dataSize = 0;
			unsigned char* _buffer = file->GetData(dataSize);

			file->Close();
			delete file;

			return loadImageFromMemory(fileName,_buffer,dataSize);

			/*int imgWidth, imgHeight, imgChannels;
			unsigned char* ptr = stbi_load(fileName.c_str(), &imgWidth, &imgHeight, &imgChannels, STBI_rgb_alpha);

			if (ptr && imgWidth && imgHeight)
			{
				// Assign the image properties
				_width  = imgWidth;
				_height = imgHeight;

				// Copy the loaded pixels to the pixel buffer
				_pixels = new unsigned char[_width * _height * 4];
				memcpy(_pixels, ptr, _width * _height * 4);

				// Free the loaded pixels (they are now in our own pixel buffer)
				stbi_image_free(ptr);

				return true;
			}
			else
			{
				return false;//error
			}*/
		}

		bool Image::loadImageFromMemory(std::string newName,void *data,std::size_t size)
		{
			if (data && size)
			{
				_fileName = newName;

				if(_pixels != 0)
				{
					delete [] _pixels;
				}

				// Load the image and get a pointer to the pixels in memory
				int imgWidth, imgHeight, imgChannels;
				const unsigned char* buffer = static_cast<const unsigned char*>(data);
				unsigned char* ptr = stbi_load_from_memory(buffer, static_cast<int>(size), &imgWidth, &imgHeight, &imgChannels, STBI_rgb_alpha);

				if (ptr && imgWidth && imgHeight)
				{
					// Assign the image properties
					_width  = imgWidth;
					_height = imgHeight;

					// Copy the loaded pixels to the pixel buffer
					_pixels = new unsigned char[_width * _height * 4];
					memcpy(_pixels, ptr, _width * _height * 4);

					// Free the loaded pixels (they are now in our own pixel buffer)
					stbi_image_free(ptr);
					delete [] data;

					return true;
				}
				else
				{
					return false;//error
				}
			}

			return false;
		}
	}
}
