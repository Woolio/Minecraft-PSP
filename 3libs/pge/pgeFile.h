/*
 * pgeFile.h: Header for File IO
 *
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

#ifndef __PGEFILE_H__
#define __PGEFILE_H__

#include <pspiofilemgr.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup pgeFile File IO Library
 *  @{
 */

/**
 * File type enumerator
 */
enum pgeFileTypes
{
	PGE_FILE_TYPE_MEM =	1,
	PGE_FILE_TYPE_FILE,
	PGE_FILE_TYPE_AFILE
};

/**
 * File seek type enumerator
 */
enum pgeFileSeekTypes
{
	PGE_FILE_SET =	0,
	PGE_FILE_CUR,
	PGE_FILE_END
};

/**
 * File flags enumerator
 */
enum pgeFileFlags
{
	PGE_FILE_RDONLY =	PSP_O_RDONLY,
	PGE_FILE_WRONLY =	PSP_O_WRONLY,
	PGE_FILE_RDWR =		PSP_O_RDWR,
	PGE_FILE_APPEND =	PSP_O_APPEND,
	PGE_FILE_CREATE =	PSP_O_CREAT,
	PGE_FILE_TRUNC =	PSP_O_TRUNC,
	PGE_FILE_ALL =		PGE_FILE_RDWR|PGE_FILE_APPEND|PGE_FILE_CREATE|PGE_FILE_TRUNC
};

/**
 * A file datatype
 */
typedef struct pgeFileStruct pgeFile;

/**
 * Open a file stream.
 *
 * @param name - Path to the file.
 *
 * @param flags - One or more of ::pgeFileFlags
 *
 * @returns A pointer to a ::pgeFile struct or NULL on error.
 */
pgeFile* pgeFileOpen(char* name, int flags);

/**
 * Open a file stream (async).
 *
 * @param name - Path to the file.
 *
 * @param flags - One or more of ::pgeFileFlags
 *
 * @returns A pointer to a ::pgeFile struct or NULL on error.
 */
pgeFile* pgeFileOpenAsync(char* name, int flags);

/**
 * Open a memory stream.
 *
 * @param data - Pointer to the data.
 *
 * @param flags - Size of the data in bytes.
 *
 * @returns A pointer to a ::pgeFile struct or NULL on error.
 */
pgeFile* pgeFileOpenMemory(char* data, int size);

/**
 * Close a stream.
 *
 * @param s - The stream to close.
 *
 * @returns < 0 on error.
 */
int pgeFileClose(pgeFile* s);

/**
 * Read from a stream.
 *
 * @param s - The stream to read from.
 *
 * @param buf - The buffer to read the data into.
 *
 * @param size - Size of the read in bytes.
 *
 * @returns Number of bytes read, < 0 on error.
 */
int pgeFileRead(pgeFile* s, void* buf, unsigned int size);

/**
 * Write to a stream.
 *
 * @param s - The stream to write to.
 *
 * @param buf - The buffer to write.
 *
 * @param size - Size of the write in bytes.
 *
 * @returns Number of bytes written, < 0 on error.
 */
int pgeFileWrite(pgeFile* s, void* buf, unsigned int size);

/**
 * Seek within a stream.
 *
 * @param s - The stream to seek.
 *
 * @param offs - The stream offset.
 *
 * @param dir - One of ::pgeFileSeekTypes.
 *
 * @returns The position within the stream, < 0 on error.
 */
int pgeFileSeek(pgeFile* s, int offs, unsigned int dir);

/**
 * Get position within a stream.
 *
 * @param s - The stream.
 *
 * @returns The position within the stream, < 0 on error.
 */
int pgeFileTell(pgeFile* s);

/**
 * Seek to the start of a file.
 *
 * @param s - The file.
 *
 * @returns The position within the file, < 0 on error.
 */
int pgeFileRewind(pgeFile* s);

/**
 * Get the size of a file.
 *
 * @param s - The file.
 *
 * @returns The size of the file.
 */
unsigned int pgeFileSize(pgeFile *s);

/**
 * Remove a file.
 *
 * @param filepath - Path to the file.
 *
 * @returns 1 on success, otherwise an error.
 */
int pgeFileRemove(const char *filepath);

/**
 * Rename a file.
 *
 * @param oldname - The file to rename.
 *
 * @param newname - The new name of the file.
 *
 * @returns 1 on success, otherwise an error.
 */
int pgeFileRename(const char *oldname, const char *newname);

/**
 * Check a file exists.
 *
 * @param filename - The filename to check exists.
 *
 * @returns 1 if the file exists, else 0.
 */
int pgeFileExists(const char *filename);

/** @} */

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __PGEFILE_H__
