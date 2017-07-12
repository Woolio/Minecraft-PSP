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

#ifndef NOISEWRITER_H
#define NOISEWRITER_H

#include <vector>
#include <map>

#include "NoiseModule.h"
#include "NoiseOutStream.h"

namespace noisepp
{
namespace utils
{

/// Class for writing a pipeline (set of modules) to a stream or file.
class Writer
{
	private:
		OutStream &mStream;

		unsigned short mModuleCount;

		typedef std::map<const Module*, unsigned short> ModuleMap;
		typedef std::vector<const Module*> ModuleVector;
		ModuleMap mModules;
		ModuleVector mModuleVec;

		void writeModule (const Module *module);
		void writeModuleRel (const Module *module);
	public:
		/// Constructor.
		/// @param stream A reference to the stream the output will be written to.
		Writer (OutStream &stream);
		/// Adds a module and returns its ID
		unsigned short addModule (const Module *module);
		/// Adds a module and returns its ID
		unsigned short addModule (const Module &module)
		{ return addModule (&module); }
		/// Returns the ID of the specified module (the module must be added before)
		unsigned short getModuleID (const Module *module);

		/// Writes the final pipeline to the stream specified in the constructor.
		void writePipeline ();
};

};
};

#endif // NOISEWRITER_H
