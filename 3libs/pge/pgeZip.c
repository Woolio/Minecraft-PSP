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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/stat.h>

#include "pgeZip.h"
#include "pge.h"

#define PGE_ZIP_BUFFER_SIZE				(16384)
#define PGE_ZIP_MAX_FILENAME_SIZE		(256)
#define PGE_ZIP_CENTRALDIR_ITEM_SIZE	(0x2e)
#define PGE_ZIP_LOCALHEADER_SIZE		(0x1e)

#define PGE_ZIP_OK				(0)
#define PGE_ZIP_EOF_LIST		(-100)
#define PGE_ZIP_ERRNO			(Z_ERRNO)
#define PGE_ZIP_EOF				(0)
#define PGE_ZIP_PARAM_ERROR		(-102)
#define PGE_ZIP_BAD_FILE		(-103)
#define PGE_ZIP_INTERNAL_ERROR	(-104)
#define PGE_ZIP_CRC_ERROR		(-105)

#define CRC32(c, b) ((*(crc32tab+(((int)(c) ^ (b)) & 0xff))) ^ ((c) >> 8))
#define zdecode(pkeys, crc32tab, c) (pgeZipUpdateKeys(pkeys, crc32tab, c ^= pgeZipDecryptByte(pkeys, crc32tab)))
#define PGE_ZIP_BUF_READ_COMMENT (0x400)

typedef struct
{
    unsigned long version;
    unsigned long versionneeded;
    unsigned long flag;
    unsigned long compressionmethod;
    unsigned long dosdate;
    unsigned long crc;
    unsigned long compressedsize;
    unsigned long uncompressedsize;
    unsigned long filenamesize;
    unsigned long fileextrasize;
    unsigned long filecommentsize;
    unsigned long disknumstart;
    unsigned long internalfileattr;
    unsigned long externalfileattr;
	
} zipFileInfo;

typedef struct
{
	unsigned long currentfileoffset;
	
} zipFileInternalInfo;

typedef struct
{
	char *buffer;
	z_stream stream;
	unsigned long posinzip;
	unsigned long streaminitialised;
	unsigned long localextrafieldoffset;
	unsigned int  localextrafieldsize;
	unsigned long localextrafieldpos;
	unsigned long crc32;
	unsigned long crc32wait;
	unsigned long restreadcompressed;
	unsigned long restreaduncompressed;
	FILE* file;
	unsigned long compressionmethod;
	unsigned long bytebeforezip;
	
} zipFileInfoInternal;

typedef struct
{
	unsigned long countentries;
	unsigned long commentsize;
	
} zipGlobalInfo;

typedef struct
{
	FILE* file;
	zipGlobalInfo gi;
	unsigned long bytebeforezip;
	unsigned long numfile;
	unsigned long posincentraldir;
	unsigned long currentfileok;
	unsigned long centralpos;
	unsigned long centraldirsize;
	unsigned long centraldiroffset;
	zipFileInfo currentfileinfo;
	zipFileInternalInfo currentfileinfointernal;
	zipFileInfoInternal* currentzipfileinfo;
	int encrypted;
	unsigned long keys[3];
	const unsigned long* crc32tab;
	
} _zip;

static int pgeZipGetByte(FILE *file, int *pi)
{
	unsigned char c;

	int err = fread(&c, 1, 1, file);
	
	if(err == 1)
	{
		*pi = (int)c;
		return PGE_ZIP_OK;
	}
	else
	{
		if(ferror(file))
			return PGE_ZIP_ERRNO;
		else
			return PGE_ZIP_EOF;
	}
}

static int pgeZipGetShort(FILE *file, unsigned long *px)
{
	unsigned long x;
	int i = 0;
	int err;
	
	err = pgeZipGetByte(file, &i);
	x = (unsigned long)i;
	
	if(err == PGE_ZIP_OK)
		err = pgeZipGetByte(file, &i);
		
	x += ((unsigned long)i)<<8;

	if(err == PGE_ZIP_OK)
		*px = x;
	else
		*px = 0;

	return err;
}

static int pgeZipGetLong(FILE *file, unsigned long *px)
{
	unsigned long x;
	int i = 0;
	int err;
	
	err = pgeZipGetByte(file, &i);
	x = (unsigned long)i;
    
	if(err == PGE_ZIP_OK)
		err = pgeZipGetByte(file, &i);

	x += ((unsigned long)i)<<8;

	if(err == PGE_ZIP_OK)
		err = pgeZipGetByte(file,&i);
		
	x += ((unsigned long)i)<<16;

	if(err == PGE_ZIP_OK)
		err = pgeZipGetByte(file,&i);
		
	x += ((unsigned long)i)<<24;
	
	if(err == PGE_ZIP_OK)
		*px = x;
	else
		*px = 0;
	
	return err;
}

