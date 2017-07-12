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
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/dirent.h>
#include <dirent.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pspgu.h>
#include <pspnet_adhocmatching.h>
#include <pspnet_adhocctl.h>
#include <psputility.h>
#include <psputility_gamesharing.h>
#include <psputility_htmlviewer.h>
#include <psputility_sysparam.h>
#include <psphttp.h>
#include <pspopenpsid.h>
#include <pspusb.h>

#include "SDL.h"
#include "SDL_opengl.h"

#include "pgeGfx.h"
#include "pgeEmulator.h"
#include "pgeTexture.h"
#include "pgeControls.h"

#ifndef RGBA
	#define RGBA(r,v,b,a)	((r) | ((v)<<8) | ((b)<<16) | ((a)<<24))
#endif

#define RGB12(r,v,b)	((((b)>>4)<<8) | (((v)>>4)<<4) | ((r)>>4) | (0xf<<12))
#define RGBA12(r,v,b,a)	((((a)>>4)<<12) | (((b)>>4)<<8) | (((v)>>4)<<4) | ((r)>>4))
#define RGB15(r,v,b)	((((b)>>3)<<10) | (((v)>>3)<<5) | ((r)>>3) | (1<<15))
#define RGBA15(r,v,b,a)	((((a)>>7)<<15) | (((b)>>3)<<10) | (((v)>>3)<<5) | ((r)>>3))
#define RGB16(r,v,b)	((((b)>>3)<<11) | (((v)>>2)<<5) | ((r)>>3))
#define RGB(r,v,b)		((r) | ((v)<<8) | ((b)<<16) | (0xff<<24))

#define emuRgbaGet8888(data, r, g, b, a)		((r)=((data)&0xff), (g)=(((data)>>8)&0xff), (b)=(((data)>>16)&0xff), (a)=(((data)>>24)&0xff))
#define emuRgbGet5650(data, r, g, b)			((r)=((data)&0x1f)<<3, (g)=(((data)>>5)&0x3f)<<2, (b)=(((data)>>11)&0x1f)<<3)
#define emuRgbaGet5551(data, r, g, b, a)		((r)=((data)&0x1f)<<3, (g)=(((data)>>5)&0x1f)<<3, (b)=(((data)>>10)&0x1f)<<3, (a)=(((data)>>15)&0x1)<<7)
#define emuRgbaGet4444(data, r, g, b, a)		((r)=((data)&0xf)<<4, (g)=(((data)>>4)&0xf)<<4, (b)=(((data)>>8)&0xf)<<4, (a)=(((data)>>12)&0xf)<<4)

#define emuRgbGet5650f(data, r, g, b)			((r)=(((data)&0x1f)*255)/31, (g)=((((data)>>5)&0x3f)*255)/63, (b)=((((data)>>11)&0x1f)*255)/31)
#define emuRgbaGet5551f(data, r, g, b, a)		((r)=(((data)&0x1f)*255)/31, (g)=((((data)>>5)&0x1f)*255)/31, (b)=((((data)>>10)&0x1f)*255)/31, (a)=(((data)>>15)&0x1)*255)
#define emuRgbaGet4444f(data, r, g, b, a)		((r)=(((data)&0xf)*255)/15, (g)=((((data)>>4)&0xf)*255)/15, (b)=((((data)>>8)&0xf)*255)/15, (a)=((((data)>>12)&0xf)*255)/15)

static void *scegugetmemory = NULL;
unsigned int emuCurrentTexturePixelFormat = GU_PSM_8888;
unsigned int emuCurrentPalettePixelFormat = GU_PSM_8888;

void *emuCurrentPalette, *emuCurrentTextureSourceData;

const int emu_pixelFormats[] = {GL_RGB5, GL_RGB5_A1, GL_RGBA4, GL_RGBA8, 0, 0};
const int emu_pixelPhysFormats[] = {GL_UNSIGNED_SHORT_5_6_5_REV, GL_UNSIGNED_SHORT_1_5_5_5_REV, GL_UNSIGNED_SHORT_4_4_4_4_REV, GL_UNSIGNED_BYTE, 0, 0};

GLuint emuGlTexture;

int emuCurrentTexSizeX = 0;
int emuCurrentTexSizeY = 0;
int emuRealMatrix = 0;

unsigned int emuCurrentTextureCache[512][512];
unsigned int emuPaletteCache[256];
int emuTextureModulate = 0;

unsigned int emuCurrentTextureState=0, emuCurrentAmbientColor = 0xffffffff;

typedef struct
{
	float u, v;
	//Weights not supported
	u32 color, colored;
	//Normal not supported
	float x, y, z;
	
} EMU_VERTEX;

int delay = 1000/60;
int thenTicks = -1;
int nowTicks;

static unsigned int currentbuttons = 0;

int sceCtrlSetIdleCancelThreshold(int idlereset, int idleback)
{
	return 0;
}

int sceCtrlPeekBufferPositive(SceCtrlData *pad_data, int count)
{
	SDL_Event event;
	
	SDL_PollEvent(&event);
			
	if(event.type == SDL_KEYDOWN)
	{
		switch(event.key.keysym.sym)
		{
			case SDLK_LEFT:
				currentbuttons |= PSP_CTRL_LEFT;
				break;
				
			case SDLK_RIGHT:
				currentbuttons |= PSP_CTRL_RIGHT;
				break;
				
			case SDLK_UP:
				currentbuttons |= PSP_CTRL_UP;
				break;
				
			case SDLK_DOWN:
				currentbuttons |= PSP_CTRL_DOWN;
				break;
				
			case SDLK_w:
				currentbuttons |= PSP_CTRL_TRIANGLE;
				break;
				
			case SDLK_s:
				currentbuttons |= PSP_CTRL_CIRCLE;
				break;
				
			case SDLK_z:
				currentbuttons |= PSP_CTRL_CROSS;
				break;
				
			case SDLK_a:
				currentbuttons |= PSP_CTRL_SQUARE;
				break;
			
			default:
				break;
		}
	}
	
	if(event.type == SDL_KEYUP)
	{			   
		switch(event.key.keysym.sym)
		{
			case SDLK_LEFT:
				currentbuttons &= ~PSP_CTRL_LEFT;
				break;
				
			case SDLK_RIGHT:
				currentbuttons &= ~PSP_CTRL_RIGHT;
				break;
				
			case SDLK_UP:
				currentbuttons &= ~PSP_CTRL_UP;
				break;
				
			case SDLK_DOWN:
				currentbuttons &= ~PSP_CTRL_DOWN;
				break;
				
			case SDLK_w:
				currentbuttons &= ~PSP_CTRL_TRIANGLE;
				break;
				
			case SDLK_s:
				currentbuttons &= ~PSP_CTRL_CIRCLE;
				break;
				
			case SDLK_z:
				currentbuttons &= ~PSP_CTRL_CROSS;
				break;
				
			case SDLK_a:
				currentbuttons &= ~PSP_CTRL_SQUARE;
				break;
			
			default:
				break;
		}
	}
	
	pad_data->Buttons = currentbuttons;
	pad_data->Lx = 128;
	pad_data->Ly = 128;
  
	return 0;
}

