/*
 * pge.h: Header for the engine
 *
 * This file is part of "Phoenix Game Engine".
 *
 * Copyright (C) 2008 Phoenix Game Engine
 * Copyright (C) 2008 InsertWittyName <tias_dp@hotmail.com>
 * Copyright (C) 2008 MK2k <pge@mk2k.net>
 *
 */

#ifndef __PGE_H__
#define __PGE_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup pge Engine
 *  @{
 */
#include <pspkernel.h>
//#include <pspkerneltypes.h>

#ifndef __PSP__
	#include "SDL/SDL.h"
	#include "SDL/SDL_opengl.h"
#endif

/**
 * Check for program running status.
 *
 * @returns 1 if program running, 0 if exit has been requested.
 */
int pgeRunning(void);

/**
 * Explicitely exit a program.
 */
void pgeExit(void);

/**
 * Get PGE version
 *
 * @returns The version of PGE running.
 */
int pgeVersion(void);

/**
 * Delay execution
 *
 * @param delay - Time to delay in microseconds
 */
void pgeDelay(unsigned int delay);

void *pgeMalloc(int size);

void *pgeRealloc(void *ptr, int size);

void pgeFree(void *ptr);

#ifdef __cplusplus
}
#endif // __cplusplus

/** @} */

#endif // __PGE_H__
