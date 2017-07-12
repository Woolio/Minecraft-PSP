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

#ifndef NOISEPP_TURBULENCE_H
#define NOISEPP_TURBULENCE_H

#include "NoisePerlin.h"

namespace noisepp
{
	class TurbulenceElement1D : public PipelineElement1D
	{
		private:
			Real mPower;
			ElementID mElement;
			const PipelineElement1D *mElementPtr;
			ElementID mPerlinX;
			const PipelineElement1D *mPerlinXPtr;

		public:
			TurbulenceElement1D (const Pipeline1D *pipe, Real power, ElementID element, ElementID perlinX) :
				mPower(power), mElement(element), mPerlinX(perlinX)
			{
				mElementPtr = pipe->getElement (mElement);
				mPerlinXPtr = pipe->getElement (mPerlinX);
			}
			virtual Real getValue (Real x, Cache *cache) const
			{
				Real x0;
				x0 = x + Real(12414.0 / 65536.0);
				Real xFinal = x + (getElementValue (mPerlinXPtr, mPerlinX, x0, cache) * mPower);
				return getElementValue (mElementPtr, mElement, xFinal, cache);
			}

	};

	class TurbulenceElement2D : public PipelineElement2D
	{
		private:
			Real mPower;
			ElementID mElement;
			const PipelineElement2D *mElementPtr;
			ElementID mPerlinX;
			const PipelineElement2D *mPerlinXPtr;
			ElementID mPerlinY;
			const PipelineElement2D *mPerlinYPtr;

		public:
			TurbulenceElement2D (const Pipeline2D *pipe, Real power, ElementID element, ElementID perlinX, ElementID perlinY) :
				mPower(power), mElement(element), mPerlinX(perlinX), mPerlinY(perlinY)
			{
				mElementPtr = pipe->getElement (mElement);
				mPerlinXPtr = pipe->getElement (mPerlinX);
				mPerlinYPtr = pipe->getElement (mPerlinY);
			}
			virtual Real getValue (Real x, Real y, Cache *cache) const
			{
				Real x0, y0;
				Real x1, y1;
				x0 = x + Real(12414.0 / 65536.0);
				y0 = y + Real(65124.0 / 65536.0);
				x1 = x + Real(26519.0 / 65536.0);
				y1 = y + Real(18128.0 / 65536.0);
				Real xFinal = x + (getElementValue (mPerlinXPtr, mPerlinX, x0, y0, cache) * mPower);
				Real yFinal = y + (getElementValue (mPerlinYPtr, mPerlinY, x1, y1, cache) * mPower);
				return getElementValue (mElementPtr, mElement, xFinal, yFinal, cache);
			}

	};

	class TurbulenceElement3D : public PipelineElement3D
	{
		private:
			Real mPower;
			ElementID mElement;
			const PipelineElement3D *mElementPtr;
			ElementID mPerlinX;
			const PipelineElement3D *mPerlinXPtr;
			ElementID mPerlinY;
			const PipelineElement3D *mPerlinYPtr;
			ElementID mPerlinZ;
			const PipelineElement3D *mPerlinZPtr;

		public:
			TurbulenceElement3D (const Pipeline3D *pipe, Real power, ElementID element, ElementID perlinX, ElementID perlinY, ElementID perlinZ) :
				mPower(power), mElement(element), mPerlinX(perlinX), mPerlinY(perlinY), mPerlinZ(perlinZ)
			{
				mElementPtr = pipe->getElement (mElement);
				mPerlinXPtr = pipe->getElement (mPerlinX);
				mPerlinYPtr = pipe->getElement (mPerlinY);
				mPerlinZPtr = pipe->getElement (mPerlinZ);
			}
			virtual Real getValue (Real x, Real y, Real z, Cache *cache) const
			{
				Real x0, y0, z0;
				Real x1, y1, z1;
				Real x2, y2, z2;
				x0 = x + Real(12414.0 / 65536.0);
				y0 = y + Real(65124.0 / 65536.0);
				z0 = z + Real(31337.0 / 65536.0);
				x1 = x + Real(26519.0 / 65536.0);
				y1 = y + Real(18128.0 / 65536.0);
				z1 = z + Real(60493.0 / 65536.0);
				x2 = x + Real(53820.0 / 65536.0);
				y2 = y + Real(11213.0 / 65536.0);
				z2 = z + Real(44845.0 / 65536.0);
				Real xFinal = x + (getElementValue (mPerlinXPtr, mPerlinX, x0, y0, z0, cache) * mPower);
				Real yFinal = y + (getElementValue (mPerlinYPtr, mPerlinY, x1, y1, z1, cache) * mPower);
				Real zFinal = z + (getElementValue (mPerlinZPtr, mPerlinZ, x2, y2, z2, cache) * mPower);
				return getElementValue (mElementPtr, mElement, xFinal, yFinal, zFinal, cache);
			}

	};

