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

#include <pspsdk.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <pspnet_adhocmatching.h>
#include <psputility.h>
#include <pspnet.h>

#include "pgeAdhoc.h"

#define PGE_NET_ADHOCCTL_EVENT_ERROR		0
#define PGE_NET_ADHOCCTL_EVENT_CONNECT		1
#define PGE_NET_ADHOCCTL_EVENT_DISCONNECT	2
#define PGE_NET_ADHOCCTL_EVENT_SCAN			3
#define PGE_NET_ADHOCCTL_EVENT_GAMEMODE		4

static int pgeAdhocErrorCode = 0;
static int pgeAdhocEventFlag = 0;

static int pgeAdhocMatchingId = 0;
static int pgeAdhocPtpHostId = 0;
static int pgeAdhocPtpClientId = 0;

static char pgeAdhocctlGroupName[8];

static pgeAdhocPeerEvent pgeAdhocPeerEvents[15];

static void pgeAdhocctlHandler(int event, int error, void *arg)
{
	(void)arg;
	
	if(event == PGE_NET_ADHOCCTL_EVENT_ERROR)
	{
		pgeAdhocErrorCode = error;
		pgeAdhocEventFlag |= PGE_ADHOC_EVENT_ERROR;
	}	
	else if(event == PGE_NET_ADHOCCTL_EVENT_CONNECT)
	{
		pgeAdhocErrorCode = 0;
		pgeAdhocEventFlag |= PGE_ADHOC_EVENT_CONNECT;
	}
	else if(event == PGE_NET_ADHOCCTL_EVENT_DISCONNECT)
	{
		pgeAdhocErrorCode = 0;
		pgeAdhocEventFlag |= PGE_ADHOC_EVENT_DISCONNECT;
	}
	else if(event == PGE_NET_ADHOCCTL_EVENT_SCAN)
	{
		pgeAdhocErrorCode = 0;
		pgeAdhocEventFlag |= PGE_ADHOC_EVENT_SCAN;
	}	
	else if(event == PGE_NET_ADHOCCTL_EVENT_GAMEMODE)
	{
		pgeAdhocErrorCode = 0;
		pgeAdhocEventFlag |= PGE_ADHOC_EVENT_GAMEMODE;
	}
	
	return;
}

static void pgeAdhocMatchingAddEvent(unsigned char *mac, int event, int optlen, char *opt)
{
	int i;
	
	char mac2[20];
	
	sceNetEtherNtostr(mac, mac2);
	
	for(i = 0;i < 15;i++)
	{
		if(pgeAdhocPeerEvents[i].event != PGE_ADHOC_MATCHING_EVENT_NONE)
		{
			if(memcmp(mac2, pgeAdhocPeerEvents[i].mac, 18) == 0)
			{
				pgeAdhocPeerEvents[i].event = event;
				return;
			}
		}
	}
	
	for(i = 0;i < 15;i++)
	{
		if(pgeAdhocPeerEvents[i].event == PGE_ADHOC_MATCHING_EVENT_NONE)
		{
			pgeAdhocPeerEvents[i].event = event;
			sceNetEtherNtostr(mac, pgeAdhocPeerEvents[i].mac);
			sceNetAdhocctlGetNameByAddr(mac, pgeAdhocPeerEvents[i].nickname);
			
			if(optlen > 0)
				strncpy(pgeAdhocPeerEvents[i].hello, opt, optlen);
				
			return;
		}
	}
}
			
static void pgeAdhocMatchingHandler(int id, int event, unsigned char *mac, int optlen, void *opt)
{
	(void)id;
	
	pgeAdhocMatchingAddEvent(mac, event, optlen, (char*)opt);	
}

int pgeAdhocInit(int type)
{		
	int result;
	
	result = sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON);
	
	if(result < 0)
	{
		printf("pgeAdhoc error: sceUtilityLoadNetModule(PSP_NET_MODULE_COMMON): 0x%08X", result);
		return result;
	}
	
	result = sceUtilityLoadNetModule(PSP_NET_MODULE_ADHOC);
	
	if(result < 0)
	{
		printf("pgeAdhoc error: sceUtilityLoadNetModule(PSP_NET_MODULE_ADHOC); 0x%08X", result);
		return result;
	}
	
	result = sceNetInit(128*1024, 42, 4*1024, 42, 4*1024);
	
	if(result < 0)
	{
		printf("pgeAdhoc error: sceNetInit(); 0x%08X", result);
		return result;
	}
	
	result = sceNetAdhocInit();
	
	if(result < 0)
		printf("pgeAdhoc error: sceNetAdhocInit(): 0x%08X", result);
	
	struct productStruct gameProduct;
	
	gameProduct.unknown = type;
	
	if(type == PGE_ADHOC_TYPE_GAMESHARING)
		memcpy(gameProduct.product, "000000001", 9);
	else
		memcpy(gameProduct.product, "ULUS99999", 9);
	
	result = sceNetAdhocctlInit(32*1024, 0x20, &gameProduct);
	
	if(result < 0)
		printf("pgeAdhoc error: sceNetAdhocctlInit(): 0x%08X", result);
	
	// Register pspnet_adhocctl event handler
	result = sceNetAdhocctlAddHandler(pgeAdhocctlHandler, NULL);
	
	if(result < 0)
	{
		printf("pgeAdhoc error: sceNetAdhocctlAddHandler\n");
		return 0;
	}
	
	return 1;
}