int sceCtrlReadBufferPositive(SceCtrlData *pad_data, int count)
{
	pad_data->Buttons = currentbuttons;
	pad_data->Lx = 128;
	pad_data->Ly = 128;
  
	return 0;
}

int sceCtrlSetSamplingCycle(int cycle)
{
	return 0;
}

int sceCtrlSetSamplingMode(int mode)
{
	return 0;
}

int sceKernelCreateCallback(const char *name, SceKernelCallbackFunction func, void *arg)
{
	return 0;
}

int sceKernelRegisterExitCallback(int cbid)
{
	return 0;
}

int scePowerRegisterCallback(int slot, SceUID cbid)
{
	return 0;
}

int scePowerUnregisterCallback(int slot)
{
	return 0;
}

int sceKernelSleepThreadCB(void)
{
	return 0;
}

SceUID sceKernelCreateThread(const char *name, SceKernelThreadEntry entry, int initPriority, int stackSize, SceUInt attr, SceKernelThreadOptParam *option)
{
	return 0;
}

int sceKernelStartThread(SceUID thid, SceSize arglen, void *argp)
{
	return 0;
}

void sceKernelExitGame(void)
{
	SDL_Quit();
}

int sceKernelDelayThread(SceUInt delay)
{
	if(delay > 0)
	{
		SDL_Delay(delay/1000);
	}
	
	return 0;
}

int sceKernelDelayThreadCB(SceUInt delay)
{
	if(delay > 0)
	{
		SDL_Delay(delay/1000);
	}
	
	return 0;
}

SceUID sceKernelCreateEventFlag(const char *name, int attr, int bits, SceKernelEventFlagOptParam *opt)
{
	return 0;
}

int sceKernelSetEventFlag(SceUID evid, u32 bits)
{
	return 0;
}

int sceKernelClearEventFlag(SceUID evid, u32 bits)
{
	return 0;
}

int sceKernelPollEventFlag(int evid, u32 bits, u32 wait, u32 *outBits)
{
	return 0;
}

int sceKernelDeleteEventFlag(int evid)
{
	return 0;
}

int sceIoChdir(const char *path)
{
	return(chdir(path));
}

int sceUtilityLoadAvModule(int module)
{
	return 0;
}

int sceNetEtherNtostr(unsigned char *mac, char *name)
{	
	return 0;
}

int sceUtilityLoadNetModule(int module)
{
	return 0;
}

int sceNetInit(int unk1, int unk2, int unk3, int unk4, int unk5)
{
	return 0;
}

void pspSdkInetTerm()
{
	return;
}

int sceHttpAddExtraHeader(int id, char *name, char *value, int unknown1)
{
	return 0;
}

int sceHttpCreateConnectionWithURL(int templateid, const char *url, int unknown1)
{
	return 0;
}

int sceHttpCreateRequestWithURL(int connectionid, PspHttpMethod method, char *url, SceULong64 contentlength)
{
	return 0;
}

int sceHttpCreateTemplate(char *agent, int unknown1, int unknown2)
{
	return 0;
}

int sceHttpDeleteConnection(int connectionid)
{
	return 0;
}

int sceHttpDeleteRequest(int requestid)
{
	return 0;
}

int sceHttpDeleteTemplate(int templateid)
{
	return 0;
}

int sceHttpEnableKeepAlive(int id)
{
	return 0;
}

int sceHttpGetContentLength(int requestid, SceULong64 *contentlength)
{
	return 0;
}

int sceHttpGetStatusCode(int requestid, int *statuscode)
{
	return 0;
}

int sceHttpReadData(int requestid, void *data, unsigned int datasize)
{
	return 0;
}

int sceHttpSendRequest(int requestid, void *data, unsigned int datasize)
{
	return 0;
}

int sceHttpSetRecvTimeOut(int id, unsigned int timeout)
{
	return 0;
}

int sceHttpSetResolveTimeOut(int id, unsigned int timeout)
{
	return 0;
}

int sceHttpSetSendTimeOut(int id, unsigned int timeout)
{
	return 0;
}

int sceNetApctlDisconnect(void)
{
	return 0;
}

int sceNetApctlGetInfo(int code, void *pInfo)
{
	return 0;
}

int sceNetApctlInit(int stackSize, int initPriority)
{
	return 0;
}

int sceNetApctlTerm(void)
{
	return 0;
}

int sceNetInetInit(void)
{
	return 0;
}

int sceNetInetTerm(void)
{
	return 0;
}

int sceNetResolverInit(void)
{
	return 0;
}

int sceNetResolverTerm(void)
{
	return 0;
}

int sceWlanGetSwitchState(void)
{
	return 1;
}

float pgeMathSqrt(float x)
{
	return sqrtf(x);
}

float pgeMathCos(float rad)
{
	return cosf(rad);
}

float pgeMathSin(float rad)
{
	return sinf(rad);
}

float pgeMathAtan2(float y, float x)
{
	return atan2f(y, x);
}

float pgeMathAtan(float x)
{
	return atanf(x);
}

float pgeMathAcos(float x)
{
	return acosf(x);
}

float pgeMathAsin(float x)
{
	return asinf(x);
}

float pgeMathCosh(float x)
{
	return coshf(x);
}

float pgeMathExp(float x)
{
	return expf(x);
}

float pgeMathFmax(float x, float y)
{
	return fmaxf(x, y);
}

float pgeMathFmin(float x, float y)
{
	return fminf(x, y);
}

float pgeMathFmod(float x, float y)
{
	return fmodf(x, y);
}

float pgeMathLog(float x)
{
	return logf(x);
}

float pgeMathPow(float x, float y)
{
	return powf(x, y);
}

void pgeMathSrand(unsigned int x)
{
	return srand(x);
}

float pgeMathRandFloat(float min, float max)
{
	return min + (max - min) * rand() / ((float) RAND_MAX);
}

