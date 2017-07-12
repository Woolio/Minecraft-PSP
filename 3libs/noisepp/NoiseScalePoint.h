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

#ifndef NOISEPP_SCALEPOINT_H
#define NOISEPP_SCALEPOINT_H

#include "NoisePipeline.h"
#include "NoiseModule.h"

namespace noisepp
{
	class ScalePointElement1D : public PipelineElement1D
	{
		private:
			ElementID mElement;
			const PipelineElement1D *mElementPtr;
			Real mScaleX;

		public:
			ScalePointElement1D (const Pipeline1D *pipe, ElementID element, Real scaleX) :
				mElement(element), mScaleX(scaleX)
			{
				mElementPtr = pipe->getElement (mElement);
			}
			virtual Real getValue (Real x, Cache *cache) const
			{
				return getElementValue (mElementPtr, mElement, x*mScaleX, cache);
			}

	};

	class ScalePointElement2D : public PipelineElement2D
	{
		private:
			ElementID mElement;
			const PipelineElement2D *mElementPtr;
			Real mScaleX;
			Real mScaleY;

		public:
			ScalePointElement2D (const Pipeline2D *pipe, ElementID element, Real scaleX, Real scaleY) :
				mElement(element), mScaleX(scaleX), mScaleY(scaleY)
			{
				mElementPtr = pipe->getElement (mElement);
			}
			virtual Real getValue (Real x, Real y, Cache *cache) const
			{
				return getElementValue (mElementPtr, mElement, x*mScaleX, y*mScaleY, cache);
			}

	};

	class ScalePointElement3D : public PipelineElement3D
	{
		private:
			ElementID mElement;
			const PipelineElement3D *mElementPtr;
			Real mScaleX;
			Real mScaleY;
			Real mScaleZ;

		public:
			ScalePointElement3D (const Pipeline3D *pipe, ElementID element, Real scaleX, Real scaleY, Real scaleZ) :
				mElement(element), mScaleX(scaleX), mScaleY(scaleY), mScaleZ(scaleZ)
			{
				mElementPtr = pipe->getElement (mElement);
			}
			virtual Real getValue (Real x, Real y, Real z, Cache *cache) const
			{
				return getElementValue (mElementPtr, mElement, x*mScaleX, y*mScaleY, z*mScaleZ, cache);
			}

	};

	/** Transform module for scaling.
		Transforms the source module by scaling the coordinates.
	*/
	class ScalePointModule : public Module
	{
		private:
			Real mScaleX;
			Real mScaleY;
			Real mScaleZ;

		public:
			/// Constructor.
			ScalePointModule() : Module(1), mScaleX(1.0), mScaleY(1.0), mScaleZ(1.0)
			{
			}
			/// Sets the scaling applied to the x coordinate.
			void setScaleX (Real v)
			{
				mScaleX = v;
			}
			/// Returns the scaling applied to the x coordinate.
			Real getScaleX () const
			{
				return mScaleX;
			}
			/// Sets the scaling applied to the y coordinate.
			void setScaleY (Real v)
			{
				mScaleY = v;
			}
			/// Returns the scaling applied to the y coordinate.
			Real getScaleY () const
			{
				return mScaleY;
			}
			/// Sets the scaling applied to the z coordinate.
			void setScaleZ (Real v)
			{
				mScaleZ = v;
			}
			/// Returns the scaling applied to the z coordinate.
			Real getScaleZ () const
			{
				return mScaleZ;
			}
			/// @copydoc noisepp::Module::addToPipeline()
			virtual ElementID addToPipeline (Pipeline1D *pipe) const
			{
				//NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				return pipe->addElement (this, new ScalePointElement1D(pipe, first, mScaleX));
			}
			/// @copydoc noisepp::Module::addToPipeline()
			virtual ElementID addToPipeline (Pipeline2D *pipe) const
			{
				//NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				return pipe->addElement (this, new ScalePointElement2D(pipe, first, mScaleX, mScaleY));
			}
			/// @copydoc noisepp::Module::addToPipeline()
			virtual ElementID addToPipeline (Pipeline3D *pipe) const
			{
				//NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				return pipe->addElement (this, new ScalePointElement3D(pipe, first, mScaleX, mScaleY, mScaleZ));
			}
			/// @copydoc noisepp::Module::getType()
			ModuleTypeId getType() const { return MODULE_SCALEPOINT; }
#if NOISEPP_ENABLE_UTILS
			/// @copydoc noisepp::Module::write()
			virtual void write (utils::OutStream &stream) const;
			/// @copydoc noisepp::Module::read()
			virtual void read (utils::InStream &stream);
#endif
	};
};

#endif
