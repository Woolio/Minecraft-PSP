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

#ifndef NOISEIMAGE_H
#define NOISEIMAGE_H

#include <cassert>

namespace noisepp
{
namespace utils
{

/// Class representing an 24-bit image.
class Image
{
	public:
		/// Constructor.
		Image();
		/// Creates the image with the specified parameters.
		/// @param width Width of the image.
		/// @param height Height of the image.
		void create (int width, int height);
		/// Clears the image data.
		void clear ();

		/// Returns a pointer to the pixel data.
		unsigned char *getPixelData () const
		{
			return mData;
		}
		/// Returns a pointer to the pixel data at the specified row.
		/// @param i The row number.
		unsigned char *getPixelData (int i) const
		{
			assert (i < mWidth*mHeight);
			return mData+i*3;
		}
		/// Returns a pointer to the pixel data at the specified position.
		unsigned char *getPixelData (int x, int y) const
		{
			return mData+(y*mWidth+x)*3;
		}
		/// Returns the width of the image.
		int getWidth () const
		{
			return mWidth;
		}
		/// Returns the height of the image.
		int getHeight () const
		{
			return mHeight;
		}
		/// Destructor.
		~Image();
		/// Writes the image to the specified bitmap file.
		bool saveBMP (const char *filename);
	protected:
	private:
		unsigned char *mData;
		int mWidth, mHeight;
};

};
};

#endif // NOISEIMAGE_H