int pgeMathRandInt(float min, float max)
{
	return (int)(min + (max - min) * rand() / ((float) RAND_MAX));
}

float pgeMathSinh(float x)
{
	return sinhf(x);
}

float pgeMathTan(float x)
{
	return tanf(x);
}

float pgeMathTanh(float x)
{
	return tanhf(x);
}

float pgeMathAbs(float x)
{
	return abs(x);
}

float pgeMathCeil(float x)
{
	return ceil(x);
}

float pgeMathFloor(float x)
{
	return floor(x);
}

float pgeMathInvSqrt(float x)
{
	return 1.0f/sqrt(x);
}

float pgeMathPow2(float x)
{
	return pow(2, x);
}

float pgeMathLog2(float x)
{
	return log2(x);
}

float pgeMathLog10(float x)
{
	return log10(x);
}

float pgeMathRound(float x)
{
	return round(x);
}

float pgeMathTrunc(float x)
{
	return trunc(x);
}

void pgeMathSincos(float r, float *s, float *c)
{
	*s = sin(r);
	*c = cos(r);
}

void* pgeVramAlloc(unsigned long size)
{
	return(void*)malloc(size);
}

inline void* pgeVramAbsolutePointer(void *ptr)
{
	return (void*)ptr;
}

inline void* pgeVramRelativePointer(void *ptr)
{
	return (void*)ptr;
}

void pgeVramFree(void* ptr)
{
	return free(ptr);
}

unsigned long pgeVramAvailable()
{
	return 0;
}

void sceKernelDcacheWritebackAll(void)
{
	return;
}

int sceIoWaitAsync(SceUID fd, SceInt64 *res)
{
	return 0;
}

SceOff sceIoLseek(SceUID fd, SceOff offset, int whence)
{
	int retval = lseek(fd, offset, whence);
	
	return retval;
}

int sceIoReadAsync(SceUID fd, void *data, SceSize size)
{
	return read(fd, data, size);
}

SceUID sceIoOpen(const char *file, int flags, SceMode mode)
{	
	int f = -1;
	
	if(flags & PSP_O_CREAT)
	{
		close(open(file, O_CREAT));
		chmod(file, 0777);
	}
	
	if((flags & 0xff) == PSP_O_RDONLY)
		f = open(file, O_RDONLY);
	else if((flags & 0xff) == PSP_O_WRONLY)
		f = open(file, O_WRONLY);
	else if((flags & 0xff) == PSP_O_RDWR)
		f = open(file, O_RDWR);
		
	return (SceUID)f;
}

int sceIoClose(SceUID fd)
{
	return close(fd);
}

int sceIoRead(SceUID fd, void *data, SceSize size)
{
	return read(fd, data, size);
}

int sceIoWrite(SceUID fd, const void *data, SceSize size)
{
	return write(fd, data, size);
}

int sceIoWriteAsync(SceUID fd, const void *data, SceSize size)
{
	return write(fd, data, size);
}

int sceIoLseek32(SceUID fd, int offset, int whence)
{
	int retval = lseek(fd, offset, whence);
	
	return retval;
}

void sceGuClearColor(unsigned int color)
{
	glClearColor((GLclampf)((color & 0xff)/255.0f), (GLclampf)(((color>>8) & 0xff)/255.0f), (GLclampf)(((color>>16) & 0xff)/255.0f), (GLclampf)255.0f);
}

void sceDisplayWaitVblankStart()
{
	if(thenTicks > 0)
	{
		nowTicks = SDL_GetTicks();
		delay += (1000/60 - (nowTicks-thenTicks));
		thenTicks = nowTicks;
		
		if(delay < 0)
			delay = 1000/60;
	}
	else
	{
		thenTicks = SDL_GetTicks();
	}

	SDL_Delay(delay);
}

void sceDisplayWaitVblankStartCB()
{
	if(thenTicks > 0)
	{
		nowTicks = SDL_GetTicks();
		delay += (1000/60 - (nowTicks-thenTicks));
		thenTicks = nowTicks;
		
		if(delay < 0)
			delay = 1000/60;
	}
	else
	{
		thenTicks = SDL_GetTicks();
	}

	SDL_Delay(delay);
}

void emuSetVertexColor(unsigned int color)
{
	unsigned int col = emuCurrentAmbientColor;
	
	if(emuCurrentTextureState && emuTextureModulate)
	{
		glColor4ub((GLubyte)(color), (GLubyte)(color>>8), (GLubyte)(color>>16), (GLubyte)((color)>>24));
		return;
	}
	
	if(emuCurrentTextureState == 0)
		col = RGBA(((color&255) * (col&255)) / 255, (((color>>8)&255) * ((col>>8)&255)) / 255, (((color>>16)&255) * ((col>>16)&255)) / 255, (((color>>24)&255) * ((col>>24)&255)) / 255);
	
	glColor4ub((GLubyte)(col), (GLubyte)(col>>8), (GLubyte)(col>>16), (GLubyte)((col)>>24));
}

void sceGuClear(int flags)
{
	int glflags = 0;
	
	if(flags & GU_COLOR_BUFFER_BIT)
		glflags |= GL_COLOR_BUFFER_BIT;
		
	if(flags & GU_STENCIL_BUFFER_BIT)
		glflags |= GL_STENCIL_BUFFER_BIT;
		
	if(flags & GU_DEPTH_BUFFER_BIT)
		glflags |= GL_DEPTH_BUFFER_BIT;
	
	if(glflags != 0)
		glClear(glflags);
}

void sceGuAmbientColor(unsigned int color)
{
	emuCurrentAmbientColor = color;
	glColor4ub((GLubyte)(color), (GLubyte)(color>>8), (GLubyte)(color>>16), (GLubyte)((color)>>24));
}

void sceGuBlendFunc(int op, int src, int dest, unsigned int srcfix, unsigned int destfix)
{		
	int glsrc = -1;
	int gldest = -1;
	
	if(dest == GU_FIX)	
	{
		if((destfix & 0x00ffffff) == 0x00ffffff)
			dest = GL_ONE;
		else
			dest = GL_ZERO;
	}
	
	if(src == GU_FIX)
	{
		if((srcfix & 0x00ffffff) == 0x00ffffff)
			src = GL_ONE;
		else
			src = GL_ZERO;
	}

	if(op == GU_ADD)
	{
		switch(src)
		{
			case GU_SRC_ALPHA:
				glsrc = GL_SRC_ALPHA;
				break;
				
			default:
				break;
		}
		
		switch(dest)
		{
			case GU_ONE_MINUS_SRC_ALPHA:
				gldest = GL_ONE_MINUS_SRC_ALPHA;
				 break;
			
			default:
				break;
		}
		
		glBlendFunc(glsrc, gldest);
	}
}

