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

#ifndef NOISEPP_MATH_H
#define NOISEPP_MATH_H

#include "NoisePrerequisites.h"

namespace noisepp
{
	/** Provides some math functions used by Noise++
	*/
	class Math
	{
		public:
			/// Performs linear interpolation
			static NOISEPP_INLINE Real InterpLinear (Real left, Real right, Real a)
			{
				return ((Real(1) - a) * left) + (a * right);
			}
			/// Performs cubic interpolation
			static NOISEPP_INLINE Real InterpCubic (Real v0, Real v1, Real v2, Real v3, Real a)
			{
				const Real x = v3 - v2 - v0 + v1;
				return x * a * a * a + (v0 - v1 - x) * a * a + (v2 - v0) * a + v1;
			}
			/// Calculates a third-order interpolant
			static NOISEPP_INLINE Real CubicCurve3 (Real a)
			{
				return (a * a * (Real(3) - Real(2) * a));
			}
			/// Calculates a fifth-order interpolant
			static NOISEPP_INLINE Real CubicCurve5 (Real a)
			{
				const Real a3 = a * a * a;
				const Real a4 = a3 * a;
				const Real a5 = a4 * a;
				return Real(10) * a3 - Real(15) * a4 + Real(6) * a5;
			}
			/// Clamps the parameter into integer range
			static NOISEPP_INLINE Real MakeInt32Range (Real n)
			{
				if (n >= Real(1073741824.0))
				{
					return (Real(2.0) * fmod (n, Real(1073741824.0))) - Real(1073741824.0);
				}
				else if (n <= Real(-1073741824.0))
				{
					return (Real(2.0) * fmod (n, Real(1073741824.0))) + Real(1073741824.0);
				}
				else
					return n;
			}
	};
};

#endif
