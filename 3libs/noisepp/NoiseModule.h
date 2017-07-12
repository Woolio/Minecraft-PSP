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

#ifndef NOISEPP_MODULE_H
#define NOISEPP_MODULE_H

#include "NoisePrerequisites.h"
#include "NoisePipeline.h"

namespace noisepp
{
	typedef unsigned short ModuleTypeId;
	enum
	{
		MODULE_PERLIN=0,
		MODULE_BILLOW=1,
		MODULE_ADDITION=2,
		MODULE_ABSOLUTE=3,
		MODULE_BLEND=4,
		MODULE_CHECKERBOARD=5,
		MODULE_CLAMP=6,
		MODULE_CONSTANT=7,
		MODULE_CURVE=8,
		MODULE_EXPONENT=9,
		MODULE_INVERT=10,
		MODULE_MAXIMUM=11,
		MODULE_MINIMUM=12,
		MODULE_MULTIPLY=13,
		MODULE_POWER=14,
		MODULE_RIDGEDMULTI=15,
		MODULE_SCALEBIAS=16,
		MODULE_SELECT=17,
		MODULE_SCALEPOINT=18,
		MODULE_TURBULENCE=19,
		MODULE_TERRACE=20,
		MODULE_TRANSLATEPOINT=21,
		MODULE_VORONOI=22
	};

#if NOISEPP_ENABLE_UTILS
	namespace utils
	{
		class InStream;
		class OutStream;
	};
#endif

	/// Module base class.
	class Module
	{
		protected:
			/// Array of source modules.
			const Module **mSourceModules;
			/// Number of source modules.
			size_t mSourceModuleCount;

		public:
			/// @param sourceModuleCount The number of source modules.
			Module (size_t sourceModuleCount=0) : mSourceModules(NULL), mSourceModuleCount(sourceModuleCount)
			{
				if (mSourceModuleCount)
				{
					mSourceModules = new const Module *[mSourceModuleCount];
					memset (mSourceModules, 0, mSourceModuleCount*sizeof(Module*));
				}
			}
			/// Returns the number of source modules.
			size_t getSourceModuleCount () const
			{
				return mSourceModuleCount;
			}
			/// Recursively looks for the specified module in all source modules.
			/// @param module The module to look for.
			bool walkTree (const Module *module) const
			{
				for (size_t i=0;i<mSourceModuleCount;++i)
				{
					if (mSourceModules[i] == module)
						return true;
				}
				for (size_t i=0;i<mSourceModuleCount;++i)
				{
					if (mSourceModules[i] && mSourceModules[i]->walkTree(module))
						return true;
				}
				return false;
			}
			/// Sets a source module
			/// @param id The place to put the module.
			/// @param module The source module.
			void setSourceModule (size_t id, const Module *module)
			{
//				NoiseAssertRange (id, mSourceModuleCount);
//				NoiseAssert (module != NULL, module);
//				NoiseAssert (module != this, module);
//				NoiseAssert (!module->walkTree(this), module);
				mSourceModules[id] = module;
			}
			/// @copydoc setSourceModule(size_t, const Module*).
			void setSourceModule (size_t id, const Module &module)
			{
				setSourceModule (id, &module);
			}
			/// Returns the specified source module.
			const Module *getSourceModule (size_t id) const
			{
//				NoiseAssertRange (id, mSourceModuleCount);
				return mSourceModules[id];
			}
			/// Adds the module to the specified pipeline.
			virtual ElementID addToPipeline (Pipeline1D *pipe) const = 0;
			/// Adds the module to the specified pipeline.
			virtual ElementID addToPipeline (Pipeline2D *pipe) const = 0;
			/// Adds the module to the specified pipeline.
			virtual ElementID addToPipeline (Pipeline3D *pipe) const = 0;
			/// Adds the module to the specified pipeline.
			ElementID addToPipe (Pipeline1D &pipe) const
			{
				return addToPipeline(&pipe);
			}
			/// Adds the module to the specified pipeline.
			ElementID addToPipe (Pipeline2D &pipe) const
			{
				return addToPipeline(&pipe);
			}
			/// Adds the module to the specified pipeline.
			ElementID addToPipe (Pipeline3D &pipe) const
			{
				return addToPipeline(&pipe);
			}
			/// Returns the module type ID.
			virtual ModuleTypeId getType() const = 0;
			virtual ~Module ()
			{
				if (mSourceModules)
				{
					delete[] mSourceModules;
					mSourceModules = NULL;
				}
			}
#if NOISEPP_ENABLE_UTILS
			/// Writes the module to the specified data stream
			virtual void write (utils::OutStream &stream) const;
			/// Reads the module from the specified data stream
			virtual void read (utils::InStream &stream);
#endif
	};

