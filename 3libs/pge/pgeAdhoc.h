/*
 * pgeAdhoc.h: Header for adhoc
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

#ifndef __PGEADHOC_H__
#define __PGEADHOC_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup pgeAdhoc Adhoc Library
 *  @{
 */

// INCLUDES
#include <psptypes.h>
#include <pspnet_adhoc.h>
#include <pspnet_adhocctl.h>
#include <pspnet.h>

#define PGE_ADHOC_EVENT_ERROR			0x0001
#define PGE_ADHOC_EVENT_CONNECT			0x0002
#define PGE_ADHOC_EVENT_DISCONNECT		0x0004
#define PGE_ADHOC_EVENT_SCAN			0x0008
#define PGE_ADHOC_EVENT_GAMEMODE		0x0010
#define PGE_ADHOC_EVENT_CANCEL			0x0020

#define PGE_ADHOC_MATCHING_EVENT_NONE			0
#define PGE_ADHOC_MATCHING_EVENT_HELLO			1
#define PGE_ADHOC_MATCHING_EVENT_REQUEST		2
#define PGE_ADHOC_MATCHING_EVENT_LEAVE			3
#define PGE_ADHOC_MATCHING_EVENT_DENY			4
#define PGE_ADHOC_MATCHING_EVENT_CANCEL			5
#define PGE_ADHOC_MATCHING_EVENT_ACCEPT			6
#define PGE_ADHOC_MATCHING_EVENT_ESTABLISHED	7
#define PGE_ADHOC_MATCHING_EVENT_TIMEOUT		8
#define PGE_ADHOC_MATCHING_EVENT_ERROR			9
#define PGE_ADHOC_MATCHING_EVENT_BYE			10

#define PGE_ADHOC_TYPE_NORMAL			0
#define PGE_ADHOC_TYPE_GAMESHARING		2

#define PGE_ADHOC_MATCHING_MODE_GAME	0
#define PGE_ADHOC_MATCHING_MODE_PTP		1

#define PGE_ADHOC_MATCHING_TYPE_HOST	0
#define PGE_ADHOC_MATCHING_TYPE_CLIENT	1

typedef struct
{
	char mac[18];
	char nickname[128];
	char hello[128];
	int event;
	
} pgeAdhocPeerEvent;

// PROTOTYPES

int pgeAdhocInit(int type);

int pgeAdhocShutdown(void);

int pgeAdhocGameHost(const char *name);

int pgeAdhocConnect(const char *name);

int pgeAdhocPeerExists(const char *mac);

int pgeAdhocMatchingInit(int mode, int type);

int pgeAdhocMatchingStart(const char *hello);

int pgeAdhocMatchingAccept(const char *mac);

int pgeAdhocMatchingDecline(const char *mac);

int pgeAdhocMatchingShutdown(void);

pgeAdhocPeerEvent* pgeAdhocMatchingGetEvents(void);

int pgeAdhocMatchingClearEvent(pgeAdhocPeerEvent *event);

int pgeAdhocPtpHostStart(void);

int pgeAdhocPtpClientStart(const char *servermac);

int pgeAdhocPtpReceive(void *data, int *length);

int pgeAdhocPtpSend(void *data, int *length);

int pgeAdhocPtpCheckForData(void);

int pgeAdhocPtpFlush(void);

int pgeAdhocPtpHostShutdown(void);

int pgeAdhocPtpClientShutdown(void);

int pgeAdhocGetState(void);

int pgeAdhocGetError(void);

/** @} */

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __PGEADHOC_H__
