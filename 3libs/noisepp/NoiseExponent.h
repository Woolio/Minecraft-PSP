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

#ifndef NOISEPP_EXPONENT_H
#define NOISEPP_EXPONENT_H

#include "NoisePipeline.h"
#include "NoiseModule.h"

namespace noisepp
{
	class ExponentElement1D : public PipelineElement1D
	{
		private:
			ElementID mElement;
			Real mExponent;
			const PipelineElement1D *mElementPtr;

		public:
			ExponentElement1D (const Pipeline1D *pipe, ElementID element, Real exponent) : mElement(element), mExponent(exponent)
			{
				mElementPtr = pipe->getElement(element);
			}
			virtual Real getValue (Real x, Cache *cache) const
			{
				Real value;
				value = getElementValue (mElementPtr, mElement, x, cache);
				return (std::pow (std::fabs ((value + Real(1.0)) / Real(2.0)), mExponent) * Real(2.0) - Real(1.0));
			}
	};

	class ExponentElement2D : public PipelineElement2D
	{
		private:
			ElementID mElement;
			Real mExponent;
			const PipelineElement2D *mElementPtr;

		public:
			ExponentElement2D (const Pipeline2D *pipe, ElementID element, Real exponent) : mElement(element), mExponent(exponent)
			{
				mElementPtr = pipe->getElement(element);
			}
			virtual Real getValue (Real x, Real y, Cache *cache) const
			{
				Real value;
				value = getElementValue (mElementPtr, mElement, x, y, cache);
				return (std::pow (std::fabs ((value + Real(1.0)) / Real(2.0)), mExponent) * Real(2.0) - Real(1.0));
			}
	};

	class ExponentElement3D : public PipelineElement3D
	{
		private:
			ElementID mElement;
			Real mExponent;
			const PipelineElement3D *mElementPtr;

		public:
			ExponentElement3D (const Pipeline3D *pipe, ElementID element, Real exponent) : mElement(element), mExponent(exponent)
			{
				mElementPtr = pipe->getElement(element);
			}
			virtual Real getValue (Real x, Real y, Real z, Cache *cache) const
			{
				Real value;
				value = getElementValue (mElementPtr, mElement, x, y, z, cache);
				return (std::pow (std::fabs ((value + Real(1.0)) / Real(2.0)), mExponent) * Real(2.0) - Real(1.0));
			}
	};

	/** Exponent module.
		Normalizes the output of the source module and exponentiates it
	*/
	class ExponentModule : public Module
	{
		private:
			Real mExponent;

		public:
			/// Constructor.
			ExponentModule() : Module(1), mExponent(1.0)
			{
			}
			/// Sets the exponent.
			void setExponent (Real v)
			{
				mExponent = v;
			}
			/// Returns the exponent.
			Real getExponent () const
			{
				return mExponent;
			}
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline1D *pipe) const
			{
				//NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				return pipe->addElement (this, new ExponentElement1D(pipe, first, mExponent));
			}
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline2D *pipe) const
			{
				//NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				return pipe->addElement (this, new ExponentElement2D(pipe, first, mExponent));
			}
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline3D *pipe) const
			{
				//NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				return pipe->addElement (this, new ExponentElement3D(pipe, first, mExponent));
			}
			/// @copydoc noisepp::Module::getType()
			ModuleTypeId getType() const { return MODULE_EXPONENT; }
#if NOISEPP_ENABLE_UTILS
			/// @copydoc noisepp::Module::write()
			virtual void write (utils::OutStream &stream) const;
			/// @copydoc noisepp::Module::read()
			virtual void read (utils::InStream &stream);
#endif
	};
};

#endif
