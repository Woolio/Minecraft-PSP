/*
 * pgeUtils.h: Header for utility functions
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

#ifndef __PGEUTILS_H__
#define __PGEUTILS_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup pgeUtils Utility Library
 *  @{
 */

#define PGE_UTILS_NET_DIALOG_ADHOC_CONNECT		2
#define PGE_UTILS_NET_DIALOG_ADHOC_CREATE		4
#define PGE_UTILS_NET_DIALOG_ADHOC_JOIN			5
#define PGE_UTILS_ADHOC_GAMESHARE_NAME			"GameShar"

#define PGE_UTILS_MSG_DIALOG_NO_OPTIONS			0x00000000
#define PGE_UTILS_MSG_DIALOG_YESNO_BUTTONS		0x00000010
#define PGE_UTILS_MSG_DIALOG_DEFAULT_BUTTON_NO	0x00000100

#define PGE_UTILS_MSG_DIALOG_RESULT_YES			1
#define PGE_UTILS_MSG_DIALOG_RESULT_NO			2
#define PGE_UTILS_MSG_DIALOG_RESULT_BACK		3

#define PGE_UTILS_SAVEDATA_TYPE_AUTOLOAD		0
#define PGE_UTILS_SAVEDATA_TYPE_AUTOSAVE		1
#define PGE_UTILS_SAVEDATA_TYPE_LISTLOAD		4
#define PGE_UTILS_SAVEDATA_TYPE_LISTSAVE		5
#define PGE_UTILS_SAVEDATA_TYPE_LISTDELETE		7

#define PGE_UTILS_DIALOG_RUNNING				-1

/**
 * Save or load savedata dialog
 *
 * @param type - PGE_UTILS_SAVEDATA_TYPE_AUTOLOAD, PGE_UTILS_SAVEDATA_TYPE_LISTSAVE etc.
 * 
 * @param data - The data to save, or a buffer to load the data into.
 *
 * @param datasize - Size of the data.
 *
 * @param gamename - The game name to save under (must be 9 characters long).
 *
 * @param key - The unique key (password) for the save (must be 16 characters long).
 *
 * @param title - The game title that is seen on the save/load/VSH screen (max 128 characters).
 *
 * @param subtitle - The second title that is seen on the save/load/VSH screen (max 128 characters).
 *
 * @param detail - The detail that is seen on the save/load/VSH screen (max 1024 characters).
 *
 * @returns 1 on success, 0 on error.
 */
int pgeUtilsSavedataInit(int type, void *data, unsigned int datasize, const char *gamename, const char *key, const char *title, const char *subtitle, const char *detail);

/**
 * Update savedata dialog
 *
 * @returns -1 if dialog is still running, 0 on success, 1 if user cancelled.
 */
int pgeUtilsSavedataUpdate(void);

/**
 * Init browser dialog
 *
 * @param memory - The amount of memory to use for the browser (in bytes)
 *
 * @param url - The initial URL to open on launch
 *
 * @returns 1 on success, 0 on error.
 */
int pgeUtilsBrowserInit(int memorysize, const char *url);

/**
 * Update browser dialog
 *
 * @returns -1 if dialog is still running, 0 on success, 1 if user cancelled.
 */
int pgeUtilsBrowserUpdate(void);

/**
 * Init net dialog
 *
 * @returns 1 on success, 0 on error.
 */
int pgeUtilsNetDialogInit(void);

/**
 * Update net dialog
 *
 * @returns -1 if dialog is still running, 0 on success, 1 if user cancelled.
 */
int pgeUtilsNetDialogUpdate(void);

/**
 * Init adhoc dialog
 *
 * @param type - Either PGE_UTILS_NET_DIALOG_ADHOC_CONNECT, PGE_UTILS_NET_DIALOG_ADHOC_CREATE or PGE_UTILS_NET_DIALOG_ADHOC_JOIN
 *
 * @param name - Either PGE_UTILS_ADHOC_GAMESHARE_NAME for gamesharing or an 8 character string for the group name
 *
 * @returns 1 on success, 0 on error.
 */
int pgeUtilsAdhocDialogInit(int type, char *name);

/**
 * Update adhoc dialog
 *
 * @returns -1 if dialog is still running, 0 on success, 1 if user cancelled.
 */
int pgeUtilsAdhocDialogUpdate(void);

/**
 * Init gameshare dialog
 *
 * @param filepath - Path to the EBOOT to share
 *
 * @param name - Name of the program (that will be displayed to the receiver)
 *
 * @returns 1 on success, 0 on error.
 */
int pgeUtilsGameShareInit(const char *filepath, const char *name);

/**
 * Update gameshare dialog
 *
 * @returns -1 if dialog is still running, 0 on success, 1 if user cancelled.
 */
int pgeUtilsGameShareUpdate(void);

/**
 * Init message dialog
 *
 * @param options - One or more of PGE_UTILS_MSG_DIALOG_NO_OPTIONS, PGE_UTILS_MSG_DIALOG_YESNO_BUTTONS, PGE_UTILS_MSG_DIALOG_DEFAULT_BUTTON_NO
 *
 * @param text - Text that is displayed in the dialog
 *
 * @returns 1 on success, 0 on error.
 */
int pgeUtilsMsgDialogInit(int options, const char *text, ...);

/**
 * Init message dialog (error)
 *
 * @param error - Error number to display
 *
 * @returns 1 on success, 0 on error.
 */
int pgeUtilsMsgDialogErrorInit(unsigned int error);

/**
 * Update gameshare dialog
 *
 * @returns -1 if dialog is still running, PGE_UTILS_MSG_DIALOG_RESULT_YES, PGE_UTILS_MSG_DIALOG_RESULT_NO or PGE_UTILS_MSG_DIALOG_RESULT_BACK if button pressed.
 */
int pgeUtilsMsgDialogUpdate(void);

/**
 * Abort a running message dialog
 *
 * @returns 1 on success, < 0 on error.
 */
int pgeUtilsMsgDialogAbort(void);

/**
 * Init OSK dialog
 *
 * @param description - Description that is displayed to the user (pass NULL for none)
 *
 * @param initialtext - Initial text in the entry field (pass NULL for none)
 *
 * @returns 1 on success, 0 on error.
 */
int pgeUtilsOskInit(char *description, char *initialtext);

/**
 * Update OSK dialog
 *
 * @param outtext - Pointer to a buffer to store the text that has been input.
 *
 * @returns -1 if dialog is still running, 0 if the text is unchanged, 1 if cancelled or 2 if the text has been changed.
 */
int pgeUtilsOskUpdate(char *outtext);

/** @} */

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __PGEUTILS_H__
