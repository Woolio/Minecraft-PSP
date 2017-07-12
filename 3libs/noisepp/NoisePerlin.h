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

#ifndef NOISEPP_PERLIN_H
#define NOISEPP_PERLIN_H

#include "NoisePrerequisites.h"
#include "NoiseModule.h"
#include "NoiseGenerator.h"
#include "NoisePipeline.h"

namespace noisepp
{
	/// Perlin base class.
	class PerlinModuleBase : public Module
	{
		protected:
			/// The noise frequency.
			Real mFrequency;
			/// The number of octaves.
			int mOctaveCount;
			/// The noise seed.
			int mSeed;
			/// The noise quality.
			int mQuality;
			/// The noise lacunarity.
			Real mLacunarity;
			/// The noise persistence.
			Real mPersistence;
			/// The noise scale factor.
			Real mScale;

		public:
			/// Constructor.
			PerlinModuleBase () :
			mFrequency(1.0), mOctaveCount(6), mSeed(0), mQuality(NOISE_QUALITY_STD), mLacunarity(2.0), mPersistence(0.05), mScale(2.12) {}
			virtual ~PerlinModuleBase () {}

			/// Sets the frequency.
			void setFrequency (Real v)
			{
				mFrequency = v;
			}
			/// Returns the frequency.
			Real getFrequency () const
			{
				return mFrequency;
			}
			/// Sets the number of octaves.
			void setOctaveCount (int v)
			{
				mOctaveCount = v;
			}
			/// Returns the number of octaves.
			int getOctaveCount () const
			{
				return mOctaveCount;
			}
			/// Sets the noise seed.
			void setSeed (int v)
			{
				mSeed = v;
			}
			/// Returns the noise seed.
			int getSeed () const
			{
				return mSeed;
			}
			/// Sets the noise quality.
			void setQuality (int v)
			{
				mQuality = v;
			}
			/// Returns the noise quality.
			int getQuality () const
			{
				return mQuality;
			}
			/// Sets the noise lacunarity.
			void setLacunarity (Real v)
			{
				mLacunarity = v;
			}
			/// Returns the noise lacunarity.
			Real getLacunarity () const
			{
				return mLacunarity;
			}
			/// Sets the noise persistence.
			void setPersistence (Real v)
			{
				mPersistence = v;
			}
			/// Returns the noise persistence.
			Real getPersistence () const
			{
				return mPersistence;
			}
			/// Sets the noise scale factor.
			void setScale (Real v)
			{
				mScale = v;
			}
			/// Returns the noise scale factor.
			Real getScale ()
			{
				return mScale;
			}
#if NOISEPP_ENABLE_UTILS
			/// @copydoc noisepp::Module::write()
			virtual void write (utils::OutStream &stream) const;
			/// @copydoc noisepp::Module::read()
			virtual void read (utils::InStream &stream);
#endif
	};

	class PerlinElement1D : public PipelineElement1D
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
			PerlinElement1D (size_t octaves, Real frequency, Real lacunarity, Real persistence, int mainSeed, int quality, Real nscale) : mOctaveCount(octaves), mQuality(quality), mScale(nscale)
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
			virtual ~PerlinElement1D ()
			{
				delete mOctaves;
				mOctaves = NULL;
			}
			virtual Real getValue (Real x, Cache *cache) const
			{
				Real value = 0.0;
				Real signal = 1.0;

				for (size_t o=0;o<mOctaveCount;++o)
				{
					const Real nx = Math::MakeInt32Range (x * mOctaves[o].scale);
					signal = calculateGradient(nx, mOctaves[o].seed);

					value += signal * mOctaves[o].persistence;
				}

				return value;
			}
	};

	class PerlinElement2D : public PipelineElement2D
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
			PerlinElement2D (size_t octaves, Real frequency, Real lacunarity, Real persistence, int mainSeed, int quality, Real nscale) : mOctaveCount(octaves), mQuality(quality), mScale(nscale)
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
			virtual ~PerlinElement2D ()
			{
				delete mOctaves;
				mOctaves = NULL;
			}
			virtual Real getValue (Real x, Real y, Cache *cache) const
			{
				Real value = 0.0;
				Real signal = 1.0;

				for (size_t o=0;o<mOctaveCount;++o)
				{
					const Real nx = Math::MakeInt32Range (x * mOctaves[o].scale);
					const Real ny = Math::MakeInt32Range (y * mOctaves[o].scale);
					signal = calculateGradient(nx, ny, mOctaves[o].seed);

					value += signal * mOctaves[o].persistence;
				}

				return value;
			}
	};

	class PerlinElement3D : public PipelineElement3D
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
			PerlinElement3D (size_t octaves, Real frequency, Real lacunarity, Real persistence, int mainSeed, int quality, Real nscale) : mOctaveCount(octaves), mQuality(quality), mScale(nscale)
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
			virtual ~PerlinElement3D ()
			{
				delete mOctaves;
				mOctaves = NULL;
			}
			virtual Real getValue (Real x, Real y, Real z, Cache *cache) const
			{
				Real value = 0.0;
				Real signal = 1.0;

				for (size_t o=0;o<mOctaveCount;++o)
				{
					const Real nx = Math::MakeInt32Range (x * mOctaves[o].scale);
					const Real ny = Math::MakeInt32Range (y * mOctaves[o].scale);
					const Real nz = Math::MakeInt32Range (z * mOctaves[o].scale);
					signal = calculateGradient(nx, ny, nz, mOctaves[o].seed);

					value += signal * mOctaves[o].persistence;
				}

				return value;
			}
	};

	/** Module for generating perlin noise.
		Generates perlin noise.
	*/
	class PerlinModule : public PerlinModuleBase
	{
		public:
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline1D *pipe) const
			{
				return pipe->addElement (this, new PerlinElement1D(mOctaveCount, mFrequency, mLacunarity, mPersistence, mSeed+pipe->getSeed(), mQuality, mScale));
			}
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline2D *pipe) const
			{
				return pipe->addElement (this, new PerlinElement2D(mOctaveCount, mFrequency, mLacunarity, mPersistence, mSeed+pipe->getSeed(), mQuality, mScale));
			}
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline3D *pipe) const
			{
				return pipe->addElement (this, new PerlinElement3D(mOctaveCount, mFrequency, mLacunarity, mPersistence, mSeed+pipe->getSeed(), mQuality, mScale));
			}
			/// @copydoc noisepp::Module::getType()
			ModuleTypeId getType() const { return MODULE_PERLIN; }
	};
};

#endif