void sceGuTexFunc(int tfx, int tcc)
{
	if(tfx == GU_TFX_MODULATE)
		emuTextureModulate = 1;
	else
		emuTextureModulate = 0;
}

void sceGuColorFunc(int func, unsigned int color, unsigned int mask)
{
	return;
}

void sceGuColor(unsigned int color)
{
	emuSetVertexColor(color);
}

void sceGuDepthBuffer(void* zbp, int zbw)
{
	return;
}

void sceGuDepthMask(int mask)
{
	glDepthMask(mask);
}

void sceGuDispBuffer(int width, int height, void* dispbp, int dispbw)
{
	return;
}

int sceGuDisplay(int state)
{
	return 0;
}

void sceGuDrawBuffer(int psm, void* fbp, int fbw)
{
	return;
}

void sceGuDrawBufferList(int psm, void* fbp, int fbw)
{
	return;
}

void sceGuTexFilter(int min, int mag)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
}

void sceGuScissor(int x, int y, int w, int h)
{

}

void* sceGuSwapBuffers(void)
{
	SDL_GL_SwapBuffers();
	
	return NULL;
}

int sceGuSync(int mode, int a1)
{
	glFinish();
	
	return 0;
}

void sceGuStart(int cid, void* list)
{
	
}

int sceGuFinish(void)
{	
	return 0;
}

void sceGuFrontFace(int order)
{
	if(order == GU_CCW)
	{
		glFrontFace(GL_CCW);
	}		
	else
		glFrontFace(GU_CW);
}

void sceGuOffset(unsigned int x, unsigned int y)
{
	return;
}

void sceGuShadeModel(int mode)
{

}

void sceGuInit(void)
{

}

void sceGuTerm()
{

}

void sceGuTexEnvColor(unsigned int color)
{
	return;
}

void sceGuTexOffset(float u, float v)
{
	return;
}

void sceGuTexScale(float u, float v)
{
	return;
}

void sceGuTexWrap(int u, int v)
{
	return;
}

void sceGuViewport(int cx, int cy, int width, int height)
{
	glViewport(0, 0, 480, 272);
	
	return;
}

void sceGumLoadIdentity(void)
{
	if(emuRealMatrix)
		glLoadIdentity();
}

void sceGumOrtho(float left, float right, float bottom, float top, float near, float far)
{
	gluOrtho2D(0, 480, 272, 0);
}

void sceGumPerspective(float fovy, float aspect, float near, float far)
{
	gluPerspective(fovy, aspect, near, far); // ???
}

void sceGumTranslate(const ScePspFVector3* v)
{
	glTranslatef(v->x, v->y, v->z);
}

void sceGuEnable(int state)
{	
	int glstate = 0;
	
	switch(state)
	{
		case GU_ALPHA_TEST:
			glstate = GL_ALPHA_TEST;
			break;
			
		case GU_DEPTH_TEST:
			glstate = GL_DEPTH_TEST;
			break;
			
		case GU_STENCIL_TEST:
			glstate = GL_STENCIL_TEST;
			break;
		
		case GU_BLEND:
			glstate = GL_BLEND;
			break;
			
		case GU_CULL_FACE:
			glstate = GL_CULL_FACE;
			break;
			
		case GU_DITHER:
			glstate = GL_DITHER;
			break;
			
		case GU_FOG:
			glstate = GL_FOG;
			break;
			
		case GU_TEXTURE_2D:
			emuCurrentTextureState = 1;
			glstate = GL_TEXTURE_2D;
			break;
			
		default:
			return;			
	}
	
	glEnable(glstate);
}

void sceGuDisable(int state)
{
	int glstate = 0;
	
	switch(state)
	{
		case GU_ALPHA_TEST:
			glstate = GL_ALPHA_TEST;
			break;
			
		case GU_DEPTH_TEST:
			glstate = GL_DEPTH_TEST;
			break;
			
		case GU_STENCIL_TEST:
			glstate = GL_STENCIL_TEST;
			break;
		
		case GU_BLEND:
			glstate = GL_BLEND;
			break;
			
		case GU_CULL_FACE:
			glstate = GL_CULL_FACE;
			break;
			
		case GU_DITHER:
			glstate = GL_DITHER;
			break;
			
		case GU_FOG:
			glstate = GL_FOG;
			break;
			
		case GU_TEXTURE_2D:
			emuCurrentTextureState = 0;
			glstate = GL_TEXTURE_2D;
			break;
			
		default:
			return;			
	}
	
	glDisable(glstate);
}

void sceGuClearDepth(unsigned int depth)
{
	glClearDepth(depth);
}

void sceGuDepthFunc(int function)
{
	// FIX ME!
	//glDepthFunc(function);
}

void sceGuDepthRange(int near, int far)
{
	// FIX ME!
	//glDepthRange(GLclampd zNear, GLclampd zFar);
}

void sceGuSetDither(const ScePspIMatrix4* matrix)
{
	// FIX ME!
}

void sceGumRotateXYZ(const ScePspFVector3* v)
{
	glRotatef(v->x * (180.0f/M_PI),1.0f,0.0f,0.0f);
	glRotatef(v->y * (180.0f/M_PI),0.0f,1.0f,0.0f);
	glRotatef(v->z * (180.0f/M_PI),0.0f,0.0f,1.0f);
}

void sceGumRotateZ(float angle)
{
	glRotatef(angle * (180.0f/M_PI),0.0f,0.0f,1.0f);
}

void sceGumMatrixMode(int mode)
{	
	switch(mode)
	{
		case GU_PROJECTION:
			glMatrixMode(GL_PROJECTION);
			emuRealMatrix = 1;
			break;
			
		case GU_MODEL:
			glMatrixMode(GL_MODELVIEW);
			emuRealMatrix = 1;
			break;
			
		case GU_TEXTURE:
			glMatrixMode(GL_TEXTURE);
			emuRealMatrix = 1;
			break;
			
		default:
			emuRealMatrix = 0;
			break;
	}
}

void* sceGuGetMemory(int size)
{
	if(scegugetmemory == NULL)
		scegugetmemory = malloc(2*1024*1024);
		
	return scegugetmemory;
}

void sceGumLookAt(ScePspFVector3* eye, ScePspFVector3* center, ScePspFVector3* up)
{
	gluLookAt(eye->x, eye->y, eye->z, center->x, center->y, center->z, up->x, up->y, up->z);
}

