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

#include <pspaudio.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <pspiofilemgr.h>
#include <pspthreadman.h>

#include "pge.h"
#include "pgeWav.h"
#include "pgeFile.h"

#define PGE_WAV_MAX_SLOTS 16

#define WAV_CHUNK_RIFF	0x46464952
#define WAV_CHUNK_WAVE	0x45564157
#define WAV_CHUNK_FMT	0x20746D66
#define WAV_CHUNK_FACT	0x74636166
#define WAV_CHUNK_DATA	0x61746164

static pgeWav pgeWavInfo[PGE_WAV_MAX_SLOTS];
static int pgeWavPlaying[PGE_WAV_MAX_SLOTS];
static unsigned int pgeWavId[PGE_WAV_MAX_SLOTS];
static int pgeWavIdFlag = 0;
static int pgeWavInitFlag = 0;

#define PGE_NUM_AUDIO_CHANNELS	4
#define PGE_NUM_AUDIO_SAMPLES	1024
#define PGE_VOLUME_MAX			65535

typedef void (* pgeAudioLibCallback)(void *buf, unsigned int reqn, void *pdata);

typedef struct
{
	int threadhandle;
	int handle;
	int volumeleft;
	int volumeright;
	pgeAudioLibCallback callback;
	void *data;
	
} pgeAudioLibChannelInfo;

static int pgeAudioLibReady = 0;
static short pgeAudioLibSoundBuffer[PGE_NUM_AUDIO_CHANNELS][2][PGE_NUM_AUDIO_SAMPLES][2];
static pgeAudioLibChannelInfo pgeAudioLibStatus[PGE_NUM_AUDIO_CHANNELS];
static volatile int pgeAudioLibTerminate = 0;

static void pgeAudioLibSetChannelCallback(int channel, pgeAudioLibCallback callback, void *data)
{
	(void)data;
	
	volatile pgeAudioLibChannelInfo *pci = &pgeAudioLibStatus[channel];
        
	if(callback == 0)
		pci->callback = 0;
	else
	{
		pci->callback = callback;
		sceKernelWakeupThread(pci->threadhandle);
	}
}

static int pgeAudioLibOutBlocking(unsigned int channel, unsigned int left, unsigned int right, void *data)
{
	if(!pgeAudioLibReady)
		return -1;

	if(channel >= PGE_NUM_AUDIO_CHANNELS)
		return -1;

	if(left > PGE_VOLUME_MAX)
		left = PGE_VOLUME_MAX;

	if(right > PGE_VOLUME_MAX)
		right = PGE_VOLUME_MAX;

	return sceAudioOutputPannedBlocking(pgeAudioLibStatus[channel].handle, left, right, data);
}

static int pgeAudioLibChannelThread(int args, void *argp)
{
	(void)args;
	
	volatile int bufidx = 0;

	int channel = *(int *) argp;
	
	while(pgeAudioLibTerminate == 0 && pgeRunning())
	{
		void *bufptr = &pgeAudioLibSoundBuffer[channel][bufidx];
		pgeAudioLibCallback callback;
		callback = pgeAudioLibStatus[channel].callback;

		if(callback)
		{
			callback(bufptr, PGE_NUM_AUDIO_SAMPLES, pgeAudioLibStatus[channel].data);
			pgeAudioLibOutBlocking(channel, pgeAudioLibStatus[channel].volumeleft, pgeAudioLibStatus[channel].volumeright, bufptr);
		} 
		else
		{
			sceKernelSleepThread();
		}

	bufidx = (bufidx ? 0:1);
	}

	sceKernelExitDeleteThread(0);

	return 0;
}

