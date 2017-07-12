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

#include "NoisePrerequisites.h"
#include "NoiseSystem.h"

#if NOISEPP_ENABLE_THREADS
#	include "NoiseThreadedPipeline.h"
#endif

#if NOISEPP_PLATFORM == NOISEPP_PLATFORM_UNIX
#	include <unistd.h>
#elif NOISEPP_PLATFORM == NOISEPP_PLATFORM_WINDOWS
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#endif

namespace noisepp
{
namespace utils
{

int System::mNumberOfCPUs = System::calculateNumberOfCPUs();

int System::calculateNumberOfCPUs()
{
	return 1;
}

int System::getNumberOfCPUs()
{
	return mNumberOfCPUs;
}

Pipeline1D *System::createOptimalPipeline1D ()
{
#if NOISEPP_ENABLE_THREADS
	if (mNumberOfCPUs > 1)
		return new ThreadedPipeline1D (mNumberOfCPUs);
#endif
	return new Pipeline1D;
}

Pipeline2D *System::createOptimalPipeline2D ()
{
#if NOISEPP_ENABLE_THREADS
	if (mNumberOfCPUs > 1)
		return new ThreadedPipeline2D (mNumberOfCPUs);
#endif
	return new Pipeline2D;
}

Pipeline3D *System::createOptimalPipeline3D ()
{
#if NOISEPP_ENABLE_THREADS
	if (mNumberOfCPUs > 1)
		return new ThreadedPipeline3D (mNumberOfCPUs);
#endif
	return new Pipeline3D;
}

JobQueue *System::createOptimalJobQueue ()
{
#if NOISEPP_ENABLE_THREADS
	if (mNumberOfCPUs > 1)
		return new ThreadedJobQueue (mNumberOfCPUs);
#endif
	return new JobQueue;
}

};
};
