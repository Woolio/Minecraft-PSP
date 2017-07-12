/*
 * This file is part of "Phoenix Game Engine".
 *
 * Copyright (C) 2008 Phoenix Game Engine
 * Copyright (C) 2008 InsertWittyName <tias_dp@hotmail.com>
 * Copyright (C) 2008 MK2k <pge@mk2k.net>
 *
 * Phoenix Game Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 
 * Phoenix Game Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 
 * You should have received a copy of the GNU General Public License
 * along with Phoenix Game Engine.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#ifdef __PSP__
	#include <sys/stat.h>
#endif

#include "pgeFile.h"
#include "pge.h"

typedef pgeFile* (*pgeFileOpenFunc)(char*, int);
typedef int  (*pgeFileCloseFunc)(pgeFile*);
typedef int (*pgeFileReadFunc)(pgeFile*, void*, unsigned int);
typedef int (*pgeFileWriteFunc)(pgeFile*, void*, unsigned int);
typedef int (*pgeFileSeekFunc)(pgeFile*, int, unsigned int);
typedef int (*pgeFileTellFunc)(pgeFile*);
typedef unsigned int (*pgeFileSizeFunc)(pgeFile*);

// Internal Prototypes
pgeFile* pgeFileOpenFile(char* name, int flags);
int pgeFileCloseFile(pgeFile* s);
int pgeFileReadFile(pgeFile* s, void* buf, unsigned int size);
int pgeFileWriteFile(pgeFile* s, void* buf, unsigned int size);
int pgeFileSeekFile(pgeFile* s, int offs, unsigned int dir);
int pgeFileTellFile(pgeFile* s);
unsigned int pgeFileSizeFile(pgeFile* s);

pgeFile* pgeFileOpenAsync(char* name, int flags);
int pgeFileCloseAsync(pgeFile* s);
int pgeFileReadAsync(pgeFile* s, void* buf, unsigned int size);
int pgeFileWriteAsync(pgeFile* s, void* buf, unsigned int size);
int pgeFileSeekAsync(pgeFile* s, int offs, unsigned int dir);
int pgeFileTellAsync(pgeFile* s);
unsigned int pgeFileSizeAsync(pgeFile* s);

pgeFile* pgeFileOpenMemory(char* data, int size);
int pgeFileMemoryClose(pgeFile* s);
int pgeFileMemoryRead(pgeFile* s, void* buf, unsigned int size);
int pgeFileMemoryWrite(pgeFile* s, void* buf, unsigned int size);
int pgeFileMemorySeek(pgeFile* s, int offs, unsigned int dir);
int pgeFileMemoryTell(pgeFile* s);
unsigned int pgeFileMemorySize(pgeFile* s);

typedef struct
{
	int					type;			// Stream type ID
	int					eos;			// EOS flag (0 for protocols)
	unsigned int		size;			// Stream size in bytes (0 for protocols)
	unsigned int		mode;			// Stream open mode (possible open modes for protocols)
	char				name[256];		// Stream name (filename/memory address)

	pgeFileOpenFunc		open;
	pgeFileCloseFunc	close;
	pgeFileReadFunc		read;
	pgeFileWriteFunc	write;
	pgeFileSeekFunc		seek;
	pgeFileTellFunc		tell;
	pgeFileSizeFunc		fsize;
	
} pgeFileBase, pgeFileProtocol;

typedef struct
{
	pgeFileBase	s;
	char*			data;
	char*			cur;
	unsigned int	size;
	unsigned int	pos;
	
} pgeMemoryStream;

typedef struct
{
	pgeFileBase		s;
	int				fd;
	unsigned int	fsize;
	
} pgeFileStream;

struct pgeFileStruct
{
	pgeFileBase*	_stream;
};

pgeFileProtocol pgeFileProtocolFile =
{
	PGE_FILE_TYPE_FILE,
	0,
	0,
	PGE_FILE_ALL,
	{0},
	pgeFileOpen,
	pgeFileCloseFile,
	pgeFileReadFile,
	pgeFileWriteFile,
	pgeFileSeekFile,
	pgeFileTellFile,
	pgeFileSizeFile
};

pgeFile* pgeFileOpen(char* name, int flags)
{
	pgeFile* s = (pgeFile*)pgeMalloc(sizeof(pgeFile) + sizeof(pgeFileStream));
	
	if(s == 0)
		return NULL;
	
	s->_stream = (pgeFileBase*)(s + 1);
	*s->_stream = pgeFileProtocolFile;
	s->_stream->mode &= flags;
	
	pgeFileStream* f = (pgeFileStream*)s->_stream;

	f->fd = sceIoOpen(name, s->_stream->mode, 0777);
	
	if(f->fd < 0)
	{
		pgeFree(s);
		return NULL;
	}
	
	snprintf(s->_stream->name, 256, "%s", name);
	
	f->fsize = sceIoLseek(f->fd, 0, PSP_SEEK_END);
	s->_stream->size = f->fsize;
	sceIoLseek(f->fd, 0, PSP_SEEK_SET);
	
	return s;
}

int pgeFileCloseFile(pgeFile* s)
{
	return sceIoClose(((pgeFileStream*)s->_stream)->fd);
}

int pgeFileReadFile(pgeFile* s, void* buf, unsigned int size)
{
	int read = sceIoRead(((pgeFileStream*)s->_stream)->fd, buf, size);
	
	if(((pgeFileStream*)s->_stream)->fsize == (unsigned int)sceIoLseek32(((pgeFileStream*)s->_stream)->fd, 0, PGE_FILE_CUR))
		s->_stream->eos = 1;

	return read;
}

int pgeFileWriteFile(pgeFile* s, void* buf, unsigned int size)
{
	int written = sceIoWrite(((pgeFileStream*)s->_stream)->fd, buf, size);
	
	if(written > 0)
	{
		int oldsize = ((pgeFileStream*)s->_stream)->fsize;
		int newsize = sceIoLseek32(((pgeFileStream*)s->_stream)->fd, 0, PGE_FILE_CUR);
		
		if(newsize > oldsize || (s->_stream->mode & PGE_FILE_TRUNC))
		{
			((pgeFileStream*)s->_stream)->fsize = newsize;
			s->_stream->size = newsize;
			s->_stream->eos = 1;
		}
	}
	
	return written;
}

int pgeFileSeekFile(pgeFile* s, int offs, unsigned int dir)
{
	s->_stream->eos = 0;
	
	unsigned int pos = sceIoLseek32(((pgeFileStream*)s->_stream)->fd, offs, dir);
	
	if(pos == ((pgeFileStream*)s->_stream)->fsize)
		s->_stream->eos = 1;
		
	return pos;
}

int pgeFileTellFile(pgeFile* s)
{
	return sceIoLseek32(((pgeFileStream*)s->_stream)->fd, 0, PGE_FILE_CUR);
}

unsigned int pgeFileSizeFile(pgeFile* s)
{
	return ((pgeFileStream*)s->_stream)->fsize;
}

pgeFileProtocol pgeFileAsyncProtocol =
{
	PGE_FILE_TYPE_FILE,
	0,
	0,
	PGE_FILE_ALL,
	{0},
	pgeFileOpenAsync,
	pgeFileCloseAsync,
	pgeFileReadAsync,
	pgeFileWriteAsync,
	pgeFileSeekAsync,
	pgeFileTellAsync,
	pgeFileSizeAsync
};

pgeFile* pgeFileOpenAsync(char* name, int flags)
{
	pgeFile* s = (pgeFile*)pgeMalloc(sizeof(pgeFile) + sizeof(pgeFileStream));
	
	if(s == 0)
		return NULL;
	
	s->_stream = (pgeFileBase*)(s + 1);
	*s->_stream = pgeFileAsyncProtocol;
	s->_stream->mode &= flags;
	
	pgeFileStream* f = (pgeFileStream*)s->_stream;

	f->fd = sceIoOpen(name, s->_stream->mode, 0777);
	
	if (f->fd < 0)
	{
		pgeFree(s);
		return NULL;
	}
	
	sceIoChangeAsyncPriority(f->fd, 0x10);
	
	snprintf(s->_stream->name, 256, "%s", name);
	
	f->fsize = sceIoLseek(f->fd, 0, PSP_SEEK_END);
	s->_stream->size = f->fsize;
	sceIoLseek(f->fd, 0, PSP_SEEK_SET);
	
	return s;
}

int pgeFileCloseAsync(pgeFile* s)
{
	return sceIoClose(((pgeFileStream*)s->_stream)->fd);
}

int pgeFileReadAsync(pgeFile* s, void* buf, unsigned int size)
{
	long long result;
	
	int read = sceIoReadAsync(((pgeFileStream*)s->_stream)->fd, buf, size);
	
	sceIoWaitAsync(((pgeFileStream*)s->_stream)->fd, &result);
	
	if(((pgeFileStream*)s->_stream)->fsize == (unsigned int)sceIoLseek32(((pgeFileStream*)s->_stream)->fd, 0, PGE_FILE_CUR))
		s->_stream->eos = 1;

	return read;
}

int pgeFileWriteAsync(pgeFile* s, void* buf, unsigned int size)
{
	long long result;
	
	int written = sceIoWriteAsync(((pgeFileStream*)s->_stream)->fd, buf, size);
	
	sceIoWaitAsync(((pgeFileStream*)s->_stream)->fd, &result);
	
	if(written > 0)
	{
		int oldsize = ((pgeFileStream*)s->_stream)->fsize;
		int newsize = sceIoLseek32(((pgeFileStream*)s->_stream)->fd, 0, PGE_FILE_CUR);
		
		if(newsize > oldsize || (s->_stream->mode & PGE_FILE_TRUNC))
		{
			((pgeFileStream*)s->_stream)->fsize = newsize;
			s->_stream->size = newsize;
			s->_stream->eos = 1;
		}
	}
	
	return written;
}

int pgeFileSeekAsync(pgeFile* s, int offs, unsigned int dir)
{
	s->_stream->eos = 0;
	
	unsigned int pos = sceIoLseek32(((pgeFileStream*)s->_stream)->fd, offs, dir);
	
	if(pos == ((pgeFileStream*)s->_stream)->fsize)
		s->_stream->eos = 1;
		
	return pos;
}

int pgeFileTellAsync(pgeFile* s)
{
	return sceIoLseek32(((pgeFileStream*)s->_stream)->fd, 0, PGE_FILE_CUR);
}

unsigned int pgeFileSizeAsync(pgeFile* s)
{
	return ((pgeFileStream*)s->_stream)->fsize;
}

pgeFileProtocol pgeFileMemoryProtocol =
{
	PGE_FILE_TYPE_MEM,
	0,
	0,
	PGE_FILE_RDWR,
	{0},
	pgeFileOpenMemory,
	pgeFileMemoryClose,
	pgeFileMemoryRead,
	pgeFileMemoryWrite,
	pgeFileMemorySeek,
	pgeFileMemoryTell,
	pgeFileMemorySize
};

pgeFile* pgeFileOpenMemory(char* data, int size)
{
	pgeFile* s = (pgeFile*)pgeMalloc(sizeof(pgeFile) + sizeof(pgeMemoryStream));
	
	if(s == 0)
		return NULL;
	
	s->_stream = (pgeFileBase*)(s + 1);
	
	if(data == 0)
	{
		s->_stream->mode |= PGE_FILE_CREATE|PGE_FILE_APPEND;
		data = pgeMalloc(size);
		
		if(data == 0)
			size = 0;
	}
	
	*s->_stream = pgeFileMemoryProtocol;
	
	snprintf(s->_stream->name, 256, "%p", data );
	s->_stream->size = size;
	
	((pgeMemoryStream*)s->_stream)->data = data;
	((pgeMemoryStream*)s->_stream)->cur  = data;
	((pgeMemoryStream*)s->_stream)->size = size;
	((pgeMemoryStream*)s->_stream)->pos  = 0;
	
	return s;
}

int pgeFileMemoryClose(pgeFile* s)
{
	if(s->_stream->mode & PGE_FILE_CREATE)
	{
		pgeFree(((pgeMemoryStream*)s->_stream)->data);
		((pgeMemoryStream*)s->_stream)->data = 0;
	}
	
	return 0;
}

int pgeFileMemoryRead(pgeFile* s, void* buf, unsigned int size)
{
	pgeMemoryStream* m = (pgeMemoryStream*)s->_stream;
	
	if(m->data == 0 || m->size == 0)
		return -1;
	
	if(m->cur == 0) m->cur = m->data + m->pos;
	
	if(m->pos + size < m->size)
	{
		memcpy(buf, m->cur, size);
		m->pos += size;
		m->cur += size;
		return size;
	}
	
	int remain = m->size-m->pos;
	memcpy(buf, m->cur, remain);
	m->pos += remain;
	m->cur += remain;
	s->_stream->eos = 1;
	return remain;
}

int pgeFileMemoryWrite(pgeFile* s, void* buf, unsigned int size)
{
	pgeMemoryStream* m = (pgeMemoryStream*)s->_stream;
	
	if(m->data == 0 || m->size == 0)
	{
		s->_stream->mode |= PGE_FILE_CREATE|PGE_FILE_APPEND;
		m->data = pgeMalloc(size);
		
		if(m->data == 0)
			m->size = 0;
			
		snprintf(s->_stream->name, 256, "%p", m->data);
	}
	
	if(m->data == 0 || m->size == 0)
		return -1;
	
	if(m->cur == 0)
		m->cur = m->data + m->pos;
	
	if(m->pos + size < m->size)
	{
		memcpy(m->cur, buf, size);
		m->pos += size;
		m->cur += size;
		return size;
	}
	
	// do a realloc to make room for the extra write request if stream can be appended to
	if(s->_stream->mode & PGE_FILE_APPEND)
	{
		m->data = pgeRealloc(m->data, m->pos + size);

		if(m->data == 0)
		{
			m->pos = 0;
			m->cur = 0;
			m->size = 0;
			s->_stream->size = 0;
			s->_stream->eos = 1;
			return -1;
		}
		
		m->cur = m->data + m->pos;
		m->size = m->pos + size;
		s->_stream->size = m->size;
	}
	
	int remain = m->size - m->pos;
	memcpy(m->cur, buf, remain);
	m->pos += remain;
	m->cur += remain;
	s->_stream->eos = 1;
	
	return remain;
}

int pgeFileMemorySeek(pgeFile* s, int offs, unsigned int dir)
{
	pgeMemoryStream* m = (pgeMemoryStream*)s->_stream;
	
	switch(dir)
	{
		case PGE_FILE_SET:
			m->pos = offs;
			break;
		case PGE_FILE_END:
			m->pos = m->size + offs;
			break;
		case PGE_FILE_CUR:
		default:
			m->pos += offs;
			break;
	}
	
	s->_stream->eos = 0;
	
	if(m->pos >= m->size)
	{
		m->pos = m->size;
		s->_stream->eos = 1;
	}
	
	m->cur = m->data + m->pos;
	
	return m->pos;
}

int pgeFileMemoryTell(pgeFile* s)
{
	return ((pgeMemoryStream*)s->_stream)->pos;
}

unsigned int pgeFileMemorySize(pgeFile* s)
{
	return ((pgeMemoryStream*)s->_stream)->size;
}

int pgeFileClose(pgeFile* s)
{
	if(s == 0 || s->_stream == 0 || s->_stream->close == 0)
		return -1;
		
	int ret = s->_stream->close(s);
	s->_stream = 0;
	pgeFree(s);
	
	return ret;
}

int pgeFileRead(pgeFile* s, void* buf, unsigned int size)
{
	if(s == 0 || s->_stream == 0 || s->_stream->read == 0 || (s->_stream->mode & (PGE_FILE_RDONLY|PGE_FILE_RDWR)) == 0)
		return -1;
		
	return s->_stream->read(s, buf, size);
}

int pgeFileWrite(pgeFile* s, void* buf, unsigned int size)
{
	if(s == 0 || s->_stream == 0 || s->_stream->write == 0 || (s->_stream->mode & (PGE_FILE_WRONLY|PGE_FILE_RDWR)) == 0)
		return -1;
		
	return s->_stream->write(s, buf, size);
}

int pgeFileSeek(pgeFile* s, int offs, unsigned int dir)
{
	if(s == 0 || s->_stream == 0 || s->_stream->seek == 0)
		return -1;
		
	return s->_stream->seek(s, offs, dir);
}

int pgeFileTell(pgeFile* s)
{
	if(s == 0 || s->_stream == 0 || s->_stream->tell == 0)
		return -1;
		
	return s->_stream->tell(s);
}

int pgeFileRewind(pgeFile* s)
{
	return pgeFileSeek(s, 0, PGE_FILE_SET);
}

unsigned int pgeFileSize(pgeFile *s)
{
	return s->_stream->fsize(s);
}

int pgeFileRemove(const char *filepath)
{
	int result = sceIoRemove(filepath);
	
	if(result == 0)
		return 1;
		
	return result;
}

int pgeFileRename(const char *oldname, const char *newname)
{
	int result = sceIoRename(oldname, newname);
	
	if(result == 0)
		return 1;
		
	return result;
}

int pgeFileExists(const char *filename)
{ 
	SceIoStat stats;
	
	int result = sceIoGetstat(filename, &stats);
	
	if(result < 0)
		return 0;
	
	if(stats.st_mode & FIO_S_IFDIR) 
		return 0; 
	 
	return 1;  
}
