/*
 * pgeSystem.h: Header for System modules
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

#ifndef __PGESYSTEM_H__
#define __PGESYSTEM_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup pgeSystem System Info Library
 *  @{
 */
 
/**
 * Enumerator for languages.
 */
enum pgeSystemLanguages
{
	PGE_LANGUAGE_JAPANESE,
	PGE_LANGUAGE_ENGLISH,
	PGE_LANGUAGE_FRENCH,
	PGE_LANGUAGE_SPANISH,
	PGE_LANGUAGE_GERMAN,
	PGE_LANGUAGE_ITALIAN,
	PGE_LANGUAGE_DUTCH,
	PGE_LANGUAGE_PORTUGUESE,
	PGE_LANGUAGE_RUSSIAN,
	PGE_LANGUAGE_KOREAN,
	PGE_LANGUAGE_CHINESE_TRADITIONAL,
	PGE_LANGUAGE_CHINESE_SIMPLIFIED
};

/**
 * Get the nickname of the PSP
 *
 * @param nickname - Pointer to a buffer to store the nickname (should be 128 bytes).
 */
void pgeSystemGetNickname(char *nickname);

/**
 * Get the PSID of the PSP
 *
 * @param psid - Pointer to a buffer to store the PSID (should be 16 bytes).
 */
void pgeSystemGetPsid(unsigned char *psid);

/**
 * Get the language of the PSP
 *
 * @returns One of ::pgeSystemLanguages.
 */
int pgeSystemGetLanguage(void);

/**
 * Get the MAC address of the PSP
 *
 * @param mac - Pointer to a buffer to store the mac (should be 8 bytes).
 */
void pgeSystemGetMac(unsigned char *mac);

/**
 * Get the memory stick size.
 *
 * @returns The memory stick size in MB.
 */
float pgeSystemMsSize(void);

/**
 * Get the free memory stick size.
 *
 * @returns The free memory stick size in MB.
 */
float pgeSystemMsFreeSize(void);

/**
 * Get the free RAM size.
 *
 * @returns The free RAM size in MB (accurate to 512kb).
 */
unsigned int pgeSystemGetFreeRam(void);

/**
 * Get the CPU speed.
 *
 * @param freq - The frequency of the CPU 1-333
 *
 * @returns 1 on success, 0 on error.
 */
int pgeSystemSetCpu(unsigned int freq);

/**
 * Get the battery charge as a percentage.
 *
 * @returns The battery charge as a percentage (0-100).
 */
int pgeSystemGetBatteryPercent(void);

/**
 * Get the battery charge in minutes.
 *
 * @returns The battery charge in minutes.
 */
int pgeSystemGetBatteryTime(void);

/**
 * Get if the battery is charging or not.
 *
 * @returns 1 if charging, 0 if not.
 */
int pgeSystemGetBatteryCharging(void);

/**
 * Get if the battery exists.
 *
 * @returns 1 if exists, 0 if not.
 */
int pgeSystemGetBatteryExists(void);

/**
 * Get if the battery charge is low.
 *
 * @returns 1 if low, 0 if not.
 */
int pgeSystemGetBatteryIsLow(void);

/** @} */

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __PGESYSTEM_H__