void emuConvertPaletteTexture(int width, int height, const void* tbp)
{
	unsigned char *data = (unsigned char*)tbp;
	unsigned int *tex = (unsigned int*)emuCurrentTextureCache;
	int x, y;
	
	if(emuCurrentTexturePixelFormat == GU_PSM_T4)
	{
		for(y = 0;y < height;y++)
		{
			for(x = 0;x < width;x += 2)
			{
				tex[y*width+x] = ((unsigned int*)emuCurrentPalette)[*data & 0xf];
				tex[y*width+x+1] = ((unsigned int*)emuCurrentPalette)[(*data & 0xf0) >> 4];
				data++;
			}
		}
	}
	else if (emuCurrentTexturePixelFormat == GU_PSM_T8)
	{
		for(y = 0;y < height;y++)
			for (x = 0;x < width;x++)
				tex[y*width+x] = ((unsigned int*)emuCurrentPalette)[*data++];
	}
}

int emuConvertColor(int pfDst, int pfSrc, int color)
{
	int r = 0, g = 0, b = 0, a = 0;
	
	if(pfSrc == pfDst)
		return color;
	if(pfSrc == GU_PSM_8888)
		emuRgbaGet8888(color, r, g, b, a);
	else if(pfSrc == GU_PSM_5650)
		emuRgbGet5650f(color, r, g, b), a = 0xff;
	else if(pfSrc == GU_PSM_5551)
		emuRgbaGet5551f(color, r, g, b, a);
	else if(pfSrc == GU_PSM_4444)
		emuRgbaGet4444f(color, r, g, b, a);
		
	if(pfDst == GU_PSM_8888)
		color = RGBA(r, g, b, a);
	else if(pfDst == GU_PSM_5650)
		color = RGB16(r, g, b);
	else if(pfDst == GU_PSM_5551)
		color = RGBA15(r, g, b, a);
	else if(pfDst == GU_PSM_4444)
		color = RGBA12(r, g, b, a);
	return color;
}

void sceGuClutLoad(int num_blocks, const void* cbp)
{
	if(emuCurrentPalettePixelFormat == GU_PSM_8888)
		emuCurrentPalette = (void*)cbp;
	else
	{
		int i;
		
		emuCurrentPalette = (void*)emuPaletteCache;
		
		for(i = 0;i < num_blocks<<3;i++)
			emuPaletteCache[i] = emuConvertColor(GU_PSM_8888, emuCurrentPalettePixelFormat, ((u32*)cbp)[i]);
	}
	
	if(emuCurrentTexturePixelFormat == GU_PSM_T4 || emuCurrentTexturePixelFormat == GU_PSM_T8)
	{
		sceGuTexImage(0, emuCurrentTexSizeX, emuCurrentTexSizeY, emuCurrentTexSizeX, emuCurrentTextureSourceData);
	}
}

void sceGuClutMode(unsigned int cpsm, unsigned int a1, unsigned int a2, unsigned int a3)
{
	emuCurrentPalettePixelFormat = cpsm;
}

void sceGuTexMode(int tpsm, int maxmips, int a2, int swizzle)
{
	emuCurrentTexturePixelFormat = tpsm;
}

void sceGuTexImage(int mipmap, int width, int height, int tbw, const void* tbp)
{
	unsigned int pf = emuCurrentTexturePixelFormat;
	
	emuCurrentTextureSourceData = (void*)tbp;
	
	if(emuCurrentTexturePixelFormat == GU_PSM_T4 || emuCurrentTexturePixelFormat == GU_PSM_T8)
	{
		emuConvertPaletteTexture(width, height, tbp);
		tbp = emuCurrentTextureCache;
		pf = GU_PSM_8888;
	}
	else
		pf = emuCurrentTexturePixelFormat;
	
	emuCurrentTextureSourceData = (void*)tbp;
	
	glBindTexture(GL_TEXTURE_2D, emuGlTexture);
	
	glTexImage2D(GL_TEXTURE_2D, mipmap, emu_pixelFormats[pf], width, height, 0, GL_RGBA, emu_pixelPhysFormats[pf], tbp);
		
	emuCurrentTexSizeX = width;
	emuCurrentTexSizeY = height;
}

const void *emuGetVertex(const void *vertices, int vtype, EMU_VERTEX *v)
{
	u8 *ptr = (u8*)vertices;
	u32 value, align = 1;

	value = vtype & GU_TEXTURE_BITS;
	
	if(value == GU_TEXTURE_8BIT)
	{
		v->u = (float)(*(s8*)ptr);
		ptr += 1;
		v->v = (float)(*(s8*)ptr);
		ptr += 1;
	}
	else if(value == GU_TEXTURE_16BIT)
	{
		v->u = (float)(*(s16*)ptr);
		ptr += 2;
		v->v = (float)(*(s16*)ptr);
		ptr += 2;
		
		if(align < 2)
			align = 2;
	}
	else if(value == GU_TEXTURE_32BITF)
	{
		v->u = *(float*)ptr;
		ptr += 4;
		v->v = *(float*)ptr;
		ptr += 4;
		
		if(align < 4)
			align = 4;
	}

	value = vtype & GU_COLOR_BITS;
	v->colored = value;
	
	/*if(value == GU_COLOR_5650)
	 {
		ptr += 2;
	 
		if(align < 2)
			align = 2;
	}
	else if (value == GU_COLOR_5551)
	{
		ptr += 2;
	 
		if(align < 2)
			align = 2;
	}
	else if(value == GU_COLOR_4444)	
	{
		ptr += 2;
	 
		if(align < 2)
			align = 2;
	}
	else*/ if (value == GU_COLOR_8888)
	{
		v->color = (unsigned int)(*(unsigned int*)ptr);
		ptr += 4;
		
		if(align < 4)
			align = 4;
	}

	value = vtype & GU_VERTEX_BITS;

	if(value == GU_VERTEX_8BIT)
	{
		v->x = (float)(*(s8*)ptr);
		ptr += 1;
		v->y = (float)(*(s8*)ptr);
		ptr += 1;
		v->z = (float)(*(s8*)ptr);
		ptr += 1;
	}
	else if (value == GU_VERTEX_16BIT)
	{
		v->x = (float)(*(s16*)ptr);
		ptr += 2;
		v->y = (float)(*(s16*)ptr);
		ptr += 2;
		v->z = (float)(*(s16*)ptr);
		ptr += 2;
		
		if(align < 2)
			align = 2;
	}
	else if(value == GU_VERTEX_32BITF)
	{
		v->x = (float)(*(float*)ptr);
		ptr += 4;
		v->y = (float)(*(float*)ptr);
		ptr += 4;
		v->z = (float)(*(float*)ptr);
		ptr += 4;
		
		if(align < 4)
			align = 4;
	}
	
	v->u /= emuCurrentTexSizeX;
	v->v /= emuCurrentTexSizeY;
	
	if(vtype & GU_TRANSFORM_2D)
	{
		sceGumMatrixMode(GU_PROJECTION);
		sceGumLoadIdentity();
		sceGumOrtho(0.0f, 480.0f, 272.0f, 0.0f, -1.0f, 1.0f);
	
		sceGumMatrixMode(GU_VIEW);
		sceGumLoadIdentity();
	
		sceGumMatrixMode(GU_MODEL);
		sceGumLoadIdentity();
	}

	value = (unsigned int)ptr - (unsigned int)vertices;
	
	if(value % align)
		ptr += align - (value % align);
		
	return ptr;
}

