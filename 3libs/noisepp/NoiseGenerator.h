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

#ifndef NOISEPP_GENERATOR_H
#define NOISEPP_GENERATOR_H

#include "NoiseMath.h"
#include "NoiseVectorTable.h"
#include "NoisePlatform.h"

namespace noisepp
{
	#define NOISE_GENERATOR_INTEGER_CLAMP_X \
		const int x0 = (x > Real(0.0) ? (int)x : (int)x - 1); \
		const int x1 = x0 + 1;
	#define NOISE_GENERATOR_INTEGER_CLAMP_Y \
		const int y0 = (y > Real(0.0) ? (int)y : (int)y - 1); \
		const int y1 = y0 + 1;
	#define NOISE_GENERATOR_INTEGER_CLAMP_Z \
		const int z0 = (z > Real(0.0) ? (int)z : (int)z - 1); \
		const int z1 = z0 + 1;

	#define NOISE_GENERATOR_INTEGER_CLAMP_1D \
		NOISE_GENERATOR_INTEGER_CLAMP_X

	#define NOISE_GENERATOR_INTEGER_CLAMP_2D \
		NOISE_GENERATOR_INTEGER_CLAMP_X \
		NOISE_GENERATOR_INTEGER_CLAMP_Y

	#define NOISE_GENERATOR_INTEGER_CLAMP_3D \
		NOISE_GENERATOR_INTEGER_CLAMP_X \
		NOISE_GENERATOR_INTEGER_CLAMP_Y \
		NOISE_GENERATOR_INTEGER_CLAMP_Z

	enum { NOISE_QUALITY_LOW=0, NOISE_QUALITY_STD=1, NOISE_QUALITY_HIGH=2, NOISE_QUALITY_FAST_LOW=3, NOISE_QUALITY_FAST_STD=4, NOISE_QUALITY_FAST_HIGH=5 };

	const int NOISE_X_FACTOR = 1619;
	const int NOISE_Y_FACTOR = 31337;
	const int NOISE_Z_FACTOR = 6971;
	const int NOISE_SEED_FACTOR = 1013;
	const int NOISE_SHIFT = 8;

	const Real FAST_NOISE_SCALE_FACTOR = 0.5;

	class Generator1D
	{
		private:
			static NOISEPP_INLINE Real calcGradientNoise (Real fx, int ix, int seed)
			{
				int vIndex = (NOISE_X_FACTOR * ix + NOISE_SEED_FACTOR * seed) & 0xffffffff;
				vIndex ^= (vIndex >> NOISE_SHIFT);
				vIndex &= 0xff;

				const Real xGradient = randomVectors3D[(vIndex<<2)];

				const Real xDelta = fx - Real(ix);
				return xDelta * xGradient;
			}

			static NOISEPP_INLINE Real interpGradientCoherentNoise (Real x, int x0, int x1, Real xs, int seed, Real scale)
			{
				Real n0, n1;
				n0 = calcGradientNoise(x, x0, seed);
				n1 = calcGradientNoise(x, x1, seed);
				return Math::InterpLinear (n0, n1, xs) * scale;
			}

			static NOISEPP_INLINE Real calcGradientFastNoise (Real fx, int ix, int seed)
			{
				int vIndex = (NOISE_X_FACTOR * ix + NOISE_SEED_FACTOR * seed) & 0xffffffff;
				vIndex ^= (vIndex >> NOISE_SHIFT);
				vIndex &= 0xff;
				return gradientVector[vIndex];
			}

			static NOISEPP_INLINE Real interpGradientCoherentFastNoise (Real x, int x0, int x1, Real xs, int seed, Real scale)
			{
				Real n0, n1;
				n0 = calcGradientFastNoise(x, x0, seed);
				n1 = calcGradientFastNoise(x, x1, seed);
				return Math::InterpLinear (n0, n1, xs) * scale;
			}

