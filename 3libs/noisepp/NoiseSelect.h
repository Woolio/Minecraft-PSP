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

#ifndef NOISEPP_SELECT_H
#define NOISEPP_SELECT_H

#include "NoisePipeline.h"
#include "NoiseModule.h"

namespace noisepp
{
	class SelectElement1D : public PipelineElement1D
	{
		private:
			ElementID mLeft, mRight, mControl;
			const PipelineElement1D *mLeftPtr;
			const PipelineElement1D *mRightPtr;
			const PipelineElement1D *mControlPtr;
			Real mLowerBound, mUpperBound;
			Real mLowerBoundPlusFalloff, mLowerBoundMinusFalloff;
			Real mUpperBoundPlusFalloff, mUpperBoundMinusFalloff;
			Real mEdgeFalloff, mTwoEdgeFalloff;

		public:
			SelectElement1D (const Pipeline1D *pipe, ElementID left, ElementID right, ElementID control, Real lowerBound, Real upperBound, Real edgeFalloff) : mLeft(left), mRight(right), mControl(control), mLowerBound(lowerBound), mUpperBound(upperBound), mEdgeFalloff(edgeFalloff)
			{
				//NoiseAssert (mLowerBound < mUpperBound, (mLowerBound, mUpperBound));

				mLeftPtr = pipe->getElement(mLeft);
				mRightPtr = pipe->getElement(mRight);
				mControlPtr = pipe->getElement(mControl);

				mLowerBoundPlusFalloff = mLowerBound + mEdgeFalloff;
				mLowerBoundMinusFalloff = mLowerBound - mEdgeFalloff;
				mUpperBoundPlusFalloff = mUpperBound + mEdgeFalloff;
				mUpperBoundMinusFalloff = mUpperBound - mEdgeFalloff;
				mTwoEdgeFalloff = Real(2.0) * mEdgeFalloff;
			}
			virtual Real getValue (Real x, Cache *cache) const
			{
				Real controlValue = getElementValue (mControlPtr, mControl, x, cache);
				Real alpha;
				if (mEdgeFalloff > 0.0)
				{
					if (controlValue < mLowerBoundMinusFalloff)
					{
						return getElementValue (mLeftPtr, mLeft, x, cache);
					}
					else if (controlValue < mLowerBoundPlusFalloff)
					{
						alpha = Math::CubicCurve3 ((controlValue - mLowerBoundMinusFalloff) / mTwoEdgeFalloff);
						return Math::InterpLinear (
							getElementValue (mLeftPtr, mLeft, x, cache),
							getElementValue (mRightPtr, mRight, x, cache),
							alpha);
					}
					else if (controlValue < mUpperBoundMinusFalloff)
					{
						return getElementValue (mRightPtr, mRight, x, cache);
					}
					else if (controlValue < mUpperBoundPlusFalloff)
					{
						alpha = Math::CubicCurve3 ((controlValue - mUpperBoundMinusFalloff) / mTwoEdgeFalloff);
						return Math::InterpLinear (
							getElementValue (mRightPtr, mRight, x, cache),
							getElementValue (mLeftPtr, mLeft, x, cache),
							alpha);
					}
					else
					{
						return getElementValue (mLeftPtr, mLeft, x, cache);
					}
				}
				else
				{
					if (controlValue < mLowerBound || controlValue > mUpperBound)
					{
						return getElementValue (mLeftPtr, mLeft, x, cache);
					}
					else
					{
						return getElementValue (mRightPtr, mRight, x, cache);
					}
				}
			}
	};

	class SelectElement2D : public PipelineElement2D
	{
		private:
			ElementID mLeft, mRight, mControl;
			const PipelineElement2D *mLeftPtr;
			const PipelineElement2D *mRightPtr;
			const PipelineElement2D *mControlPtr;
			Real mLowerBound, mUpperBound;
			Real mLowerBoundPlusFalloff, mLowerBoundMinusFalloff;
			Real mUpperBoundPlusFalloff, mUpperBoundMinusFalloff;
			Real mEdgeFalloff, mTwoEdgeFalloff;