void sceGuDrawArray(int prim, int vtype, int count, const void* indices, const void* vertices)
{	
	EMU_VERTEX *v = (EMU_VERTEX*)malloc(sizeof(EMU_VERTEX) * count);
	
	if(v == NULL)
		printf("Error allocating\n");
	
	if(prim == GU_SPRITES)
	{
		while(count > 0)
		{
			vertices = emuGetVertex(vertices, vtype, &v[0]);
			vertices = emuGetVertex(vertices, vtype, &v[1]);
			
			glBegin(GL_QUADS);
				if(v[0].colored)
					emuSetVertexColor(v[0].color);
				 
				 glTexCoord2f(v[0].u, v[0].v); glVertex3f(v[0].x, v[0].y, 0); //1
				 glTexCoord2f(v[0].u, v[1].v); glVertex3f(v[0].x, v[1].y, 0); //4
				 glTexCoord2f(v[1].u, v[1].v); glVertex3f(v[1].x, v[1].y, 0); //3
				 glTexCoord2f(v[1].u, v[0].v); glVertex3f(v[1].x, v[0].y, 0); //2
			glEnd();
		
			count -= 2;
		}
	}
	else if(prim == GU_TRIANGLE_STRIP)
	{
		while(count > 0)
		{
			vertices = emuGetVertex(vertices, vtype, &v[0]);
			vertices = emuGetVertex(vertices, vtype, &v[1]);
			vertices = emuGetVertex(vertices, vtype, &v[2]);
			vertices = emuGetVertex(vertices, vtype, &v[3]);
			glBegin(GL_TRIANGLE_STRIP);
				if(v[0].colored)
					emuSetVertexColor(v[0].color);
				glTexCoord2f(v[0].u, v[0].v); glVertex3f(v[0].x, v[0].y, v[0].z);
				if(v[1].colored)
					emuSetVertexColor(v[1].color);
				glTexCoord2f(v[1].u, v[1].v); glVertex3f(v[1].x, v[1].y, v[1].z);
				if(v[2].colored)
					emuSetVertexColor(v[2].color);
				glTexCoord2f(v[2].u, v[2].v); glVertex3f(v[2].x, v[2].y, v[2].z);
				if(v[3].colored)
					emuSetVertexColor(v[3].color);
				glTexCoord2f(v[3].u, v[3].v); glVertex3f(v[3].x, v[3].y, v[3].z);
			glEnd();
			count -= 4;
		}
	}
	else if(prim == GU_LINES)
	{
		glBegin(GL_LINES);
		
		while(count > 0)
		{
			vertices = emuGetVertex(vertices, vtype, &v[0]);
			vertices = emuGetVertex(vertices, vtype, &v[1]);
			
			if(v[1].colored)
				emuSetVertexColor(v[1].color);
			
			glVertex3f(v[1].x, v[1].y, v[1].z);
			
			if(v[0].colored)
				emuSetVertexColor(v[0].color);
			
			glVertex3f(v[0].x, v[0].y, v[0].z);
			
			count -= 2;
		}
		glEnd();
	}
	else if(prim == GU_LINE_STRIP)
	{
		int i = 0;
		
		glBegin(GL_LINE_STRIP);
		
		for(i = 0;i < count;i++)
		{
			vertices = emuGetVertex(vertices, vtype, &v[i]);
			
			if(v[i].colored)
				emuSetVertexColor(v[i].color);
				
			glTexCoord2f(v[i].u, v[i].v); glVertex3f(v[i].x, v[i].y, 0);
		}
		
		glEnd();
	}
	else if(prim == GU_TRIANGLE_FAN)
	{
		int i = 0;
		
		glBegin(GL_TRIANGLE_FAN);
		
		for(i=0;i < count;i++)
		{
			vertices = emuGetVertex(vertices, vtype, &v[i]);
			
			if(v[i].colored)
				emuSetVertexColor(v[i].color);
				
			glTexCoord2f(v[i].u, v[i].v); glVertex3f(v[i].x, v[i].y, 0);
		}
		
		glEnd();
	}
	
	free(v);
}

void sceGumDrawArray(int prim, int vtype, int count, const void* indices, const void* vertices)
{
	sceGuDrawArray(prim, vtype, count, indices, vertices);
}

void sceGumPopMatrix(void)
{
	glPopMatrix();
}

void sceGumPushMatrix(void)
{
	glPushMatrix();
}

int sceNetAdhocInit(void)
{
	return 0;
}

int sceNetAdhocTerm(void)
{
	return 0;
}

int sceNetAdhocPtpOpen(unsigned char *srcmac, unsigned short srcport, unsigned char *destmac, unsigned short destport, unsigned int bufsize, unsigned int delay, int count, int unk1)
{
	return 0;
}

int sceNetAdhocPtpConnect(int id, unsigned int timeout, int unk1)
{
	return 0;
}

int sceNetAdhocPtpListen(unsigned char *srcmac, unsigned short srcport, unsigned int bufsize, unsigned int delay, int count, int queue, int unk2)
{
	return 0;
}

int sceNetAdhocPtpAccept(int id, unsigned char *mac, unsigned short *port, unsigned int timeout, int unk1)
{
	return 0;
}

int sceNetAdhocPtpSend(int id, void *data, int *datasize, unsigned int timeout, int unk1)
{
	return 0;
}

int sceNetAdhocPtpRecv(int id, void *data, int *datasize, unsigned int timeout, int unk1)
{
	return 0;
}

