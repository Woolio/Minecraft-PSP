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

#include <pspiofilemgr.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#ifdef __PSP__
	#include <sys/stat.h>
#endif

#include "pgeDir.h"
#include "pge.h"

int pgeDirMake(const char *name)
{
	int result = mkdir(name, 0777);
	
	if(result == 0)
		return 1;
		
	return result;
}

int pgeDirRemove(const char *path)
{
	int result = rmdir(path);
	
	if(result == 0)
		return 1;
		
	return result;
}

int pgeDirChdir(const char *path)
{
	int result = chdir(path);
	
	if(result == 0)
		return 1;
		
	return result;
}

int pgeDirGetCwd(char *path, unsigned int size)
{
	if(getcwd(path, size) == NULL)
		return 0;
		
	return 1;
}

pgeDir *pgeDirOpen(const char *path)
{	
	pgeDir *pgedir = (pgeDir *)pgeMalloc(sizeof(pgeDir));
	
	if(pgedir == NULL)
		return NULL;
		
	SceIoDirent dir;
	
	memset(&dir, 0, sizeof(SceIoDirent));
	
	pgedir->fd = sceIoDopen(path);
	
	if(pgedir->fd < 0)
	{
		pgeFree(pgedir);
		return NULL;
	}
	
	return pgedir;
}

int pgeDirRead(pgeDir *dir)
{
	int count = 0;
	
	SceIoDirent dirent;
	
	memset(&dirent, 0, sizeof(SceIoDirent));
	
	dir->entries = (pgeDirEntry *) pgeMalloc(sizeof(pgeDirEntry));
	
	if(dir->entries == NULL)
		return -1;
	
	while((sceIoDread(dir->fd, &dirent) > 0))
	{
		if((strcmp(dirent.d_name, ".") != 0) && (strcmp(dirent.d_name, "..") != 0))
		{
			dir->entries = (pgeDirEntry *) pgeRealloc(dir->entries, sizeof(pgeDirEntry) * (count + 1));
			
			if(dir->entries == NULL)
				return -1;
			
			strcpy(dir->entries[count].name, dirent.d_name);
		
			dir->entries[count].size = dirent.d_stat.st_size;
		
			dir->entries[count].type = (FIO_SO_IFDIR & dirent.d_stat.st_attr) == FIO_SO_IFDIR;
		
			count++;
		}
	}
		
	if(count == 0)
		return 0;
		
	dir->count = count;
		
	return count;
}

void pgeDirClose(pgeDir *dir)
{	
	if(dir != NULL)
	{
		sceIoDclose(dir->fd);
		
		if(dir->entries != NULL)
			pgeFree(dir->entries);
		
		pgeFree(dir);
	}
}

int pgeDirExists(const char *path)
{ 
	SceIoStat stats;
	
	int result = sceIoGetstat(path, &stats);
	
	if(result < 0)
		return 0;
	
	if(stats.st_mode & FIO_S_IFDIR) 
		return 1; 
	 
	return 0;  
}

int pgeDirRename(const char *oldname, const char *newname)
{
	int result = sceIoRename(oldname, newname);
	
	if(result == 0)
		return 1;
		
	return result;
}
