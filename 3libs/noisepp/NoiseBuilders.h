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

#ifndef NOISEBUILDERS_H
#define NOISEBUILDERS_H

#include "NoisePrerequisites.h"
#include "NoisePipelineJobs.h"
#include "NoiseModule.h"

namespace noisepp
{
namespace utils
{

/// Builder callback class.
/// Overwrite the progress() function to get the current progress (ranges from 0.0 to 1.0)
class BuilderCallback
{
	private:
		int cur;

	public:
		/// Constructor.
		BuilderCallback () : cur(0)
		{}
		/// The callback function
		void callback ()
		{
			progress(++cur);
		}
		/// Resets the progress
		void reset ()
		{
			cur = 0;
		}
		/// Overwrite this function to get the current progress (ranges from 0.0 to 1.0)
		virtual void progress (int cur) = 0;
		/// Destructor.
		virtual ~BuilderCallback ()
		{}
};

/// Base builder class.
/// A builder is a helper class that makes it easy to generate geometrical objects like a plane.
/// You just have to specify the output size, destination and geometric specific options(like the geom size).
/// Now you can set the source module and the build function will create the pipeline and element for you using
/// the optimal system settings or create them manually and pass them to the build function.
class Builder
{
	protected:
		/// Destination.
		Real *mDest;
		/// Output width.
		int mWidth;
		/// Output height.
		int mHeight;
		/// Source module.
		Module *mModule;

		/// Check the parameters.
		void checkParameters ();

		/// Callback
		BuilderCallback *mCallback;

	public:
		/// Constructor.
		Builder ();
		/// Sets the output size.
		/// @param width The output width.
		/// @param height The output height.
		void setSize (int width, int height);
		/// Sets the output destination.
		void setDestination (Real *dest);
		/// Sets the source module.
		void setModule (Module *module);
		/// Sets the source module.
		inline void setModule (Module &module)
		{
			setModule (&module);
		}
		/// Sets a callback
		void setCallback (BuilderCallback *callback);
		/// Build.
		virtual void build () = 0;
		/// Get progress maximum
		virtual int getProgressMaximum () const = 0;
		/// Destructor.
		virtual ~Builder ();
};

/// Builder class for a 2D plane
class PlaneBuilder2D : public Builder
{
	private:
		Real mLowerBoundX, mLowerBoundY;
		Real mUpperBoundX, mUpperBoundY;
		bool mSeamless;

	public:
		/// Constructor.
		PlaneBuilder2D ();
		/// Build using the specified pipeline and element.
		void build (Pipeline2D *pipeline, PipelineElement2D *element);
		/// @copydoc noisepp::utils::Builder::build()
		virtual void build ();
		/// @copydoc noisepp::utils::Builder::getProgressMaximum()
		int getProgressMaximum () const;

		/// Sets the plane bounds.
		/// @param lowerBoundX The x-coordinate of the lower bound.
		/// @param lowerBoundY The y-coordinate of the lower bound.
		/// @param upperBoundX The x-coordinate of the upper bound.
		/// @param upperBoundY The y-coordinate of the upper bound.
		void setBounds (Real lowerBoundX, Real lowerBoundY, Real upperBoundX, Real upperBoundY);
		/// Returns the x-coordinate of the lower bound.
		Real getLowerBoundX () const;
		/// Returns the y-coordinate of the lower bound.
		Real getLowerBoundY () const;
		/// Returns the x-coordinate of the upper bound.
		Real getUpperBoundX () const;
		/// Returns the y-coordinate of the upper bound.
		Real getUpperBoundY () const;
		/// Enables or disables building a seamless plane.
		void setSeamless (bool v=true);
		/// Returns if building a seamless plane is enabled.
		bool isSeamless () const;
};

};
};

#endif // NOISEBUILDERS_H