static int pgeAudioLibInit(int priority)
{
	int i, ret;
	int failed = 0;
	char str[32];

	pgeAudioLibTerminate = 0;
	pgeAudioLibReady = 0;
	
	for(i = 0; i < PGE_NUM_AUDIO_CHANNELS; i++)
	{
		pgeAudioLibStatus[i].handle = -1;
		pgeAudioLibStatus[i].threadhandle = -1;
		pgeAudioLibStatus[i].volumeright = PGE_VOLUME_MAX;
		pgeAudioLibStatus[i].volumeleft  = PGE_VOLUME_MAX;
		pgeAudioLibStatus[i].callback = 0;
		pgeAudioLibStatus[i].data = 0;
	}

	for(i = 0; i < PGE_NUM_AUDIO_CHANNELS; i++)
	{
		if((pgeAudioLibStatus[i].handle = sceAudioChReserve(-1, PGE_NUM_AUDIO_SAMPLES, 0)) < 0)
			failed = 1;
	}
	
	if(failed)
	{
		for(i = 0; i < PGE_NUM_AUDIO_CHANNELS; i++)
		{
			if(pgeAudioLibStatus[i].handle != -1)
				sceAudioChRelease(pgeAudioLibStatus[i].handle);

			pgeAudioLibStatus[i].handle = -1;
		}

		return 0;
	}
	
	pgeAudioLibReady = 1;

	strcpy(str, "PgeAudioThread0");
	
	for(i = 0; i < PGE_NUM_AUDIO_CHANNELS; i++)
	{
		str[14] = '0' + i;
		pgeAudioLibStatus[i].threadhandle = sceKernelCreateThread(str, (void *)&pgeAudioLibChannelThread, priority, 0x10000, PSP_THREAD_ATTR_USER, NULL);
		
		if(pgeAudioLibStatus[i].threadhandle < 0)
		{
			pgeAudioLibStatus[i].threadhandle = -1;
			failed = 1;
			break;
		}

		ret = sceKernelStartThread(pgeAudioLibStatus[i].threadhandle, sizeof(i), &i);

		if(ret != 0)
		{
			failed = 1;
			break;
		}
	}

	if(failed)
	{
		pgeAudioLibTerminate = 1;

		for(i = 0; i < PGE_NUM_AUDIO_CHANNELS; i++)
		{
			if(pgeAudioLibStatus[i].threadhandle != -1)
			{
				sceKernelDeleteThread(pgeAudioLibStatus[i].threadhandle);
			}

			pgeAudioLibStatus[i].threadhandle = -1;
		}

		pgeAudioLibReady = 0;

		return 0;
	}

	return 1;
}

static void pgeAudioLibShutdown(void)
{
	int i;
	pgeAudioLibReady = 0;
	pgeAudioLibTerminate = 1;

	for(i = 0; i < PGE_NUM_AUDIO_CHANNELS; i++)
	{
		if(pgeAudioLibStatus[i].threadhandle != -1)
			sceKernelDeleteThread(pgeAudioLibStatus[i].threadhandle);

		pgeAudioLibStatus[i].threadhandle = -1;
	}

	for(i = 0; i < PGE_NUM_AUDIO_CHANNELS; i++)
	{
		if(pgeAudioLibStatus[i].handle != -1)
		{
			sceAudioChRelease(pgeAudioLibStatus[i].handle);
			pgeAudioLibStatus[i].handle = -1;
		}
	}
}

static void pgeWavAudioCallback(void *_buf, unsigned int _reqn, void *pdata)
{
	(void)pdata;
	
	unsigned int i;
	int slot;
	pgeWav *wi;
	unsigned long ptr, frac;
	short *buf = _buf;
	
	for(i = 0; i < _reqn; i++)
	{
		int outr = 0, outl = 0;
		
		for(slot = 0; slot < PGE_WAV_MAX_SLOTS; slot++)
		{
			if(!pgeWavPlaying[slot])
				continue;
			
			wi = &pgeWavInfo[slot];
			frac = wi->playptrfrac + wi->rateratio;
			wi->playptr = ptr = wi->playptr + (frac>>16);
			wi->playptrfrac = (frac & 0xffff);
			
			if(ptr >= wi->samplecount)
			{
				if(wi->loop)
				{
					wi->playptr = 0;
					wi->playptrfrac = 0;
					ptr = 0;
				}
				else
				{
					pgeWavPlaying[slot] = 0;
					break;
				}
			}
			
			short *src16 = (short *)wi->data;
			unsigned char *src8 = (unsigned char *)wi->data;

			if(wi->channels == 1)
			{
				if(wi->bits == 8)
				{
					outl += (src8[ptr] * 256) - 32768 * wi->leftvolume;
					outr += (src8[ptr] * 256) - 32768 * wi->rightvolume;
				}
				else
				{
					outl += src16[ptr] * wi->leftvolume;
					outr += src16[ptr] * wi->rightvolume;
				}
			}
			else
			{
				if(wi->bits == 8)
				{
					outl += (src8[ptr*2] * 256) - 32768 * wi->leftvolume;
					outr += (src8[ptr*2+1] * 256) - 32768 * wi->rightvolume;
				}
				else
				{
					outl += src16[ptr*2] * wi->leftvolume;
					outr += src16[ptr*2+1] * wi->rightvolume;
				}
			}
		}
		
		if(outl < -32768)
			outl = -32768;
		else if (outl > 32767)
			outl = 32767;
			
		if(outr < -32768)
			outr = -32768;
		else if (outr > 32767)
			outr = 32767;

		*(buf++) = outl;
		*(buf++) = outr;
	}
}

