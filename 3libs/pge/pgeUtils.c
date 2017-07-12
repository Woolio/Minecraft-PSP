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
#include <stdio.h>
#include <stdarg.h>
#include <pspnet_adhocmatching.h>
#include <pspnet_adhoc.h>
#include <psputility_gamesharing.h>
#include <psputility_htmlviewer.h>
#include <psphttp.h>

#ifdef __PSP__
	#include <malloc.h>
#endif

#include "pgeUtils.h"
#include "pge.h"

static char pgeUtilsSavedataSaveName[20] = "0000";
static pspUtilityHtmlViewerParam pgeUtilsBrowserParams;
static SceUtilityOskParams pgeUtilsOskDialogParams;
static SceUtilityOskData pgeUtilsOskDialogData;
static unsigned short pgeUtilsOskOutText[512];
SceUtilitySavedataParam pgeUtilsSavedataParams;
static void *pgeUtilsSaveDataData = NULL;
static pspUtilityNetconfData pgeUtilsNetconfData;
static struct pspUtilityNetconfAdhoc adhocparam;
static pspUtilityMsgDialogParams pgeUtilsMsgDialogParams;
static pspUtilityGameSharingParams pgeUtilsGameShareParams;

typedef struct
{
	char signature[4];
	int version;
	int offset[8];
		
} pgeUtilsEbootHeader;

char pgeUtilsSaveNameMultiple[][20] =
{
	"0000",
	"0001",
	"0002",
	"0003",
	"0004",
	""
};

int pgeUtilsBrowserInit(int memorysize, const char *url)
{
	sceHttpInit(0x25800);

	memset(&pgeUtilsBrowserParams, 0, sizeof(pgeUtilsBrowserParams));
	pgeUtilsBrowserParams.base.size = sizeof(pgeUtilsBrowserParams);
	sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &pgeUtilsBrowserParams.base.language);
	sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN, &pgeUtilsBrowserParams.base.buttonSwap);
	pgeUtilsBrowserParams.base.graphicsThread = 17;
	pgeUtilsBrowserParams.base.accessThread = 19;
	pgeUtilsBrowserParams.base.fontThread = 18;
	pgeUtilsBrowserParams.base.soundThread = 16;
	
	pgeUtilsBrowserParams.memsize = memorysize;
	pgeUtilsBrowserParams.initialurl = (char*)url;
	pgeUtilsBrowserParams.numtabs = 3;
	pgeUtilsBrowserParams.textsize = PSP_UTILITY_HTMLVIEWER_TEXTSIZE_SMALL;
	pgeUtilsBrowserParams.connectmode = PSP_UTILITY_HTMLVIEWER_CONNECTMODE_MANUAL_ALL;
	pgeUtilsBrowserParams.textsize = PSP_UTILITY_HTMLVIEWER_TEXTSIZE_NORMAL;
	pgeUtilsBrowserParams.interfacemode = PSP_UTILITY_HTMLVIEWER_INTERFACEMODE_FULL;
	pgeUtilsBrowserParams.displaymode = PSP_UTILITY_HTMLVIEWER_DISPLAYMODE_SMART_FIT;
	
	pgeUtilsBrowserParams.memaddr = pgeMalloc(memorysize);
	
	if(!pgeUtilsBrowserParams.memaddr)
		return -1;
	
	int result = sceUtilityHtmlViewerInitStart(&pgeUtilsBrowserParams);
		
	if(result == 0)
		return 1;
	
	return result;
}

int pgeUtilsBrowserUpdate(void)
{
	int result = -1;
	
	int status = sceUtilityHtmlViewerGetStatus();
		
	switch(status)
	{
		case PSP_UTILITY_DIALOG_NONE:
			result = pgeUtilsBrowserParams.base.result;
			
			if(pgeUtilsBrowserParams.memaddr)
				pgeFree(pgeUtilsBrowserParams.memaddr);
				
			sceHttpEnd();
			break;

		case PSP_UTILITY_DIALOG_VISIBLE:
			sceUtilityHtmlViewerUpdate(1);
			break;

		case PSP_UTILITY_DIALOG_QUIT:
			sceUtilityHtmlViewerShutdownStart();
			break;
				
		case PSP_UTILITY_DIALOG_FINISHED:
			break;

		default:
			break;
	}
		
	return result;
}