int pgeAdhocShutdown(void)
{	
	sceNetAdhocctlDisconnect();
	
	sceNetAdhocctlDelHandler(0);
	
	sceNetAdhocctlTerm(); 
	
	sceNetAdhocTerm();
	
	sceNetTerm();
	
	sceUtilityUnloadNetModule(PSP_NET_MODULE_ADHOC);
	
	sceUtilityUnloadNetModule(PSP_NET_MODULE_COMMON);
	
	return 1;
}

int pgeAdhocGameHost(const char *name)
{
	memset(&pgeAdhocctlGroupName, 0, sizeof(pgeAdhocctlGroupName));
	memcpy(&pgeAdhocctlGroupName, name, strlen(name));
	
	int ret = sceNetAdhocctlCreate(pgeAdhocctlGroupName);
	
	if(ret < 0)
	{
		printf("pgeAdhoc error: sceNetAdhocctlCreate\n");
		return 0;
	}
	
	return 1;
}

int pgeAdhocConnect(const char *name)
{	
	memset(&pgeAdhocctlGroupName, 0, sizeof(pgeAdhocctlGroupName));
	sprintf(pgeAdhocctlGroupName, name);
	
	int ret = sceNetAdhocctlConnect(pgeAdhocctlGroupName);
		
	if(ret < 0)
	{
		printf("pgeAdhoc error: sceNetAdhocctlConnect\n");
		return 0;
	}
	
	return 1;
}

int pgeAdhocGetState(void)
{
	if(pgeAdhocEventFlag & PGE_ADHOC_EVENT_ERROR)
	{
		pgeAdhocEventFlag &= ~PGE_ADHOC_EVENT_ERROR;
		return(PGE_ADHOC_EVENT_ERROR);
	}
	else if(pgeAdhocEventFlag & PGE_ADHOC_EVENT_CONNECT)
	{
		pgeAdhocEventFlag &= ~PGE_ADHOC_EVENT_CONNECT;
		return(PGE_ADHOC_EVENT_CONNECT);
	}
	else if(pgeAdhocEventFlag & PGE_ADHOC_EVENT_DISCONNECT)
	{
		pgeAdhocEventFlag &= ~PGE_ADHOC_EVENT_DISCONNECT;
		return(PGE_ADHOC_EVENT_DISCONNECT);
	}
	else if(pgeAdhocEventFlag & PGE_ADHOC_EVENT_SCAN)
	{
		pgeAdhocEventFlag &= ~PGE_ADHOC_EVENT_SCAN;
		return(PGE_ADHOC_EVENT_SCAN);
	}
	else if(pgeAdhocEventFlag & PGE_ADHOC_EVENT_GAMEMODE)
	{
		pgeAdhocEventFlag &= ~PGE_ADHOC_EVENT_GAMEMODE;
		return(PGE_ADHOC_EVENT_GAMEMODE);
	}
	else if(pgeAdhocEventFlag & PGE_ADHOC_EVENT_CANCEL)
	{
		pgeAdhocEventFlag &= ~PGE_ADHOC_EVENT_CANCEL;
		return(PGE_ADHOC_EVENT_CANCEL);
	}
	
	return 0;
}

int pgeAdhocPeerExists(const char *mac)
{
	unsigned char peermac[6];
	
	sceNetEtherStrton(mac, peermac);
	
	struct SceNetAdhocctlPeerInfo info;
	
	int ret = sceNetAdhocctlGetPeerInfo(peermac, sizeof(struct SceNetAdhocctlPeerInfo), &info);
	
	if(ret < 0)
		return 0;
		
	return 1;
}	

int pgeAdhocMatchingInit(int mode, int type)
{	
	memset(pgeAdhocPeerEvents, 0, sizeof(pgeAdhocPeerEvents));
	
	int ret, maxpeers, intmode, inttype;
	
	if(mode == PGE_ADHOC_MATCHING_MODE_PTP)
	{
		intmode = 3;
		inttype = type;
		maxpeers = 2;
	}
	else
	{
		if(type == PGE_ADHOC_MATCHING_TYPE_HOST)
			intmode = 1;
		else
			intmode = 2;
			
		inttype = type;
		maxpeers = 16;
	}
	
	ret = sceNetAdhocMatchingInit(32*1024);
	
	if(ret < 0)
	{
		printf("pgeAdhoc error: sceNetAdhocMatchingInit\n");
		return 0;
	}
	
	ret = sceNetAdhocMatchingCreate(3, 2, 1, 2048, 200*1000, 200*1000, 30, 200*1000, pgeAdhocMatchingHandler);
	
	if(ret < 0)
	{
		printf("pgeAdhoc error: sceNetAdhocMatchingCreate\n");
		return 0;
	}
	
	pgeAdhocMatchingId = ret;
	
	return 1;
}

