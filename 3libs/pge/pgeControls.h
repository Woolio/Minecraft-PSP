/*
 * pgeControls.h: Header for controls (button input)
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

#ifndef __PGECONTROLS_H__
#define __PGECONTROLS_H__

#include <pspctrl.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup pgeControls Controller Library
 *  @{
 */
 
/**
 * Enumerator for buttons
 */
enum pgeCtrlButtons
{
	/** Select button. */
	PGE_CTRL_SELECT =	PSP_CTRL_SELECT,
	/** Start button. */
	PGE_CTRL_START =	PSP_CTRL_START,
	/** Up D-Pad button. */
	PGE_CTRL_UP =		PSP_CTRL_UP,
	/** Right D-Pad button. */
	PGE_CTRL_RIGHT =	PSP_CTRL_RIGHT,
	/** Down D-Pad button. */
	PGE_CTRL_DOWN =		PSP_CTRL_DOWN,
	/** Left D-Pad button. */
	PGE_CTRL_LEFT =		PSP_CTRL_LEFT,
	/** Left trigger. */
	PGE_CTRL_LTRIGGER =	PSP_CTRL_LTRIGGER,
	/** Right trigger. */
	PGE_CTRL_RTRIGGER =	PSP_CTRL_RTRIGGER,
	/** Triangle button. */
	PGE_CTRL_TRIANGLE =	PSP_CTRL_TRIANGLE,
	/** Circle button. */
	PGE_CTRL_CIRCLE =	PSP_CTRL_CIRCLE,
	/** Cross button. */
	PGE_CTRL_CROSS =	PSP_CTRL_CROSS,
	/** Square button. */
	PGE_CTRL_SQUARE =	PSP_CTRL_SQUARE
};

/**
 * Init the controls
 *
 * @returns 1 on success
 */
int pgeControlsInit(void);

/**
 * Shutdown the controls
 */
void pgeControlsShutdown(void);

/**
 * Update the controls
 *
 * Should be called once at the start of each iteration of your main loop
 */
void pgeControlsUpdate(void);

/**
 * Check for ANY button being pressed
 *
 * @returns 1 on any button being pressed, 0 if no buttons are pressed
 */
int pgeControlsPressedAny(void);

/**
 * Check for a button being pressed
 *
 * @param buttons - The button types (PGE_CTRL_*) to check for
 *
 * @returns 1 if buttons pressed, 0 if not pressed
 */
int pgeControlsPressed(const unsigned int buttons);

/**
 * Check for ANY button being held
 *
 * @returns 1 on any button being pressed, 0 if no buttons are pressed
 */
int pgeControlsHeldAny(void);

/**
 * Check for a button being held
 *
 * @param buttons - The button types (PGE_CTRL_*) to check for
 *
 * @returns 1 if buttons pressed, 0 if not pressed
 */
int pgeControlsHeld(const unsigned int buttons);

/**
 * Check for ANY button being released
 *
 * @returns 1 on any button being released, 0 if no buttons are released
 */
int pgeControlsReleasedAny(void);

/**
 * Check for a button being released
 *
 * @param buttons - The button types (PGE_CTRL_*) to check for
 *
 * @returns 1 if buttons released, 0 if not pressed
 */
int pgeControlsReleased(const unsigned int buttons);

/**
 * Get the analog X position
 *
 * @returns The current value of the analog X position, -128 to 127, -128 being fully left, 127 being fully right
 */
char pgeControlsAnalogX(void);

/**
 * Get the analog Y position
 *
 * @returns The current value of the analog Y position, -128 to 127, -128 being fully up, 127 being fully down
 */
char pgeControlsAnalogY(void);

/**
 * Get the analog angle
 *
 * @returns The angle in radians
 */
float pgeControlsAnalogAngle(void);

/**
 * Get the analog magnitude
 *
 * @returns The magnitude
 */
float pgeControlsAnalogMag(void);

/**
 * Enables the controls
 *
 * @note The controls are enabled by default
 */
void pgeControlsEnable(void);

/**
 * Disables the controls
 */
void pgeControlsDisable(void);

/** @} */

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __PGECONTROLS_H__
