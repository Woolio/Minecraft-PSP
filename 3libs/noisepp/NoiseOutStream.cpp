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

#include <cassert>
#include "NoiseOutStream.h"
#include "NoiseEndianUtils.h"

#define BLOCK_SIZE 1024

namespace noisepp
{
namespace utils
{

OutStream::OutStream()
{
}

OutStream::~OutStream()
{
}

FileOutStream::FileOutStream ()
{
}

FileOutStream::FileOutStream(const std::string &filename)
{
	open (filename);
}

bool FileOutStream::open (const std::string &filename)
{
	mFile.open (filename.c_str(), std::ios::binary);
	return mFile.is_open ();
}

bool FileOutStream::isOpen ()
{
	return mFile.is_open ();
}

void FileOutStream::close ()
{
	mFile.close ();
}

void FileOutStream::write (const void *buffer, size_t len)
{
	mFile.write ((const char*)buffer, (std::streamsize)len);
}

size_t FileOutStream::tell ()
{
	return mFile.tellp ();
}

void FileOutStream::seek (size_t pos)
{
	mFile.seekp ((std::streamoff)pos);
}

MemoryOutStream::MemoryOutStream () : mBuffer(NULL), mPosition(0), mSize(0), mRealSize(0)
{
}

MemoryOutStream::~MemoryOutStream ()
{
	clear ();
}

void MemoryOutStream::clear ()
{
	if (mBuffer)
	{
		std::free (mBuffer);
		mBuffer = NULL;
	}
	mPosition = mSize = mRealSize = 0;
}

void MemoryOutStream::write (const void *buffer, size_t len)
{
	assert (len > 0);
	if (mPosition+len > mRealSize || mBuffer == NULL)
	{
		mRealSize += BLOCK_SIZE;
		mBuffer = (char *)std::realloc (mBuffer, mRealSize);
	}
	std::memcpy (mBuffer+mPosition, buffer, len);
	mPosition += len;
	if (mPosition > mSize)
		mSize = mPosition;
}

size_t MemoryOutStream::tell ()
{
	return mPosition;
}

void MemoryOutStream::seek (size_t pos)
{
	assert (pos < mSize);
	mPosition = pos;
}

};
};
