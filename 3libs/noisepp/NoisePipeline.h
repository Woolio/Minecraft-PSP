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

#ifndef NOISEPP_PIPELINE_H
#define NOISEPP_PIPELINE_H

#include "NoisePrerequisites.h"

namespace noisepp
{
	class Module;

	/// Cache structure for faster pipeline processing.
	struct Cache
	{
		/// Cached value.
		Real value;
		/// Cache filled flag.
		bool filled;
		/// Last x coordinate.
		Real x;
		/// Last y coordinate.
		Real y;
		/// Last z coordinate.
		Real z;
		/// Constructor.
		Cache () : value(0), filled(false) {}
	};

	/// A job which can be added to the queue inside a pipeline for multi-threaded execution.
	class PipelineJob
	{
		public:
			/// Destructor.
			virtual ~PipelineJob () {}
			/// This function is called when the job is executed. You must overwrite this.
			/// Don't do anything thread unsafe in there!
			virtual void execute (Cache *cache) = 0;
			/// This function is called when the job is finished. It is always called in the main thread,
			/// so you can update a progress bar here or whatever.
			virtual void finish () {}
	};

	/// A queue of pipeline jobs
	typedef std::queue<PipelineJob*> PipelineJobQueue;

	/** Pipeline base class.
		In Noise++ the noise generation process is different to other libraries.
		Instead of calling a noise generation function from your module instances directly,
		you add your modules to a so called Pipeline.
		The pipeline internally creates pipeline elements from the specified noise modules.
		Then you create a cache and start generating.
	*/
	template <class Element>
	class Pipeline
	{
		private:
			int mSeed;

		protected:
			/// Element vector.
			std::vector<Element*> mElements;
			/// Map holding the module pointers.
			std::map<const Module*, ElementID> mElementIDs;
			/// The job queue.
			PipelineJobQueue mJobs;

		public:
			/// Constructor.
			Pipeline () : mSeed(0)
			{
			}
			/// Returns the element with the specified ID.
			NOISEPP_INLINE Element *getElement (ElementID i) const
			{
//				NoiseAssertRange (i, mElements.size());
				return mElements[i];
			}
			/// Returns the total number of elements.
			ElementID getElementCount () const
			{
				return mElements.size ();
			}
			/// Sets the master seed value.
			/// You have to call this BEFORE adding your modules or this will have no effect.
			void setSeed (int seed)
			{
				mSeed = seed;
			}
			/// Returns the master seed value.
			int getSeed () const
			{
				return mSeed;
			}
			/// Creates a clean cache.
			/// You need only one cache per pipeline and thread.
			/// You have to call this AFTER adding your modules or there will be memory acces errors.
			/// Don't forget to free the cache.
			Cache *createCache () const
			{
				return new Cache[mElements.size()];
			}
			/// Cleans the specified cache.
			/// You should call this each time you use it.
			NOISEPP_INLINE void cleanCache (Cache *cache) const
			{
				memset (cache, 0, sizeof(Cache)*mElements.size());
			}
			/// Frees the specified cache.
			void freeCache (Cache *cache) const
			{
				delete[] cache;
			}
			/// Adds the specified element to the pipeline.
			/// This is used internally by modules.
			ElementID addElement (const Module *parent, Element *element)
			{
//				NoiseAssert (element != NULL, element);
//				NoiseAssert (parent != NULL, parent);
				std::map<const Module*, ElementID>::iterator it = mElementIDs.find(parent);
				if (it != mElementIDs.end())
				{
					delete element;
					return it->second;
				}
				ElementID id = mElements.size ();
				mElementIDs.insert (std::make_pair(parent, id));
				mElements.push_back(element);
				return id;
			}
			/// Returns the ID of the element belonging to the specified module or ELEMENTID_INVALID if not found.
			ElementID getElementID (const Module *module) const
			{
				std::map<const Module*, ElementID>::const_iterator it = mElementIDs.find(module);
				if (it != mElementIDs.end())
					return it->second;
				return ELEMENTID_INVALID;
			}
			/// @copydoc noisepp::Pipeline::getElementID(const noisepp::Module *) const
			inline ElementID getElementID (const Module &module) const
			{
				return getElementID(&module);
			}
			/// Returns a pointer to the element belonging to the specified module or NULL if not found
			Element *getElementPtr (const Module *module) const
			{
				std::map<const Module*, ElementID>::const_iterator it = mElementIDs.find(module);
				if (it != mElementIDs.end())
					return getElement(it->second);
				return 0;
			}
			/// @copydoc noisepp::Pipeline::getElementPtr(const noisepp::Module *) const
			inline Element *getElementPtr (const Module &module) const
			{
				return getElementPtr(&module);
			}
			/// Adds a job to the queue.
			virtual void addJob (PipelineJob *job)
			{
//				NoiseAssert (job != NULL, job);
				mJobs.push (job);
			}
			/// executes the jobs in queue
			virtual void executeJobs ()
			{
				Cache *cache = createCache();
				PipelineJob *job;
				while (!mJobs.empty())
				{
					job = mJobs.front ();
					mJobs.pop ();
					job->execute(cache);
					job->finish();
					delete job;
				}
				freeCache (cache);
			}
			/// Destructor.
			virtual ~Pipeline ()
			{
				typename std::vector<Element*>::iterator itEnd = mElements.end();
				for (typename std::vector<Element*>::iterator it=mElements.begin();it!=itEnd;++it)
				{
					delete *it;
				}
				mElements.clear ();
				mElementIDs.clear ();
				while (!mJobs.empty())
				{
					delete mJobs.front ();
					mJobs.pop ();
				}
			}
	};