int pgeUtilsNetDialogInit(void)
{		
	memset(&pgeUtilsNetconfData, 0, sizeof(pgeUtilsNetconfData));
	pgeUtilsNetconfData.base.size = sizeof(pgeUtilsNetconfData);
	sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &pgeUtilsNetconfData.base.language);
	sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN, &pgeUtilsNetconfData.base.buttonSwap);
	pgeUtilsNetconfData.base.graphicsThread = 17;
	pgeUtilsNetconfData.base.accessThread = 19;
	pgeUtilsNetconfData.base.fontThread = 18;
	pgeUtilsNetconfData.base.soundThread = 16;
	pgeUtilsNetconfData.action = 0;
	
	struct pspUtilityNetconfAdhoc adhocparam;
	memset(&adhocparam, 0, sizeof(adhocparam));
	pgeUtilsNetconfData.adhocparam = &adhocparam;

	int result = sceUtilityNetconfInitStart(&pgeUtilsNetconfData);
	
	if(result == 0)
		return 1;
		
	return result;
}
	
int pgeUtilsNetDialogUpdate(void)
{
		int result = -1;
		
		switch(sceUtilityNetconfGetStatus())
		{
			case PSP_UTILITY_DIALOG_NONE:
				result = pgeUtilsNetconfData.base.result;
				break;

			case PSP_UTILITY_DIALOG_VISIBLE:
				sceUtilityNetconfUpdate(1);
				break;

			case PSP_UTILITY_DIALOG_QUIT:
				sceUtilityNetconfShutdownStart();
				break;
				
			case PSP_UTILITY_DIALOG_FINISHED:
				break;

			default:
				break;
		}
		
	return(result);
}

int pgeUtilsAdhocDialogInit(int type, char *name)
{	
	memset(&adhocparam, 0, sizeof(adhocparam));
	memcpy(&adhocparam.name, name, sizeof(adhocparam.name));
	adhocparam.timeout = 60;
	
	memset(&pgeUtilsNetconfData, 0, sizeof(pgeUtilsNetconfData));
	pgeUtilsNetconfData.base.size = sizeof(pgeUtilsNetconfData);
	sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &pgeUtilsNetconfData.base.language);
	sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN, &pgeUtilsNetconfData.base.buttonSwap);
	pgeUtilsNetconfData.base.graphicsThread = 17;
	pgeUtilsNetconfData.base.accessThread = 19;
	pgeUtilsNetconfData.base.fontThread = 18;
	pgeUtilsNetconfData.base.soundThread = 16;
	pgeUtilsNetconfData.action = type;
	
	pgeUtilsNetconfData.adhocparam = &adhocparam;

	int result = sceUtilityNetconfInitStart(&pgeUtilsNetconfData);
	
	if(result == 0)
		return 1;
		
	return result;
}
	
int pgeUtilsAdhocDialogUpdate(void)
{
	int result = -1;
	
	switch(sceUtilityNetconfGetStatus())
	{
		case PSP_UTILITY_DIALOG_NONE:
			result = pgeUtilsNetconfData.base.result;
			break;

		case PSP_UTILITY_DIALOG_VISIBLE:
			sceUtilityNetconfUpdate(1);
			break;

		case PSP_UTILITY_DIALOG_QUIT:
			sceUtilityNetconfShutdownStart();
			break;
				
		case PSP_UTILITY_DIALOG_FINISHED:
			break;

		default:
			break;
		}
		
	return(result);
}

int pgeUtilsGameShareInit(const char *filepath, const char *name)
{	
	sceNetAdhocMatchingInit(32*1024);

	memset(&pgeUtilsGameShareParams, 0, sizeof(pgeUtilsGameShareParams));
	pgeUtilsGameShareParams.base.size = sizeof(pgeUtilsGameShareParams);
	sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &pgeUtilsGameShareParams.base.language);
	sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN, &pgeUtilsGameShareParams.base.buttonSwap);
	pgeUtilsGameShareParams.base.graphicsThread = 17;
	pgeUtilsGameShareParams.base.accessThread = 19;
	pgeUtilsGameShareParams.base.fontThread = 18;
	pgeUtilsGameShareParams.base.soundThread = 16;
	
	int lSize;
	
	int fd = sceIoOpen(filepath, PSP_O_RDONLY, 0777);
	
	if(fd < 0)
	{
		return -1;
	}

	lSize = sceIoLseek32(fd, 0, PSP_SEEK_END);
		
	unsigned char *fileBuffer = pgeMalloc(lSize);
	
	if(fileBuffer == NULL)
	{
		sceIoClose(fd);
		return -1;
	}

	sceIoLseek32(fd, 0, PSP_SEEK_SET);

	int read = sceIoRead(fd, fileBuffer, lSize);
	
	if(read < lSize)
	{
		sceIoClose(fd);
		pgeFree(fileBuffer);
		return -1;
	}
	
	sceIoClose(fd);
	
	fileBuffer[276] = 0x57;
	strncpy((char *) &fileBuffer[320], name, 127);

	memcpy(&pgeUtilsGameShareParams.name, "GameShar", 8);
	
	pgeUtilsGameShareParams.mode = 1;
	pgeUtilsGameShareParams.datatype = 2;
	
	pgeUtilsGameShareParams.data = fileBuffer;
	
	pgeUtilsGameShareParams.datasize = lSize;

	int result = sceUtilityGameSharingInitStart(&pgeUtilsGameShareParams);
	
	if(result == 0)
		return 1;
		
	return result;
}