		public:
			SelectElement2D (const Pipeline2D *pipe, ElementID left, ElementID right, ElementID control, Real lowerBound, Real upperBound, Real edgeFalloff) : mLeft(left), mRight(right), mControl(control), mLowerBound(lowerBound), mUpperBound(upperBound), mEdgeFalloff(edgeFalloff)
			{
				//NoiseAssert (mLowerBound < mUpperBound, (mLowerBound, mUpperBound));

				mLeftPtr = pipe->getElement(mLeft);
				mRightPtr = pipe->getElement(mRight);
				mControlPtr = pipe->getElement(mControl);

				mLowerBoundPlusFalloff = mLowerBound + mEdgeFalloff;
				mLowerBoundMinusFalloff = mLowerBound - mEdgeFalloff;
				mUpperBoundPlusFalloff = mUpperBound + mEdgeFalloff;
				mUpperBoundMinusFalloff = mUpperBound - mEdgeFalloff;
				mTwoEdgeFalloff = Real(2.0) * mEdgeFalloff;
			}
			virtual Real getValue (Real x, Real y, Cache *cache) const
			{
				Real controlValue = getElementValue (mControlPtr, mControl, x, y, cache);
				Real alpha;
				if (mEdgeFalloff > 0.0)
				{
					if (controlValue < mLowerBoundMinusFalloff)
					{
						return getElementValue (mLeftPtr, mLeft, x, y, cache);
					}
					else if (controlValue < mLowerBoundPlusFalloff)
					{
						alpha = Math::CubicCurve3 ((controlValue - mLowerBoundMinusFalloff) / mTwoEdgeFalloff);
						return Math::InterpLinear (
							getElementValue (mLeftPtr, mLeft, x, y, cache),
							getElementValue (mRightPtr, mRight, x, y, cache),
							alpha);
					}
					else if (controlValue < mUpperBoundMinusFalloff)
					{
						return getElementValue (mRightPtr, mRight, x, y, cache);
					}
					else if (controlValue < mUpperBoundPlusFalloff)
					{
						alpha = Math::CubicCurve3 ((controlValue - mUpperBoundMinusFalloff) / mTwoEdgeFalloff);
						return Math::InterpLinear (
							getElementValue (mRightPtr, mRight, x, y, cache),
							getElementValue (mLeftPtr, mLeft, x, y, cache),
							alpha);
					}
					else
					{
						return getElementValue (mLeftPtr, mLeft, x, y, cache);
					}
				}
				else
				{
					if (controlValue < mLowerBound || controlValue > mUpperBound)
					{
						return getElementValue (mLeftPtr, mLeft, x, y, cache);
					}
					else
					{
						return getElementValue (mRightPtr, mRight, x, y, cache);
					}
				}
			}
	};

	class SelectElement3D : public PipelineElement3D
	{
		private:
			ElementID mLeft, mRight, mControl;
			const PipelineElement3D *mLeftPtr;
			const PipelineElement3D *mRightPtr;
			const PipelineElement3D *mControlPtr;
			Real mLowerBound, mUpperBound;
			Real mLowerBoundPlusFalloff, mLowerBoundMinusFalloff;
			Real mUpperBoundPlusFalloff, mUpperBoundMinusFalloff;
			Real mEdgeFalloff, mTwoEdgeFalloff;

		public:
			SelectElement3D (const Pipeline3D *pipe, ElementID left, ElementID right, ElementID control, Real lowerBound, Real upperBound, Real edgeFalloff) : mLeft(left), mRight(right), mControl(control), mLowerBound(lowerBound), mUpperBound(upperBound), mEdgeFalloff(edgeFalloff)
			{
				///NoiseAssert (mLowerBound < mUpperBound, (mLowerBound, mUpperBound));

				mLeftPtr = pipe->getElement(mLeft);
				mRightPtr = pipe->getElement(mRight);
				mControlPtr = pipe->getElement(mControl);

				mLowerBoundPlusFalloff = mLowerBound + mEdgeFalloff;
				mLowerBoundMinusFalloff = mLowerBound - mEdgeFalloff;
				mUpperBoundPlusFalloff = mUpperBound + mEdgeFalloff;
				mUpperBoundMinusFalloff = mUpperBound - mEdgeFalloff;
				mTwoEdgeFalloff = Real(2.0) * mEdgeFalloff;
			}
			virtual Real getValue (Real x, Real y, Real z, Cache *cache) const
			{
				Real controlValue = getElementValue (mControlPtr, mControl, x, y, z, cache);
				Real alpha;
				if (mEdgeFalloff > 0.0)
				{
					if (controlValue < mLowerBoundMinusFalloff)
					{
						return getElementValue (mLeftPtr, mLeft, x, y, z, cache);
					}
					else if (controlValue < mLowerBoundPlusFalloff)
					{
						alpha = Math::CubicCurve3 ((controlValue - mLowerBoundMinusFalloff) / mTwoEdgeFalloff);
						return Math::InterpLinear (
							getElementValue (mLeftPtr, mLeft, x, y, z, cache),
							getElementValue (mRightPtr, mRight, x, y, z, cache),
							alpha);
					}
					else if (controlValue < mUpperBoundMinusFalloff)
					{
						return getElementValue (mRightPtr, mRight, x, y, z, cache);
					}
					else if (controlValue < mUpperBoundPlusFalloff)
					{
						alpha = Math::CubicCurve3 ((controlValue - mUpperBoundMinusFalloff) / mTwoEdgeFalloff);
						return Math::InterpLinear (
							getElementValue (mRightPtr, mRight, x, y, z, cache),
							getElementValue (mLeftPtr, mLeft, x, y, z, cache),
							alpha);
					}
					else
					{
						return getElementValue (mLeftPtr, mLeft, x, y, z, cache);
					}
				}
				else
				{
					if (controlValue < mLowerBound || controlValue > mUpperBound)
					{
						return getElementValue (mLeftPtr, mLeft, x, y, z, cache);
					}
					else
					{
						return getElementValue (mRightPtr, mRight, x, y, z, cache);
					}
				}
			}
	};