int pgeWavInit(void)
{
	int i;

	pgeAudioLibInit(0x12);

	pgeAudioLibSetChannelCallback(0, pgeWavAudioCallback, 0);

	for(i = 0; i < PGE_WAV_MAX_SLOTS; i++)
		pgeWavPlaying[i] = 0;

	pgeWavInitFlag = 1;

	return 1;
}

void pgeWavShutdown(void)
{
	if(pgeWavInitFlag)
		pgeAudioLibShutdown();
}

void pgeWavStop(pgeWav *wav)
{
	int i;
	
	for(i = 0; i < PGE_WAV_MAX_SLOTS; i++)
	{
		if(wav->id == pgeWavId[i])
			pgeWavPlaying[i] = 0;
	}
}

int pgeWavIsPlaying(pgeWav *wav)
{
	int i;
	
	for(i = 0; i < PGE_WAV_MAX_SLOTS; i++)
	{
		if(wav->id == pgeWavId[i])
		{
			if(pgeWavPlaying[i] == 1)
				return 1;
		}
	}
	
	return 0;
}

void pgeWavStopAll(void)
{
	int i;
	
	for(i = 0; i < PGE_WAV_MAX_SLOTS; i++)
		pgeWavPlaying[i] = 0;
}

void pgeWavLoop(pgeWav *wav, unsigned int loop)
{
	wav->loop = loop;
}

void pgeWavVolume(pgeWav *wav, unsigned char leftvolume, unsigned char rightvolume)
{
	if(leftvolume > 100)
		leftvolume = 100;
		
	if(rightvolume > 100)
		rightvolume = 100;
	
	wav->leftvolume = leftvolume/100.0f;
	wav->rightvolume = rightvolume/100.0f;
}

void pgeWavPitch(pgeWav *wav, float pitch)
{
	wav->rateratio = ((wav->samplerate*0x4000)/11025) * pitch;
	
	if(wav->rateratio < (2000*0x4000)/11025)
		wav->rateratio = (2000*0x4000)/11025;
	else if(wav->rateratio > (100000*0x4000)/11025)
		wav->rateratio = (100000*0x4000)/11025;
}

int pgeWavPlay(pgeWav *wav)
{
	if(!pgeWavInitFlag)
		return 0;

	int i;
	
	pgeWav *wid;
	
	for(i = 0;i < PGE_WAV_MAX_SLOTS;i++)
	{
		if(pgeWavPlaying[i] == 0)
			break;
	}
	
	if(i == PGE_WAV_MAX_SLOTS)
		return 0;
	
	wid = &pgeWavInfo[i];
	wid->channels = wav->channels;
	wid->samplerate = wav->samplerate;
	wid->samplecount = wav->samplecount;
	wid->datalength = wav->datalength;
	wid->data = wav->data;
	wid->rateratio = wav->rateratio;
	wid->playptr = 0;
	wid->playptrfrac = 0;
	wid->loop = wav->loop;
	wid->id = wav->id;
	wid->bits = wav->bits;
	wid->leftvolume = wav->leftvolume;
	wid->rightvolume = wav->rightvolume;
	pgeWavPlaying[i] = 1;
	pgeWavId[i] = wav->id;

	return 1;
}