	/** Turbulence module.
		Adds turbulence to the source module by randomly modificating the coordinates.
	*/
	class TurbulenceModule : public Module
	{
		private:
			Real mPower;
			PerlinModule mPerlinX;
			PerlinModule mPerlinY;
			PerlinModule mPerlinZ;

		public:
			/// Constructor.
			TurbulenceModule() : Module(1), mPower(1.0)
			{
				setRoughness (3);
				setSeed (0);
			}
			/// Sets the power.
			void setPower (Real v)
			{
				mPower = v;
			}
			/// Sets the roughness.
			void setRoughness (int v)
			{
				mPerlinX.setOctaveCount (v);
				mPerlinY.setOctaveCount (v);
				mPerlinZ.setOctaveCount (v);
			}
			/// Sets the seed.
			void setSeed (int v)
			{
				mPerlinX.setSeed (v);
				mPerlinY.setSeed (v+1);
				mPerlinZ.setSeed (v+2);
			}
			/// Sets the frequency.
			void setFrequency (Real v)
			{
				mPerlinX.setFrequency (v);
				mPerlinY.setFrequency (v);
				mPerlinZ.setFrequency (v);
			}
			/// Sets the quality.
			void setQuality (int v)
			{
				mPerlinX.setQuality (v);
				mPerlinY.setQuality (v);
				mPerlinZ.setQuality (v);
			}
			/// Returns the power.
			Real getPower () const
			{
				return mPower;
			}
			/// Returns the roughness.
			int getRoughness () const
			{
				return mPerlinX.getOctaveCount ();
			}
			/// Returns the seed.
			int getSeed () const
			{
				return mPerlinX.getSeed ();
			}
			/// Returns the frequency.
			Real getFrequency () const
			{
				return mPerlinX.getFrequency ();
			}
			/// Returns the quality.
			int getQuality () const
			{
				return mPerlinX.getQuality ();
			}
			/// @copydoc noisepp::Module::addToPipeline()
			virtual ElementID addToPipeline (Pipeline1D *pipe) const
			{
				//NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				ElementID perlinX = mPerlinX.addToPipeline(pipe);
				return pipe->addElement (this, new TurbulenceElement1D(pipe, mPower, first, perlinX));
			}
			/// @copydoc noisepp::Module::addToPipeline()
			virtual ElementID addToPipeline (Pipeline2D *pipe) const
			{
				//NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				ElementID perlinX = mPerlinX.addToPipeline(pipe);
				ElementID perlinY = mPerlinY.addToPipeline(pipe);
				return pipe->addElement (this, new TurbulenceElement2D(pipe, mPower, first, perlinX, perlinY));
			}
			/// @copydoc noisepp::Module::addToPipeline()
			virtual ElementID addToPipeline (Pipeline3D *pipe) const
			{
				//NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				ElementID perlinX = mPerlinX.addToPipeline(pipe);
				ElementID perlinY = mPerlinY.addToPipeline(pipe);
				ElementID perlinZ = mPerlinZ.addToPipeline(pipe);
				return pipe->addElement (this, new TurbulenceElement3D(pipe, mPower, first, perlinX, perlinY, perlinZ));
			}
			/// @copydoc noisepp::Module::getType()
			ModuleTypeId getType() const { return MODULE_TURBULENCE; }
#if NOISEPP_ENABLE_UTILS
			/// @copydoc noisepp::Module::write()
			virtual void write (utils::OutStream &stream) const;
			/// @copydoc noisepp::Module::read()
			virtual void read (utils::InStream &stream);
#endif
	};
};

#endif