int pgeUtilsGameShareUpdate(void)
{
	int result = -1;
		
	switch(sceUtilityGameSharingGetStatus())
	{
		case PSP_UTILITY_DIALOG_NONE:
			sceNetAdhocMatchingTerm();
			result = pgeUtilsGameShareParams.base.result;
			break;

		case PSP_UTILITY_DIALOG_VISIBLE:
			sceUtilityGameSharingUpdate(1);
			break;

		case PSP_UTILITY_DIALOG_QUIT:
			sceUtilityGameSharingShutdownStart();
			break;

		default:
			break;
	}
	
	return(result);
}

int pgeUtilsMsgDialogInit(int options, const char *text, ...)
{
	char buffer[512];
	va_list ap;
	
	va_start(ap, text);
	vsnprintf(buffer, 512, text, ap);
	va_end(ap);

	memset(&pgeUtilsMsgDialogParams, 0, sizeof(pgeUtilsMsgDialogParams));
	pgeUtilsMsgDialogParams.base.size = sizeof(pgeUtilsMsgDialogParams);
	sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &pgeUtilsMsgDialogParams.base.language);
	sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN, &pgeUtilsMsgDialogParams.base.buttonSwap);
	pgeUtilsMsgDialogParams.base.graphicsThread = 17;
	pgeUtilsMsgDialogParams.base.accessThread = 19;
	pgeUtilsMsgDialogParams.base.fontThread = 18;
	pgeUtilsMsgDialogParams.base.soundThread = 16;
	
	pgeUtilsMsgDialogParams.mode = PSP_UTILITY_MSGDIALOG_MODE_TEXT;
	pgeUtilsMsgDialogParams.options = PSP_UTILITY_MSGDIALOG_OPTION_TEXT;
	pgeUtilsMsgDialogParams.options |= options;
	
	strcpy(pgeUtilsMsgDialogParams.message, buffer);
	
	int result = sceUtilityMsgDialogInitStart(&pgeUtilsMsgDialogParams);
	
	if(result == 0)
		return 1;
		
	return result;
}

int pgeUtilsMsgDialogErrorInit(unsigned int error)
{
	memset(&pgeUtilsMsgDialogParams, 0, sizeof(pgeUtilsMsgDialogParams));
	pgeUtilsMsgDialogParams.base.size = sizeof(pgeUtilsMsgDialogParams);
	sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &pgeUtilsMsgDialogParams.base.language);
	sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN, &pgeUtilsMsgDialogParams.base.buttonSwap);
	pgeUtilsMsgDialogParams.base.graphicsThread = 17;
	pgeUtilsMsgDialogParams.base.accessThread = 19;
	pgeUtilsMsgDialogParams.base.fontThread = 18;
	pgeUtilsMsgDialogParams.base.soundThread = 16;
	
	pgeUtilsMsgDialogParams.mode = PSP_UTILITY_MSGDIALOG_MODE_ERROR;
	pgeUtilsMsgDialogParams.options = PSP_UTILITY_MSGDIALOG_OPTION_ERROR;
	pgeUtilsMsgDialogParams.errorValue = error;
	
	int result = sceUtilityMsgDialogInitStart(&pgeUtilsMsgDialogParams);
	
	if(result == 0)
		return 1;
		
	return result;
}