static pgeWav *pgeWavLoadInternal(unsigned char *wavfile, int size)
{	
	pgeWav *wav = pgeMalloc(sizeof(pgeWav));
	
	if(wav == NULL)
		return NULL;
	
	int riffcheck;
	int wavecheck;
	int chunk;
	int chunksize;
	int formatdatasize;
	short bitrate;
	short compressioncode;
	short blockalign;
	short extraformatbytes;
	
	pgeFile *file = pgeFileOpenMemory((char *)wavfile, size);
	
	//riffcheck = *(int *)memread(sizeof(int));
	pgeFileRead(file, &riffcheck, sizeof(int));
	
	if(riffcheck != WAV_CHUNK_RIFF)
	{
		pgeFree(wav);
		return NULL;
	}
	
	//chunksize = *(int *)memread(sizeof(int));
	pgeFileRead(file, &chunksize, sizeof(int));
	
	//wavecheck = *(int *)memread(sizeof(int));
	pgeFileRead(file, &wavecheck, sizeof(int));
	
	if(wavecheck != WAV_CHUNK_WAVE)
	{
		pgeFree(wav);
		return NULL;
	}
	
	while(1)
	{
		//printf("Reading Chunk...0x%08X\n", filepos);
		//chunk = *(int *)memread(sizeof(int));
		pgeFileRead(file, &chunk, sizeof(int));
		//printf("CHUNK: 0x%08X\n", chunk);
		
		if(chunk == WAV_CHUNK_FMT)
		{
			//printf("FMT\n");
			//chunksize = *(int *)memread(sizeof(int));
			pgeFileRead(file, &chunksize, sizeof(int));
			//compressioncode = *(short *)memread(sizeof(short));
			pgeFileRead(file, &compressioncode, sizeof(short));
			
			if(compressioncode != 1)
			{
				pgeFree(wav);
				return NULL;
			}
			
			//wav->channels = *(short *)memread(sizeof(short));
			pgeFileRead(file, &wav->channels, sizeof(short));
			//wav->samplerate = *(int *)memread(sizeof(int));
			pgeFileRead(file, &wav->samplerate, sizeof(int));
			//bitrate = *(int *)memread(sizeof(int));
			pgeFileRead(file, &bitrate, sizeof(int));
			//blockalign = *(short *)memread(sizeof(short));
			pgeFileRead(file, &blockalign, sizeof(short));
			//wav->bits = *(short *)memread(sizeof(short));
			pgeFileRead(file, &wav->bits, sizeof(short));
			
			if(chunksize > 16)
			{
				//extraformatbytes = *(short *)memread(sizeof(short));
				pgeFileRead(file, &extraformatbytes, sizeof(short));
				//printf("EFB %d\n", extraformatbytes);
				
				if(extraformatbytes > 0)
					pgeFileSeek(file, extraformatbytes, PGE_FILE_CUR);
			}
		}
		else if(chunk == WAV_CHUNK_FACT)
		{
			//printf("FACT\n");
			//chunksize = *(int *)memread(sizeof(int));
			pgeFileRead(file, &chunksize, sizeof(int));
			//formatdatasize = *(int *)memread(sizeof(int));
			pgeFileRead(file, &formatdatasize, sizeof(int));
		}
		else if(chunk == WAV_CHUNK_DATA)
		{
			//printf("DATA\n");
			//chunksize = *(int *)memread(sizeof(int));
			pgeFileRead(file, &chunksize, sizeof(int));
			wav->data = pgeMalloc(chunksize);
			pgeFileRead(file, wav->data, chunksize);
			//memcpy(wav->data, filepos, chunksize);
			wav->datalength = chunksize;			
			break;
		}
		else
		{
			pgeFree(wav);
			return NULL;
		}
	}
		
	if(wav->channels != 2 && wav->channels != 1)
	{
		pgeFree(wav);
		return NULL;
	}
	
	if(wav->channels == 2)
		wav->samplecount = wav->datalength/(wav->bits>>2);
	else
		wav->samplecount = wav->datalength/((wav->bits>>2)>>1);
	
	if(wav->samplecount <= 0)
	{
		pgeFree(wav);
		return NULL;
	}
	
	wav->rateratio = (wav->samplerate * 0x4000)/11025;
	wav->playptr = 0;
	wav->playptrfrac = 0;
	wav->loop = 0;
	pgeWavIdFlag++;
	wav->id = pgeWavIdFlag;
	wav->leftvolume = 1.0f;
	wav->rightvolume = 1.0f;
	
	//printf("DONE\n");
	
	pgeFileClose(file);
	
	return wav;
}

pgeWav *pgeWavLoad(const char *filename)
{
	//printf("SIZE: %d\n", sizeof(pgeWav));
	
	unsigned char *buffer;
	
	int fd = sceIoOpen(filename, PSP_O_RDONLY, 0777);
	
	if(fd < 0)
		return NULL;

	int size;
	
	size = sceIoLseek32(fd, 0, PSP_SEEK_END);
	sceIoLseek32(fd, 0, PSP_SEEK_SET);
	
	buffer = (unsigned char *)pgeMalloc(size);
	
	if(buffer == NULL)
	{
		sceIoClose(fd);
		return NULL;
	}
		
	sceIoRead(fd, buffer, size);
	
	sceIoClose(fd);
	
	pgeWav *wav = pgeWavLoadInternal(buffer, size);
	
	pgeFree(buffer);
	
	return(wav);
}

pgeWav *pgeWavLoadMemory(unsigned char *buffer, int size)
{	
	return(pgeWavLoadInternal(buffer, size));
}

void pgeWavDestroy(pgeWav *wav)
{
	if(wav != NULL)
		pgeFree(wav);
}
