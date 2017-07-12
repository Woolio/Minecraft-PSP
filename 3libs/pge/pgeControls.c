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

#include <string.h>
#include <stdio.h>

#include "pgeControls.h"
#include "pgeMath.h"

#define PGE_CONTROLS_PI (3.141593f)

typedef struct pgeVec2Char
{
	char x, y;
	
} pgeVec2Char;

typedef struct pgeControls
{
	SceCtrlData input;
	pgeVec2Char analog;
	unsigned int pressed;
	unsigned int held;
	unsigned int released;
	
} pgeControls;

static pgeControls pad;

static int enabled = 0;

static void pgeControlsAddInput(const unsigned int button)
{
	if((pad.input.Buttons & button) != 0)
	{
		pad.released &= ~button;
		
		if((pad.held & button) == 0)
		{
			pad.pressed |= button;
			pad.held |= button;
		}
	}
	else
	{
		if(pad.held & button)
			pad.released |= button;
		
		pad.pressed &= ~button;
		pad.held &= ~button;
	}
}

int pgeControlsInit(void)
{
	memset(&pad, 0, sizeof(pgeControls));

	sceCtrlSetSamplingCycle(0);

	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);
	
	sceCtrlSetIdleCancelThreshold(25, 25);
	
	enabled = 1;

	return 1;
}

void pgeControlsShutdown(void)
{
	// Nowt
}

void pgeControlsUpdate(void)
{	
	sceCtrlPeekBufferPositive(&pad.input, 1);
	
	pad.pressed = 0;
	pad.released = 0;
	
	if(pad.input.Buttons & PSP_CTRL_HOME)
		return;		
	
	pad.analog.x = (signed)pad.input.Lx - 128;
	pad.analog.y = (signed)pad.input.Ly - 128;

	pgeControlsAddInput(PSP_CTRL_SELECT);
	pgeControlsAddInput(PSP_CTRL_START);
	pgeControlsAddInput(PSP_CTRL_UP);
	pgeControlsAddInput(PSP_CTRL_RIGHT);
	pgeControlsAddInput(PSP_CTRL_DOWN);
	pgeControlsAddInput(PSP_CTRL_LEFT);
	pgeControlsAddInput(PSP_CTRL_LTRIGGER);
	pgeControlsAddInput(PSP_CTRL_RTRIGGER);
	pgeControlsAddInput(PSP_CTRL_TRIANGLE);
	pgeControlsAddInput(PSP_CTRL_CIRCLE);
	pgeControlsAddInput(PSP_CTRL_CROSS);
	pgeControlsAddInput(PSP_CTRL_SQUARE);
}

int pgeControlsPressed(const unsigned int buttons)
{
	if(!enabled)
		return 0;

	if((pad.pressed & buttons) == buttons)
	{		
		return 1;
	}

	return 0;
}

int pgeControlsHeld(const unsigned int buttons)
{
	if(!enabled)
		return 0;

	return (pad.held & buttons) == buttons;
}

int pgeControlsReleased(const unsigned int buttons)
{
	if(!enabled)
		return 0;

	if((pad.released & buttons) == buttons)
	{		
		return 1;
	}

	return 0;
}

int pgeControlsHeldAny(void)
{
	if(!enabled)
		return 0;

	if (pgeControlsHeld (PSP_CTRL_SELECT)	||
		pgeControlsHeld (PSP_CTRL_START)	||
		pgeControlsHeld (PSP_CTRL_UP)		||
		pgeControlsHeld (PSP_CTRL_RIGHT)	||
		pgeControlsHeld (PSP_CTRL_DOWN)		||
		pgeControlsHeld (PSP_CTRL_LEFT)		||
		pgeControlsHeld (PSP_CTRL_LTRIGGER)	||
		pgeControlsHeld (PSP_CTRL_RTRIGGER)	||
		pgeControlsHeld (PSP_CTRL_TRIANGLE)	||
		pgeControlsHeld (PSP_CTRL_CIRCLE)	||
		pgeControlsHeld (PSP_CTRL_CROSS)	||
		pgeControlsHeld (PSP_CTRL_SQUARE))
			return 1;

	return 0;
}