			static NOISEPP_INLINE int intNoise (int x, int seed)
			{
				int n = (NOISE_X_FACTOR * x + NOISE_SEED_FACTOR * seed) & 0x7fffffff;
				n = (n >> 13) ^ n;
				return (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
			}
		public:
			static NOISEPP_INLINE Real calcGradientCoherentNoiseHigh (Real x, int seed, Real scale)
			{
				NOISE_GENERATOR_INTEGER_CLAMP_1D;

				const Real xs = Math::CubicCurve5 (x - Real(x0));

				return interpGradientCoherentNoise (x, x0, x1, xs, seed, scale);
			}

			static NOISEPP_INLINE Real calcGradientCoherentNoiseStd (Real x, int seed, Real scale)
			{
				NOISE_GENERATOR_INTEGER_CLAMP_1D;

				const Real xs = Math::CubicCurve3 (x - Real(x0));

				return interpGradientCoherentNoise (x, x0, x1, xs, seed, scale);
			}

			static NOISEPP_INLINE Real calcGradientCoherentNoiseLow (Real x, int seed, Real scale)
			{
				NOISE_GENERATOR_INTEGER_CLAMP_1D;

				const Real xs = x - Real(x0);

				return interpGradientCoherentNoise (x, x0, x1, xs, seed, scale);
			}

			static NOISEPP_INLINE Real calcGradientCoherentFastNoiseHigh (Real x, int seed, Real scale)
			{
				NOISE_GENERATOR_INTEGER_CLAMP_1D;

				const Real xs = Math::CubicCurve5 (x - Real(x0));

				return interpGradientCoherentFastNoise (x, x0, x1, xs, seed, scale);
			}

			static NOISEPP_INLINE Real calcGradientCoherentFastNoiseStd (Real x, int seed, Real scale)
			{
				NOISE_GENERATOR_INTEGER_CLAMP_1D;

				const Real xs = Math::CubicCurve3 (x - Real(x0));

				return interpGradientCoherentFastNoise (x, x0, x1, xs, seed, scale);
			}

			static NOISEPP_INLINE Real calcGradientCoherentFastNoiseLow (Real x, int seed, Real scale)
			{
				NOISE_GENERATOR_INTEGER_CLAMP_1D;

				const Real xs = x - Real(x0);

				return interpGradientCoherentFastNoise (x, x0, x1, xs, seed, scale);
			}

			static NOISEPP_INLINE Real calcNoise (int x, int seed)
			{
				return Real(1.0) - ((Real)intNoise(x, seed) / Real(1073741824.0));
			}
	};
	class Generator2D
	{
		private:
			static NOISEPP_INLINE Real calcGradientNoise (Real fx, Real fy, int ix, int iy, int seed)
			{
				int vIndex = (NOISE_X_FACTOR * ix + NOISE_Y_FACTOR * iy + NOISE_SEED_FACTOR * seed) & 0xffffffff;
				vIndex ^= (vIndex >> NOISE_SHIFT);
				vIndex &= 0xff;

				const Real xGradient = randomVectors3D[(vIndex<<2)];
				const Real yGradient = randomVectors3D[(vIndex<<2)+1];

				const Real xDelta = fx - Real(ix);
				const Real yDelta = fy - Real(iy);
				return (xGradient * xDelta + yGradient * yDelta);
			}

			static NOISEPP_INLINE Real interpGradientCoherentNoise (Real x, Real y, int x0, int x1, int y0, int y1, Real xs, Real ys, int seed, Real scale)
			{
				Real n0, n1, ix0, ix1;
				n0 = calcGradientNoise(x, y, x0, y0, seed);
				n1 = calcGradientNoise(x, y, x1, y0, seed);
				ix0 = Math::InterpLinear (n0, n1, xs);
				n0 = calcGradientNoise(x, y, x0, y1, seed);
				n1 = calcGradientNoise(x, y, x1, y1, seed);
				ix1 = Math::InterpLinear (n0, n1, xs);
				return Math::InterpLinear (ix0, ix1, ys) * scale;
			}

			static NOISEPP_INLINE Real calcGradientFastNoise (Real fx, Real fy, int ix, int iy, int seed)
			{
				int vIndex = (NOISE_X_FACTOR * ix + NOISE_Y_FACTOR * iy + NOISE_SEED_FACTOR * seed) & 0xffffffff;
				vIndex ^= (vIndex >> NOISE_SHIFT);
				vIndex &= 0xff;
				return gradientVector[vIndex];
			}

