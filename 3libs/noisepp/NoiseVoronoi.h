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

#ifndef NOISEPP_VORONOI_H
#define NOISEPP_VORONOI_H

#include "NoisePrerequisites.h"
#include "NoiseModule.h"
#include "NoiseGenerator.h"
#include "NoisePipeline.h"

namespace noisepp
{
	class VoronoiElement2D : public PipelineElement2D
	{
		private:
			Real mFrequency;
			int mSeed;
			Real mDisplacement;
			bool mEnableDistance;

		public:
			VoronoiElement2D (Real frequency, int seed, Real displacement, bool enableDistance) : mFrequency(frequency), mSeed(seed), mDisplacement(displacement), mEnableDistance(enableDistance)
			{
			}
			virtual Real getValue (Real x, Real y, Cache *cache) const
			{
				x *= mFrequency;
				y *= mFrequency;

				int xi = (x > Real(0.0) ? (int)x : (int)x - 1);
				int yi = (y > Real(0.0) ? (int)y : (int)y - 1);

				Real minDist = Real(2147483647.0);
				Real xmin = Real(0);
				Real ymin = Real(0);

				for (int xc=xi-2;xc<=xi+2;++xc)
				{
					for (int yc=yi-2;yc<=yi+2;++yc)
					{
						Real xp = xc + Generator2D::calcNoise(xc, yc, mSeed);
						Real yp = yc + Generator2D::calcNoise(xc, yc, mSeed+1);
						Real xd = xp - x;
						Real yd = yp - y;
						Real dist = xd * xd + yd * yd;
						if (dist < minDist)
						{
							minDist = dist;
							xmin = xp;
							ymin = yp;
						}
					}
				}

				Real value;
				if (mEnableDistance)
				{
					const Real SQRT_TWO = 1.4142135623730951;
					Real xDist = xmin - x;
					Real yDist = ymin - y;
					value = (std::sqrt(xDist * xDist + yDist * yDist)) * SQRT_TWO - Real(1.0);
				}
				else
				{
					value = Real(0.0);
				}

				return value + (mDisplacement * (Real)Generator2D::calcNoise((int)floor(xmin), (int)floor(ymin)));
			}
	};

	class VoronoiElement3D : public PipelineElement3D
	{
		private:
			Real mFrequency;
			int mSeed;
			Real mDisplacement;
			bool mEnableDistance;

		public:
			VoronoiElement3D (Real frequency, int seed, Real displacement, bool enableDistance) : mFrequency(frequency), mSeed(seed), mDisplacement(displacement), mEnableDistance(enableDistance)
			{
			}
			virtual Real getValue (Real x, Real y, Real z, Cache *cache) const
			{
				x *= mFrequency;
				y *= mFrequency;
				z *= mFrequency;

				int xi = (x > Real(0.0) ? (int)x : (int)x - 1);
				int yi = (y > Real(0.0) ? (int)y : (int)y - 1);
				int zi = (z > Real(0.0) ? (int)z : (int)z - 1);

				Real minDist = Real(2147483647.0);
				Real xmin = Real(0);
				Real ymin = Real(0);
				Real zmin = Real(0);

				for (int xc=xi-2;xc<=xi+2;++xc)
				{
					for (int yc=yi-2;yc<=yi+2;++yc)
					{
						for (int zc=zi-2;zc<=zi+2;++zc)
						{
							Real xp = xc + Generator3D::calcNoise(xc, yc, zc, mSeed);
							Real yp = yc + Generator3D::calcNoise(xc, yc, zc, mSeed+1);
							Real zp = zc + Generator3D::calcNoise(xc, yc, zc, mSeed+2);
							Real xd = xp - x;
							Real yd = yp - y;
							Real zd = zp - z;
							Real dist = xd * xd + yd * yd + zd * zd;
							if (dist < minDist)
							{
								minDist = dist;
								xmin = xp;
								ymin = yp;
								zmin = zp;
							}
						}
					}
				}

				Real value;
				if (mEnableDistance)
				{
					const Real SQRT_THREE = 1.7320508075688772;
					Real xDist = xmin - x;
					Real yDist = ymin - y;
					Real zDist = zmin - z;
					value = (std::sqrt(xDist * xDist + yDist * yDist + zDist * zDist)) * SQRT_THREE - Real(1.0);
				}
				else
				{
					value = Real(0.0);
				}

				return value + (mDisplacement * (Real)Generator3D::calcNoise((int)floor(xmin), (int)floor(ymin), (int)floor(zmin)));
			}
	};

	/** Module for generating Voronoi cells.
		Generates voronoi cells.
	*/
	class VoronoiModule : public Module
	{
		protected:
			/// The frequency.
			Real mFrequency;
			/// The seed.
			int mSeed;
			/// The displacement factor.
			Real mDisplacement;
			/// Distance factor enabled.
			bool mEnableDistance;

		public:
			/// Constructor.
			VoronoiModule () :
					mFrequency(1.0), mSeed(0), mDisplacement(1.0), mEnableDistance(false) {}

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
			/// Sets the seed.
			void setSeed (int v)
			{
				mSeed = v;
			}
			/// Returns the seed.
			int getSeed () const
			{
				return mSeed;
			}
			/// Sets the displacement factor.
			void setDisplacement (Real v)
			{
				mDisplacement = v;
			}
			/// Returns the displacement factor.
			Real getDisplacement () const
			{
				return mDisplacement;
			}
			/// Enables the distance factor.
			void setEnableDistance (bool v=true)
			{
				mEnableDistance = v;
			}
			/// Returns if distance factor is enabled.
			bool isDistanceEnabled () const
			{
				return mEnableDistance;
			}
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline1D *pipe) const
			{
				//NoiseThrowNotImplementedException;
				return NULL;
			}
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline2D *pipe) const
			{
				return pipe->addElement (this, new VoronoiElement2D(mFrequency, mSeed+pipe->getSeed(), mDisplacement, mEnableDistance));
			}
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline3D *pipe) const
			{
				return pipe->addElement (this, new VoronoiElement3D(mFrequency, mSeed+pipe->getSeed(), mDisplacement, mEnableDistance));
			}
			/// @copydoc noisepp::Module::getType()
			ModuleTypeId getType() const { return MODULE_VORONOI; }
#if NOISEPP_ENABLE_UTILS
			/// @copydoc noisepp::Module::write()
			virtual void write (utils::OutStream &stream) const;
			/// @copydoc noisepp::Module::read()
			virtual void read (utils::InStream &stream);
#endif
	};
};

#endif // NOISEPP_VORONOI_H
