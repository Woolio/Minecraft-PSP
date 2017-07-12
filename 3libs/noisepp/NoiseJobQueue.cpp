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

#include "NoiseJobQueue.h"

namespace noisepp
{
namespace utils
{

void JobQueue::addJob (Job *job)
{
//	NoiseAssert (job != NULL, job);
	mJobs.push (job);
}

void JobQueue::executeJobs ()
{
	Job *job = 0;
	while (!mJobs.empty())
	{
		job = mJobs.front ();
		mJobs.pop ();
		job->execute();
		job->finish();
		delete job;
		job = 0;
	}
}

JobQueue::~JobQueue ()
{
	while (!mJobs.empty())
	{
		delete mJobs.front ();
		mJobs.pop ();
	}
}

#if NOISEPP_ENABLE_THREADS
void ThreadedJobQueue::threadFunction ()
{
	threadpp::Mutex::Lock lk(mMutex);
	while (!mThreadsDone)
	{
		if (mJobs.empty())
			mCond.wait(lk);
		if (!mJobs.empty())
		{
			Job *job = mJobs.front ();
			mJobs.pop ();
			++mWorkingThreads;
			lk.unlock ();
			job->execute();
			lk.lock ();
			--mWorkingThreads;
			mJobsDone.push (job);
			mMainCond.notifyOne ();
		}
	}
}

void *ThreadedJobQueue::threadEntry (void *queue)
{
	(static_cast<ThreadedJobQueue*>(queue))->threadFunction ();
	return NULL;
}

ThreadedJobQueue::ThreadedJobQueue (size_t numberOfThreads) : mThreadsDone(false), mWorkingThreads(0)
{
	NoiseAssert (numberOfThreads > 0, numberOfThreads);
	for (size_t i=0;i<numberOfThreads;++i)
	{
		mThreads.createThread (threadEntry, this);
	}
}

void ThreadedJobQueue::executeJobs ()
{
	mCond.notifyAll();
	threadpp::Mutex::Lock lk(mMutex);
	while (!mJobs.empty() || mWorkingThreads > 0)
	{
		if (!mJobs.empty() || mWorkingThreads > 0)
			mMainCond.wait(lk);
		while (!mJobsDone.empty())
		{
			Job *job = mJobsDone.front ();
			mJobsDone.pop ();
			lk.unlock ();
			job->finish ();
			delete job;
			lk.lock ();
		}
	}
}

void ThreadedJobQueue::addJob (Job *job)
{
	NoiseAssert (job != NULL, job);
	threadpp::Mutex::Lock lk(mMutex);
	mJobs.push (job);
}

ThreadedJobQueue::~ThreadedJobQueue ()
{
	mThreadsDone = true;
	mCond.notifyAll();
	mThreads.join ();
}
#endif

};
};