int pgeAdhocMatchingStart(const char *hello)
{
	int ret;
	
	if(hello == NULL)
		ret = sceNetAdhocMatchingStart(pgeAdhocMatchingId, 16, 32 * 1024, 16, 0, 0, NULL);
	else
		ret = sceNetAdhocMatchingStart(pgeAdhocMatchingId, 16, 32 * 1024, 16, 0, strlen(hello) + 1, (void*)hello);
	 
	if(ret < 0)
	{
		printf("pgeAdhoc error: sceNetAdhocMatchingStart\n");
		return 0;
	}
	
	return 1;
}

int pgeAdhocMatchingAccept(const char *mac)
{
	unsigned char mac2[6];
	
	sceNetEtherStrton(mac, mac2);
	
	int ret = sceNetAdhocMatchingSelectTarget(pgeAdhocMatchingId, mac2, 0, 0);
	
	if(ret < 0)
	{
		printf("pgeAdhoc error: sceNetAdhocMatchingSelectTarget: 0x%08X\n", ret);
		return 0;
	}
		
	return 1;
}

int pgeAdhocMatchingDecline(const char *mac)
{
	unsigned char mac2[6];
	
	sceNetEtherStrton(mac, mac2);
	
	int ret = sceNetAdhocMatchingCancelTarget(pgeAdhocMatchingId, mac2);
	
	if(ret < 0)
	{
		printf("pgeAdhoc error: sceNetAdhocMatchingCancelTarget\n");
		return 0;
	}
		
	return 1;
}

int pgeAdhocMatchingShutdown(void)
{	
	int ret = sceNetAdhocMatchingStop(pgeAdhocMatchingId);
	
	if(ret < 0)
	{
		printf("pgeAdhoc error: sceNetAdhocMatchingStop\n");
		return 0;
	}
	
	ret = sceNetAdhocMatchingDelete(pgeAdhocMatchingId);
	
	if(ret < 0)
	{
		printf("pgeAdhoc error: sceNetAdhocMatchingDelete\n");
		return 0;
	}
	
	ret = sceNetAdhocMatchingTerm();
	
	if(ret < 0)
	{
		printf("pgeAdhoc error: sceNetAdhocMatchingTerm\n");
		return 0;
	}
	
	return 1;
}

pgeAdhocPeerEvent* pgeAdhocMatchingGetEvents(void)
{
	return(pgeAdhocPeerEvent*) &pgeAdhocPeerEvents[0];
}

int pgeAdhocMatchingClearEvent(pgeAdhocPeerEvent *event)
{
	event->event = PGE_ADHOC_MATCHING_EVENT_NONE;
	
	return 1;
}

int pgeAdhocPtpHostStart(void)
{
	unsigned char mac[6];
	unsigned char clientmac[6];
	unsigned short clientport;
	
	pgeAdhocPtpHostId = -1;
	pgeAdhocPtpClientId = -1;
	
	int ret = sceNetGetLocalEtherAddr(mac);
		
	if(ret < 0)
	{
		printf("pgeAdhoc error: sceNetGetLocalEtherAddr\n");
		return 0;
	}
	
	ret = sceNetAdhocPtpListen(mac, 1, 8192, 200*1000, 300, 1, 0);
	
	if(ret < 0)
	{
		printf("pgeAdhoc error: sceNetAdhocPtpListen 0x%08X\n", ret);
		
		if(pgeAdhocPtpHostId > 0)
			sceNetAdhocPtpClose(pgeAdhocPtpHostId, 0);
			
		if(pgeAdhocPtpClientId > 0)
			sceNetAdhocPtpClose(pgeAdhocPtpClientId, 0);
		
		return ret;
	}
	
	pgeAdhocPtpHostId = ret;
		
	ret = sceNetAdhocPtpAccept(pgeAdhocPtpHostId, &clientmac[0], &clientport, 0, 0);
	
	if(ret < 0)
	{
		printf("pgeAdhoc error: sceNetAdhocPtpAccept\n");
		
		if(pgeAdhocPtpHostId > 0)
			sceNetAdhocPtpClose(pgeAdhocPtpHostId, 0);
			
		if(pgeAdhocPtpClientId > 0)
			sceNetAdhocPtpClose(pgeAdhocPtpClientId, 0);
		
		return ret;
	}
	
	pgeAdhocPtpClientId = ret;
	
	return ret;
}