			static NOISEPP_INLINE Real interpGradientCoherentFastNoise (Real x, Real y, int x0, int x1, int y0, int y1, Real xs, Real ys, int seed, Real scale)
			{
				Real n0, n1, ix0, ix1;
				n0 = calcGradientFastNoise(x, y, x0, y0, seed);
				n1 = calcGradientFastNoise(x, y, x1, y0, seed);
				ix0 = Math::InterpLinear (n0, n1, xs);
				n0 = calcGradientFastNoise(x, y, x0, y1, seed);
				n1 = calcGradientFastNoise(x, y, x1, y1, seed);
				ix1 = Math::InterpLinear (n0, n1, xs);
				return Math::InterpLinear (ix0, ix1, ys) * scale;
			}

			static NOISEPP_INLINE int intNoise (int x, int y, int seed)
			{
				int n = (NOISE_X_FACTOR * x + NOISE_Y_FACTOR * y + NOISE_SEED_FACTOR * seed) & 0x7fffffff;
				n = (n >> 13) ^ n;
				return (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
			}
		public:
			static NOISEPP_INLINE Real calcGradientCoherentNoiseHigh (Real x, Real y, int seed, Real scale)
			{
				NOISE_GENERATOR_INTEGER_CLAMP_2D;

				const Real xs = Math::CubicCurve5 (x - Real(x0));
				const Real ys = Math::CubicCurve5 (y - Real(y0));

				return interpGradientCoherentNoise (x, y, x0, x1, y0, y1, xs, ys, seed, scale);
			}

			static NOISEPP_INLINE Real calcGradientCoherentNoiseStd (Real x, Real y, int seed, Real scale)
			{
				NOISE_GENERATOR_INTEGER_CLAMP_2D;

				const Real xs = Math::CubicCurve3 (x - Real(x0));
				const Real ys = Math::CubicCurve3 (y - Real(y0));

				return interpGradientCoherentNoise (x, y, x0, x1, y0, y1, xs, ys, seed, scale);
			}

			static NOISEPP_INLINE Real calcGradientCoherentNoiseLow (Real x, Real y, int seed, Real scale)
			{
				NOISE_GENERATOR_INTEGER_CLAMP_2D;

				const Real xs = x - Real(x0);
				const Real ys = y - Real(y0);

				return interpGradientCoherentNoise (x, y, x0, x1, y0, y1, xs, ys, seed, scale);
			}

			static NOISEPP_INLINE Real calcGradientCoherentFastNoiseHigh (Real x, Real y, int seed, Real scale)
			{
				NOISE_GENERATOR_INTEGER_CLAMP_2D;

				const Real xs = Math::CubicCurve5 (x - Real(x0));
				const Real ys = Math::CubicCurve5 (y - Real(y0));

				return interpGradientCoherentFastNoise (x, y, x0, x1, y0, y1, xs, ys, seed, scale);
			}

			static NOISEPP_INLINE Real calcGradientCoherentFastNoiseStd (Real x, Real y, int seed, Real scale)
			{
				NOISE_GENERATOR_INTEGER_CLAMP_2D;

				const Real xs = Math::CubicCurve3 (x - Real(x0));
				const Real ys = Math::CubicCurve3 (y - Real(y0));

				return interpGradientCoherentFastNoise (x, y, x0, x1, y0, y1, xs, ys, seed, scale);
			}

			static NOISEPP_INLINE Real calcGradientCoherentFastNoiseLow (Real x, Real y, int seed, Real scale)
			{
				NOISE_GENERATOR_INTEGER_CLAMP_2D;

				const Real xs = x - Real(x0);
				const Real ys = y - Real(y0);

				return interpGradientCoherentFastNoise (x, y, x0, x1, y0, y1, xs, ys, seed, scale);
			}

			static NOISEPP_INLINE Real calcNoise (int x, int y, int seed=0)
			{
				return Real(1.0) - ((Real)intNoise(x, y, seed) / Real(1073741824.0));
			}
	};
	class Generator3D
	{
		private:
			static NOISEPP_INLINE Real calcGradientNoise (Real fx, Real fy, Real fz, int ix, int iy, int iz, int seed)
			{
				int vIndex = (NOISE_X_FACTOR * ix + NOISE_Y_FACTOR * iy + NOISE_Z_FACTOR * iz + NOISE_SEED_FACTOR * seed) & 0xffffffff;
				vIndex ^= (vIndex >> NOISE_SHIFT);
				vIndex &= 0xff;

				const Real xGradient = randomVectors3D[(vIndex<<2)];
				const Real yGradient = randomVectors3D[(vIndex<<2)+1];
				const Real zGradient = randomVectors3D[(vIndex<<2)+2];

				const Real xDelta = fx - Real(ix);
				const Real yDelta = fy - Real(iy);
				const Real zDelta = fz - Real(iz);
				return (xGradient * xDelta + yGradient * yDelta + zGradient * zDelta);
			}

