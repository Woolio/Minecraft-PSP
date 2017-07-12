// Noise++ Library
// Copyright (c) 2008, Urs C. Hanselmann
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright notice,
//      this list of conditions and the following disclaimer in the documentation
//      and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#include "NoiseImage.h"
#include "NoiseOutStream.h"

namespace noisepp
{
namespace utils
{

Image::Image() : mData(0), mWidth(0), mHeight(0)
{
}

void Image::create (int width, int height)
{
//	NoiseAssert (width > 0, width);
	//NoiseAssert (height > 0, height);
	mWidth = width;
	mHeight = height;

	mData = new unsigned char [mWidth*mHeight*3];
}

void Image::clear ()
{
	if (mData)
	{
		delete[] mData;
		mData = 0;
	}
	mWidth = 0;
	mHeight = 0;
}

bool Image::saveBMP (const char *filename)
{//
	//NoiseAssert (mData != NULL, mData);
	//NoiseAssert (mWidth > 0, mWidth);
	//NoiseAssert (mHeight > 0, mHeight);
	FileOutStream stream(filename);
	if (!stream.isOpen())
		return false;
	// BMP Header
	stream.write ("BM", 2);
	unsigned size = 14 + 40 + mWidth * mHeight * 3;
	stream.write (size);
	unsigned res = 0;
	stream.write (res);
	unsigned offset = 54;
	stream.write (offset);

	// BMP properties
	unsigned psize = 40;
	stream.write (psize);
	int w = mWidth;
	stream.write (w);
	int h = mHeight;
	stream.write (h);
	unsigned short planes = 1;
	stream.write (planes);
	unsigned short bpp = 24;
	stream.write (bpp);
	unsigned compression = 0;
	stream.write (compression);
	unsigned sizeImage = mWidth * mHeight * 3;
	stream.write (sizeImage);
	int ppm = 0;
	stream.write (ppm);
	stream.write (ppm);
	unsigned clrUsed = 0;
	stream.write (clrUsed);
	unsigned clrImportant = 0;
	stream.write (clrImportant);
	assert (stream.tell() == offset);

	for (int y=mHeight-1;y>=0;--y)
	{
		unsigned char *buffer = mData+y*mWidth*3;
		for (int x=0;x<mWidth;++x)
		{
			unsigned char r = *buffer++;
			unsigned char g = *buffer++;
			unsigned char b = *buffer++;
			stream.write (b);
			stream.write (g);
			stream.write (r);
		}
	}

	stream.close ();
	return true;
}

Image::~Image()
{
	clear ();
}

};
};
