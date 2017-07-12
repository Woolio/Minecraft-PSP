/*
 * pgeObj.h: Header for OBJ parsing
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

#ifndef __PGEOBJ_H__
#define __PGEOBJ_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup pgeObj OBJ Library
 *  @{
 */

/**
 * A WAV file struct
 */
typedef struct
{
	unsigned int numvertices;
	unsigned int drawflag;
	void *vertices;
		
} pgeObj;

/**
 * Load an OBJ file
 *
 * @param filename - Path of the file to load.
 *
 * @returns A pointer to a ::pgeObj struct or NULL on error.
 */
pgeObj *pgeObjLoad(const char *filename);

/**
 * Load an OBJ file (from memory)
 *
 * @param buffer - Pointer to the OBJ file data.
 *
 * @param size - Size of the OBJ file data
 *
 * @returns A pointer to a ::pgeObj struct or NULL on error.
 */
pgeObj *pgeObjLoadMemory(unsigned char *buffer, unsigned int size);

/**
 * Destroy an OBJ file
 *
 * @param obj - The ::pgeObj struct to destroy
 */
void pgeObjDestroy(pgeObj *obj);

/** @} */

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __PGEOBJ_H__
