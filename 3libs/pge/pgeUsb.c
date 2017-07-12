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
#include <pspsdk.h>
#include <pspusb.h>
#include <pspusbstor.h>

#include "pgeUsb.h"

int pgeSupportLoadStartModule(const char *path);
int pgeSupportStopUnloadModule(SceUID modid);

static int pgeUsbInitialised = 0;
static SceUID pgeUsbModuleId[7];

int pgeUsbInit(void)
{	
	if(pgeUsbInitialised)
		return 1;
		
    pgeUsbModuleId[0] = pgeSupportLoadStartModule("flash0:/kd/chkreg.prx");
	
	if(pgeUsbModuleId[0] < 0)
		return pgeUsbModuleId[0];
		
    pgeUsbModuleId[1] = pgeSupportLoadStartModule("flash0:/kd/npdrm.prx");
	
	if(pgeUsbModuleId[1] < 0)
		return pgeUsbModuleId[1];
		
    pgeUsbModuleId[2] = pgeSupportLoadStartModule("flash0:/kd/semawm.prx");
	
	if(pgeUsbModuleId[2] < 0)
		return pgeUsbModuleId[2];
		
    pgeUsbModuleId[3] = pgeSupportLoadStartModule("flash0:/kd/usbstor.prx");
	
	if(pgeUsbModuleId[3] < 0)
		return pgeUsbModuleId[3];
		
    pgeUsbModuleId[4] = pgeSupportLoadStartModule("flash0:/kd/usbstormgr.prx");
	
	if(pgeUsbModuleId[4] < 0)
		return pgeUsbModuleId[4];
		
    pgeUsbModuleId[5] = pgeSupportLoadStartModule("flash0:/kd/usbstorms.prx");
	
	if(pgeUsbModuleId[5] < 0)
		return pgeUsbModuleId[5];
		
    pgeUsbModuleId[6] = pgeSupportLoadStartModule("flash0:/kd/usbstorboot.prx");
	
	if(pgeUsbModuleId[6] < 0)
		return pgeUsbModuleId[6];

    int result = sceUsbStart(PSP_USBBUS_DRIVERNAME, 0, 0);
    
	if(result < 0)
		return result;
		
    result = sceUsbStart(PSP_USBSTOR_DRIVERNAME, 0, 0);
    
	if(result < 0)
		return result;

    result = sceUsbstorBootSetCapacity(0x800000);
    
	if(result < 0)
		return result;
		
	pgeUsbInitialised = 1;
	
    return 1;
}

int pgeUsbShutdown(void)
{
    int i;

    sceUsbStop(PSP_USBSTOR_DRIVERNAME, 0, 0);
    sceUsbStop(PSP_USBBUS_DRIVERNAME, 0, 0);
	
    for(i = 6; i >= 0; i--)
        pgeSupportStopUnloadModule(pgeUsbModuleId[i]);
		
	pgeUsbInitialised = 0;
		
    return 1;
}

int pgeUsbGetState(void)
{
	int state = sceUsbGetState();
	
	return state;
}

int pgeUsbActivate(void)
{
    int result = sceUsbActivate(0x1c8);
	
	if(result < 0)
		return 0;
	
    return 1;
}

int pgeUsbDeactivate(void)
{
    int result = sceUsbDeactivate(0x1c8);
	
    sceIoDevctl("fatms0:", 0x0240D81E, NULL, 0, NULL, 0);
	
	if(result < 0)
		return 0;
	
    return 1;
}
