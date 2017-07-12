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

#ifndef NOISEPP_CURVE_H
#define NOISEPP_CURVE_H

#include "NoisePipeline.h"
#include "NoiseModule.h"

namespace noisepp
{
	/// Curve control point.
	struct CurveControlPoint
	{
		/// Input value.
		Real inValue;
		/// Output value.
		Real outValue;
		/// Compare operator for sorting.
		bool operator< (const CurveControlPoint &p) const
		{
			return inValue < p.inValue;
		}
	};

	typedef std::vector<CurveControlPoint> CurveControlPointVector;

	template <class PipelineElement>
	class CurveElementBase : public PipelineElement
	{
		protected:
			ElementID mElement;
			const PipelineElement *mElementPtr;
			CurveControlPoint *mControlPoints;
			int mControlPointCount;

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
					if (value < mControlPoints[index].inValue)
					{
						break;
					}
				}

				const int index0 = clampValue (index-2, 0, mControlPointCount-1);
				const int index1 = clampValue (index-1, 0, mControlPointCount-1);
				const int index2 = clampValue (index, 0, mControlPointCount-1);
				const int index3 = clampValue (index+1, 0, mControlPointCount-1);

				if (index1 == index2)
				{
					return mControlPoints[index1].outValue;
				}

				const Real in0 = mControlPoints[index1].inValue;
				const Real in1 = mControlPoints[index2].inValue;
				const Real a = (value - in0) / (in1 - in0);
				return Math::InterpCubic (mControlPoints[index0].outValue, mControlPoints[index1].outValue, mControlPoints[index2].outValue, mControlPoints[index3].outValue, a);
			}
		public:
			CurveElementBase (const Pipeline<PipelineElement> *pipe, ElementID element, CurveControlPoint *points, int count) : mElement(element), mControlPoints(points), mControlPointCount(count)
			{
				//NoiseAssert (points != NULL, points);
				//NoiseAssert (count >= 4, count);
				mElementPtr = pipe->getElement (mElement);
			}
			virtual ~CurveElementBase ()
			{
				delete[] mControlPoints;
				mControlPoints = NULL;
			}
	};

	class CurveElement1D : public CurveElementBase<PipelineElement1D>
	{
		public:
			CurveElement1D (const Pipeline1D *pipe, ElementID element, CurveControlPoint *points, int count) : CurveElementBase<PipelineElement1D>(pipe, element, points, count)
			{
			}
			virtual Real getValue (Real x, Cache *cache) const
			{
				Real value;
				value = getElementValue (mElementPtr, mElement, x, cache);
				return CurveElementBase<PipelineElement1D>::mapValue(value);
			}
	};

	class CurveElement2D : public CurveElementBase<PipelineElement2D>
	{
		public:
			CurveElement2D (const Pipeline2D *pipe, ElementID element, CurveControlPoint *points, int count) : CurveElementBase<PipelineElement2D>(pipe, element, points, count)
			{
			}
			virtual Real getValue (Real x, Real y, Cache *cache) const
			{
				Real value;
				value = getElementValue (mElementPtr, mElement, x, y, cache);
				return CurveElementBase<PipelineElement2D>::mapValue(value);
			}
	};

	class CurveElement3D : public CurveElementBase<PipelineElement3D>
	{
		public:
			CurveElement3D (const Pipeline3D *pipe, ElementID element, CurveControlPoint *points, int count) : CurveElementBase<PipelineElement3D>(pipe, element, points, count)
			{
			}
			virtual Real getValue (Real x, Real y, Real z, Cache *cache) const
			{
				Real value;
				value = getElementValue (mElementPtr, mElement, x, y, z, cache);
				return CurveElementBase<PipelineElement3D>::mapValue(value);
			}
	};

	/** Module that maps the values from the source module onto a curve.
		Maps the values from the source modules onto the curve defined by the specified control points (at least 4)
	*/
	class CurveModule : public Module
	{
		private:
			CurveControlPointVector mControlPoints;

		public:
			/// Constructor.
			CurveModule() : Module(1)
			{
			}
			/// Adds a control point.
			void addControlPoint (Real inValue, Real outValue)
			{
				CurveControlPoint point;
				point.inValue = inValue;
				point.outValue = outValue;
				mControlPoints.push_back (point);
				std::sort (mControlPoints.begin(), mControlPoints.end());
			}
			/// Clears all control points.
			void clearControlPoints ()
			{
				mControlPoints.clear ();
			}
			/// Returns a reference to the control point vector.
			CurveControlPointVector &getControlPoints ()
			{
				return mControlPoints;
			}
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline1D *pipe) const
			{
				//NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				int count = (int)mControlPoints.size ();
				//NoiseAssert (count >= 4, count);
				CurveControlPoint *points = new CurveControlPoint[count];
				for (int i=0;i<count;++i)
				{
					points[i] = mControlPoints[i];
				}
				return pipe->addElement (this, new CurveElement1D(pipe, first, points, count));
			}
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline2D *pipe) const
			{
				//NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				int count = (int)mControlPoints.size ();
				//NoiseAssert (count >= 4, count);
				CurveControlPoint *points = new CurveControlPoint[count];
				for (int i=0;i<count;++i)
				{
					points[i] = mControlPoints[i];
				}
				return pipe->addElement (this, new CurveElement2D(pipe, first, points, count));
			}
			/// @copydoc noisepp::Module::addToPipeline()
			ElementID addToPipeline (Pipeline3D *pipe) const
			{
				//NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				int count = (int)mControlPoints.size ();
				//NoiseAssert (count >= 4, count);
				CurveControlPoint *points = new CurveControlPoint[count];
				for (int i=0;i<count;++i)
				{
					points[i] = mControlPoints[i];
				}
				return pipe->addElement (this, new CurveElement3D(pipe, first, points, count));
			}
			/// @copydoc noisepp::Module::getType()
			ModuleTypeId getType() const { return MODULE_CURVE; }
#if NOISEPP_ENABLE_UTILS
			/// @copydoc noisepp::Module::write()
			virtual void write (utils::OutStream &stream) const;
			/// @copydoc noisepp::Module::read()
			virtual void read (utils::InStream &stream);
#endif
	};
};

#endif