	/** Select module.
		Selects the output value controlled by the control module.
		If the value of the select module is within the range specified
		the value of source module 1 is returned. Otherwise, the value of source
		module 0 is returned.
		The transition can be smoothed by setting an edge falloff.
	*/
	class SelectModule : public TripleSourceModuleBase
	{
		private:
			Real mLowerBound, mUpperBound;
			Real mEdgeFalloff;

		public:
			SelectModule() : mLowerBound(-1.0), mUpperBound(1.0), mEdgeFalloff(0.0)
			{
			}
			/// Sets the lower bound.
			void setLowerBound (Real v)
			{
				mLowerBound = v;
			}
			/// Returns the lower bound.
			Real getLowerBound () const
			{
				return mLowerBound;
			}
			/// Sets the upper bound.
			void setUpperBound (Real v)
			{
				mUpperBound = v;
			}
			/// Returns the upper bound.
			Real getUpperBound () const
			{
				return mUpperBound;
			}
			/// Sets the edge falloff
			void setEdgeFalloff (Real v)
			{
				const Real range = Real(0.5) * (mUpperBound - mLowerBound);
				mEdgeFalloff = (v > range) ? range : v;
			}
			/// Returns the edge falloff
			Real getEdgeFalloff () const
			{
				return mEdgeFalloff;
			}
			/// @copydoc noisepp::Module::addToPipeline()
			virtual ElementID addToPipeline (Pipeline1D *pipe) const
			{
				//NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				ElementID second = getSourceModule(1)->addToPipeline(pipe);
				ElementID third = getSourceModule(2)->addToPipeline(pipe);
				return pipe->addElement (this, new SelectElement1D(pipe, first, second, third, mLowerBound, mUpperBound, mEdgeFalloff));
			}
			/// @copydoc noisepp::Module::addToPipeline()
			virtual ElementID addToPipeline (Pipeline2D *pipe) const
			{
				//NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				ElementID second = getSourceModule(1)->addToPipeline(pipe);
				ElementID third = getSourceModule(2)->addToPipeline(pipe);
				return pipe->addElement (this, new SelectElement2D(pipe, first, second, third, mLowerBound, mUpperBound, mEdgeFalloff));
			}
			/// @copydoc noisepp::Module::addToPipeline()
			virtual ElementID addToPipeline (Pipeline3D *pipe) const
			{
				//NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				ElementID second = getSourceModule(1)->addToPipeline(pipe);
				ElementID third = getSourceModule(2)->addToPipeline(pipe);
				return pipe->addElement (this, new SelectElement3D(pipe, first, second, third, mLowerBound, mUpperBound, mEdgeFalloff));
			}
			/// @copydoc noisepp::Module::getType()
			ModuleTypeId getType() const { return MODULE_SELECT; }
#if NOISEPP_ENABLE_UTILS
			/// @copydoc noisepp::Module::write()
			virtual void write (utils::OutStream &stream) const;
			/// @copydoc noisepp::Module::read()
			virtual void read (utils::InStream &stream);
#endif
	};
};

#endif