			static NOISEPP_INLINE Real interpGradientCoherentNoise (Real x, Real y, Real z, int x0, int x1, int y0, int y1, int z0, int z1, Real xs, Real ys, Real zs, int seed, Real scale)
			{
				Real n0, n1, ix0, ix1, iy0, iy1;
				n0 = calcGradientNoise(x, y, z, x0, y0, z0, seed);
				n1 = calcGradientNoise(x, y, z, x1, y0, z0, seed);
				ix0 = Math::InterpLinear (n0, n1, xs);
				n0 = calcGradientNoise(x, y, z, x0, y1, z0, seed);
				n1 = calcGradientNoise(x, y, z, x1, y1, z0, seed);
				ix1 = Math::InterpLinear (n0, n1, xs);
				iy0 = Math::InterpLinear (ix0, ix1, ys);
				n0 = calcGradientNoise(x, y, z, x0, y0, z1, seed);
				n1 = calcGradientNoise(x, y, z, x1, y0, z1, seed);
				ix0 = Math::InterpLinear (n0, n1, xs);
				n0 = calcGradientNoise(x, y, z, x0, y1, z1, seed);
				n1 = calcGradientNoise(x, y, z, x1, y1, z1, seed);
				ix1 = Math::InterpLinear (n0, n1, xs);
				iy1 = Math::InterpLinear (ix0, ix1, ys);

				return Math::InterpLinear (iy0, iy1, zs) * scale;
			}

			static NOISEPP_INLINE Real interpGradientCoherentFastNoise (Real x, Real y, Real z, int x0, int x1, int y0, int y1, int z0, int z1, Real xs, Real ys, Real zs, int seed, Real scale)
			{
				Real n0, n1, ix0, ix1, iy0, iy1;
				n0 = calcGradientFastNoise(x, y, z, x0, y0, z0, seed);
				n1 = calcGradientFastNoise(x, y, z, x1, y0, z0, seed);
				ix0 = Math::InterpLinear (n0, n1, xs);
				n0 = calcGradientFastNoise(x, y, z, x0, y1, z0, seed);
				n1 = calcGradientFastNoise(x, y, z, x1, y1, z0, seed);
				ix1 = Math::InterpLinear (n0, n1, xs);
				iy0 = Math::InterpLinear (ix0, ix1, ys);
				n0 = calcGradientFastNoise(x, y, z, x0, y0, z1, seed);
				n1 = calcGradientFastNoise(x, y, z, x1, y0, z1, seed);
				ix0 = Math::InterpLinear (n0, n1, xs);
				n0 = calcGradientFastNoise(x, y, z, x0, y1, z1, seed);
				n1 = calcGradientFastNoise(x, y, z, x1, y1, z1, seed);
				ix1 = Math::InterpLinear (n0, n1, xs);
				iy1 = Math::InterpLinear (ix0, ix1, ys);

				return Math::InterpLinear (iy0, iy1, zs) * scale;
			}

			static NOISEPP_INLINE Real calcGradientFastNoise (Real fx, Real fy, Real fz, int ix, int iy, int iz, int seed)
			{
				int vIndex = (NOISE_X_FACTOR * ix + NOISE_Y_FACTOR * iy + NOISE_Z_FACTOR * iz + NOISE_SEED_FACTOR * seed) & 0xffffffff;
				vIndex ^= (vIndex >> NOISE_SHIFT);
				vIndex &= 0xff;
				return gradientVector[vIndex];
			}

