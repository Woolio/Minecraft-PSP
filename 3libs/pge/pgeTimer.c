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

#include <psptypes.h>
#include <time.h>
#include <psprtc.h>
#include <stdlib.h>

#include "pge.h"
#include "pgeTimer.h"

pgeTimer* pgeTimerCreate(void)
{
	pgeTimer* timer = (pgeTimer*) pgeMalloc(sizeof(pgeTimer));
	
	if(!timer)
		return NULL;

	sceRtcGetCurrentTick(&timer->timelastask);
	timer->tickresolution = sceRtcGetTickResolution();
	timer->totaltime = 0;
	timer->paused = 0;
	
	return timer;
}

void pgeTimerUpdate(pgeTimer *timer)
{	
	sceRtcGetCurrentTick(&timer->timenow);
	
	if(timer->paused)
		timer->deltatime = 0;
	else
		timer->deltatime = (timer->timenow - timer->timelastask) / ((float)timer->tickresolution);
		
	timer->timelastask = timer->timenow;
	timer->totaltime += timer->deltatime;
}

float pgeTimerGetDeltaTime(pgeTimer *timer)
{	   
	return timer->deltatime;
}

float pgeTimerPeekDeltaTime(pgeTimer *timer)
{
	if(timer->paused)
		return 0.0f;
	
	sceRtcGetCurrentTick(&timer->timenow);
	float time = (timer->timenow - timer->timelastask) / ((float) timer->tickresolution);	
	timer->totaltime += time;
	
	return time;
}

unsigned long long pgeTimerGetTotalTime(pgeTimer *timer)
{	   
	if(timer->paused)
		return timer->totaltime;
	
	sceRtcGetCurrentTick(&timer->timenow);	
	float time = (timer->timenow - timer->timelastask) / ((float) timer->tickresolution);	
	timer->totaltime += time;
	
	return timer->totaltime;
}

void pgeTimerDestroy(pgeTimer *timer)
{
	if(timer)
		pgeFree(timer);
}

void pgeTimerPause(pgeTimer *timer)
{
	timer->paused = 1;
}

void pgeTimerUnpause(pgeTimer *timer)
{
	timer->paused = 0;
}