	#define NoiseModuleCheckSourceModules \
		for (size_t n=0;n<mSourceModuleCount;++n) \
		{ \
			if (!mSourceModules[n]) \
				NoiseThrowNoModuleException; \
		}

	/// Template class for a module with one source module
	template <class Element1D, class Element2D, class Element3D>
	class SingleSourceModule : public Module
	{
		public:
			/// Constructor.
			SingleSourceModule() : Module(1)
			{
			}
			/// @copydoc noisepp::Module::addToPipeline()
			virtual ElementID addToPipeline (Pipeline1D *pipe) const
			{
//				NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				return pipe->addElement (this, new Element1D(pipe, first));
			}
			/// @copydoc noisepp::Module::addToPipeline()
			virtual ElementID addToPipeline (Pipeline2D *pipe) const
			{
//				NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				return pipe->addElement (this, new Element2D(pipe, first));
			}
			/// @copydoc noisepp::Module::addToPipeline()
			virtual ElementID addToPipeline (Pipeline3D *pipe) const
			{
//				NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				return pipe->addElement (this, new Element3D(pipe, first));
			}
	};

	/// Template class for a module with two source modules
	template <class Element1D, class Element2D, class Element3D>
	class DualSourceModule : public Module
	{
		public:
			/// Constructor.
			DualSourceModule() : Module(2)
			{
			}
			/// @copydoc noisepp::Module::addToPipeline()
			virtual ElementID addToPipeline (Pipeline1D *pipe) const
			{
///				NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				ElementID second = getSourceModule(1)->addToPipeline(pipe);
				return pipe->addElement (this, new Element1D(pipe, first, second));
			}
			/// @copydoc noisepp::Module::addToPipeline()
			virtual ElementID addToPipeline (Pipeline2D *pipe) const
			{
//				NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				ElementID second = getSourceModule(1)->addToPipeline(pipe);
				return pipe->addElement (this, new Element2D(pipe, first, second));
			}
			/// @copydoc noisepp::Module::addToPipeline()
			virtual ElementID addToPipeline (Pipeline3D *pipe) const
			{
//				NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				ElementID second = getSourceModule(1)->addToPipeline(pipe);
				return pipe->addElement (this, new Element3D(pipe, first, second));
			}
	};

	/// Basic template class for a module with two source modules and a control module
	class TripleSourceModuleBase : public Module
	{
		public:
			/// Constructor.
			TripleSourceModuleBase() : Module(3)
			{
			}
			/// Sets the control module.
			void setControlModule (const Module &module)
			{
				setSourceModule (2, module);
			}
			/// Sets the control module.
			void setControlModule (const Module *module)
			{
				setSourceModule (2, module);
			}
			/// Returns the control module.
			const Module *getControlModule () const
			{
				return getSourceModule(2);
			}
	};

	/// Template class for a module with two source modules and a control module
	template <class Element1D, class Element2D, class Element3D>
	class TripleSourceModule : public TripleSourceModuleBase
	{
		public:
			/// @copydoc noisepp::Module::addToPipeline()
			virtual ElementID addToPipeline (Pipeline1D *pipe) const
			{
//				NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				ElementID second = getSourceModule(1)->addToPipeline(pipe);
				ElementID third = getSourceModule(2)->addToPipeline(pipe);
				return pipe->addElement (this, new Element1D(pipe, first, second, third));
			}
			/// @copydoc noisepp::Module::addToPipeline()
			virtual ElementID addToPipeline (Pipeline2D *pipe) const
			{
//				NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				ElementID second = getSourceModule(1)->addToPipeline(pipe);
				ElementID third = getSourceModule(2)->addToPipeline(pipe);
				return pipe->addElement (this, new Element2D(pipe, first, second, third));
			}
			/// @copydoc noisepp::Module::addToPipeline()
			virtual ElementID addToPipeline (Pipeline3D *pipe) const
			{
//				NoiseModuleCheckSourceModules;
				ElementID first = getSourceModule(0)->addToPipeline(pipe);
				ElementID second = getSourceModule(1)->addToPipeline(pipe);
				ElementID third = getSourceModule(2)->addToPipeline(pipe);
				return pipe->addElement (this, new Element3D(pipe, first, second, third));
			}
	};
};

#endif