int sceNetAdhocPtpFlush(int id, unsigned int timeout, int unk1)
{
	return 0;
}

int sceNetAdhocPtpClose(int id, int unk1)
{
	return 0;
}

int sceNetAdhocMatchingInit(int memsize)
{
	return 0;
}

int sceNetAdhocMatchingTerm(void)
{
	return 0;
}

int sceNetAdhocMatchingCreate(int mode, int maxpeers, unsigned short port, int bufsize, unsigned int hellodelay, unsigned int pingdelay, int initcount, unsigned int msgdelay, pspAdhocMatchingCallback callback)
{
	return 0;
}

int sceNetAdhocMatchingDelete(int matchingid)
{
	return 0;
}

int sceNetAdhocMatchingStart(int matchingid, int evthpri, int evthstack, int inthpri, int inthstack, int optlen, void *optdata)
{
	return 0;
}

int sceNetAdhocMatchingStop(int matchingid)
{
	return 0;
}

int sceNetAdhocMatchingSelectTarget(int matchingid, unsigned char *mac, int optlen, void *optdata)
{
	return 0;
}

int sceNetAdhocMatchingCancelTarget(int matchingid, unsigned char *mac)
{
	return 0;
}

int sceNetAdhocctlInit(int unk1, int unk2, struct productStruct *product)
{
	return 0;
}

int sceNetAdhocctlTerm(void)
{
	return 0;
}

int sceNetAdhocctlConnect(const char *name)
{
	return 0;
}

int sceNetAdhocctlDisconnect(void)
{
	return 0;
}

int sceNetAdhocctlGetState(int *event)
{
	return 0;
}

int sceNetAdhocctlCreate(const char *name)
{
	return 0;
}

int sceNetAdhocctlJoin(struct SceNetAdhocctlScanInfo *scaninfo)
{
	return 0;
}

int sceNetAdhocctlCreateEnterGameMode(const char *name, int unknown, int num, unsigned char *macs, unsigned int timeout, int unknown2)
{
	return 0;
}

int sceNetAdhocctlJoinEnterGameMode(const char *name, unsigned char *hostmac, unsigned int timeout, int unknown)
{
	return 0;
}

int sceNetAdhocctlGetPeerList(int *length, void *buf)
{
	return 0;
}

int sceNetAdhocctlGetPeerInfo(unsigned char *mac, int size, struct SceNetAdhocctlPeerInfo *peerinfo)
{
	return 0;
}

int sceNetAdhocctlScan(void)
{
	return 0;
}

int sceNetAdhocctlGetScanInfo(int *length, void *buf)
{
	return 0;
}

int sceNetAdhocctlAddHandler(sceNetAdhocctlHandler handler, void *unknown)
{
	return 0;
}

int sceNetAdhocctlDelHandler(int id)
{
	return 0;
}

int sceNetAdhocctlGetNameByAddr(unsigned char *mac, char *nickname)
{
	return 0;
}

int sceNetGetLocalEtherAddr(unsigned char *mac)
{
	return 0;
}

int sceNetTerm(void)
{
	return 0;
}

int sceUtilityUnloadNetModule(int module)
{
	return 0;
}

int sceRtcGetCurrentTick(u64 *tick)
{
	*tick = (u64)clock();
	
	return 0;
}
 
u32 sceRtcGetTickResolution()
{
	return CLOCKS_PER_SEC;
}

int sceAudioChReserve(int channel, int samplecount, int format)
{
	return 0;
}

int sceAudioChRelease(int channel)
{
	return 0;
}

int sceAudioOutputPannedBlocking(int channel, int leftvol, int rightvol, void *buffer)
{
	return 0;
}

int sceKernelDeleteThread(SceUID thid)
{
	return 0;
}

int sceKernelExitThread(int status)
{
	return 0;
}

int sceKernelSleepThread(void)
{
	return 0;
}

int sceKernelWakeupThread(SceUID thid)
{
	return 0;
}

int sceAudioOutputBlocking(int channel, int vol, void *buf)
{
	return 0;
}

int sceAudiocodecCheckNeedMem(unsigned long *Buffer, int Type)
{
	return 0;
}

int sceAudiocodecInit(unsigned long *Buffer, int Type)
{
	return 0;
}

int sceAudiocodecDecode(unsigned long *Buffer, int Type)
{
	return 0;
}

int sceAudiocodecGetEDRAM(unsigned long *Buffer, int Type)
{
	return 0;
}

int sceAudiocodecReleaseEDRAM(unsigned long *Buffer)
{
	return 0;
}

int sceIoChangeAsyncPriority(SceUID fd, int pri)
{
	return 0;
}

int sceKernelExitDeleteThread(int status)
{
	return 0;
}

void *memalign(size_t align, size_t size)
{
	return malloc(size);
}

int sceHttpInit(unsigned int unknown1)
{
	return 0;
}

int sceHttpEnd(void)
{
	return 0;
}

int sceKernelAllocateVpl(SceUID uid, unsigned int size, void **data, unsigned int *timeout)
{
	return 0;
}

SceUID sceKernelCreateVpl(const char *name, int part, int attr, unsigned int size, struct SceKernelVplOptParam *opt)
{
	return 0;
}

int sceKernelDeleteVpl(SceUID uid)
{
	return 0;
}

int sceKernelFreeVpl(SceUID uid, void *data)
{
	return 0;
}

int sceUtilityGameSharingInitStart(pspUtilityGameSharingParams *params)
{
	return 0;
}

void sceUtilityGameSharingShutdownStart(void)
{
	return;
}

int sceUtilityGameSharingGetStatus(void)
{
	return 4;
}

void sceUtilityGameSharingUpdate(int n)
{
	return;
}

int sceUtilityHtmlViewerInitStart(pspUtilityHtmlViewerParam *params)
{
	return 0;
}

int sceUtilityHtmlViewerShutdownStart(void)
{
	return 0;
}

int sceUtilityHtmlViewerUpdate(int n)
{
	return 0;
}

int sceUtilityHtmlViewerGetStatus(void)
{
	return 4;
}

int sceUtilityMsgDialogInitStart(pspUtilityMsgDialogParams *params)
{
	return 0;
}

void sceUtilityMsgDialogShutdownStart(void)
{
	return;
}

int sceUtilityMsgDialogGetStatus(void)
{
	return 4;
}

void sceUtilityMsgDialogUpdate(int n)
{
	return;
}

int sceUtilityMsgDialogAbort(void)
{
	return 0;
}

