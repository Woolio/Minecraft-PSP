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

#ifndef NOISEPP_BILLOW_H
#define NOISEPP_BILLOW_H

#include "NoisePerlin.h"

namespace noisepp
{
	typedef PerlinModuleBase BillowModuleBase;

	class BillowElement1D : public PipelineElement1D
	{
		private:
			struct Octave
			{
				int seed;
				Real scale;
				Real persistence;
			};
			Octave *mOctaves;
			size_t mOctaveCount;
			int mQuality;
			Real mScale;

			NOISEPP_INLINE Real calculateGradient (Real x, int seed) const
			{
				if (mQuality == NOISE_QUALITY_STD)
					return Generator1D::calcGradientCoherentNoiseStd (x, seed, mScale);
				else if (mQuality == NOISE_QUALITY_HIGH)
					return Generator1D::calcGradientCoherentNoiseHigh (x, seed, mScale);
				else if (mQuality == NOISE_QUALITY_LOW)
					return Generator1D::calcGradientCoherentNoiseLow (x, seed, mScale);
				else if (mQuality == NOISE_QUALITY_FAST_STD)
					return Generator1D::calcGradientCoherentFastNoiseStd (x, seed, mScale);
				else if (mQuality == NOISE_QUALITY_FAST_HIGH)
					return Generator1D::calcGradientCoherentFastNoiseHigh (x, seed, mScale);
				else
					return Generator1D::calcGradientCoherentFastNoiseLow (x, seed, mScale);
			}
		public:
			BillowElement1D (size_t octaves, Real frequency, Real lacunarity, Real persistence, int mainSeed, int quality, Real nscale) : mOctaveCount(octaves), mQuality(quality), mScale(nscale)
			{
				if (quality > NOISE_QUALITY_HIGH)
					mScale *= FAST_NOISE_SCALE_FACTOR;
				mOctaves = new Octave[mOctaveCount];
				Real curPersistence = 1.0;
				int seed;
				Real scale = frequency;
				for (size_t o=0;o<mOctaveCount;++o)
				{
					seed = (mainSeed + int(o)) & 0xffffffff;
					mOctaves[o].persistence = curPersistence;
					mOctaves[o].scale = scale;
					mOctaves[o].seed = seed;

					scale *= lacunarity;
					curPersistence *= persistence;
				}
			}
			virtual ~BillowElement1D ()
			{
				delete mOctaves;
				mOctaves = NULL;
			}
			virtual Real getValue (Real x, Cache *cache) const
			{
				Real value = 0.5;
				Real signal = 1.0;

				for (size_t o=0;o<mOctaveCount;++o)
				{
					const Real nx = Math::MakeInt32Range (x * mOctaves[o].scale);
					signal = calculateGradient(nx, mOctaves[o].seed);
					signal = Real(2.0) * std::fabs (signal) - Real(1.0);

					value += signal * mOctaves[o].persistence;
				}

				return value;
			}
	};

	class BillowElement2D : public PipelineElement2D
	{
		private:
			struct Octave
			{
				int seed;
				Real scale;
				Real persistence;
			};
			Octave *mOctaves;
			size_t mOctaveCount;
			int mQuality;
			Real mScale;

			NOISEPP_INLINE Real calculateGradient (Real x, Real y, int seed) const
			{
				if (mQuality == NOISE_QUALITY_STD)
					return Generator2D::calcGradientCoherentNoiseStd (x, y, seed, mScale);
				else if (mQuality == NOISE_QUALITY_HIGH)
					return Generator2D::calcGradientCoherentNoiseHigh (x, y, seed, mScale);
				else if (mQuality == NOISE_QUALITY_LOW)
					return Generator2D::calcGradientCoherentNoiseLow (x, y, seed, mScale);
				else if (mQuality == NOISE_QUALITY_FAST_STD)
					return Generator2D::calcGradientCoherentFastNoiseStd (x, y, seed, mScale);
				else if (mQuality == NOISE_QUALITY_FAST_HIGH)
					return Generator2D::calcGradientCoherentFastNoiseHigh (x, y, seed, mScale);
				else
					return Generator2D::calcGradientCoherentFastNoiseLow (x, y, seed, mScale);
			}
		public:
			BillowElement2D (size_t octaves, Real frequency, Real lacunarity, Real persistence, int mainSeed, int quality, Real nscale) : mOctaveCount(octaves), mQuality(quality), mScale(nscale)
			{
				if (quality > NOISE_QUALITY_HIGH)
					mScale *= FAST_NOISE_SCALE_FACTOR;
				mOctaves = new Octave[mOctaveCount];
				Real curPersistence = 1.0;
				int seed;
				Real scale = frequency;
				for (size_t o=0;o<mOctaveCount;++o)
				{
					seed = (mainSeed + int(o)) & 0xffffffff;
					mOctaves[o].persistence = curPersistence;
					mOctaves[o].scale = scale;
					mOctaves[o].seed = seed;

					scale *= lacunarity;
					curPersistence *= persistence;
				}
			}
			virtual ~BillowElement2D ()
			{
				delete mOctaves;
				mOctaves = NULL;
			}
			virtual Real getValue (Real x, Real y, Cache *cache) const
			{
				Real value = 0.5;
				Real signal = 1.0;

				for (size_t o=0;o<mOctaveCount;++o)
				{
					const Real nx = Math::MakeInt32Range (x * mOctaves[o].scale);
					const Real ny = Math::MakeInt32Range (y * mOctaves[o].scale);
					signal = calculateGradient(nx, ny, mOctaves[o].seed);
					signal = Real(2.0) * std::fabs (signal) - Real(1.0);

					value += signal * mOctaves[o].persistence;
				}

				return value;
			}
	};

