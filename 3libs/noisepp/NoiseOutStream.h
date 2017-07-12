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

#ifndef NOISEOUTSTREAM_H
#define NOISEOUTSTREAM_H

#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>

#include "NoiseEndianUtils.h"

namespace noisepp
{
namespace utils
{

/// Basic output stream class.
class OutStream
{
	public:
		/// Constructor.
		OutStream();
		/// Write len bytes from buffer.
		virtual void write (const void *buffer, size_t len) = 0;
		/// Returns the current position.
		virtual size_t tell () = 0;
		/// Jumps to the specified position.
		virtual void seek (size_t pos) = 0;
		/// Destructor.
		virtual ~OutStream();

		/// Write to stream and flip endian if required.
		template <class T>
		void write (T t)
		{
			EndianUtils::flipEndian (&t, sizeof(T));
			write (&t, sizeof(T));
		}
		/// Write an integer.
		void writeInt (int t)
		{
			write (t);
		}
		/// Write a double.
		void writeDouble (double t)
		{
			write (t);
		}
	protected:
	private:
};

/// Stream for writing to files.
class FileOutStream : public OutStream
{
	private:
		std::ofstream mFile;
	public:
		/// Constructor.
		FileOutStream();
		/// Constructor.
		/// @param filename The name of the output file.
		FileOutStream(const std::string &filename);
		/// Open the specified file for writing.
		/// Returns true on success.
		bool open (const std::string &filename);
		/// Check if a file is open.
		/// Returns true if the stream is currently associated with a file, and false otherwise.
		bool isOpen ();
		/// Close the current file.
		void close ();
		/// @copydoc noisepp::utils::OutStream::write(T)
		template <class T>
		void write (T t)
		{
			EndianUtils::flipEndian (&t, sizeof(T));
			write (&t, sizeof(T));
		}
		/// @copydoc noisepp::utils::OutStream::write(const void *, size_t)
		virtual void write (const void *buffer, size_t len);
		/// @copydoc noisepp::utils::OutStream::tell()
		virtual size_t tell ();
		/// @copydoc noisepp::utils::OutStream::seek()
		virtual void seek (size_t pos);
};

/// Stream for writing to memory.
class MemoryOutStream : public OutStream
{
	private:
		char *mBuffer;
		size_t mPosition, mSize, mRealSize;
	public:
		/// Constructor.
		MemoryOutStream ();
		/// Clears the buffer.
		void clear ();
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
		/// @copydoc noisepp::utils::OutStream::write(const void *, size_t)
		template <class T>
		void write (T t)
		{
			EndianUtils::flipEndian (&t, sizeof(T));
			write (&t, sizeof(T));
		}
		/// Destructor.
		virtual ~MemoryOutStream ();
		/// @copydoc noisepp::utils::OutStream::write(const void *, size_t)
		virtual void write (const void *buffer, size_t len);
		/// @copydoc noisepp::utils::OutStream::tell()
		virtual size_t tell ();
		/// @copydoc noisepp::utils::OutStream::seek()
		virtual void seek (size_t pos);
};

};
};

#endif // NOISEOUTSTREAM_H