int sceUtilityNetconfInitStart (pspUtilityNetconfData *data)
{
	return 0;
}

int sceUtilityNetconfShutdownStart (void)
{
	return 0;
}

int sceUtilityNetconfUpdate (int unknown)
{
	return 0;
}

int sceUtilityNetconfGetStatus (void)
{
	return 4;
}

int sceUtilitySavedataInitStart(SceUtilitySavedataParam * params)
{
	return 0;
}

int sceUtilitySavedataGetStatus(void)
{
	return 4;
}

int sceUtilitySavedataShutdownStart(void)
{
	return 0;
}

void sceUtilitySavedataUpdate(int unknown)
{
	return;
}

int sceAudioSRCChReserve(int samplecount, int freq, int channels)
{
	return 0;
}

int sceAudioSRCChRelease(void)
{
	return 0;
}

int sceAudioSRCOutputBlocking(int vol, void *buf)
{
	return 0;
}

int sceDisplaySetFrameBuf(void *topaddr, int bufferwidth, int pixelformat, int sync)
{
	return 0;
}

int sceDisplaySetMode(int mode, int width, int height)
{
	return 0;
}

void * sceGeEdramGetAddr(void)
{
	return NULL;
}

int sceUtilityOskInitStart(SceUtilityOskParams* params)
{
	return 0;
}

int sceUtilityOskShutdownStart(void)
{
	return 0;
}

int sceUtilityOskUpdate(int n)
{
	return 0;
}

int sceUtilityOskGetStatus(void)
{
	return 0;
}

int sceIoRemove(const char *file)
{
	return remove(file);
}

int sceIoMkdir(const char *dir, SceMode mode)
{
	return mkdir(dir, 0777);
}

int sceIoRename(const char *oldname, const char *newname)
{
	return rename(oldname, newname);
}

int sceIoRmdir(const char *path)
{
	return rmdir(path);
}

void sceKernelDcacheWritebackInvalidateAll(void)
{

}

SceUID sceIoDopen(const char *dirname)
{
	DIR* dir = NULL;
	
	if(dirname && (strlen(dirname) == 0))
	{
		dir = opendir(".");
	}
	else
	{
		dir = opendir(dirname);
	}
	
	if(NULL != dir)
	{
		return (SceUID) dir;
	}
	
	return -1;
}


int sceIoDread(SceUID fd, SceIoDirent *dir)
{
	struct stat statbuf;
	
	struct dirent* ent = readdir((DIR*)fd);
	
	int result;
	
	if(NULL != ent)
	{
		strcpy(dir->d_name, ent->d_name);
		result = stat(dir->d_name, &statbuf);
		
		if(result != 0)
		{
			printf("Error: %d, Name: %s\n", errno, dir->d_name);
		}
			
		dir->d_stat.st_size = statbuf.st_size;
		dir->d_stat.st_attr = 0;
		
		if(S_ISDIR(statbuf.st_mode))
		{
			dir->d_stat.st_attr |= FIO_SO_IFDIR;
		}
		
		if(S_ISLNK(statbuf.st_mode))
		{
			dir->d_stat.st_attr |= FIO_SO_IFLNK;
		}
		
		if(S_ISREG(statbuf.st_mode))
		{
			dir->d_stat.st_attr |= FIO_SO_IFREG;
		}
		
		// TODO: Read, Write, Execute flags mapping
		
		return 1;
	}
	
	return -1;
}


int sceIoDclose(SceUID fd)
{
	if(0 == closedir((DIR*)fd))
		return 0;
		
	return -1;
}

int sceOpenPSIDGetOpenPSID(PspOpenPSID *openpsid)
{
	memset(&openpsid, 0xFF, 16);
	
	return 0;
}

int sceUtilityGetSystemParamString(int id, char *str, int len)
{
	if(id == PSP_SYSTEMPARAM_ID_STRING_NICKNAME)
		memcpy(str, "nickname", 8);
	
	return 0;
}

int sceUtilityGetSystemParamInt(int id, int *value)
{	
	if(id == PSP_SYSTEMPARAM_ID_INT_LANGUAGE)
		*value = 1;
	if(id == PSP_SYSTEMPARAM_ID_INT_UNKNOWN)
		*value = 1;
		
	return 0;
}

int sceWlanGetEtherAddr(u8 *etherAddr)
{
	u8 mac[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	
	memcpy(etherAddr, &mac, 6);
	
	return 0;
}

int sceIoGetstat(const char *file, SceIoStat *stat)
{
	/*struct stat statbuf;
	
	SceIoStat *pspstat = stat;
	
	(void)stat;
	
	memset(stat, '\0', sizeof(SceIoStat));
	
	stat(file, &statbuf);
	
	stat->st_ctime = statbuf->st_ctime;
	stat->st_atime = statbuf->st_atime;
	stat->st_mtime = statbuf->st_mtime;
	
	stat->st_mode = (statbuf->st_mode & 0xfff) |
			((FIO_S_ISLNK(statbuf->st_mode))?(S_IFLNK):(0)) |
			((FIO_S_ISREG(statbuf->st_mode))?(S_IFREG):(0)) |
			((FIO_S_ISDIR(statbuf->st_mode))?(S_IFDIR):(0));
			
	stat->st_size = statbuf->st_size;*/
			
	return 0;
}

SceUID kuKernelLoadModule(const char *path, int flags, SceKernelLMOption *option)
{
	return 1;
}

int sceIoDevctl(const char *dev, unsigned int cmd, void *indata, int inlen, void *outdata, int outlen)
{
	return 0;
}

SceUID sceKernelLoadModule(const char *path, int flags, SceKernelLMOption *option)
{
	return 1;
}

int sceKernelStartModule(SceUID modid, SceSize argsize, void *argp, int *status, SceKernelSMOption *option)
{
	return 0;
}

int sceUsbActivate(u32 pid)
{
	return 0;
}

int sceUsbDeactivate(u32 pid)
{
	return 0;
}

int sceUsbGetState(void)
{
	return PSP_USB_ACTIVATED|PSP_USB_CABLE_CONNECTED|PSP_USB_CONNECTION_ESTABLISHED;
}

int sceUsbStart(const char* driverName, int size, void *args)
{
	return 0;
}

int sceUsbStop(const char* driverName, int size, void *args)
{
	return 0;
}

int sceUsbstorBootSetCapacity(u32 size)
{
	return 0;
}

int pgeSupportLoadStartModule(const char *path)
{
	return 0;
}

int pgeSupportStopUnloadModule(SceUID modid)
{
	return 0;
}