int pgeUtilsMsgDialogUpdate(void)
{
	int result = -1;
		
	switch(sceUtilityMsgDialogGetStatus())
	{
		case PSP_UTILITY_DIALOG_NONE:
			result = pgeUtilsMsgDialogParams.buttonPressed;
			break;

		case PSP_UTILITY_DIALOG_VISIBLE:
			sceUtilityMsgDialogUpdate(1);
			break;

		case PSP_UTILITY_DIALOG_QUIT:
			sceUtilityMsgDialogShutdownStart();
			break;
				
		case PSP_UTILITY_DIALOG_FINISHED:
			break;

		default:
			break;
	}
	
	if(result != -1)
	{
		if(!(pgeUtilsMsgDialogParams.options & PGE_UTILS_MSG_DIALOG_YESNO_BUTTONS))
			result = PGE_UTILS_MSG_DIALOG_RESULT_BACK;
	}
		
	return(result);
}

int pgeUtilsMsgDialogAbort(void)
{
	int result = sceUtilityMsgDialogAbort();
	
	if(result == 0)
		return 1;
		
	return result;
}

int pgeUtilsSavedataInit(int type, void *data, unsigned int datasize, const char *gamename, const char *key, const char *title, const char *subtitle, const char *detail)
{
	pgeUtilsSaveDataData = data;
	
	PspUtilitySavedataListSaveNewData newData;
	
	memset(&newData, 0, sizeof(newData));
		
	memset(&pgeUtilsSavedataParams, 0, sizeof(pgeUtilsSavedataParams));
	pgeUtilsSavedataParams.base.size = sizeof(pgeUtilsSavedataParams);
	
	void *icon0data = NULL;
	int icon0datasize = 0;
	void *pic1data = NULL;
	int pic1datasize = 0;
	void *snd0data = NULL;
	int snd0datasize = 0;

	char *titleshow = "New Save";
	
	sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &pgeUtilsSavedataParams.base.language);
	sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN, &pgeUtilsSavedataParams.base.buttonSwap);
	pgeUtilsSavedataParams.base.graphicsThread = 17;
	pgeUtilsSavedataParams.base.accessThread = 19;
	pgeUtilsSavedataParams.base.fontThread = 18;
	pgeUtilsSavedataParams.base.soundThread = 16;

	pgeUtilsSavedataParams.mode = type;
	
	pgeUtilsSavedataParams.overwrite = 1;
			
	if(type == PGE_UTILS_SAVEDATA_TYPE_LISTLOAD)
	{
		pgeUtilsSavedataParams.focus = PSP_UTILITY_SAVEDATA_FOCUS_LATEST;
	}
	else
	{
		pgeUtilsSavedataParams.focus = PSP_UTILITY_SAVEDATA_FOCUS_FIRSTEMPTY;
	}
		
	strncpy(pgeUtilsSavedataParams.key, key, 16);
	
	strncpy(pgeUtilsSavedataParams.gameName, gamename, 9);
	
	strcpy(pgeUtilsSavedataParams.saveName, "<>");
	
	if(type == PGE_UTILS_SAVEDATA_TYPE_AUTOLOAD || type == PGE_UTILS_SAVEDATA_TYPE_AUTOSAVE)
	{
		strcpy(pgeUtilsSavedataParams.saveName, pgeUtilsSavedataSaveName);
	}
	else
	{
		pgeUtilsSavedataParams.saveNameList = pgeUtilsSaveNameMultiple;
	}
	
	strcpy(pgeUtilsSavedataParams.fileName, "DATA.BIN");
	
	pgeUtilsSavedataParams.dataBuf = pgeMalloc(datasize);
	pgeUtilsSavedataParams.dataBufSize = datasize;
	pgeUtilsSavedataParams.dataSize = datasize;
	
	if(type == PGE_UTILS_SAVEDATA_TYPE_AUTOSAVE || type == PGE_UTILS_SAVEDATA_TYPE_LISTSAVE)
	{
		memset(pgeUtilsSavedataParams.dataBuf, 0, datasize);
		strncpy(pgeUtilsSavedataParams.dataBuf, data, datasize);

		strcpy(pgeUtilsSavedataParams.sfoParam.title, title);
		strcpy(pgeUtilsSavedataParams.sfoParam.savedataTitle, subtitle);
		strcpy(pgeUtilsSavedataParams.sfoParam.detail, detail);
		pgeUtilsSavedataParams.sfoParam.parentalLevel = 0;
		
		if(type != PGE_UTILS_SAVEDATA_TYPE_AUTOSAVE)
		{
			pgeUtilsEbootHeader ebootpgeUtilsEbootHeader;
	
			int fd;
			int ebootlength;
			int filesize;
	
			fd = sceIoOpen("EBOOT.PBP", PSP_O_RDONLY, 0777);

			ebootlength = sceIoLseek32(fd, 0, PSP_SEEK_END);

			sceIoLseek32(fd, 0, PSP_SEEK_SET);

			sceIoRead(fd, &ebootpgeUtilsEbootHeader, sizeof(ebootpgeUtilsEbootHeader));
	
			filesize = ebootpgeUtilsEbootHeader.offset[1 + 1] - ebootpgeUtilsEbootHeader.offset[1];
	
			if(filesize > 0)
			{
				sceIoLseek32(fd, ebootpgeUtilsEbootHeader.offset[1], PSP_SEEK_SET);
				icon0data = pgeMalloc(filesize);
				sceIoRead(fd, icon0data, filesize);
				icon0datasize = filesize;
			}
	
			filesize = ebootpgeUtilsEbootHeader.offset[4 + 1] - ebootpgeUtilsEbootHeader.offset[4];
	
			if(filesize > 0)
			{
				sceIoLseek32(fd, ebootpgeUtilsEbootHeader.offset[4], PSP_SEEK_SET);
				pic1data = pgeMalloc(filesize);
				sceIoRead(fd, pic1data, filesize);
				pic1datasize = filesize;
			}
	
			filesize = ebootpgeUtilsEbootHeader.offset[5 + 1] - ebootpgeUtilsEbootHeader.offset[5];
	
			if(filesize > 0)
			{
				sceIoLseek32(fd, ebootpgeUtilsEbootHeader.offset[5], PSP_SEEK_SET);
				snd0data = pgeMalloc(filesize);
				sceIoRead(fd, snd0data, filesize);
				snd0datasize = filesize;
			}
	
			sceIoClose(fd);
		}
	
		pgeUtilsSavedataParams.icon1FileData.buf = NULL;
		pgeUtilsSavedataParams.icon1FileData.bufSize = 0;
		pgeUtilsSavedataParams.icon1FileData.size = 0;
		
		pgeUtilsSavedataParams.pic1FileData.buf = pic1data;
		pgeUtilsSavedataParams.pic1FileData.bufSize = pic1datasize;
		pgeUtilsSavedataParams.pic1FileData.size = pic1datasize;
					
		pgeUtilsSavedataParams.icon0FileData.buf = icon0data;
		pgeUtilsSavedataParams.icon0FileData.bufSize = icon0datasize;
		pgeUtilsSavedataParams.icon0FileData.size = icon0datasize;
			
		pgeUtilsSavedataParams.snd0FileData.buf = snd0data;
		pgeUtilsSavedataParams.snd0FileData.bufSize = snd0datasize;
		pgeUtilsSavedataParams.snd0FileData.size = snd0datasize;

		// Set title
		newData.title = titleshow;

		// Set new data
		pgeUtilsSavedataParams.newData = &newData;
	}
	
	int result = sceUtilitySavedataInitStart(&pgeUtilsSavedataParams);
	
	if(result == 0)
		return 1;
		
	return result;
}
	
