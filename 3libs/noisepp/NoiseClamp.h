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

#ifndef NOISEPP_CLAMP_H
#define NOISEPP_CLAMP_H

#include "NoisePipeline.h"
#include "NoiseModule.h"

namespace noisepp
{
	class ClampElement1D : public PipelineElement1D
	{
		private:
			ElementID mElement;
			const PipelineElement1D *mElementPtr;
			Real mLowerBound, mUpperBound;

		public:
			ClampElement1D (const Pipeline1D *pipe, ElementID element, Real lowerBound, Real upperBound) : mElement(element), mLowerBound(lowerBound), mUpperBound(upperBound)
			{
				mElementPtr = pipe->getElement(mElement);
			}
			virtual Real getValue (Real x, Cache *cache) const
			{
				Real value;
				value = getElementValue (mElementPtr, mElement, x, cache);
				if (value < mLowerBound)
					value = mLowerBound;
				else if (value > mUpperBound)
					value = mUpperBound;
				return value;
			}
	};

	class ClampElement2D : public PipelineElement2D
	{
		private:
			ElementID mElement;
			const PipelineElement2D *mElementPtr;
			Real mLowerBound, mUpperBound;

		public:
			ClampElement2D (const Pipeline2D *pipe, ElementID element, Real lowerBound, Real upperBound) : mElement(element), mLowerBound(lowerBound), mUpperBound(upperBound)
			{
				mElementPtr = pipe->getElement(mElement);
			}
			virtual Real getValue (Real x, Real y, Cache *cache) const
			{
				Real value;
				value = getElementValue (mElementPtr, mElement, x, y, cache);
				if (value < mLowerBound)
					value = mLowerBound;
				else if (value > mUpperBound)
					value = mUpperBound;
				return value;
			}
	};

	class ClampElement3D : public PipelineElement3D
	{
		private:
			ElementID mElement;
			const PipelineElement3D *mElementPtr;
			Real mLowerBound, mUpperBound;

		public:
			ClampElement3D (const Pipeline3D *pipe, ElementID element, Real lowerBound, Real upperBound) : mElement(element), mLowerBound(lowerBound), mUpperBound(upperBound)
			{
				mElementPtr = pipe->getElement(mElement);
			}
			virtual Real getValue (Real x, Real y, Real z, Cache *cache) const
			{
				Real value;
				value = getElementValue (mElementPtr, mElement, x, y, z, cache);
				if (value < mLowerBound)
					value = mLowerBound;
				else if (value > mUpperBound)
					value = mUpperBound;
				return value;
			}
	};

	/** Module clamping the value of the source module.
		Clamps the value of the source module between the specified lower and upper bound.
	*/
	class ClampModule : public Module
	{
		private:
			Real mLowerBound, mUpperBound;

		public:
			/// Constructor.
			ClampModule() : Module(1), mLowerBound(-1.0), mUpperBound(1.0)
			{
			}
			/// Sets the lower bound
			void setLowerBound (Real v)
			{
				mLowerBound = v;
			}
			/// Returns the lower bound
			Real getLowerBound () const
			{
				return mLowerBound;
			}
			/// Sets the upper bound
			void setUpperBound (Real v)
			{
				mUpperBound = v;
			}
			/// Returns the upper bound
			Real getUpperBound () const
			{
				return mUpperBound;
			}
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline1D *pipe) const
			{
			//	NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				return pipe->addElement (this, new ClampElement1D(pipe, first, mLowerBound, mUpperBound));
			}
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline2D *pipe) const
			{
			//	NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				return pipe->addElement (this, new ClampElement2D(pipe, first, mLowerBound, mUpperBound));
			}
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline3D *pipe) const
			{
			//	NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				return pipe->addElement (this, new ClampElement3D(pipe, first, mLowerBound, mUpperBound));
			}
			/// @copydoc noisepp::Module::getType()
			ModuleTypeId getType() const { return MODULE_CLAMP; }
#if NOISEPP_ENABLE_UTILS
			/// @copydoc noisepp::Module::write()
			virtual void write (utils::OutStream &stream) const;
			/// @copydoc noisepp::Module::read()
			virtual void read (utils::InStream &stream);
#endif
	};
};

#endif
