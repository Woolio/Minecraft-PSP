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

#include <pspkernel.h>
#include <psppower.h>
#include <pspdisplay.h>
#include <pspsdk.h>
#include <psputility.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include "pge.h"
#include "support/pgesupportprx.h"

#ifdef __PSP__
	PSP_MODULE_INFO("Phoenix Game Engine", PSP_MODULE_USER, 1, PGE_VERSION);
	PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER|PSP_THREAD_ATTR_VFPU);
#endif

static int running = 1;

static int pgeExitCallback(int arg1, int arg2, void *common)
{
	(void)arg1;
	(void)arg2;
	(void)common;
	
	running = 0;
	
	return 0;
}

static int pgeCallbackThread(SceSize args, void *argp)
{
	(void)args;
	(void)argp;
	
	int cbid;

	cbid = sceKernelCreateCallback("PgeExitCallback", pgeExitCallback, NULL);
	sceKernelRegisterExitCallback(cbid);
	
	sceKernelSleepThreadCB();
	
	return 0;
}

static int pgeCallbackSetup(void)
{
	int thid = 0;

	thid = sceKernelCreateThread("PgeCallbackThread", pgeCallbackThread, 0x11, 0xFA0, PSP_THREAD_ATTR_USER, 0);

	if(thid >= 0)
		sceKernelStartThread(thid, 0, 0);

	return thid;
}

int pgeRunning(void)
{
	#ifndef __PSP__
	if (SDL_QuitRequested())
		running = 0;
	#endif
	
	return running;
}

void pgeExit(void)
{
	running = 0;
	
	sceKernelExitGame();
}

int pgeVersion(void)
{
	return PGE_VERSION;
}

void pgeDelay(unsigned int delay)
{
	sceKernelDelayThread(delay);
}

void *pgeMalloc(int size)
{
	void *ptr = malloc(size);
	return ptr;
}

void pgeFree(void *ptr)
{
	if(ptr != NULL)
		free(ptr);
}

void *pgeRealloc(void *ptr, int size)
{
	return realloc(ptr, size);
}

__attribute__((constructor)) void pgeInit()
{	
	pgeCallbackSetup();
	
	sceUtilityLoadAvModule(PSP_AV_MODULE_AVCODEC);
	
	#ifdef __PSP__
		int fd = sceIoOpen("ms0:/pgesupport.prx", PSP_O_WRONLY|PSP_O_CREAT, 0777);
	
		sceIoWrite(fd, pgesupportprx, size_pgesupportprx);
	
		sceIoClose(fd);
	
		pspSdkLoadStartModule("ms0:/pgesupport.prx", PSP_MEMORY_PARTITION_KERNEL);
	
		sceIoRemove("ms0:/pgesupport.prx");
	#endif	
}