int pgeUtilsSavedataUpdate(void)
{
	int result = -1;
		
	switch(sceUtilitySavedataGetStatus())
	{
		case PSP_UTILITY_DIALOG_NONE:
			break;
			
		case PSP_UTILITY_DIALOG_VISIBLE:
			sceUtilitySavedataUpdate(1);
			break;

		case PSP_UTILITY_DIALOG_QUIT:
			sceUtilitySavedataShutdownStart();
			
			strcpy(pgeUtilsSavedataSaveName, pgeUtilsSavedataParams.saveName);
				
			if(pgeUtilsSavedataParams.mode == PGE_UTILS_SAVEDATA_TYPE_AUTOLOAD || pgeUtilsSavedataParams.mode == PGE_UTILS_SAVEDATA_TYPE_LISTLOAD)
				memcpy(pgeUtilsSaveDataData, pgeUtilsSavedataParams.dataBuf, pgeUtilsSavedataParams.dataBufSize);
	
			if(pgeUtilsSavedataParams.pic1FileData.buf != NULL)
				pgeFree(pgeUtilsSavedataParams.pic1FileData.buf);
		
			if(pgeUtilsSavedataParams.icon0FileData.buf != NULL)
				pgeFree(pgeUtilsSavedataParams.icon0FileData.buf);
		
			if(pgeUtilsSavedataParams.snd0FileData.buf != NULL)
				pgeFree(pgeUtilsSavedataParams.snd0FileData.buf);
		
			if(pgeUtilsSavedataParams.dataBuf != NULL)
				pgeFree(pgeUtilsSavedataParams.dataBuf);
	
			result = pgeUtilsSavedataParams.base.result;
			
			break;

		default:
			break;
	}
	
	return result;	
}

