/*
 * pgeDir.h: Header for directory access
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

#ifndef __PGEDIR_H__
#define __PGEDIR_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup pgeDir Directory access Library
 *  @{
 */

/**
 * Directory entry type enumerator
 */
enum pgeDirEntryTypes
{
	PGE_DIR_ENTRY_TYPE_FILE,
	PGE_DIR_ENTRY_TYPE_DIR
};

/**
 * A directory entry
 */
typedef struct
{
	char name[256];
	unsigned int size;
	unsigned int type;
	
} pgeDirEntry;

/**
 * A directory datatype
 */
typedef struct
{
	int fd;
	int count;
	pgeDirEntry *entries;
	
} pgeDir;

/**
 * Create a directory.
 *
 * @param name - Name of the directory.
 *
 * @returns 1 on success, otherwise an error.
 */
int pgeDirMake(const char *name);

/**
 * Remove a directory (must be empty).
 *
 * @param path - Path to the directory.
 *
 * @returns 1 on success, otherwise an error.
 */
int pgeDirRemove(const char *path);

/**
 * Change directory.
 *
 * @param path - Path to change to.
 *
 * @returns 1 on success, otherwise an error.
 */
int pgeDirChdir(const char *path);

/**
 * Get current working directory.
 *
 * @param path - Pointer to the buffer to store the cwd in.
 *
 * @param size - Size of the buffer.
 *
 * @returns 1 on success, otherwise an error.
 */
int pgeDirGetCwd(char *path, unsigned int size);

/**
 * Open a directory for reading.
 *
 * @param path - Path to open.
 *
 * @returns NULL on error, otherwise a pointer to a ::pgeDir struct.
 */
pgeDir *pgeDirOpen(const char *path);

/**
 * Read a directory contents.
 *
 * @param dir - A previously opened ::pgeDir.
 *
 * @returns The number of items in the directory, < 0 on error.
 */
int pgeDirRead(pgeDir *dir);

/**
 * Close a directory.
 *
 * @param dir - A previously opened ::pgeDir.
 */
void pgeDirClose(pgeDir *dir);

/**
 * Check a directory exists.
 *
 * @param path - The directory to check exists.
 *
 * @returns 1 if the directory exists, else 0.
 */
int pgeDirExists(const char *path);

/**
 * Rename a directory.
 *
 * @param oldname - The directory to rename.
 *
 * @param newname - The new name of the directory.
 *
 * @returns 1 on success, otherwise an error.
 */
int pgeDirRename(const char *oldname, const char *newname);

/** @} */

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __PGEDIR_H__
