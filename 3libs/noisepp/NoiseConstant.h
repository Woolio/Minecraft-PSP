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

#ifndef NOISEPP_CONSTANT_H
#define NOISEPP_CONSTANT_H

#include "NoisePipeline.h"
#include "NoiseModule.h"

namespace noisepp
{
	template <class PipelineElement>
	class ConstantElement : public PipelineElement
	{
		private:
			Real mValue;

		public:
			ConstantElement (Real value) : mValue(value)
			{
			}
			virtual Real getValue (Real x, Cache *cache) const
			{
				return mValue;
			}
			virtual Real getValue (Real x, Real y, Cache *cache) const
			{
				return mValue;
			}
			virtual Real getValue (Real x, Real y, Real z, Cache *cache) const
			{
				return mValue;
			}
	};

	typedef ConstantElement<PipelineElement1D> ConstantElement1D;
	typedef ConstantElement<PipelineElement2D> ConstantElement2D;
	typedef ConstantElement<PipelineElement3D> ConstantElement3D;

	/** Module which returns a constant value.
		Returns the specified constant value.
	*/
	class ConstantModule : public Module
	{
		private:
			Real mValue;

		public:
			/// Constructor.
			ConstantModule() : Module(0), mValue(0.0)
			{
			}
			/// Sets the constant value.
			void setValue (Real v)
			{
				mValue = v;
			}
			/// Returns the constant value.
			Real getValue () const
			{
				return mValue;
			}
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline1D *pipe) const
			{
				return pipe->addElement (this, new ConstantElement1D(mValue));
			}
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline2D *pipe) const
			{
				return pipe->addElement (this, new ConstantElement2D(mValue));
			}
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline3D *pipe) const
			{
				return pipe->addElement (this, new ConstantElement3D(mValue));
			}
			/// @copydoc noisepp::Module::getType()
			ModuleTypeId getType() const { return MODULE_CONSTANT; }
#if NOISEPP_ENABLE_UTILS
			/// @copydoc noisepp::Module::write()
			virtual void write (utils::OutStream &stream) const;
			/// @copydoc noisepp::Module::read()
			virtual void read (utils::InStream &stream);
#endif
	};
};

#endif
