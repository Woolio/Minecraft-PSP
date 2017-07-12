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

#ifndef NOISEINSTREAM_H
#define NOISEINSTREAM_H

#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>

#include "NoiseEndianUtils.h"

namespace noisepp
{
namespace utils
{

/// Basic input stream class.
class InStream
{
	public:
		/// Constructor.
		InStream();
		/// Read len bytes to buffer.
		virtual void read (void *buffer, size_t len) = 0;
		/// Returns the current position.
		virtual size_t tell () = 0;
		/// Jumps to the specified position.
		virtual void seek (size_t pos) = 0;
		/// Destructor.
		virtual ~InStream();

		/// Read from stream and flip endian if required.
		template <class T>
		void read (T &t)
		{
			read (&t, sizeof(T));
			EndianUtils::flipEndian (&t, sizeof(T));
		}

		/// Read an integer from stream.
		int readInt ()
		{
			int i;
			read (i);
			return i;
		}

		/// Read a double from stream.
		double readDouble ()
		{
			double d;
			read (d);
			return d;
		}
	protected:
	private:
};

/// Stream for reading from files.
class FileInStream : public InStream
{
	private:
		std::ifstream mFile;
	public:
		/// Constructor.
		FileInStream();
		/// Constructor.
		/// @param filename The name of the input file.
		FileInStream(const std::string &filename);
		/// Open the specified file for writing.
		/// Returns true on success, and false otherwise.
		bool open (const std::string &filename);
		/// Check if a file is open.
		/// Returns true if the stream is currently associated with a file, and false otherwise.
		bool isOpen ();
		/// Close the current file.
		void close ();
		/// @copydoc noisepp::utils::InStream::read(void *, size_t)
		virtual void read (void *buffer, size_t len);
		/// @copydoc noisepp::utils::InStream::tell()
		virtual size_t tell ();
		/// @copydoc noisepp::utils::InStream::seek()
		virtual void seek (size_t pos);
};

/// Stream for reading from memory.
class MemoryInStream : public InStream
{
	private:
		char *mBuffer;
		size_t mPosition, mSize;
	public:
		/// Constructor.
		MemoryInStream ();
		/// Open the specified buffer for reading.
		/// @param buffer The buffer.
		/// @param size The size of the buffer.
		void open (char *buffer, size_t size);
		/// Returns a pointer to the buffer.
		char *getBuffer ()
		{
			return mBuffer;
		}
		/// Returns the buffer size.
		size_t getBufferSize () const
		{
			return mSize;
		}
		/// @copydoc noisepp::utils::InStream::read(void *, size_t)
		virtual void read (void *buffer, size_t len);
		/// @copydoc noisepp::utils::InStream::tell()
		virtual size_t tell ();
		/// @copydoc noisepp::utils::InStream::seek()
		virtual void seek (size_t pos);
};

};
};

#endif // NOISEINSTREAM_H