			static NOISEPP_INLINE int intNoise (int x, int y, int z, int seed)
			{
				int n = (NOISE_X_FACTOR * x + NOISE_Y_FACTOR * y + NOISE_Z_FACTOR * z + NOISE_SEED_FACTOR * seed) & 0x7fffffff;
				n = (n >> 13) ^ n;
				return (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
			}
		public:
			static NOISEPP_INLINE Real calcGradientCoherentNoiseHigh (Real x, Real y, Real z, int seed, Real scale)
			{
				NOISE_GENERATOR_INTEGER_CLAMP_3D;

				const Real xs = Math::CubicCurve5 (x - Real(x0));
				const Real ys = Math::CubicCurve5 (y - Real(y0));
				const Real zs = Math::CubicCurve5 (z - Real(z0));

				return interpGradientCoherentNoise(x, y, z, x0, x1, y0, y1, z0, z1, xs, ys, zs, seed, scale);
			}

			static NOISEPP_INLINE Real calcGradientCoherentNoiseStd (Real x, Real y, Real z, int seed, Real scale)
			{
				NOISE_GENERATOR_INTEGER_CLAMP_3D;

				const Real xs = Math::CubicCurve3 (x - Real(x0));
				const Real ys = Math::CubicCurve3 (y - Real(y0));
				const Real zs = Math::CubicCurve3 (z - Real(z0));

				return interpGradientCoherentNoise(x, y, z, x0, x1, y0, y1, z0, z1, xs, ys, zs, seed, scale);
			}

			static NOISEPP_INLINE Real calcGradientCoherentNoiseLow (Real x, Real y, Real z, int seed, Real scale)
			{
				NOISE_GENERATOR_INTEGER_CLAMP_3D;

				const Real xs = x - Real(x0);
				const Real ys = y - Real(y0);
				const Real zs = z - Real(z0);

				return interpGradientCoherentNoise(x, y, z, x0, x1, y0, y1, z0, z1, xs, ys, zs, seed, scale);
			}

			static NOISEPP_INLINE Real calcGradientCoherentFastNoiseHigh (Real x, Real y, Real z, int seed, Real scale)
			{
				NOISE_GENERATOR_INTEGER_CLAMP_3D;

				const Real xs = Math::CubicCurve5 (x - Real(x0));
				const Real ys = Math::CubicCurve5 (y - Real(y0));
				const Real zs = Math::CubicCurve5 (z - Real(z0));

				return interpGradientCoherentFastNoise(x, y, z, x0, x1, y0, y1, z0, z1, xs, ys, zs, seed, scale);
			}

			static NOISEPP_INLINE Real calcGradientCoherentFastNoiseStd (Real x, Real y, Real z, int seed, Real scale)
			{
				NOISE_GENERATOR_INTEGER_CLAMP_3D;

				const Real xs = Math::CubicCurve3 (x - Real(x0));
				const Real ys = Math::CubicCurve3 (y - Real(y0));
				const Real zs = Math::CubicCurve3 (z - Real(z0));

				return interpGradientCoherentFastNoise(x, y, z, x0, x1, y0, y1, z0, z1, xs, ys, zs, seed, scale);
			}

			static NOISEPP_INLINE Real calcGradientCoherentFastNoiseLow (Real x, Real y, Real z, int seed, Real scale)
			{
				NOISE_GENERATOR_INTEGER_CLAMP_3D;

				const Real xs = x - Real(x0);
				const Real ys = y - Real(y0);
				const Real zs = z - Real(z0);

				return interpGradientCoherentFastNoise(x, y, z, x0, x1, y0, y1, z0, z1, xs, ys, zs, seed, scale);
			}

			static NOISEPP_INLINE Real calcNoise (int x, int y, int z, int seed=0)
			{
				return Real(1.0) - ((Real)intNoise(x, y, z, seed) / Real(1073741824.0));
			}
	};
};

#endif
