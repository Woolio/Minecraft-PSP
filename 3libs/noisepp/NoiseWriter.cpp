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

#include "NoiseWriter.h"
#include "NoiseUtils.h"

namespace noisepp
{
namespace utils
{

Writer::Writer (OutStream &stream) : mStream(stream), mModuleCount(0)
{
}

unsigned short Writer::getModuleID (const Module *module)
{
//	NoiseAssert (module != NULL, module);
	ModuleMap::iterator it = mModules.find(module);
//	NoiseAssert (it != mModules.end(), module);
	return it->second;
}

unsigned short Writer::addModule (const Module *module)
{
	//NoiseAssert (module != NULL, module);
	ModuleMap::iterator it = mModules.find (module);
	if (it == mModules.end())
	{
		unsigned short id = mModuleCount++;
		mModules.insert(std::make_pair(module, id));
		mModuleVec.push_back (module);

		// add child modules
		for (size_t i=0;i<module->getSourceModuleCount();++i)
		{
			const Module *child = module->getSourceModule(i);
		//	assert (child);
			addModule (child);
		}

		return id;
	}
	return it->second;
}

void Writer::writeModule (const Module *module)
{
	//NoiseAssert (module != NULL, module);
	unsigned short typeID = module->getType();
	mStream.write (typeID);
	module->write (mStream);
}

void Writer::writeModuleRel (const Module *module)
{
	//NoiseAssert (module != NULL, module);
	for (size_t i=0;i<module->getSourceModuleCount();++i)
	{
		const Module *child = module->getSourceModule(i);
		unsigned short id = getModuleID(child);
		mStream.write (id);
	}
}

void Writer::writePipeline ()
{
	//NoiseAssert (!mModuleVec.empty(), mModuleVec);
	//NoiseAssert (mModuleCount == mModules.size(), mModuleCount);
	//NoiseAssert (mModuleCount == mModuleVec.size(), mModuleCount);

	unsigned char ver = NOISE_FILE_VERSION;
	mStream.write (ver);
	mStream.write (mModuleCount);
	// writing module properties
	for (ModuleVector::iterator it=mModuleVec.begin();it!=mModuleVec.end();++it)
		writeModule(*it);
	// writing module relations
	for (ModuleVector::iterator it=mModuleVec.begin();it!=mModuleVec.end();++it)
		writeModuleRel(*it);
}

};
};