static unsigned short* pgeUtilsConvertToUni(char* text)
{
	unsigned short* out = (unsigned short*) pgeMalloc((strlen(text) + 1) << 1);
	
	if(!out)
	{
		return NULL;
	}
	
	unsigned char* txtPtr = (unsigned char*)text;
	unsigned short* outPtr = out;
        
	while(txtPtr[0] != 0)
		*(outPtr++) = *(txtPtr++);
		
	out[strlen(text)] = 0;
 
	return out;
}
 
static int pgeUtilsUniStrLen(unsigned short* text)
{
     unsigned short* txtPtr = text;
     int out = 0;
     
     while(txtPtr[0] != 0)
     {
          out++;
          txtPtr++;
     }
     
     return out;
}
 
static void pgeUtilsConvertToAscii(char *outtext, unsigned short* text)
{	
    unsigned char* outPtr = (unsigned char*)outtext;
    unsigned short* txtPtr = text;
        
	while(txtPtr[0] != 0)
		*(outPtr++) = (*(txtPtr++) & 0x00FF);
		
    outtext[pgeUtilsUniStrLen(text)] = 0;
}

int pgeUtilsOskInit(char *description, char *initialtext)
{
	memset(&pgeUtilsOskDialogParams, 0, sizeof(pgeUtilsOskDialogParams));
	memset(&pgeUtilsOskDialogData, 0, sizeof(pgeUtilsOskDialogData));
	
	pgeUtilsOskDialogData.language = PSP_UTILITY_OSK_LANGUAGE_DEFAULT; // Use system default for text input
	pgeUtilsOskDialogData.lines = 1;
	pgeUtilsOskDialogData.unk_24 = 1;
	pgeUtilsOskDialogData.inputtype = PSP_UTILITY_OSK_INPUTTYPE_ALL; // Allow all input types
	
	if(description != NULL)
		pgeUtilsOskDialogData.desc = pgeUtilsConvertToUni(description); // convert
		
	if(initialtext != NULL)	
		pgeUtilsOskDialogData.intext = pgeUtilsConvertToUni(initialtext); //convert
		
	pgeUtilsOskDialogData.outtextlength = 512; //strlen
	pgeUtilsOskDialogData.outtextlimit = 512; // Limit input to 512 characters
	pgeUtilsOskDialogData.outtext = pgeUtilsOskOutText;
	
	pgeUtilsOskDialogParams.base.size = sizeof(pgeUtilsOskDialogParams);
	sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_LANGUAGE, &pgeUtilsOskDialogParams.base.language);
	sceUtilityGetSystemParamInt(PSP_SYSTEMPARAM_ID_INT_UNKNOWN, &pgeUtilsOskDialogParams.base.buttonSwap);
	pgeUtilsOskDialogParams.base.graphicsThread = 17;
	pgeUtilsOskDialogParams.base.accessThread = 19;
	pgeUtilsOskDialogParams.base.fontThread = 18;
	pgeUtilsOskDialogParams.base.soundThread = 16;
	
	pgeUtilsOskDialogParams.datacount = 1;
	pgeUtilsOskDialogParams.data = &pgeUtilsOskDialogData;
	
	int result = sceUtilityOskInitStart(&pgeUtilsOskDialogParams);
	
	if(result == 0)
		return 1;
		
	return result;
}

int pgeUtilsOskUpdate(char *outtext)
{
	int result = -1;
		
	switch(sceUtilityOskGetStatus())
	{
		case PSP_UTILITY_DIALOG_NONE:
			result = pgeUtilsOskDialogData.result;
			
			if(pgeUtilsOskDialogData.desc)
				pgeFree(pgeUtilsOskDialogData.desc);
			
			if(pgeUtilsOskDialogData.intext)
				pgeFree(pgeUtilsOskDialogData.intext);
				
			pgeUtilsConvertToAscii(outtext, pgeUtilsOskDialogParams.data->outtext);				
			break;

		case PSP_UTILITY_DIALOG_VISIBLE:
			sceUtilityOskUpdate(1);
			break;

		case PSP_UTILITY_DIALOG_QUIT:
			sceUtilityOskShutdownStart();
			break;
				
		case PSP_UTILITY_DIALOG_FINISHED:
			break;

		default:
			break;
	}
		
	return result;
}
