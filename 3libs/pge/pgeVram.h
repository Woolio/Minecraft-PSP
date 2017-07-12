/*
 * pgeVram.h: Header for VRAM memory management
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

#ifndef __PGEVRAM_H__
#define __PGEVRAM_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup pgeVram VRAM Library
 *  @{
 */

/**
 * Make a pointer relative to VRAM base
 *
 * @note A relative NULL pointer is NOT illegal!
 *
 * @param ptr - Pointer to make relative
 *
 * @returns relative pointer matching ptr
 */
void* pgeVramRelativePointer(void *ptr);

/**
 * Make a pointer absolute (usable by CPU).
 *
 * @note A relative NULL pointer is NOT illegal!
 *
 * @param ptr - Pointer to make absolute
 *
 * @returns absolute pointer matching ptr
 */
void* pgeVramAbsolutePointer(void *ptr);

/**
 * Allocate memory from VRAM.
 *
 * @param size - Number of bytes to allocate
 *
 * @returns Absolute pointer on success, NULL on failure
 */
void* pgeVramAlloc(unsigned long size);

/**
 * Free allocated memory from VRAM.
 *
 * @param ptr - Previously allocated pointer
 */
void pgeVramFree(void* ptr);

/**
 * Return remaining unallocated VRAM.
 *
 * @returns Size of remaining VRAM in bytes
 */
unsigned long pgeVramAvailable();

/**
 * Return largest free memory block in VRAM.
 *
 * @returns Size of largest free block in bytes
 */
unsigned long pgeVramLargestBlock();

/** @} */

#ifdef __cplusplus
}
#endif // __cplusplus

#endif  // __PGEVRAM_H__
