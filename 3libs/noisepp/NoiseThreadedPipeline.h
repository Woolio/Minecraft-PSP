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

#ifndef NOISEPP_THREADEDPIPELINE_H
#define NOISEPP_THREADEDPIPELINE_H

#include "NoisePipeline.h"

#if NOISEPP_ENABLE_THREADS == 0
#error To use this classes please set NOISEPP_ENABLE_THREADS to 1
#endif

namespace noisepp
{
	/** Threaded pipeline base class.
		In Noise++ the noise generation process is different to other libraries.
		Instead of calling a noise generation function from your module instances directly,
		you add your modules to a so called Pipeline.
		The pipeline internally creates pipeline elements from the specified noise modules.
		Then you either create a cache and start generating or add jobs to the pipeline to
		run them in several threads.
	*/
	template <class Element>
	class ThreadedPipeline : public Pipeline<Element>
	{
		private:
			threadpp::ThreadGroup mThreads;
			threadpp::Mutex mMutex;
			threadpp::Condition mCond, mMainCond;

			bool mThreadsDone;
			unsigned mWorkingThreads;
			PipelineJobQueue mJobsDone;
			void threadFunction ()
			{
				Cache *cache = NULL;
				threadpp::Mutex::Lock lk(mMutex);
				while (!mThreadsDone)
				{
					if (Pipeline<Element>::mJobs.empty())
						mCond.wait(lk);
					if (!Pipeline<Element>::mJobs.empty())
					{
						PipelineJob *job = Pipeline<Element>::mJobs.front ();
						Pipeline<Element>::mJobs.pop ();
						++mWorkingThreads;
						lk.unlock ();
						if (!cache)
							cache = Pipeline<Element>::createCache();
						job->execute(cache);
						lk.lock ();
						--mWorkingThreads;
						mJobsDone.push (job);
						mMainCond.notifyOne ();
					}
				}
				lk.unlock ();
				if (cache)
				{
					Pipeline<Element>::freeCache (cache);
				}
			}
			static void *threadEntry (void *pipe)
			{
				(static_cast<ThreadedPipeline<Element>*>(pipe))->threadFunction ();
				return NULL;
			}

		public:
			/// Constructor.
			/// @param numberOfThreads The number of threads
			ThreadedPipeline (size_t numberOfThreads) : mThreadsDone(false), mWorkingThreads(0)
			{
				NoiseAssert (numberOfThreads > 0, numberOfThreads);
				for (size_t i=0;i<numberOfThreads;++i)
				{
					mThreads.createThread (threadEntry, this);
				}
			}
			/// executes the jobs in queue
			/// WARNING: Don't change the pipeline after calling this function
			virtual void executeJobs ()
			{
				mCond.notifyAll();
				threadpp::Mutex::Lock lk(mMutex);
				while (!Pipeline<Element>::mJobs.empty() || mWorkingThreads > 0)
				{
					if (!Pipeline<Element>::mJobs.empty() || mWorkingThreads > 0)
						mMainCond.wait(lk);
					while (!mJobsDone.empty())
					{
						PipelineJob *job = mJobsDone.front ();
						mJobsDone.pop ();
						lk.unlock ();
						job->finish ();
						delete job;
						lk.lock ();
					}
				}
			}
			/// @copydoc noisepp::Pipeline::addJob()
			virtual void addJob (PipelineJob *job)
			{
				NoiseAssert (job != NULL, job);
				threadpp::Mutex::Lock lk(mMutex);
				Pipeline<Element>::mJobs.push (job);
			}
			/// Destructor.
			virtual ~ThreadedPipeline ()
			{
				mThreadsDone = true;
				mCond.notifyAll();
				mThreads.join ();
			}
	};

	/** 1D threaded pipeline.
		In Noise++ the noise generation process is different to other libraries.
		Instead of calling a noise generation function from your module instances directly,
		you add your modules to a so called Pipeline.
		The pipeline internally creates pipeline elements from the specified noise modules.
		Then you either create a cache and start generating or add jobs to the pipeline to
		run them in several threads.
	*/
	typedef ThreadedPipeline<PipelineElement1D> ThreadedPipeline1D;
	/** 2D threaded pipeline.
		In Noise++ the noise generation process is different to other libraries.
		Instead of calling a noise generation function from your module instances directly,
		you add your modules to a so called Pipeline.
		The pipeline internally creates pipeline elements from the specified noise modules.
		Then you either create a cache and start generating or add jobs to the pipeline to
		run them in several threads.
	*/
	typedef ThreadedPipeline<PipelineElement2D> ThreadedPipeline2D;
	/** 3D threaded pipeline.
		In Noise++ the noise generation process is different to other libraries.
		Instead of calling a noise generation function from your module instances directly,
		you add your modules to a so called Pipeline.
		The pipeline internally creates pipeline elements from the specified noise modules.
		Then you either create a cache and start generating or add jobs to the pipeline to
		run them in several threads.
	*/
	typedef ThreadedPipeline<PipelineElement3D> ThreadedPipeline3D;
};

#endif // NOISEPP_THREADEDPIPELINE_H
