#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <string.h>

namespace Aurora
{
	namespace Graphics
	{
		enum ImageLocation
		{
			Ram,
			Vram
		};

		class Image
		{
		public:			

			int _width;
			int _height;
			int _textureWidth;
			int _textureHeight;

			unsigned int _id;
			unsigned int _id2;
			unsigned char *_pixels;

			std::string _fileName;

			Image();
			~Image();

			bool loadImageFromFile(std::string fileName);
			bool loadImageFromMemory(std::string newName,void *data,std::size_t size);
			
			unsigned int getColour(int x,int y);
			void setColour(int x,int y, unsigned int color);
			void getColour(int x,int y,unsigned char &r,unsigned char &g,unsigned char &b,unsigned char &a);
		};
	}
}

#endif