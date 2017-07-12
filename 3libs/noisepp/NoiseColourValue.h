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

#ifndef NOISECOLOURVALUE_H
#define NOISECOLOURVALUE_H

namespace noisepp
{
namespace utils
{

/// Class representing a colour value.
class ColourValue
{
	public:
		/// Constructor.
		explicit ColourValue(float r=0.0f, float g=0.0f, float b=0.0f);

		/// Writes the RGB value to a buffer.
		void writeRGB (unsigned char *& buffer);

		/// Red value.
		float r;
		/// Green value.
		float g;
		/// Blue value.
		float b;

		/// Converts an normalized floating point(range 0.0 to 1.0) colour value to an unsigned byte(range 0 to 255).
		static unsigned char toUChar (float f);

		/// Adds two colour values together.
		inline ColourValue operator+ (const ColourValue& c) const
		{
			ColourValue result;
			result.r = r + c.r;
			result.g = g + c.g;
			result.b = b + c.b;
			return result;
		}
		/// Substract operator.
		inline ColourValue operator- (const ColourValue& c) const
		{
			ColourValue result;
			result.r = r - c.r;
			result.g = g - c.g;
			result.b = b - c.b;
			return result;
		}
		/// Mutliplication operator.
		inline ColourValue operator* (const ColourValue& c) const
		{
			ColourValue result;
			result.r = r * c.r;
			result.g = g * c.g;
			result.b = b * c.b;
			return result;
		}
		/// Division operator.
		inline ColourValue operator/ (const ColourValue& c) const
		{
			ColourValue result;
			result.r = r / c.r;
			result.g = g / c.g;
			result.b = b / c.b;
			return result;
		}
		/// Mutliplication operator.
		inline ColourValue operator* (float f) const
		{
			ColourValue result;
			result.r = r * f;
			result.g = g * f;
			result.b = b * f;
			return result;
		}
		/// Division operator.
		inline ColourValue operator/ (float f) const
		{
			ColourValue result;
			result.r = r / f;
			result.g = g / f;
			result.b = b / f;
			return result;
		}
		/// Addition operator.
		inline ColourValue &operator += (const ColourValue& c)
		{
			r += c.r;
			g += c.g;
			b += c.b;
			return *this;
		}
		/// Substract operator.
		inline ColourValue &operator -= (const ColourValue& c)
		{
			r -= c.r;
			g -= c.g;
			b -= c.b;
			return *this;
		}
		/// Multiplication operator.
		inline ColourValue &operator *= (const ColourValue& c)
		{
			r *= c.r;
			g *= c.g;
			b *= c.b;
			return *this;
		}
		/// Division operator.
		inline ColourValue &operator /= (const ColourValue& c)
		{
			r /= c.r;
			g /= c.g;
			b /= c.b;
			return *this;
		}
		/// Multiplication operator.
		inline ColourValue &operator *= (float f)
		{
			r *= f;
			g *= f;
			b *= f;
			return *this;
		}
		/// Division operator.
		inline ColourValue &operator /= (float f)
		{
			r /= f;
			g /= f;
			b /= f;
			return *this;
		}

	protected:
	private:
};

};
};

#endif // NOISECOLOURVALUE_H
