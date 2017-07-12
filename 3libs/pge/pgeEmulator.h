/*
 * pgeEmulator.h: Header for the emulator
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

#ifndef __PGEEMU_H__
#define __PGEEMU_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef PSP_MODULE_INFO
#undef PSP_MODULE_INFO
#define PSP_MODULE_INFO(name, attributes, major_version, minor_version)
#endif

#ifdef PSP_MAIN_THREAD_ATTR
#undef PSP_MAIN_THREAD_ATTR
#define PSP_MAIN_THREAD_ATTR(attr)
#endif

#ifdef PSP_HEAP_SIZE_KB
#undef PSP_HEAP_SIZE_KB
#define PSP_HEAP_SIZE_KB(size_kb)
#endif

void *	memalign(size_t align, size_t size);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif  // __PGEVRAM_H__
