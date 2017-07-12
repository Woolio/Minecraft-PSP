/*
 * pgeUsb.h: Header for USB
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

#ifndef __PGEUSB_H__
#define __PGEUSB_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup pgeUsb USB Library
 *  @{
 */
 
/**
 * Enumeration for USB states
 */
enum pgeUsbStates
{
	PGE_USB_ACTIVATED =					0x200,
	PGE_USB_CABLE_CONNECTED =			0x020,
	PGE_USB_CONNECTION_ESTABLISHED =	0x002
};
 
/**
 * Initialise USB
 *
 * @returns 1 on success.
 */
int pgeUsbInit(void);

/**
 * Shutdown USB
 *
 * @returns 1 on success.
 */
int pgeUsbShutdown(void);

/**
 * Get the state of the USB
 *
 * @returns One or more of ::pgeUsbStates.
 */
int pgeUsbGetState(void);

/**
 * Activate USB
 *
 * @returns 1 on success.
 */
int pgeUsbActivate(void);

/**
 * Deactivate USB
 *
 * @returns 1 on success.
 */
int pgeUsbDeactivate(void);

/** @} */

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __PGEUSB_H__