int pgeAdhocPtpClientStart(const char *servermac)
{
	unsigned char mac[6];
	
	sceNetEtherStrton(servermac, mac);
	
	unsigned char clientmac[6];
	
	int ret = sceNetGetLocalEtherAddr(clientmac);
	
	if(ret < 0)
	{
		printf("pgeAdhoc error: sceNetGetLocalEtherAddr\n");
		return ret;
	}
	
	ret = sceNetAdhocPtpOpen(clientmac, 0, mac, 1, 8192, 200*1000, 300, 0);
	
	if(ret < 0)
	{
		printf("pgeAdhoc error: sceNetAdhocPtpOpen\n");
		
		if(pgeAdhocPtpClientId > 0)
			sceNetAdhocPtpClose(pgeAdhocPtpClientId, 0);
		
		return ret;
	}
	
	pgeAdhocPtpClientId = ret;
	
	while(1)
	{	
		ret = sceNetAdhocPtpConnect(pgeAdhocPtpClientId, 0, 1);
			
		if(ret < 0 && ret != (int)0x80410709)
		{
			printf("pgeAdhoc error: sceNetAdhocPtpConnect 0x%08X\n", ret);
		
			if(pgeAdhocPtpClientId > 0)
				sceNetAdhocPtpClose(pgeAdhocPtpClientId, 0);
		
			return ret;
		}
		
		if(ret == 0)
			break;
		
		sceKernelDelayThread(200*1000);
	}
	
	return ret;
}

int pgeAdhocPtpReceive(void *data, int *length)
{	
	int ret = sceNetAdhocPtpRecv(pgeAdhocPtpClientId, data, length, 1000*1000, 0);
	
	if(ret < 0 && ret != (int)0x80410709)
	{
		printf("pgeAdhoc error: sceNetAdhocPtpRecv\n");
		
		if(pgeAdhocPtpClientId > 0)
			sceNetAdhocPtpClose(pgeAdhocPtpClientId, 0);
		
		return 0;
	}
	
	return 1;
}

int pgeAdhocPtpSend(void *data, int *length)
{
	int ret = sceNetAdhocPtpSend(pgeAdhocPtpClientId, data, length, 0, 0);
	
	if(ret < 0)
	{
		printf("pgeAdhoc error: sceNetAdhocPtpSend\n");
		
		if(pgeAdhocPtpHostId > 0)
			sceNetAdhocPtpClose(pgeAdhocPtpHostId, 0);
			
		if(pgeAdhocPtpClientId > 0)
			sceNetAdhocPtpClose(pgeAdhocPtpClientId, 0);
		
		return(0);
	}
		
	return 1;
}

int pgeAdhocPtpCheckForData(void)
{
	//int torecv = 0;
	
	int ret, buflen;
	
	//ptpStatStruct *buf, *ptr;
	
	ret = sceNetAdhocGetPtpStat(&buflen, NULL);
	
	if(ret < 0)
		return 0;
	else if (buflen == 0)
		return 0;
		
/*buf = malloc(buflen);
if (buf == NULL)
{
	// Could not allocate memory
	return;
}

ret = sceNetAdhocGetPtpStat(&buflen, buf);
if (ret < 0)
{
	// Error handling
}
else if (buflen == 0)
{
	// No data
}        
else
{
	for (ptr = buf; ptr != NULL; ptr = ptr->next)
	{
		// Process acquired control block
		...
	}
}*/
	
	return 1;
}

int pgeAdhocPtpFlush(void)
{
	int ret = sceNetAdhocPtpFlush(pgeAdhocPtpClientId, 0, 0);
	
	if(ret < 0)
	{
		printf("pgeAdhoc error: sceNetAdhocPtpFlush\n");
		
		if(pgeAdhocPtpHostId > 0)
			sceNetAdhocPtpClose(pgeAdhocPtpHostId, 0);
			
		if(pgeAdhocPtpClientId > 0)
			sceNetAdhocPtpClose(pgeAdhocPtpClientId, 0);
		
		return 0;
	}
	
	return 1;
}

int pgeAdhocPtpHostShutdown(void)
{
	if(pgeAdhocPtpHostId > 0)
		sceNetAdhocPtpClose(pgeAdhocPtpHostId, 0);
			
	if(pgeAdhocPtpClientId > 0)
		sceNetAdhocPtpClose(pgeAdhocPtpClientId, 0);
	
	return 1;
}

int pgeAdhocPtpClientShutdown(void)
{
	if(pgeAdhocPtpClientId > 0)
		sceNetAdhocPtpClose(pgeAdhocPtpClientId, 0);
	
	return 1;
}

int pgeAdhocGetError(void)
{
	return(pgeAdhocErrorCode);
}