static int pgeZipDecryptByte(unsigned long* pkeys, const unsigned long* crc32tab)
{
	(void)crc32tab;
	
	unsigned temp;

	temp = ((unsigned)(*(pkeys + 2)) & 0xffff) | 2;
	
	return (int)(((temp * (temp ^ 1)) >> 8) & 0xff);
}

static int pgeZipUpdateKeys(unsigned long* pkeys, const unsigned long* crc32tab, int c)
{
	(*(pkeys + 0)) = CRC32((*(pkeys + 0)), c);
	(*(pkeys + 1)) += (*(pkeys + 0)) & 0xff;
	(*(pkeys + 1)) = (*(pkeys + 1)) * 134775813L + 1;
	
	{
		register int keyshift = (int)((*(pkeys + 1)) >> 24);
		(*(pkeys + 2)) = CRC32((*(pkeys + 2)), keyshift);
	}
	
	return c;
}

static void pgeZipInitKeys(const char* passwd, unsigned long* pkeys, const unsigned long* crc32tab)
{
	*(pkeys + 0) = 305419896L;
	*(pkeys + 1) = 591751049L;
	*(pkeys + 2) = 878082192L;
	
	while(*passwd != '\0')
	{
		pgeZipUpdateKeys(pkeys, crc32tab, (int)*passwd);
		passwd++;
	}
}

static unsigned long pgeZipLocateCentralDir(FILE *file)
{
	unsigned char* buf;
	unsigned long usizefile;
	unsigned long ubackread;
	unsigned long umaxback = 0xffff;
	unsigned long uposfound = 0;
	
	if(fseek(file, 0, SEEK_END) != 0)
		return 0;

	usizefile = ftell(file);

	if(umaxback > usizefile)
		umaxback = usizefile;

	buf = (unsigned char*)pgeMalloc(PGE_ZIP_BUF_READ_COMMENT + 4);

	if(buf == NULL)
		return 0;

	ubackread = 4;

	while(ubackread < umaxback)
	{
		unsigned long ureadsize, ureadpos;
		int i;

		if(ubackread + PGE_ZIP_BUF_READ_COMMENT > umaxback)
			ubackread = umaxback;
		else
			ubackread += PGE_ZIP_BUF_READ_COMMENT;

		ureadpos = usizefile - ubackread;
		
		ureadsize = ((PGE_ZIP_BUF_READ_COMMENT + 4) < (usizefile - ureadpos)) ? (PGE_ZIP_BUF_READ_COMMENT+  4) : (usizefile - ureadpos);

		if(fseek(file, ureadpos, SEEK_SET) != 0)
			break;

		if(fread(buf, (unsigned int)ureadsize, 1, file) != 1)
			break;

		for(i = (int)ureadsize - 3; (i--) > 0;)
		
		if(((*(buf + i)) == 0x50) && ((*(buf + i + 1)) == 0x4b) && ((*(buf + i + 2)) == 0x05) && ((*(buf + i + 3)) == 0x06))
		{
			uposfound = ureadpos + i;
			break;
		}

		if (uposfound != 0)
			break;
	}

	pgeFree(buf);

	return uposfound;
}