	/** 1D pipeline.
		In Noise++ the noise generation process is much different to other libraries.
		Instead of calling a noise generation function from your module instances directly,
		you add your modules to a so called Pipeline.
		The pipeline internally creates pipeline elements from the specified noise modules.
		Then you create a cache and start generating.
	*/
	typedef Pipeline<PipelineElement1D> Pipeline1D;
	/** 2D pipeline.
		In Noise++ the noise generation process is much different to other libraries.
		Instead of calling a noise generation function from your module instances directly,
		you add your modules to a so called Pipeline.
		The pipeline internally creates pipeline elements from the specified noise modules.
		Then you create a cache and start generating.
	*/
	typedef Pipeline<PipelineElement2D> Pipeline2D;
	/** 3D pipeline.
		In Noise++ the noise generation process is much different to other libraries.
		Instead of calling a noise generation function from your module instances directly,
		you add your modules to a so called Pipeline.
		The pipeline internally creates pipeline elements from the specified noise modules.
		Then you create a cache and start generating.
	*/
	typedef Pipeline<PipelineElement3D> Pipeline3D;

	class PipelineElement1D
	{
		protected:
			NOISEPP_INLINE Real getElementValue (const PipelineElement1D *elementPtr, ElementID element, Real x, Cache *cache) const
			{
				if (cache[element].filled && cache[element].x == x)
				{
					return cache[element].value;
				}
				else
				{
					cache[element].filled = true;
					cache[element].x = x;
					return (cache[element].value = elementPtr->getValue(x, cache));
				}
			}

			bool mCached;
		public:
			virtual Real getValue (Real x, Cache *cache) const = 0;
			virtual ~PipelineElement1D () {}
	};

	class PipelineElement2D
	{
		protected:
			NOISEPP_INLINE Real getElementValue (const PipelineElement2D *elementPtr, ElementID element, Real x, Real y, Cache *cache) const
			{
				if (cache[element].filled && cache[element].x == x && cache[element].y == y)
				{
					return cache[element].value;
				}
				else
				{
					cache[element].filled = true;
					cache[element].x = x;
					cache[element].y = y;
					return (cache[element].value = elementPtr->getValue(x, y, cache));
				}
			}

		public:
			virtual Real getValue (Real x, Real y, Cache *cache) const = 0;
			virtual ~PipelineElement2D () {}
	};

	class PipelineElement3D
	{
		protected:
			NOISEPP_INLINE Real getElementValue (const PipelineElement3D *elementPtr, ElementID element, Real x, Real y, Real z, Cache *cache) const
			{
				if (cache[element].filled && cache[element].x == x && cache[element].y == y && cache[element].z == z)
				{
					return cache[element].value;
				}
				else
				{
					cache[element].filled = true;
					cache[element].x = x;
					cache[element].y = y;
					cache[element].z = z;
					return (cache[element].value = elementPtr->getValue(x, y, z, cache));
				}
			}

		public:
			virtual Real getValue (Real x, Real y, Real z, Cache *cache) const = 0;
			virtual ~PipelineElement3D () {}
	};
};

#endif