	class BillowElement3D : public PipelineElement3D
	{
		private:
			struct Octave
			{
				int seed;
				Real scale;
				Real persistence;
			};
			Octave *mOctaves;
			size_t mOctaveCount;
			int mQuality;
			Real mScale;

			NOISEPP_INLINE Real calculateGradient (Real x, Real y, Real z, int seed) const
			{
				if (mQuality == NOISE_QUALITY_STD)
					return Generator3D::calcGradientCoherentNoiseStd (x, y, z, seed, mScale);
				else if (mQuality == NOISE_QUALITY_HIGH)
					return Generator3D::calcGradientCoherentNoiseHigh (x, y, z, seed, mScale);
				else if (mQuality == NOISE_QUALITY_LOW)
					return Generator3D::calcGradientCoherentNoiseLow (x, y, z, seed, mScale);
				else if (mQuality == NOISE_QUALITY_FAST_STD)
					return Generator3D::calcGradientCoherentFastNoiseStd (x, y, z, seed, mScale);
				else if (mQuality == NOISE_QUALITY_FAST_HIGH)
					return Generator3D::calcGradientCoherentFastNoiseHigh (x, y, z, seed, mScale);
				else
					return Generator3D::calcGradientCoherentFastNoiseLow (x, y, z, seed, mScale);
			}
		public:
			BillowElement3D (size_t octaves, Real frequency, Real lacunarity, Real persistence, int mainSeed, int quality, Real nscale) : mOctaveCount(octaves), mQuality(quality), mScale(nscale)
			{
				if (quality > NOISE_QUALITY_HIGH)
					mScale *= FAST_NOISE_SCALE_FACTOR;
				mOctaves = new Octave[mOctaveCount];
				Real curPersistence = 1.0;
				int seed;
				Real scale = frequency;
				for (size_t o=0;o<mOctaveCount;++o)
				{
					seed = (mainSeed + int(o)) & 0xffffffff;
					mOctaves[o].persistence = curPersistence;
					mOctaves[o].scale = scale;
					mOctaves[o].seed = seed;

					scale *= lacunarity;
					curPersistence *= persistence;
				}
			}
			virtual ~BillowElement3D ()
			{
				delete mOctaves;
				mOctaves = NULL;
			}
			virtual Real getValue (Real x, Real y, Real z, Cache *cache) const
			{
				Real value = 0.5;
				Real signal = 1.0;

				for (size_t o=0;o<mOctaveCount;++o)
				{
					const Real nx = Math::MakeInt32Range (x * mOctaves[o].scale);
					const Real ny = Math::MakeInt32Range (y * mOctaves[o].scale);
					const Real nz = Math::MakeInt32Range (z * mOctaves[o].scale);
					signal = calculateGradient(nx, ny, nz, mOctaves[o].seed);
					signal = Real(2.0) * std::fabs (signal) - Real(1.0);

					value += signal * mOctaves[o].persistence;
				}

				return value;
			}
	};

	/** Module for generating "billowy" perlin noise.
		Generates "billowy" perlin noise.
	*/
	class BillowModule : public BillowModuleBase
	{
		public:
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline1D *pipe) const
			{
				return pipe->addElement (this, new BillowElement1D(mOctaveCount, mFrequency, mLacunarity, mPersistence, mSeed+pipe->getSeed(), mQuality, mScale));
			}
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline2D *pipe) const
			{
				return pipe->addElement (this, new BillowElement2D(mOctaveCount, mFrequency, mLacunarity, mPersistence, mSeed+pipe->getSeed(), mQuality, mScale));
			}
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline3D *pipe) const
			{
				return pipe->addElement (this, new BillowElement3D(mOctaveCount, mFrequency, mLacunarity, mPersistence, mSeed+pipe->getSeed(), mQuality, mScale));
			}
			/// @copydoc noisepp::Module::getType()
			ModuleTypeId getType() const { return MODULE_BILLOW; }
	};
};

#endif

