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

#include "NoiseInStream.h"
#include "NoiseEndianUtils.h"
#include <stdexcept>

namespace noisepp
{
namespace utils
{

InStream::InStream()
{
}

InStream::~InStream()
{
}

FileInStream::FileInStream ()
{
}

FileInStream::FileInStream(const std::string &filename)
{
	open (filename);
}

bool FileInStream::open (const std::string &filename)
{
	mFile.open (filename.c_str(), std::ios::binary);
	return mFile.is_open ();
}

bool FileInStream::isOpen ()
{
	return mFile.is_open ();
}

void FileInStream::close ()
{
	mFile.close ();
}

void FileInStream::read (void *buffer, size_t len)
{
	mFile.read ((char*)buffer, (std::streamsize)len);
	//if (mFile.bad())
	//	throw std::runtime_error ("Unexpected EOF");
}

size_t FileInStream::tell ()
{
	return mFile.tellg ();
}

void FileInStream::seek (size_t pos)
{
	mFile.seekg ((std::streamoff)pos);
}

MemoryInStream::MemoryInStream () : mBuffer(NULL), mPosition(0), mSize(0)
{
}

void MemoryInStream::open (char *buffer, size_t size)
{
	mBuffer = buffer;
	mSize = size;
	mPosition = 0;
}

void MemoryInStream::read (void *buffer, size_t len)
{
	//if (mPosition+len > mSize)
	//	throw std::runtime_error ("Unexpected EOF");
	std::memcpy (buffer, mBuffer+mPosition, len);
	mPosition += len;
}

size_t MemoryInStream::tell ()
{
	return mPosition;
}

void MemoryInStream::seek (size_t pos)
{
	mPosition = pos;
}

};
};
