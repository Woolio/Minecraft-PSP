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

#ifndef NOISESYSTEM_H
#define NOISESYSTEM_H

#include "NoisePipeline.h"
#include "NoiseJobQueue.h"

namespace noisepp
{
namespace utils
{

/// Class providing some system specific functions.
class System
{
	public:
		/// Returns the number of CPU cores avaible on the running system.
		static int getNumberOfCPUs();
		/// Creates an optimal 1D pipeline using as many threads as there are CPU cores avaible.
		static Pipeline1D *createOptimalPipeline1D ();
		/// Creates an optimal 2D pipeline using as many threads as there are CPU cores avaible.
		static Pipeline2D *createOptimalPipeline2D ();
		/// Creates an optimal 3D pipeline using as many threads as there are CPU cores avaible.
		static Pipeline3D *createOptimalPipeline3D ();
		/// Creates an optimal job queue using as many threads as there are CPU cores avaible.
		static JobQueue *createOptimalJobQueue ();
	protected:
	private:
		static int mNumberOfCPUs;
		static int calculateNumberOfCPUs();
};

};
};

#endif // NOISESYSTEM_H
