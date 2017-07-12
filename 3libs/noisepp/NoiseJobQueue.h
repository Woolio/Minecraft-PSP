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

#ifndef NOISEJOBQUEUE_H
#define NOISEJOBQUEUE_H

#include "NoisePrerequisites.h"

namespace noisepp
{
namespace utils
{

/// A job which can be added to a job queue for multi-threaded execution.
class Job
{
	public:
		/// Destructor.
		virtual ~Job () {}
		/// This function is called when the job is executed. You must overwrite this.
		/// Don't do anything thread unsafe in there!
		virtual void execute () = 0;
		/// This function is called when the job is finished. It is always called in the main thread,
		/// so you can update a progress bar here or whatever.
		virtual void finish () {}
};

/// A job queue.
/// You can add jobs to this queue which will be executed when you call the executeJobs() function.
class JobQueue
{
	protected:
		/// The job queue
		std::queue<Job*> mJobs;

	public:
		/// Adds a job to the queue.
		virtual void addJob (Job *job);
		/// executes the jobs in queue
		virtual void executeJobs ();
		/// Destructor.
		virtual ~JobQueue ();
};

#if NOISEPP_ENABLE_THREADS
/// A threaded job queue.
/// You can add jobs to this queue which will be executed in several threads when you call the executeJobs() function.
class ThreadedJobQueue : public JobQueue
{
	private:
		/// A queue for done jobs
		std::queue<Job*> mJobsDone;

		threadpp::ThreadGroup mThreads;
		threadpp::Mutex mMutex;
		threadpp::Condition mCond, mMainCond;

		bool mThreadsDone;
		unsigned mWorkingThreads;

		void threadFunction ();
		static void *threadEntry (void *queue);
	public:
		/// Constructor.
		/// @param numberOfThreads The number of threads
		ThreadedJobQueue (size_t numberOfThreads);
		/// @copydoc noisepp::utils::JobQueue::executeJobs()
		virtual void executeJobs ();
		/// @copydoc noisepp::utils::JobQueue::addJob()
		virtual void addJob (Job *job);
		/// Destructor.
		virtual ~ThreadedJobQueue ();
};
#endif

};
};

#endif // NOISEJOBQUEUE_H
