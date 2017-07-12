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

#ifndef NOISEPP_TERRACE_H
#define NOISEPP_TERRACE_H

#include "NoisePipeline.h"
#include "NoiseModule.h"

namespace noisepp
{
	typedef std::vector<Real> TerraceControlPointVector;

	template <class PipelineElement>
	class TerraceElementBase : public PipelineElement
	{
		protected:
			ElementID mElement;
			const PipelineElement *mElementPtr;
			Real *mControlPoints;
			int mControlPointCount;
			bool mInvert;

			NOISEPP_INLINE int clampValue (int i, int lo, int hi) const
			{
				if (i < lo)
					return lo;
				else if (i > hi)
					return hi;
				else
					return i;
			}

			NOISEPP_INLINE Real mapValue (Real value) const
			{
				int index;
				for (index=0;index<mControlPointCount;++index)
				{
					if (value < mControlPoints[index])
					{
						break;
					}
				}

				const int index0 = clampValue (index-1, 0, mControlPointCount-1);
				const int index1 = clampValue (index, 0, mControlPointCount-1);

				if (index0 == index1)
				{
					return mControlPoints[index1];
				}

				const Real in0 = mControlPoints[index0];
				const Real in1 = mControlPoints[index1];
				Real a = (value - in0) / (in1 - in0);
				if (mInvert)
				{
					a = Real(1.0) - a;
					return Math::InterpLinear (in1, in0, a*a);
				}
				return Math::InterpLinear (in0, in1, a*a);
			}
		public:
			TerraceElementBase (const Pipeline<PipelineElement> *pipe, ElementID element, Real *points, int count, bool invert) : mElement(element), mControlPoints(points), mControlPointCount(count), mInvert(invert)
			{
				//NoiseAssert (points != NULL, points);
				//NoiseAssert (count >= 2, count);
				mElementPtr = pipe->getElement (mElement);
			}
			virtual ~TerraceElementBase ()
			{
				delete[] mControlPoints;
				mControlPoints = NULL;
			}
	};

	class TerraceElement1D : public TerraceElementBase<PipelineElement1D>
	{
		public:
			TerraceElement1D (const Pipeline1D *pipe, ElementID element, Real *points, int count, bool invert) : TerraceElementBase<PipelineElement1D>(pipe, element, points, count, invert)
			{
			}
			virtual Real getValue (Real x, Cache *cache) const
			{
				Real value;
				value = getElementValue (mElementPtr, mElement, x, cache);
				return TerraceElementBase<PipelineElement1D>::mapValue(value);
			}
	};

	class TerraceElement2D : public TerraceElementBase<PipelineElement2D>
	{
		public:
			TerraceElement2D (const Pipeline2D *pipe, ElementID element, Real *points, int count, bool invert) : TerraceElementBase<PipelineElement2D>(pipe, element, points, count, invert)
			{
			}
			virtual Real getValue (Real x, Real y, Cache *cache) const
			{
				Real value;
				value = getElementValue (mElementPtr, mElement, x, y, cache);
				return TerraceElementBase<PipelineElement2D>::mapValue(value);
			}
	};

	class TerraceElement3D : public TerraceElementBase<PipelineElement3D>
	{
		public:
			TerraceElement3D (const Pipeline3D *pipe, ElementID element, Real *points, int count, bool invert) : TerraceElementBase<PipelineElement3D>(pipe, element, points, count, invert)
			{
			}
			virtual Real getValue (Real x, Real y, Real z, Cache *cache) const
			{
				Real value;
				value = getElementValue (mElementPtr, mElement, x, y, z, cache);
				return TerraceElementBase<PipelineElement3D>::mapValue(value);
			}
	};

	/** Terrace forming module.
		Maps the output value of the source module onto a terrace-forming curve.
	*/
	class TerraceModule : public Module
	{
		private:
			TerraceControlPointVector mControlPoints;
			bool mInvert;

		public:
			/// Constructor.
			TerraceModule() : Module(1), mInvert(false)
			{
			}
			/// Adds a control point.
			void addControlPoint (Real value)
			{
				mControlPoints.push_back (value);
				std::sort (mControlPoints.begin(), mControlPoints.end());
			}
			/// Clears all control points.
			void clearControlPoints ()
			{
				mControlPoints.clear ();
			}
			/// Returns a reference to the control point vector.
			TerraceControlPointVector &getControlPoints ()
			{
				return mControlPoints;
			}
			/// Enables inversion.
			void invert (bool v=true)
			{
				mInvert = v;
			}
			/// Returns if inverted.
			bool isInverted () const
			{
				return mInvert;
			}
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline1D *pipe) const
			{
				//NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				int count = (int)mControlPoints.size ();
				//NoiseAssert (count >= 2, count);
				Real *points = new Real[count];
				for (int i=0;i<count;++i)
				{
					points[i] = mControlPoints[i];
				}
				return pipe->addElement (this, new TerraceElement1D(pipe, first, points, count, mInvert));
			}
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline2D *pipe) const
			{
				//NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				int count = (int)mControlPoints.size ();
				//NoiseAssert (count >= 2, count);
				Real *points = new Real[count];
				for (int i=0;i<count;++i)
				{
					points[i] = mControlPoints[i];
				}
				return pipe->addElement (this, new TerraceElement2D(pipe, first, points, count, mInvert));
			}
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline3D *pipe) const
			{
				//NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				int count = (int)mControlPoints.size ();
				//NoiseAssert (count >= 2, count);
				Real *points = new Real[count];
				for (int i=0;i<count;++i)
				{
					points[i] = mControlPoints[i];
				}
				return pipe->addElement (this, new TerraceElement3D(pipe, first, points, count, mInvert));
			}
			/// @copydoc noisepp::Module::getType()
			ModuleTypeId getType() const { return MODULE_TERRACE; }
#if NOISEPP_ENABLE_UTILS
			/// @copydoc noisepp::Module::write()
			virtual void write (utils::OutStream &stream) const;
			/// @copydoc noisepp::Module::read()
			virtual void read (utils::InStream &stream);
#endif
	};
};

#endif // NOISEPP_TERRACE_H