int pgeControlsPressedAny(void)
{
	if(!enabled)
		return 0;

	if (pgeControlsPressed (PSP_CTRL_SELECT)	||
		pgeControlsPressed (PSP_CTRL_START)		||
		pgeControlsPressed (PSP_CTRL_UP)		||
		pgeControlsPressed (PSP_CTRL_RIGHT)		||
		pgeControlsPressed (PSP_CTRL_DOWN)		||
		pgeControlsPressed (PSP_CTRL_LEFT)		||
		pgeControlsPressed (PSP_CTRL_LTRIGGER)	||
		pgeControlsPressed (PSP_CTRL_RTRIGGER)	||
		pgeControlsPressed (PSP_CTRL_TRIANGLE)	||
		pgeControlsPressed (PSP_CTRL_CIRCLE)	||
		pgeControlsPressed (PSP_CTRL_CROSS)		||
		pgeControlsPressed (PSP_CTRL_SQUARE))
			return 1;

	return 0;
}

int pgeControlsReleasedAny(void)
{
	if(!enabled)
		return 0;

	if (pgeControlsReleased (PSP_CTRL_SELECT)	||
		pgeControlsReleased (PSP_CTRL_START)	||
		pgeControlsReleased (PSP_CTRL_UP)		||
		pgeControlsReleased (PSP_CTRL_RIGHT)	||
		pgeControlsReleased (PSP_CTRL_DOWN)		||
		pgeControlsReleased (PSP_CTRL_LEFT)		||
		pgeControlsReleased (PSP_CTRL_LTRIGGER)	||
		pgeControlsReleased (PSP_CTRL_RTRIGGER)	||
		pgeControlsReleased (PSP_CTRL_TRIANGLE)	||
		pgeControlsReleased (PSP_CTRL_CIRCLE)	||
		pgeControlsReleased (PSP_CTRL_CROSS)	||
		pgeControlsReleased (PSP_CTRL_SQUARE))
			return 1;

	return 0;
}

char pgeControlsAnalogX(void)
{
	if(!enabled)
		return 0;

	return pad.analog.x;
}

char pgeControlsAnalogY(void)
{
	if(!enabled)
		return 0;

	return pad.analog.y;
}

float pgeControlsAnalogAngle(void)
{
	if(!enabled)
		return 0;
		
	float x = (float)128.0f - (pad.analog.x + 128.0f);
	
	if(x == 0.0f)
		x = 1.0f;
	
	float y = (float)128.0f - (pad.analog.y + 128.0f);
	
	if(y == 0.0f)
		y = 1.0f;
		
	float theta = 0.0f;
	
	if(y >= 0.0f && x > 0.0f)
		theta = pgeMathAtan(y/x);
	else if(y >= 0.0f && x < 0.0f)
		theta = -pgeMathAtan(y/-x)-PGE_CONTROLS_PI;
	else if(y <= 0.0f && x > 0.0f)
		theta = -pgeMathAtan(-y/x);
	else if(y <= 0.0f && x < 0.0f)
		theta = pgeMathAtan(-y/-x)-PGE_CONTROLS_PI;
		
	theta -= (PGE_CONTROLS_PI * 0.5f);
	
	theta += 2 * PGE_CONTROLS_PI;
	
	return theta;
}

float pgeControlsAnalogMag(void)
{
	if(!enabled)
		return 0;
		
	float x = (float)pad.analog.x;
	float y = (float)pad.analog.y;
		
	float magnitude = pgeMathSqrt(x*x + y*y)/127.0f;
	
	if(magnitude > 1.0f)
		magnitude = 1.0f;
	
	return magnitude;
}

void pgeControlsEnable(void)
{
	enabled = 1;
}

void pgeControlsDisable(void)
{
	enabled = 0;
}
