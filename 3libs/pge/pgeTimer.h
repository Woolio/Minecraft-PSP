/*
 * pgeTimer.h: Header for timers
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

#ifndef __PGETIMER_H__
#define __PGETIMER_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup pgeTimer Timer Library
 *  @{
 */

/**
 * A timer struct
 */
typedef struct
{
	float deltatime;				/**<  The delta time */
	unsigned long long timenow;		/**<  Current time */
	unsigned long long timelastask;	/**<  Time of last update */
	unsigned long long totaltime;	/**<  Time passed since creation */
	unsigned int tickresolution;	/**<  Tick resolution */
	int paused;
	
} pgeTimer;

/**
 * Create a timer
 *
 * @returns A pointer to a ::pgeTimer struct.
 */
pgeTimer* pgeTimerCreate(void);

/**
 * Update a timer
 *
 * Should be called once at the start of each iteration of your main loop
 *
 * @param timer - A pointer to a valid ::pgeTimer struct
 */
void pgeTimerUpdate(pgeTimer *timer);

/**
 * Get the delta time of a timer
 *
 * @param timer - A pointer to a valid ::pgeTimer struct
 *
 * @returns The delta time member of the timer
 */
float pgeTimerGetDeltaTime(pgeTimer *timer);

/**
 * Peek at delta time of a timer (without updating)
 *
 * @param timer - A pointer to a valid ::pgeTimer struct
 *
 * @returns The delta time member of the timer
 */
float pgeTimerPeekDeltaTime(pgeTimer *timer);

/**
 * Get the time of a timer since creation.
 *
 * @param timer - A pointer to a valid ::pgeTimer struct
 *
 * @returns The total time member of the timer
 */
unsigned long long pgeTimerGetTotalTime(pgeTimer *timer);

/**
 * Free a previously created timer
 *
 * @param timer - A pointer to a valid ::pgeTimer struct
 */
void pgeTimerDestroy(pgeTimer *timer);

void pgeTimerPause(pgeTimer *timer);

void pgeTimerUnpause(pgeTimer *timer);

/** @} */

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __PGETIMER_H__
