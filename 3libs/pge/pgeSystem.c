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
#include <psputility.h>
#include <pspnet.h>
#include <pspnet_inet.h>
#include <pspnet_apctl.h>
#include <string.h>
#include <stdlib.h>
#include <pspnet_adhocmatching.h>
#include <pspnet_adhoc.h>
#include <psputility_gamesharing.h>
#include <psputility_htmlviewer.h>
#include <psphttp.h>
#include <pspopenpsid.h>
#include <pspwlan.h>
#include <psppower.h>

#ifdef __PSP__
	#include <malloc.h>
#endif

#include "pgeSystem.h"
#include "pgeMath.h"
#include "pge.h"

typedef struct
{
	unsigned long		maxclusters;
	unsigned long		freeclusters;
	int					unk1;
	unsigned int		sectorsize;
	u64					sectorcount;
	
} pgeSystemDevCtl;

typedef struct
{
	pgeSystemDevCtl *pdevinf;
	
} pgeSystemDevCommand;

void pgeSystemGetNickname(char *nickname)
{
	sceUtilityGetSystemParamString(PSP_SYSTEMPARAM_ID_STRING_NICKNAME, nickname, 128);
}

void pgeSystemGetPsid(unsigned char *psid)
{
	PspOpenPSID thepsid;
	
	sceOpenPSIDGetOpenPSID(&thepsid);
	
	memcpy(psid, &thepsid, 16);
}

int pgeSystemGetLanguage(void)
{
	int language;
	
	sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &language);
	
	return language;
}

void pgeSystemGetMac(unsigned char *mac)
{
	sceWlanGetEtherAddr(mac);
}

float pgeSystemMsSize(void)
{
	pgeSystemDevCtl devctl;
	
	memset(&devctl, 0, sizeof(pgeSystemDevCtl));
	
	pgeSystemDevCommand command;
	
	command.pdevinf = &devctl;
	
	sceIoDevctl("ms0:", 0x02425818, &command, sizeof(pgeSystemDevCommand), NULL, 0);
	
	u64 mssize = (devctl.maxclusters * devctl.sectorcount) * devctl.sectorsize; 
	
	return (float)mssize/1048576.0f;
}

float pgeSystemMsFreeSize(void)
{
	pgeSystemDevCtl devctl;
	
	memset(&devctl, 0, sizeof(pgeSystemDevCtl));
	
	pgeSystemDevCommand command;
	
	command.pdevinf = &devctl;
	
	sceIoDevctl("ms0:", 0x02425818, &command, sizeof(pgeSystemDevCommand), NULL, 0);
	
	u64 freesize = (devctl.freeclusters * devctl.sectorcount) * devctl.sectorsize; 
	
	return (float)freesize/1048576.0f;
}

unsigned int pgeSystemGetFreeRam(void)
{
	void* buf[128];
	
	int i = 0;
	
	for(i = 0; i < 128; i++)
	{
		buf[i] = pgeMalloc(512 * 1024);
		
		if(!buf[i])
			break;
	}
	
	int result = i;
	
	for(; i >= 0; i--)
	{
		pgeFree(buf[i]);
	}
	
	return (result * 512 * 1024);
}

int pgeSystemSetCpu(unsigned int freq)
{
	int cpu = freq;
	
	if(cpu > 333)
		cpu = 333;
		
	int pll = cpu;
	
	if(pll < 190)
		pll = 190;
		
	int bus = pgeMathFloor(pll/2);
	
	int result = scePowerSetClockFrequency(pll, cpu, bus);
	
	if(result < 0)
		return 0;
		
	return 1;
}

int pgeSystemGetBatteryPercent(void)
{
	int percent = scePowerGetBatteryLifePercent();
	
	if(percent < 0)
		return 0;
		
	return percent;
}

int pgeSystemGetBatteryTime(void)
{
	int mins = scePowerGetBatteryLifeTime();
	
	if(mins < 0)
		return 0;
		
	return mins;
}

int pgeSystemGetBatteryCharging(void)
{
	int charging = scePowerIsBatteryCharging();
	
	if(charging < 0)
		return 0;
		
	return charging;
}

int pgeSystemGetBatteryExists(void)
{
	int exist = scePowerIsBatteryExist();
	
	if(exist < 0)
		return 0;
		
	return exist;
}

int pgeSystemGetBatteryIsLow(void)
{
	int low = scePowerIsLowBattery();
	
	if(low < 0)
		return 0;
		
	return low;
}
