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

#include "NoiseGradientRenderer.h"
#include "NoiseSystem.h"

namespace noisepp
{
namespace utils
{

GradientRenderer::GradientRenderer() : mCallback(0)
{
}

void GradientRenderer::addGradient (Real value, const ColourValue &color)
{
	Gradient gradient;
	gradient.value = value;
	gradient.color = color;
	mGradients.push_back (gradient);
	std::sort (mGradients.begin(), mGradients.end());
}

void GradientRenderer::renderImage (Image &image, const Real *data, JobQueue *jobQueue)
{
//	NoiseAssert (mGradients.size() >= 2, mGradients);
	if (!jobQueue)
		jobQueue = System::createOptimalJobQueue();
	unsigned char *buffer = image.getPixelData ();
	int width = image.getWidth();
	for (int y=0;y<image.getHeight();++y)
	{
		jobQueue->addJob (new GradientRendererJob(this, width, data+(y*width), buffer+(y*width*3)));
	}
	jobQueue->executeJobs();
	if (mCallback)
		mCallback->reset ();
	delete jobQueue;
	jobQueue = 0;
}

void GradientRenderer::setCallback(BuilderCallback *callback)
{
	if (mCallback)
		delete mCallback;
	mCallback = callback;
}

GradientRenderer::~GradientRenderer()
{
	if (mCallback)
	{
		delete mCallback;
		mCallback = 0;
	}
}

GradientRenderer::GradientRendererJob::GradientRendererJob(GradientRenderer *renderer, int width, const Real *data, unsigned char *buffer) :
	renderer(renderer), width(width), data(data), buffer(buffer)
{
	assert (renderer);
}

void GradientRenderer::GradientRendererJob::execute ()
{
	size_t n;
	for (int x=0;x<width;++x)
	{
		Real value = *data++;
		ColourValue color = renderer->mGradients.front().color;
		for (n=0;n<renderer->mGradients.size();++n)
		{
			if (renderer->mGradients[n].value > value)
				break;
		}
		if (n < renderer->mGradients.size())
		{
			if (n > 0)
			{
				const Gradient &left = renderer->mGradients[n-1];
				const Gradient &right = renderer->mGradients[n];
				const float a = (float)((value - left.value) / (right.value - left.value));
				color = left.color * (1.0f-a) + right.color * a;
			}
		}
		else
			color = renderer->mGradients.back().color;
		color.writeRGB (buffer);
	}
}

void GradientRenderer::GradientRendererJob::finish ()
{
	if (renderer->mCallback)
		renderer->mCallback->callback ();
}

};
};