static int pgeZipGetZipFileInfoInternal(pgeZip* file, zipFileInfo *pfileinfo, zipFileInternalInfo *pfileinfointernal, char *filename, unsigned long filenamebuffersize, void *extrafield, unsigned long extrafieldbuffersize, char *comment, unsigned long commentbuffersize)
{
	_zip* s;
	zipFileInfo fileinfo;
	zipFileInternalInfo fileinfointernal;
	int err = PGE_ZIP_OK;
	unsigned long umagic;
	long lseek = 0;

	if(file == NULL)
		return PGE_ZIP_PARAM_ERROR;

	s = (_zip*)file;

	if(fseek(s->file, s->posincentraldir + s->bytebeforezip, SEEK_SET) != 0)
		err = PGE_ZIP_ERRNO;

	if(err == PGE_ZIP_OK)
	{
		if(pgeZipGetLong(s->file, &umagic) != PGE_ZIP_OK)
			err = PGE_ZIP_ERRNO;
		else if(umagic != 0x02014b50)
			err = PGE_ZIP_BAD_FILE;
	}

	if (pgeZipGetShort(s->file, &fileinfo.version) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;

	if (pgeZipGetShort(s->file, &fileinfo.versionneeded) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;

	if (pgeZipGetShort(s->file, &fileinfo.flag) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;

	if (pgeZipGetShort(s->file, &fileinfo.compressionmethod) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;

	if (pgeZipGetLong(s->file, &fileinfo.dosdate) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;

	if (pgeZipGetLong(s->file, &fileinfo.crc) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;

	if (pgeZipGetLong(s->file, &fileinfo.compressedsize) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;

	if (pgeZipGetLong(s->file, &fileinfo.uncompressedsize) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;

	if (pgeZipGetShort(s->file, &fileinfo.filenamesize) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;

	if (pgeZipGetShort(s->file, &fileinfo.fileextrasize) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;

	if (pgeZipGetShort(s->file, &fileinfo.filecommentsize) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;

	if (pgeZipGetShort(s->file, &fileinfo.disknumstart) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;

	if (pgeZipGetShort(s->file, &fileinfo.internalfileattr) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;

	if (pgeZipGetLong(s->file, &fileinfo.externalfileattr) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;

	if (pgeZipGetLong(s->file, &fileinfointernal.currentfileoffset) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;

	lseek += fileinfo.filenamesize;

	if((err == PGE_ZIP_OK) && (filename != NULL))
	{
		unsigned long usizeread;

		if(fileinfo.filenamesize < filenamebuffersize)
		{
			*(filename + fileinfo.filenamesize) = '\0';
			usizeread = fileinfo.filenamesize;
		}
		else
			usizeread = filenamebuffersize;

		if((fileinfo.filenamesize > 0) && (filenamebuffersize > 0))
		{
			if(fread(filename, (unsigned int)usizeread, 1, s->file) != 1)
				err = PGE_ZIP_ERRNO;
		}
		
		lseek -= usizeread;
	}

	if((err == PGE_ZIP_OK) && (extrafield != NULL))
	{
		unsigned long usizeread;

		if (fileinfo.fileextrasize < extrafieldbuffersize)
			usizeread = fileinfo.fileextrasize;
		else
			usizeread = extrafieldbuffersize;

		if(lseek != 0)
		{
			if(fseek(s->file, lseek, SEEK_CUR) == 0)
				lseek = 0;
			else
				err = PGE_ZIP_ERRNO;
		}

		if((fileinfo.fileextrasize > 0) && (extrafieldbuffersize > 0))
		{
			if(fread(extrafield, (unsigned int)usizeread, 1, s->file) != 1)
				err = PGE_ZIP_ERRNO;
		}
		
		lseek += fileinfo.fileextrasize - usizeread;
	}
	else
		lseek += fileinfo.fileextrasize;
	
	if((err == PGE_ZIP_OK) && (comment != NULL))
	{
		unsigned long usizeread;

		if(fileinfo.filecommentsize < commentbuffersize)
		{
			*(comment + fileinfo.filecommentsize) = '\0';
			usizeread = fileinfo.filecommentsize;
		}
		else
			usizeread = commentbuffersize;

		if(lseek != 0)
		{
			if(fseek(s->file, lseek, SEEK_CUR) == 0)
				lseek = 0;
			else
				err = PGE_ZIP_ERRNO;
		}

		if((fileinfo.filecommentsize > 0) && (commentbuffersize > 0))
		{
			if(fread(comment, (unsigned int)usizeread, 1, s->file) != 1)
				err = PGE_ZIP_ERRNO;
		}
		
		lseek += fileinfo.filecommentsize - usizeread;
	}
	else
		lseek += fileinfo.filecommentsize;

	if((err == PGE_ZIP_OK) && (pfileinfo != NULL))
		*pfileinfo = fileinfo;

	if((err == PGE_ZIP_OK) && (pfileinfointernal != NULL))
		*pfileinfointernal = fileinfointernal;

	return err;
}

static int pgeZipGetGlobalInfo(pgeZip* file, zipGlobalInfo *zipinfo)
{
    _zip* s;
	
    if(file == NULL)
        return PGE_ZIP_PARAM_ERROR;
		
    s = (_zip*)file;
	
    *zipinfo = s->gi;
	
    return PGE_ZIP_OK;
}

static int pgeZipGotoFirstFile(pgeZip* file)
{
	int err = PGE_ZIP_OK;

	_zip* s;

	if(file == NULL)
		return PGE_ZIP_PARAM_ERROR;

	s = (_zip*)file;
	s->posincentraldir = s->centraldiroffset;
	s->numfile = 0;
	err = pgeZipGetZipFileInfoInternal(file, &s->currentfileinfo, &s->currentfileinfointernal, NULL, 0, NULL, 0, NULL, 0);
	s->currentfileok = (err == PGE_ZIP_OK);
	
	return err;
}

static int pgeZipCloseCurrentFile(pgeZip* file)
{
	int err = PGE_ZIP_OK;

	_zip* s;
	zipFileInfoInternal *pfileinzipreadinfo;

	if(file == NULL)
		return PGE_ZIP_PARAM_ERROR;

	s = (_zip*)file;
	pfileinzipreadinfo = s->currentzipfileinfo;

	if(pfileinzipreadinfo == NULL)
		return PGE_ZIP_PARAM_ERROR;

	if(pfileinzipreadinfo->restreaduncompressed == 0)
	{
		if(pfileinzipreadinfo->crc32 != pfileinzipreadinfo->crc32wait)
			err = PGE_ZIP_CRC_ERROR;
	}

	pgeFree(pfileinzipreadinfo->buffer);
	pfileinzipreadinfo->buffer = NULL;

	if(pfileinzipreadinfo->streaminitialised)
		inflateEnd(&pfileinzipreadinfo->stream);

	pfileinzipreadinfo->streaminitialised = 0;

	pgeFree(pfileinzipreadinfo);

	s->currentzipfileinfo = NULL;

	return err;
}

static int pgeZipGetCurrentFileInfo(pgeZip* file, zipFileInfo *pfileinfo, char *filename, unsigned long filenamebuffersize, void *extrafield, unsigned long extrafieldbuffersize, char *comment, unsigned long commentbuffersize)
{
	return pgeZipGetZipFileInfoInternal(file, pfileinfo, NULL, filename, filenamebuffersize, extrafield, extrafieldbuffersize, comment, commentbuffersize);
}

static int pgeZipGotoNextFile(pgeZip* file)
{
	_zip* s;	
	int err;

	if(file == NULL)
		return PGE_ZIP_PARAM_ERROR;

	s = (_zip*)file;
	
	if(!s->currentfileok)
		return PGE_ZIP_EOF_LIST;

	if(s->numfile + 1 == s->gi.countentries)
		return PGE_ZIP_EOF_LIST;

	s->posincentraldir += PGE_ZIP_CENTRALDIR_ITEM_SIZE + s->currentfileinfo.filenamesize + s->currentfileinfo.fileextrasize + s->currentfileinfo.filecommentsize ;
	s->numfile++;
	
	err = pgeZipGetZipFileInfoInternal(file, &s->currentfileinfo, &s->currentfileinfointernal, NULL, 0, NULL, 0, NULL, 0);
	
	s->currentfileok = (err == PGE_ZIP_OK);
	
	return err;
}

static int pgeZipLocateFile(pgeZip* file, const char *filename, int casesensitive)
{
	(void)casesensitive;
	
	_zip* s;	
	int err;

	unsigned long numfilesaved;
	unsigned long posincentraldirsaved;

	if(file == NULL)
		return PGE_ZIP_PARAM_ERROR;

	if(strlen(filename) >= PGE_ZIP_MAX_FILENAME_SIZE)
		return PGE_ZIP_PARAM_ERROR;

	s = (_zip*)file;

	if(!s->currentfileok)
		return PGE_ZIP_EOF_LIST;

	numfilesaved = s->numfile;
	posincentraldirsaved = s->posincentraldir;

	err = pgeZipGotoFirstFile(file);
	
	char currentfilename[PGE_ZIP_MAX_FILENAME_SIZE + 1];

	while(err == PGE_ZIP_OK)
	{
		pgeZipGetCurrentFileInfo(file,NULL, currentfilename, sizeof(currentfilename) - 1, NULL, 0, NULL, 0);

		if(strcmp(currentfilename, filename) == 0)
			return PGE_ZIP_OK;

		err = pgeZipGotoNextFile(file);
	}

	s->numfile = numfilesaved;
	s->posincentraldir = posincentraldirsaved;
	
	return err;
}

static int pgeZipCheckCurrentFileCoherencyHeader(_zip *s, unsigned int *pisizevar, unsigned long *poffsetstaticextrafield, unsigned int *psizestaticextrafield)
{
	unsigned long umagic, udata, uflags;
	unsigned long filenamesize;
	unsigned long sizeextrafield;
	int err = PGE_ZIP_OK;

	*pisizevar = 0;
	*poffsetstaticextrafield = 0;
	*psizestaticextrafield = 0;

	if(fseek(s->file, s->currentfileinfointernal.currentfileoffset + s->bytebeforezip, SEEK_SET) != 0)
		return PGE_ZIP_ERRNO;

	if(err == PGE_ZIP_OK)
	{
		if(pgeZipGetLong(s->file, &umagic) != PGE_ZIP_OK)
			err = PGE_ZIP_ERRNO;
		else if(umagic != 0x04034b50)
			err = PGE_ZIP_BAD_FILE;
	}

	if(pgeZipGetShort(s->file, &udata) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;
		 
	if(pgeZipGetShort(s->file, &uflags) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;

	if(pgeZipGetShort(s->file, &udata) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;
	else if((err == PGE_ZIP_OK) && (udata != s->currentfileinfo.compressionmethod))
		err = PGE_ZIP_BAD_FILE;

	if((err == PGE_ZIP_OK) && (s->currentfileinfo.compressionmethod != 0) && (s->currentfileinfo.compressionmethod != Z_DEFLATED))
		err = PGE_ZIP_BAD_FILE;

	if(pgeZipGetLong(s->file, &udata) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;

	if(pgeZipGetLong(s->file, &udata) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;
	else if((err == PGE_ZIP_OK) && (udata != s->currentfileinfo.crc) && ((uflags & 8) == 0))
		err = PGE_ZIP_BAD_FILE;

	if(pgeZipGetLong(s->file, &udata) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;
	else if((err == PGE_ZIP_OK) && (udata != s->currentfileinfo.compressedsize) && ((uflags & 8) == 0))
		err = PGE_ZIP_BAD_FILE;

	if(pgeZipGetLong(s->file, &udata) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;
	else if((err == PGE_ZIP_OK) && (udata != s->currentfileinfo.uncompressedsize) && ((uflags & 8) == 0))
		err = PGE_ZIP_BAD_FILE;

	if(pgeZipGetShort(s->file, &filenamesize) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;
	else if((err==PGE_ZIP_OK) && (filenamesize != s->currentfileinfo.filenamesize))
		err = PGE_ZIP_BAD_FILE;

	*pisizevar += (unsigned int)filenamesize;

	if(pgeZipGetShort(s->file, &sizeextrafield) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;

	*poffsetstaticextrafield = s->currentfileinfointernal.currentfileoffset + PGE_ZIP_LOCALHEADER_SIZE + filenamesize;
	*psizestaticextrafield = (unsigned int)sizeextrafield;
	*pisizevar += (unsigned int)sizeextrafield;

	return err;
}
												
static int pgeZipOpenCurrentFile(pgeZip* file, const char *password)
{
	int err = PGE_ZIP_OK;
	int store;
	unsigned int isizevar;
	_zip* s;
	zipFileInfoInternal* pfileinzipreadinfo;
	unsigned long localextrafieldoffset;
	unsigned int localextrafieldsize;
	
	char source[12];

	if(file == NULL)
		return PGE_ZIP_PARAM_ERROR;

	s = (_zip*)file;

	if(!s->currentfileok)
		return PGE_ZIP_PARAM_ERROR;

	if(s->currentzipfileinfo != NULL)
		pgeZipCloseCurrentFile(file);

	if(pgeZipCheckCurrentFileCoherencyHeader(s, &isizevar, &localextrafieldoffset, &localextrafieldsize) != PGE_ZIP_OK)
		return PGE_ZIP_BAD_FILE;

	pfileinzipreadinfo = (zipFileInfoInternal*) pgeMalloc(sizeof(zipFileInfoInternal));

	if(pfileinzipreadinfo == NULL)
		return PGE_ZIP_INTERNAL_ERROR;

	pfileinzipreadinfo->buffer = (char*)pgeMalloc(PGE_ZIP_BUFFER_SIZE);
	pfileinzipreadinfo->localextrafieldoffset = localextrafieldoffset;
	pfileinzipreadinfo->localextrafieldsize = localextrafieldsize;
	pfileinzipreadinfo->localextrafieldpos = 0;

	if(pfileinzipreadinfo->buffer == NULL)
	{
		pgeFree(pfileinzipreadinfo);
		return PGE_ZIP_INTERNAL_ERROR;
	}

	pfileinzipreadinfo->streaminitialised = 0;
	
	if((s->currentfileinfo.compressionmethod != 0) && (s->currentfileinfo.compressionmethod != Z_DEFLATED))
		err = PGE_ZIP_BAD_FILE;

	store = s->currentfileinfo.compressionmethod == 0;

	pfileinzipreadinfo->crc32wait = s->currentfileinfo.crc;
	pfileinzipreadinfo->crc32 = 0;
	pfileinzipreadinfo->compressionmethod = s->currentfileinfo.compressionmethod;
	pfileinzipreadinfo->file = s->file;
	pfileinzipreadinfo->bytebeforezip = s->bytebeforezip;

	pfileinzipreadinfo->stream.total_out = 0;

	if(!store)
	{
		pfileinzipreadinfo->stream.zalloc = (alloc_func)0;
	  	pfileinzipreadinfo->stream.zfree = (free_func)0;
	  	pfileinzipreadinfo->stream.opaque = (voidpf)0;

	  	err = inflateInit2(&pfileinzipreadinfo->stream, -MAX_WBITS);

	  	if(err == Z_OK)
			pfileinzipreadinfo->streaminitialised = 1;
	}

	pfileinzipreadinfo->restreadcompressed = s->currentfileinfo.compressedsize;
	pfileinzipreadinfo->restreaduncompressed = s->currentfileinfo.uncompressedsize;

	pfileinzipreadinfo->posinzip = s->currentfileinfointernal.currentfileoffset + PGE_ZIP_LOCALHEADER_SIZE + isizevar;

	pfileinzipreadinfo->stream.avail_in = (unsigned int)0;

	s->currentzipfileinfo = pfileinzipreadinfo;
	
	if(password != NULL)
	{
		int i;
		s->crc32tab = get_crc_table();
		pgeZipInitKeys(password, s->keys, s->crc32tab);
		
		if(fseek(pfileinzipreadinfo->file, s->currentzipfileinfo->posinzip + s->currentzipfileinfo->bytebeforezip, SEEK_SET) != 0)
		{
			pgeFree(pfileinzipreadinfo->buffer);
			pgeFree(pfileinzipreadinfo);
			
			return PGE_ZIP_INTERNAL_ERROR;
		}
			
		if(fread(source, 1, 12, pfileinzipreadinfo->file) < 12)
		{
			pgeFree(pfileinzipreadinfo->buffer);
			pgeFree(pfileinzipreadinfo);
			
			return PGE_ZIP_INTERNAL_ERROR;
		}

		for(i = 0; i < 12; i++)
			zdecode(s->keys, s->crc32tab, source[i]);

		s->currentzipfileinfo->posinzip += 12;
		s->encrypted = 1;
	}
	
	return PGE_ZIP_OK;
}

static int pgeZipReadCurrentFile(pgeZip* file, void* buf, unsigned int len)
{
	int err = PGE_ZIP_OK;
	unsigned int iread = 0;
	_zip* s;
	zipFileInfoInternal* pfileinzipreadinfo;

	if(file == NULL)
		return PGE_ZIP_PARAM_ERROR;

	s = (_zip*)file;
	pfileinzipreadinfo = s->currentzipfileinfo;

	if(pfileinzipreadinfo == NULL)
		return PGE_ZIP_PARAM_ERROR;

	if((pfileinzipreadinfo->buffer == NULL))
		return PGE_ZIP_EOF_LIST;

	if(len == 0)
		return 0;

	pfileinzipreadinfo->stream.next_out = (Bytef*)buf;

	pfileinzipreadinfo->stream.avail_out = (unsigned int)len;
	
	if(len > pfileinzipreadinfo->restreaduncompressed)
		pfileinzipreadinfo->stream.avail_out = (unsigned int)pfileinzipreadinfo->restreaduncompressed;

	while(pfileinzipreadinfo->stream.avail_out > 0)
	{
		if((pfileinzipreadinfo->stream.avail_in == 0) && (pfileinzipreadinfo->restreadcompressed > 0))
		{
			unsigned int ureadthis = PGE_ZIP_BUFFER_SIZE;

			if(pfileinzipreadinfo->restreadcompressed < ureadthis)
				ureadthis = (unsigned int)pfileinzipreadinfo->restreadcompressed;

			if(ureadthis == 0)
				return PGE_ZIP_EOF;

			if(fseek(pfileinzipreadinfo->file, pfileinzipreadinfo->posinzip + pfileinzipreadinfo->bytebeforezip, SEEK_SET) != 0)
				return PGE_ZIP_ERRNO;

			if(fread(pfileinzipreadinfo->buffer, ureadthis, 1, pfileinzipreadinfo->file) != 1)
				return PGE_ZIP_ERRNO;
				
			if(s->encrypted)
			{
				unsigned int i;
				
				for(i = 0;i < ureadthis;i++)
					pfileinzipreadinfo->buffer[i] = zdecode(s->keys, s->crc32tab, pfileinzipreadinfo->buffer[i]);
			}

			pfileinzipreadinfo->posinzip += ureadthis;

			pfileinzipreadinfo->restreadcompressed -= ureadthis;
			
			pfileinzipreadinfo->stream.next_in = (Bytef*)pfileinzipreadinfo->buffer;
			pfileinzipreadinfo->stream.avail_in = (unsigned int)ureadthis;
		}

		if(pfileinzipreadinfo->compressionmethod == 0)
		{
			unsigned int udocopy, i;

			if((pfileinzipreadinfo->stream.avail_in == 0) && (pfileinzipreadinfo->restreadcompressed == 0))
				return (iread == 0) ? PGE_ZIP_EOF : iread;
			
			if(pfileinzipreadinfo->stream.avail_out < pfileinzipreadinfo->stream.avail_in)
				udocopy = pfileinzipreadinfo->stream.avail_out;
			else
				udocopy = pfileinzipreadinfo->stream.avail_in;
			
			for(i = 0;i < udocopy; i++)
				*(pfileinzipreadinfo->stream.next_out + i) = *(pfileinzipreadinfo->stream.next_in + i);

			pfileinzipreadinfo->crc32 = crc32(pfileinzipreadinfo->crc32, pfileinzipreadinfo->stream.next_out, udocopy);
			pfileinzipreadinfo->restreaduncompressed -= udocopy;
			pfileinzipreadinfo->stream.avail_in -= udocopy;
			pfileinzipreadinfo->stream.avail_out -= udocopy;
			pfileinzipreadinfo->stream.next_out += udocopy;
			pfileinzipreadinfo->stream.next_in += udocopy;
			pfileinzipreadinfo->stream.total_out += udocopy;
			iread += udocopy;
		}
		else
		{
			unsigned long utotaloutbefore, utotaloutafter;
			const Bytef *bufbefore;
			unsigned long uoutthis;
			int flush = Z_SYNC_FLUSH;

			utotaloutbefore = pfileinzipreadinfo->stream.total_out;
			bufbefore = pfileinzipreadinfo->stream.next_out;

			err = inflate(&pfileinzipreadinfo->stream, flush);

			utotaloutafter = pfileinzipreadinfo->stream.total_out;
			uoutthis = utotaloutafter - utotaloutbefore;
			
			pfileinzipreadinfo->crc32 = crc32(pfileinzipreadinfo->crc32, bufbefore, (unsigned int)(uoutthis));

			pfileinzipreadinfo->restreaduncompressed -= uoutthis;

			iread += (unsigned int)(utotaloutafter - utotaloutbefore);
            
			if(err == Z_STREAM_END)
				return (iread == 0) ? PGE_ZIP_EOF : iread;

			if(err != Z_OK)
				break;
		}
	}

	if(err == Z_OK)
		return iread;

	return err;
}

pgeZip* pgeZipOpen(const char *filename)
{
	//printf("Opening zip\n");
	_zip us;
	_zip *s;
	unsigned long centralpos, ul;
	FILE *file;

	unsigned long numberdisk;
	unsigned long numberdiskwithCD;
	unsigned long numberentryCD;

	int err = PGE_ZIP_OK;

	file = fopen(filename, "rb");

	if(file == NULL)
		return NULL;

	centralpos = pgeZipLocateCentralDir(file);

	if(centralpos == 0)
		err = PGE_ZIP_ERRNO;

	if(fseek(file, centralpos, SEEK_SET) != 0)
		err = PGE_ZIP_ERRNO;

	if(pgeZipGetLong(file, &ul) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;

	if(pgeZipGetShort(file, &numberdisk) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;

	if(pgeZipGetShort(file, &numberdiskwithCD) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;

	if(pgeZipGetShort(file, &us.gi.countentries) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;

	if(pgeZipGetShort(file, &numberentryCD) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;

	if((numberentryCD != us.gi.countentries) || (numberdiskwithCD != 0) || (numberdisk != 0))
		err = PGE_ZIP_BAD_FILE;

	if(pgeZipGetLong(file, &us.centraldirsize) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;

	if(pgeZipGetLong(file, &us.centraldiroffset) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;

	if(pgeZipGetShort(file, &us.gi.commentsize) != PGE_ZIP_OK)
		err = PGE_ZIP_ERRNO;

	if((centralpos < us.centraldiroffset + us.centraldirsize) && (err == PGE_ZIP_OK))
		err = PGE_ZIP_BAD_FILE;

	if(err != PGE_ZIP_OK)
	{
		fclose(file);
		return NULL;
	}

	us.file = file;
	us.bytebeforezip = centralpos - (us.centraldiroffset + us.centraldirsize);
	us.centralpos = centralpos;
	us.currentzipfileinfo = NULL;
	us.encrypted = 0;

	s = (_zip*)pgeMalloc(sizeof(_zip));

	*s = us;

	pgeZipGotoFirstFile((pgeZip*)s);

	return(pgeZip*)s;
}

int pgeZipClose(pgeZip* zip)
{
	_zip* s;

	if(zip == NULL)
		return 0;

	s = (_zip*)zip;

	if(s->currentzipfileinfo != NULL)
		pgeZipCloseCurrentFile(zip);

	//printf("Fclose: %d\n", fclose(s->file));
	
	pgeFree(s);

	return 1;
}

int pgeZipExtractCurrentFile(pgeZip *zip, int *nopath, const char *password)
{
	//void(overwrite);
	
	char filenameinzip[256];
	char *filenameWithoutPath;
	char *p;
	void *buffer;
	unsigned int buffersize = 64*1024;
	int err = 0;
	
	FILE *fout = NULL;
	
	zipFileInfo fileInfo;
	
	err = pgeZipGetCurrentFileInfo(zip, &fileInfo, filenameinzip, sizeof(filenameinzip), NULL, 0, NULL, 0);

	if(err != 0)
	{
		printf("error %d with zipfile in pgeZipGetCurrentFileInfo\n", err);
		return -1;
	}
	
	buffer = (void *)pgeMalloc(buffersize);
	
	if(!buffer)
	{
		printf("Error allocating buffer\n");
		
		return 0;
	}
	
	p = filenameWithoutPath = filenameinzip;
	
	while((*p) != '\0')
	{
		if(((*p) == '/') || ((*p) == '\\'))
			filenameWithoutPath = p + 1;
		
		p++;
	}
	
	if((*filenameWithoutPath) == '\0')
	{
		if((*nopath) == 0)
		{
			//printf("Creating directory: %s\n", filenameinzip);
			mkdir(filenameinzip, 0777);
		}
	}
	else
	{
		const char *writeFilename;
		
		if((*nopath) == 0)
			writeFilename = filenameinzip;
		else
			writeFilename = filenameWithoutPath;
			
		err = pgeZipOpenCurrentFile(zip, password);
		
		if(err != PGE_ZIP_OK)
			printf("Error with zipfile in pgeZipOpenCurrentFile\n");
			
		fout = fopen(writeFilename, "wb");
		
		if((fout == NULL) && ((*nopath) == 0) && (filenameWithoutPath != (char *)filenameinzip))
		{
			char c = *(filenameWithoutPath - 1);
			*(filenameWithoutPath - 1) = '\0';
			mkdir(writeFilename, 0777);
			*(filenameWithoutPath - 1) = c;
			fout = fopen(writeFilename, "wb");
		}
		
		if(fout == NULL)
			printf("Error opening file\n");
			
		do
		{
			err = pgeZipReadCurrentFile(zip, buffer, buffersize);
			
			if(err < 0)
			{
				printf("Error with zipfile in pgeZipReadCurrentFile\n");
				break;
			}
			
			if(err > 0)
			{
				fwrite(buffer, 1, err, fout);
			}
			
		} while (err > 0);
		
		fclose(fout);
		
		err = pgeZipCloseCurrentFile(zip);
		
		if(err != PGE_ZIP_OK)
			printf("Error with zipfile in pgeZipCloseCurrentFile\n");
	}
			
	if(buffer)
		pgeFree(buffer);
			
	return err;
}

int pgeZipExtract(pgeZip* zip, const char *password)
{
	unsigned int i;
	zipGlobalInfo gi;
	memset(&gi, 0, sizeof(zipGlobalInfo));
	int err;
	int nopath = 0;
	
	err = pgeZipGetGlobalInfo(zip, &gi);
	
	if(err != PGE_ZIP_OK)
		printf("Error with zipfile in pgeZipGetGlobalInfo\n");
		
	for(i = 0;i < gi.countentries;i++)
	{
		if(pgeZipExtractCurrentFile(zip, &nopath, password) != PGE_ZIP_OK)
			break;
			
		if((i + 1) < gi.countentries)
		{
			err = pgeZipGotoNextFile(zip);
			
			if(err != PGE_ZIP_OK)
				printf("Error with zipfile in pgeZipGotoNextFile\n");
		}
	}
	
	return 1;	
}

pgeZipFile* pgeZipFileRead(pgeZip* zip, const char *filename, const char *password)
{
	char filenameinzip[256];
	int err = 0;

	pgeZipFile* zipfile = (pgeZipFile*) pgeMalloc(sizeof(pgeZipFile));
	
	if(!zipfile)
		return NULL;

	if(pgeZipLocateFile(zip, filename, 0) != 0)
	{
		pgeFree(zipfile);
		return NULL;
	}

	zipFileInfo fileinfo;
	
	err = pgeZipGetCurrentFileInfo(zip, &fileinfo, filenameinzip, sizeof(filenameinzip), NULL, 0, NULL, 0);

	if(err != 0)
	{
		printf("error %d with zipfile in pgeZipGetCurrentFileInfo\n", err);
		pgeFree(zipfile);
		return NULL;
	}

  	err = pgeZipOpenCurrentFile(zip, password);

	if(err != 0)
	{
		printf("error %d with zipfile in pgeZipOpenCurrentFile\n", err);
		pgeFree(zipfile);
		return NULL;
	}

	zipfile->size = fileinfo.uncompressedsize;

	zipfile->data = (unsigned char*)pgeMalloc(fileinfo.uncompressedsize);
	
	if(!zipfile->data)
	{
		printf("error allocating data for zipfile\n");
		pgeFree(zipfile);
		return NULL;
	}

	unsigned int count = 0;
	err = 1;

	while(err > 0)
	{
		err = pgeZipReadCurrentFile(zip, &zipfile->data[count], fileinfo.uncompressedsize);

		if(err < 0)
		{
			printf("error %d with zipfile in pgeZipReadCurrentFile\n", err);
				break;
		}
		else
			count += err;
	}

	if(err == 0)
	{
		err = pgeZipCloseCurrentFile(zip);

		if(err != 0)
		{
			printf("error %d with zipfile in pgeZipCloseCurrentFile\n", err);
			pgeFree(zipfile->data);
			pgeFree(zipfile);
			return NULL;
		}
		    
		return zipfile;
	}
	else
	{
		pgeZipCloseCurrentFile(zip);
		pgeFree(zipfile->data);
		pgeFree(zipfile);
		
		return NULL;
	}
}

void pgeZipFileFree(pgeZipFile* file)
{
	if(file->data)
		pgeFree(file->data);
		
	if(file)
		pgeFree(file);
}
