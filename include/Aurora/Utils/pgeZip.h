/*
 * pgeZip.h: Header for Zip file access
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

#ifndef __PGEZIP_H__
#define __PGEZIP_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup pgeZip Zip Library
 *  @{
 */

#include <zlib.h>

/**
 * A zip
 */
typedef void pgeZip;

/**
 * A file within a zip
 */
typedef struct
{
	unsigned char *data;	/**<  The file data */
	int size;				/**<  Size of the data */
	
} pgeZipFile;

/**
 * Open a Zip file
 *
 * @param filename - Path of the zip to load.
 *
 * @returns A pointer to a ::pgeZip struct or NULL on error.
 */
pgeZip* pgeZipOpen(const char *filename);

/**
 * Close a Zip file
 *
 * @param zip - A valid (previously opened) ::pgeZip
 *
 * @returns 1 on success, 0 on error
 */
int pgeZipClose(pgeZip *zip);

/**
 * Read a file from a zip
 *
 * @param zip - A valid (previously opened) ::pgeZip
 *
 * @param filename - The file to read within the zip
 *
 * @param password - The password of the file (pass NULL if no password)
 *
 * @returns A ::pgeZipFile struct containing the file
 */
pgeZipFile* pgeZipFileRead(pgeZip *zip, const char *filename, const char *password);

/**
 * Extract all files from a zip
 *
 * @param zip - A valid (previously opened) ::pgeZip file
 *
 * @param password - The password of the file (pass NULL if no password)
 *
 * @returns 1 on success, 0 on error.
 */
int pgeZipExtract(pgeZip *zip, const char *password);

/**
 * Free the file data previously loaded from a zip
 *
 * @param file - A valid (previously read) ::pgeZipFile
 */
void pgeZipFileFree(pgeZipFile *file);

/** @} */

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __PGEZIP_H__
