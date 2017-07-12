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

#ifndef GRADIENTRENDERER_H
#define GRADIENTRENDERER_H

#include "NoisePrerequisites.h"
#include "NoiseColourValue.h"
#include "NoiseImage.h"
#include "NoiseBuilders.h"
#include "NoiseJobQueue.h"

namespace noisepp
{
namespace utils
{

/// Gradient color renderer.
class GradientRenderer
{
	public:
		/// Constructor.
		GradientRenderer();
		/// Adds a gradient point.
		/// @param value Position of the gradient point.
		/// @param color Color value of the gradient point.
		void addGradient (Real value, const ColourValue &color);
		/// Renders the data to an image.
		/// @param image The image to render to.
		/// @param data The source data.
		/// @param jobQueue A pointer to a JobQueue. The JobQueue will be deleted after usage. Passing NULL will use an system optimal queue.
		void renderImage (Image &image, const Real *data, JobQueue *jobQueue=0);
		/// Sets a callback
		void setCallback (BuilderCallback *callback);
		/// Destructor.
		~GradientRenderer();
	protected:
	private:
		struct Gradient
		{
			Real value;
			ColourValue color;
			bool operator< (const Gradient &g) const
			{
				return (value < g.value);
			}
		};
		typedef std::vector<Gradient> GradientVector;
		GradientVector mGradients;
		BuilderCallback *mCallback;
		class GradientRendererJob : public Job
		{
			private:
				GradientRenderer *renderer;
				int width;
				const Real *data;
				unsigned char *buffer;

			public:
				GradientRendererJob(GradientRenderer *renderer, int width, const Real *data, unsigned char *buffer);
				void execute ();
				void finish ();
		};
};

};
};

#endif // GRADIENTRENDERER_H
