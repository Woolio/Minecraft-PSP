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

#ifndef NOISEPP_BLEND_H
#define NOISEPP_BLEND_H

#include "NoisePipeline.h"
#include "NoiseModule.h"

namespace noisepp
{
	class BlendElement1D : public PipelineElement1D
	{
		private:
			ElementID mLeft, mRight, mControl;
			const PipelineElement1D *mLeftPtr;
			const PipelineElement1D *mRightPtr;
			const PipelineElement1D *mControlPtr;

		public:
			BlendElement1D (const Pipeline1D *pipe, ElementID left, ElementID right, ElementID control) : mLeft(left), mRight(right), mControl(control)
			{
				mLeftPtr = pipe->getElement(mLeft);
				mRightPtr = pipe->getElement(mRight);
				mControlPtr = pipe->getElement(mControl);
			}
			virtual Real getValue (Real x, Cache *cache) const
			{
				Real leftValue, rightValue, blendValue;

				leftValue = getElementValue (mLeftPtr, mLeft, x, cache);
				rightValue = getElementValue (mRightPtr, mRight, x, cache);
				blendValue = getElementValue (mControlPtr, mControl, x, cache);

				return Math::InterpLinear (leftValue, rightValue, (blendValue + Real(1.0)) / Real(2.0));
			}
	};

	class BlendElement2D : public PipelineElement2D
	{
		private:
			ElementID mLeft, mRight, mControl;
			const PipelineElement2D *mLeftPtr;
			const PipelineElement2D *mRightPtr;
			const PipelineElement2D *mControlPtr;

		public:
			BlendElement2D (const Pipeline2D *pipe, ElementID left, ElementID right, ElementID control) : mLeft(left), mRight(right), mControl(control)
			{
				mLeftPtr = pipe->getElement(mLeft);
				mRightPtr = pipe->getElement(mRight);
				mControlPtr = pipe->getElement(mControl);
			}
			virtual Real getValue (Real x, Real y, Cache *cache) const
			{
				Real leftValue, rightValue, blendValue;

				leftValue = getElementValue (mLeftPtr, mLeft, x, y, cache);
				rightValue = getElementValue (mRightPtr, mRight, x, y, cache);
				blendValue = getElementValue (mControlPtr, mControl, x, y, cache);

				return Math::InterpLinear (leftValue, rightValue, (blendValue + Real(1.0)) / Real(2.0));
			}
	};

	class BlendElement3D : public PipelineElement3D
	{
		private:
			ElementID mLeft, mRight, mControl;
			const PipelineElement3D *mLeftPtr;
			const PipelineElement3D *mRightPtr;
			const PipelineElement3D *mControlPtr;

		public:
			BlendElement3D (const Pipeline3D *pipe, ElementID left, ElementID right, ElementID control) : mLeft(left), mRight(right), mControl(control)
			{
				mLeftPtr = pipe->getElement(mLeft);
				mRightPtr = pipe->getElement(mRight);
				mControlPtr = pipe->getElement(mControl);
			}
			virtual Real getValue (Real x, Real y, Real z, Cache *cache) const
			{
				Real leftValue, rightValue, blendValue;

				leftValue = getElementValue (mLeftPtr, mLeft, x, y, z, cache);
				rightValue = getElementValue (mRightPtr, mRight, x, y, z, cache);
				blendValue = getElementValue (mControlPtr, mControl, x, y, z, cache);

				return Math::InterpLinear (leftValue, rightValue, (blendValue + Real(1.0)) / Real(2.0));
			}
	};

	/** Module for blending.
		Blends the two values of the source modules controlled by the value of the control module.
	*/
	class BlendModule : public TripleSourceModule<BlendElement1D, BlendElement2D, BlendElement3D>
	{
		public:
			/// @copydoc noisepp::Module::getType()
			ModuleTypeId getType() const { return MODULE_BLEND; }
	};
};

#endif
