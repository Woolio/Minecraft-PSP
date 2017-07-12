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
#include <pspdisplay.h>
#include <pspgu.h>
#include <pspgum.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pspge.h>
#include <stdarg.h>
#include <stdlib.h>
#include <png.h>

#include "pgeGfx.h"
#include "pgeVram.h"
#include "pgeMath.h"

#ifndef __PSP__
	#include "SDL.h"
	#include "SDL_opengl.h"
	GLuint emuGlTexture;
	SDL_Surface* drawContext;
#endif

#define RGBA8888(col,r,g,b,a)	{ \
		a=(col>>24)&0xFF;\
		b=(col>>16)&0xFF;\
		g=(col>>8)&0xFF;\
		r=(col&0xFF);\
		}
		
#define A(color) ((u8)(color >> 24 & 0xFF))
#define B(color) ((u8)(color >> 16 & 0xFF))
#define G(color) ((u8)(color >> 8 & 0xFF))
#define R(color) ((u8)(color & 0xFF))

#define PGE_SCREEN_WIDTH	480
#define PGE_SCREEN_HEIGHT	272

#define PGE_FRAME_BUFFER_WIDTH	512
#define PGE_FRAME_BUFFER_SIZE	(PGE_FRAME_BUFFER_WIDTH*PGE_SCREEN_HEIGHT)

static unsigned int __attribute__((aligned(64))) pgeGfxDisplayList[2][262144/4];
static unsigned int pgeGfxDisplayListId = 0;

static void* pgeGfxFrontbuffer = NULL;
static void* pgeGfxDepthbuffer = NULL;
static void* pgeGfxBackbuffer = NULL;
static void* pgeGfxFramebuffer = NULL;
static unsigned int pgeGfxInitialized = 0;
static unsigned int pgeGfxDisplayListAvail = 0;
static unsigned int pgeGfxPsm = PGE_PIXEL_FORMAT_8888;
static unsigned int pgeGfxBpp = 4;
static int pgeGfxBlendMode = PGE_BLEND_MODE_REGULAR;

static const ScePspIMatrix4 pgeGfxDitherMatrix =	{{0,  8,  2, 10},
													{12,  4, 14,  6},
													{3,  11,  1,  9},
													{15,  7, 13,  5}};
				      
unsigned int *pgeGfxGetFramebuffer(void)
{
	return (unsigned int *)pgeGfxFramebuffer;
}

int pgeGfxInit(enum pgePixelFormat pixelformat)
{
	if (pgeGfxInitialized != 0)
		return 0;

	pgeGfxInitialized = 1;
	
	#ifndef __PSP__
	int error;
    
    error = SDL_Init(SDL_INIT_VIDEO);
    
    // Create a double-buffered draw context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
                        
    Uint32 flags;
    flags = SDL_OPENGL;
    drawContext = SDL_SetVideoMode(480, 272, 0, flags);
	
	glGenTextures(1, &emuGlTexture);
	glBindTexture(GL_TEXTURE_2D, emuGlTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	SDL_WM_SetCaption("Phoenix Game Engine", NULL);
	#endif

	switch(pixelformat)
	{
		case PGE_PIXEL_FORMAT_4444:
		case PGE_PIXEL_FORMAT_5650:
		case PGE_PIXEL_FORMAT_5551:
			pgeGfxFrontbuffer = pgeVramRelativePointer(pgeVramAlloc(PGE_FRAME_BUFFER_SIZE<<1));
			pgeGfxBackbuffer = pgeVramRelativePointer(pgeVramAlloc(PGE_FRAME_BUFFER_SIZE<<1));
			pgeGfxDepthbuffer = pgeVramRelativePointer(pgeVramAlloc(PGE_FRAME_BUFFER_SIZE<<1));
			pgeGfxBpp = 2;
			pgeGfxPsm = pixelformat;
			break;
		default:
			pgeGfxFrontbuffer = pgeVramRelativePointer(pgeVramAlloc(PGE_FRAME_BUFFER_SIZE<<2));
			pgeGfxBackbuffer = pgeVramRelativePointer(pgeVramAlloc(PGE_FRAME_BUFFER_SIZE<<2));
			pgeGfxDepthbuffer = pgeVramRelativePointer(pgeVramAlloc(PGE_FRAME_BUFFER_SIZE<<1));
			pgeGfxBpp = 4;
			pgeGfxPsm = PGE_PIXEL_FORMAT_8888;
			break;
	}

	pgeGfxFramebuffer = pgeVramAbsolutePointer(pgeGfxFrontbuffer);

	sceGuInit();

	// setup GU
	sceGuStart(GU_DIRECT, pgeGfxDisplayList[pgeGfxDisplayListId]);
	sceGuDrawBuffer(pgeGfxPsm, pgeGfxFrontbuffer, PGE_FRAME_BUFFER_WIDTH);
	sceGuDispBuffer(512, 512, pgeGfxBackbuffer, PGE_FRAME_BUFFER_WIDTH);
	sceGuDepthBuffer(pgeGfxDepthbuffer, PGE_FRAME_BUFFER_WIDTH);

	sceGuOffset(2048 - (PGE_SCREEN_WIDTH>>1), 2048 - (PGE_SCREEN_HEIGHT>>1));
	sceGuViewport(2048, 2048, PGE_SCREEN_WIDTH, PGE_SCREEN_HEIGHT);

	// Scissoring
	sceGuScissor(0, 0, PGE_SCREEN_WIDTH, PGE_SCREEN_HEIGHT);
	sceGuEnable(GU_SCISSOR_TEST);

	// Backface culling
	sceGuFrontFace(GU_CCW);
	sceGuEnable(GU_CULL_FACE);

	// Depth test
	sceGuDepthRange(65535, 0);
	sceGuDepthFunc(GU_GEQUAL);
	sceGuDisable(GU_DEPTH_TEST);
	//sceGuDepthMask(GU_TRUE);		// disable z-writes

	sceGuEnable(GU_CLIP_PLANES);

	// Texturing
	sceGuEnable(GU_TEXTURE_2D);
	sceGuShadeModel(GU_SMOOTH);
	sceGuTexWrap(GU_REPEAT, GU_REPEAT);
	sceGuTexFilter(GU_LINEAR,GU_LINEAR);
	sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
	sceGuTexEnvColor(0xFFFFFFFF);
	sceGuColor(0xFFFFFFFF);
	sceGuAmbientColor(0xFFFFFFFF);
	sceGuTexOffset(0.0f, 0.0f);
	sceGuTexScale(1.0f, 1.0f);
	
	// Blending
	sceGuEnable(GU_BLEND);
	sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

	if(pgeGfxBpp < 4)
	{
		sceGuSetDither(&pgeGfxDitherMatrix);
		sceGuEnable(GU_DITHER);
	}

	// Projection
	sceGumMatrixMode(GU_PROJECTION);
	sceGumLoadIdentity();
	sceGumPerspective(60.0f, 480.0f/272.0f, 1.0f, 1000.0f);
	
	sceGumMatrixMode(GU_VIEW);
	sceGumLoadIdentity();
	
	sceGumMatrixMode(GU_MODEL);
	sceGumLoadIdentity();
	
	sceGuClearColor(0x0);
	sceGuClear(GU_COLOR_BUFFER_BIT);
	
	sceGuFinish();
	sceGuSync(0,0);

	sceDisplayWaitVblankStartCB();
	pgeGfxFramebuffer = pgeVramAbsolutePointer(sceGuSwapBuffers());
	sceGuDisplay(1);
	
	return 1;
}

/*void pgeGfxSetMode2D(void)
{
	sceGumMatrixMode(GU_PROJECTION);
	sceGumLoadIdentity();
	sceGumOrtho(0.0f, 480.0f, 272.0f, 0.0f, -1.0f, 1.0f);
	
	sceGumMatrixMode(GU_VIEW);
	sceGumLoadIdentity();
	
	sceGumMatrixMode(GU_MODEL);
	sceGumLoadIdentity();
}

void pgeGfxSetMode3D(float fov)
{
	sceGumMatrixMode(GU_PROJECTION);
	sceGumLoadIdentity();
	sceGumPerspective(fov, 480.0f/272.0f, 1.0f, 1000.0f);
	
	sceGumMatrixMode(GU_VIEW);
	sceGumLoadIdentity();
	
	sceGumMatrixMode(GU_MODEL);
	sceGumLoadIdentity();
}*/

int currentmatrix;

void pgeGfxLoadIdentity(void)
{
	sceGumLoadIdentity();
}

void pgeGfxMatrixMode(int mode)
{
	sceGumMatrixMode(mode);
	currentmatrix = mode;
}

static float pgeGfxNear = 0.0f;
static float pgeGfxFar = 0.0f;

void pgeGfxPerspective(float fovy, float aspect, float near, float far)
{
	sceGumPerspective(fovy, aspect, near, far);
	
	pgeGfxNear = near;
	pgeGfxFar = far;
}

void pgeGfxLookAt(ScePspFVector3* eye, ScePspFVector3* center, ScePspFVector3* up)
{
	sceGumLookAt(eye, center, up);
}

void pgeGfxRotateX(float angle)
{
	sceGumRotateX(angle);
}

void pgeGfxRotateY(float angle)
{
	sceGumRotateY(angle);
}

/*void pgeGfxSetMode3DCamera(float fov, pgeCamera *cam)
{
	sceGumMatrixMode(GU_PROJECTION);
	sceGumLoadIdentity();
	sceGumPerspective(fov, 480.0f/272.0f, 1.0f, 1000.0f);
	
	sceGumMatrixMode(GU_VIEW);
	sceGumLoadIdentity();
	sceGumLookAt(&cam->eye, &cam->center, &cam->up);
	
	sceGumStoreMatrix(&view);
	
	printf("x.x = %f, x.y = %f, x.z = %f, x.w = %f,"
			"y.x = %f, y.y = %f, y.z = %f, y.w = %f,"
			"z.x = %f, z.y = %f, z.z = %f, z.w = %f,"
			"w.x = %f, w.y = %f, w.z = %f, w.w = %f\n", view.x.x, view.x.y, view.x.z, view.x.w, view.y.x, view.y.y, view.y.z, view.y.w, view.z.x, view.z.y, view.z.z, view.z.w, view.w.x, view.w.y, view.w.z, view.w.w);
	
	sceGumMatrixMode(GU_MODEL);
	sceGumLoadIdentity();
}*/

int pgeGfxGetMatrix(ScePspFMatrix4 *m, int mode)
{	
	sceGumMatrixMode(mode);
	sceGumStoreMatrix(m);
	
	sceGumMatrixMode(currentmatrix);
	
	return 1;
}

int pgeGfxProject(ScePspFVector3 *object, ScePspFVector3 *screen)
{ 
    ScePspFMatrix4 m, proj, view, model; 
    float w;
	
	pgeGfxGetMatrix(&proj, PGE_GFX_PROJECTION);
	pgeGfxGetMatrix(&view, PGE_GFX_VIEW);
	pgeGfxGetMatrix(&model, PGE_GFX_MODEL);
	
    gumMultMatrix(&m, &proj, &view); 
    gumMultMatrix(&m, &m, &model); 
	
    screen->x = object->x * m.x.x + object->y * m.y.x + object->z * m.z.x + m.w.x; 
    screen->y = object->x * m.x.y + object->y * m.y.y + object->z * m.z.y + m.w.y; 
    screen->z = object->x * m.x.z + object->y * m.y.z + object->z * m.z.z + m.w.z; 
            w = object->x * m.x.w + object->y * m.y.w + object->z * m.z.w + m.w.w; 

    if(w == 0)
		return 0;
		
    screen->x /= w; 
    screen->y /= w; 
    screen->z /= w; 
 
    screen->x = screen->x * 0.5f + 0.5f; 
    screen->y = screen->y * 0.5f + 0.5f;
	//screen->z = screen->z * 0.5f + 0.5f;
	
    screen->x = screen->x * 480.0f;
    screen->y = (1.0f - screen->y) * 272.0f;
	
    return 1; 
}

int pgeGfxUnproject(ScePspFVector3 *screen, ScePspFVector3 *object)
{
    screen->x = screen->x / 480.f;
    screen->y = (272.0f - screen->y) / 272.f;
	screen->x = screen->x * 2 - 1;
	screen->y = screen->y * 2 - 1;
	
    ScePspFMatrix4 m, proj, view, model; 
    float w;
	
	pgeGfxGetMatrix(&proj, PGE_GFX_PROJECTION);
	pgeGfxGetMatrix(&view, PGE_GFX_VIEW);
	pgeGfxGetMatrix(&model, PGE_GFX_MODEL);
	
	gumMultMatrix(&m, &proj, &view); 
    gumMultMatrix(&m, &m, &model); 
	
	gumFullInverse(&m, &m);

    object->x = screen->x * m.x.x + screen->y * m.y.x + screen->z * m.z.x + m.w.x; 
    object->y = screen->x * m.x.y + screen->y * m.y.y + screen->z * m.z.y + m.w.y; 
    object->z = screen->x * m.x.z + screen->y * m.y.z + screen->z * m.z.z + m.w.z; 
            w = screen->x * m.x.w + screen->y * m.y.w + screen->z * m.z.w + m.w.w; 

    if(w == 0)
		return 0;

    object->x /= w; 
    object->y /= w; 
    object->z /= w; 
	
    return 1; 
}

void pgeGfxShutdown(void)
{
	if (!pgeGfxInitialized)
		return;
		
	pgeGfxStartDrawing();

	pgeVramFree(pgeVramAbsolutePointer(pgeGfxBackbuffer));
	pgeVramFree(pgeVramAbsolutePointer(pgeGfxFrontbuffer));
	pgeGfxInitialized = 0;
	pgeGfxDisplayListAvail = 0;
	sceGuTerm();
}

void pgeGfxStartDrawing(void)
{	
	if(pgeGfxDisplayListAvail)
		return;

	sceGuStart(GU_DIRECT, pgeGfxDisplayList[pgeGfxDisplayListId]);
	pgeGfxDisplayListId ^= 1;
	pgeGfxDisplayListAvail = 1;
}

void pgeGfxEndDrawing(void)
{
	if(!pgeGfxDisplayListAvail)
		return;
	
	pgeGfxDisplayListAvail = 0;
	sceGuFinish();
	sceGuSync(0,0);
}

void pgeGfxSwapBuffers(enum pgeWaitVsync vsync)
{	
	if(vsync)
		sceDisplayWaitVblankStart();

	pgeGfxFramebuffer = pgeVramAbsolutePointer(sceGuSwapBuffers());
}

int pgeGfxSetBlendMode(enum pgeBlendMode mode)
{
	int oldmode = pgeGfxBlendMode;
	
	if(mode == PGE_BLEND_MODE_TRANSPARENT)
	{
		sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_FIX, 0xFFFFFFFF, 0xFFFFFFFF);
		//sceGuDepthMask(GU_TRUE);
	}
	else
	{
		sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
		sceGuDepthMask(GU_FALSE);
	}
	
	pgeGfxBlendMode = mode;
	
	return oldmode;
}

void pgeGfxClearScreen(unsigned int color)
{
	pgeGfxStartDrawing();

	sceGuClearColor(color);
	sceGuClearDepth(0);
	sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT|GU_FAST_CLEAR_BIT);
}

unsigned int pgeGfxCreateColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return a << 24 | b << 16 | g << 8 | r;
}

void pgeGfxRenderToTexture(pgeTexture *texture)
{
	if(texture->location != PGE_VRAM)
		return;
	
	pgeGfxStartDrawing();
	
	//sceGuSendCommandi(211,((GU_COLOR_BUFFER_BIT|GU_STENCIL_BUFFER_BIT) << 8)|0x1);
	
	sceGuSync(0, 2);
	
	sceKernelDcacheWritebackAll();

	sceGuDrawBufferList(texture->format, pgeVramRelativePointer(texture->data), texture->textureWidth);
	
	sceGuOffset(2048 - (texture->width>>1), 2048 - (texture->height>>1));
	sceGuViewport(2048, 2048, texture->width, texture->height);
	sceGuScissor(0, 0, texture->width, texture->height);
}

void pgeGfxRenderToScreen()
{	
	pgeGfxStartDrawing();
	
	//sceGuSendCommandi(211,0);
	
	sceGuSync(0, 2);
	
	sceKernelDcacheWritebackAll();

	sceGuDrawBufferList(pgeGfxPsm, pgeVramRelativePointer(pgeGfxFramebuffer), PGE_FRAME_BUFFER_WIDTH);
	
	sceGuOffset(2048 - (PGE_SCREEN_WIDTH>>1), 2048 - (PGE_SCREEN_HEIGHT>>1));
	sceGuViewport(2048, 2048, PGE_SCREEN_WIDTH, PGE_SCREEN_HEIGHT);
	sceGuScissor(0, 0, PGE_SCREEN_WIDTH, PGE_SCREEN_HEIGHT);
}

void pgeGfxSetTransparent(unsigned int color)
{
	sceGuColorFunc(GU_NOTEQUAL, color, 0xFFFFFFFF);
	sceGuEnable(GU_COLOR_TEST);
}

void pgeGfxUnsetTransparent(void)
{
	sceGuDisable(GU_COLOR_TEST);
}

void pgeGfxDrawLine(float startx, float starty, float endx, float endy, unsigned int color)
{
	pgeGfxStartDrawing();

	pgeVertV* vertices = (pgeVertV*)sceGuGetMemory((sizeof(pgeVertV))<<1);

	vertices[0].x = startx;
	vertices[0].y = starty;
	vertices[0].z = 0.0f;

	vertices[1].x = endx;
	vertices[1].y = endy;
	vertices[1].z = 0.0f;

	sceGuDisable(GU_TEXTURE_2D);
	sceGuColor(color);
	sceGuShadeModel(GU_FLAT);
	sceGuDrawArray(GU_LINES, GU_VERTEX_32BITF|GU_TRANSFORM_2D, 2, 0, vertices);
	sceGuShadeModel(GU_SMOOTH);
	sceGuEnable(GU_TEXTURE_2D);
}

void pgeGfxDrawStar(float x, float y, float outerradius, float innerradius, unsigned int numpoints, unsigned int color, float angle)
{
	angle = -angle;
	
	pgeGfxStartDrawing();

	const unsigned int numPointsReal = numpoints<<1;
	const float stepSize = (GU_PI*2)/(numPointsReal);
	unsigned int step;
	unsigned int stepPlusOne;
	const unsigned int numStepsPlusOne = numPointsReal + 1;

	pgeVertV* vertices = (pgeVertV*) sceGuGetMemory((numPointsReal + 2) * sizeof(pgeVertV));
	
	vertices[0].x = x;
	vertices[0].y = y;
	vertices[0].z = 0.0f;

	for(step = 1; step < numStepsPlusOne; step += 2)
	{
		vertices[step].x = x + pgeMathSin(step * stepSize + angle) * outerradius;
		vertices[step].y = y + pgeMathCos(step * stepSize + angle) * outerradius;
		vertices[step].z = 0.0f;

		stepPlusOne = step + 1;

		vertices[stepPlusOne].x = x + pgeMathSin(stepPlusOne * stepSize + angle) * innerradius;
		vertices[stepPlusOne].y = y + pgeMathCos(stepPlusOne * stepSize + angle) * innerradius;
		vertices[stepPlusOne].z = 0.0f;
	}

	vertices[numStepsPlusOne].x = vertices[1].x;
	vertices[numStepsPlusOne].y = vertices[1].y;
	vertices[numStepsPlusOne].z = 0.0f;
		
	sceGuDisable(GU_TEXTURE_2D);
	sceGuColor(color);
	sceGuShadeModel(GU_FLAT);
	sceGuDrawArray(GU_TRIANGLE_FAN, GU_VERTEX_32BITF|GU_TRANSFORM_2D, numPointsReal + 2, 0, vertices);
	sceGuShadeModel(GU_SMOOTH);
	sceGuEnable(GU_TEXTURE_2D);
}

void pgeGfxDrawStarOutline(float x, float y, float outerradius, float innerradius, unsigned int numpoints, unsigned int color, float angle)
{
	angle = -angle;
	
	pgeGfxStartDrawing();

	const unsigned int numPointsReal = numpoints<<1;
	const float stepSize = (GU_PI*2)/(numPointsReal);
	unsigned int step;
	unsigned int stepPlusOne;
	const unsigned int numStepsPlusOne = numPointsReal + 1;

	pgeVertV* vertices = (pgeVertV*) sceGuGetMemory(numStepsPlusOne * sizeof(pgeVertV));

	for(step = 0; step < numStepsPlusOne; step += 2)
	{
		vertices[step].x = x - pgeMathSin(step * stepSize + angle) * outerradius;
		vertices[step].y = y - pgeMathCos(step * stepSize + angle) * outerradius;
		vertices[step].z = 0.0f;

		stepPlusOne = step + 1;

		vertices[stepPlusOne].x = x - pgeMathSin(stepPlusOne * stepSize + angle) * innerradius;
		vertices[stepPlusOne].y = y - pgeMathCos(stepPlusOne * stepSize + angle) * innerradius;
		vertices[stepPlusOne].z = 0.0f;
	}

	vertices[numStepsPlusOne].x = vertices[0].x;
	vertices[numStepsPlusOne].y = vertices[0].y;
	vertices[numStepsPlusOne].z = 0.0f;

	sceGuDisable(GU_TEXTURE_2D);
	sceGuColor(color);
	sceGuShadeModel(GU_FLAT);
	sceGuDrawArray(GU_LINE_STRIP, GU_VERTEX_32BITF|GU_TRANSFORM_2D, numStepsPlusOne, 0, vertices);
	sceGuShadeModel(GU_SMOOTH);
	sceGuEnable(GU_TEXTURE_2D);
}

void pgeGfxDrawCircle(float x, float y, float radius, unsigned int numsteps, unsigned int color)
{
	pgeGfxStartDrawing();

	const float stepSize = (GU_PI*2)/numsteps;
	
	const unsigned int numStepsPlusOne = numsteps + 1;
	const unsigned int numStepsPlusTwo = numsteps + 2;
	
	pgeVertV* vertices = (pgeVertV*) sceGuGetMemory(numStepsPlusTwo * sizeof(pgeVertV));

	vertices[0].x = x;
	vertices[0].y = y;
	vertices[0].z = 0.0f;
		
	unsigned int step;
		
	for(step = 1; step < numStepsPlusOne; step++)
	{
		vertices[step].x = x + pgeMathSin(step * stepSize) * radius;
		vertices[step].y = y + pgeMathCos(step * stepSize) * radius;
		vertices[step].z = 0.0f;
	}

	vertices[numStepsPlusOne].x = vertices[1].x;
	vertices[numStepsPlusOne].y = vertices[1].y;
	vertices[numStepsPlusOne].z = 0.0f;
		
	sceGuDisable(GU_TEXTURE_2D);
	sceGuColor(color);
	sceGuShadeModel(GU_FLAT);
	sceGuDrawArray(GU_TRIANGLE_FAN, GU_VERTEX_32BITF|GU_TRANSFORM_2D, numStepsPlusTwo, 0, vertices);
	sceGuShadeModel(GU_SMOOTH);
	sceGuEnable(GU_TEXTURE_2D);
}

void pgeGfxDrawCircleOutline(float x, float y, float radius, unsigned int numsteps, unsigned int color)
{
	pgeGfxStartDrawing();

	const float stepSize = (GU_PI*2)/numsteps;
	
	const unsigned int numStepsPlusOne = numsteps + 1;
	
	pgeVertV* vertices = (pgeVertV*) sceGuGetMemory(numStepsPlusOne * sizeof(pgeVertV));

	unsigned int step;

	for(step = 0; step < numStepsPlusOne; step++)
	{
		vertices[step].x = x + pgeMathSin(step * stepSize) * radius;
		vertices[step].y = y + pgeMathCos(step * stepSize) * radius;
		vertices[step].z = 0.0f;
	}

	vertices[numStepsPlusOne].x = vertices[0].x;
	vertices[numStepsPlusOne].y = vertices[0].y;
	vertices[numStepsPlusOne].z = 0.0f;
		
	sceGuDisable(GU_TEXTURE_2D);
	sceGuColor(color);
	sceGuShadeModel(GU_FLAT);
	sceGuDrawArray(GU_LINE_STRIP, GU_VERTEX_32BITF|GU_TRANSFORM_2D, numStepsPlusOne, 0, vertices);
	sceGuShadeModel(GU_SMOOTH);
	sceGuEnable(GU_TEXTURE_2D);
}

void pgeGfxDrawRectOutline(pgeRect *destrect, unsigned int color, float angle)
{
	pgeGfxStartDrawing();
	
	if(angle == 0.0f)
	{
		pgeVertV* vertices = (pgeVertV*)sceGuGetMemory(5 * sizeof(pgeVertV));

		vertices[0].x = destrect->x; 
		vertices[0].y = destrect->y;
		vertices[0].z = 0.0f;

		vertices[1].x = destrect->x + destrect->w;
		vertices[1].y = destrect->y;
		vertices[1].z = 0.0f;

		vertices[2].x = destrect->x + destrect->w + 1;
		vertices[2].y = destrect->y + destrect->h;
		vertices[2].z = 0.0f;

		vertices[3].x = destrect->x;
		vertices[3].y = destrect->y + destrect->h;
		vertices[3].z = 0.0f;

		vertices[4].x = destrect->x;
		vertices[4].y = destrect->y;
		vertices[4].z = 0.0f;

		sceGuDisable(GU_TEXTURE_2D);
		sceGuColor(color);
		sceGuShadeModel(GU_FLAT);
		sceGuDrawArray(GU_LINE_STRIP, GU_VERTEX_32BITF|GU_TRANSFORM_2D, 5, 0, vertices);
		sceGuShadeModel(GU_SMOOTH);
		sceGuEnable(GU_TEXTURE_2D);
	}
	else
	{
		float x = destrect->x;
		float y = destrect->y;
		float width = destrect->w;
		float height = destrect->h;
		
		x += width * 0.5f;
		y += height * 0.5f;
		
		float c, s;
		
		pgeMathSincos(angle, &s, &c);
		
		width *= 0.5f;
		height *= 0.5f;
		
		float cw = c*width;
		float sw = s*width;
		float ch = c*height;
		float sh = s*height;

		pgeVertV* vertices = (pgeVertV*)sceGuGetMemory(5 * sizeof(pgeVertV));
	
		vertices[0].x = x - cw + sh;
		vertices[0].y = y - sw - ch;
		vertices[0].z = 0;
				
		vertices[1].x = x - cw - sh;
		vertices[1].y = y - sw + ch;
		vertices[1].z = 0;
		
		vertices[2].x = x + cw - sh;
		vertices[2].y = y + sw + ch;
		vertices[2].z = 0;
		
		vertices[3].x = x + cw + sh;
		vertices[3].y = y + sw - ch;
		vertices[3].z = 0;
		
		vertices[4].x = x - cw + sh;
		vertices[4].y = y - sw - ch;
		vertices[4].z = 0;
	
		sceGuDisable(GU_TEXTURE_2D);
		sceGuColor(color);
		sceGuShadeModel(GU_FLAT);
		sceGuDrawArray(GU_LINE_STRIP, GU_VERTEX_32BITF|GU_TRANSFORM_2D, 5, 0, vertices);
		sceGuShadeModel(GU_SMOOTH);
		sceGuEnable(GU_TEXTURE_2D);
	}
}

void pgeGfxDrawRect(pgeRect *destrect, unsigned int color, float angle)
{	
	pgeGfxStartDrawing();

	if(angle == 0.0f)
	{
		pgeVertV* vertices = (pgeVertV*)sceGuGetMemory((sizeof(pgeVertV)<<1));

		vertices[0].x = destrect->x;
		vertices[0].y = destrect->y;
		vertices[0].z = 0.0f;

		vertices[1].x = destrect->x + destrect->w;
		vertices[1].y = destrect->y + destrect->h;
		vertices[1].z = 0.0f;

		sceGuDisable(GU_TEXTURE_2D);
		sceGuColor(color);
		sceGuShadeModel(GU_FLAT);
		sceGuDrawArray(GU_SPRITES, GU_VERTEX_32BITF|GU_TRANSFORM_2D, 2, 0, vertices);
		sceGuShadeModel(GU_SMOOTH);
		sceGuEnable(GU_TEXTURE_2D);
	}
	else
	{
		float x = destrect->x;
		float y = destrect->y;
		float width = destrect->w;
		float height = destrect->h;
		
		x += width * 0.5f;
		y += height * 0.5f;
		
		float c, s;
		
		pgeMathSincos(angle, &s, &c);
		
		width *= 0.5f;
		height *= 0.5f;
		
		float cw = c*width;
		float sw = s*width;
		float ch = c*height;
		float sh = s*height;

		pgeVertV* vertices = (pgeVertV*)sceGuGetMemory(5 * sizeof(pgeVertV));
	
		vertices[0].x = x - cw + sh;
		vertices[0].y = y - sw - ch;
		vertices[0].z = 0;
				
		vertices[1].x = x - cw - sh;
		vertices[1].y = y - sw + ch;
		vertices[1].z = 0;
		
		vertices[2].x = x + cw - sh;
		vertices[2].y = y + sw + ch;
		vertices[2].z = 0;
		
		vertices[3].x = x + cw + sh;
		vertices[3].y = y + sw - ch;
		vertices[3].z = 0;
	
		sceGuDisable(GU_TEXTURE_2D);
		sceGuColor(color);
		sceGuShadeModel(GU_FLAT);
		sceGuDrawArray(GU_TRIANGLE_FAN, GU_VERTEX_32BITF|GU_TRANSFORM_2D, 4, 0, vertices);
		sceGuShadeModel(GU_SMOOTH);
		sceGuEnable(GU_TEXTURE_2D);	
	}
}

void pgeGfxDrawRectGrad(pgeRect *destrect, unsigned int color1, unsigned int color2, unsigned int color3, unsigned int color4, float angle)
{
	pgeGfxStartDrawing();
	
	if(angle == 0.0f)
	{
		pgeVertCV* vertices = (pgeVertCV*)sceGuGetMemory((sizeof(pgeVertCV))<<2);
		
		vertices[0].color = color1;
		vertices[0].x = destrect->x; 
		vertices[0].y = destrect->y;
		vertices[0].z = 0.0f;
		
		vertices[1].color = color2;
		vertices[1].x = destrect->x;
		vertices[1].y = destrect->y + destrect->h;
		vertices[1].z = 0.0f;
		
		vertices[2].color = color3;
		vertices[2].x = destrect->x + destrect->w;
		vertices[2].y = destrect->y + destrect->h;
		vertices[2].z = 0.0f;

		vertices[3].color = color4;
		vertices[3].x = destrect->x + destrect->w;
		vertices[3].y = destrect->y;
		vertices[3].z = 0.0f;
		
		sceGuDisable(GU_TEXTURE_2D);
		sceGuDrawArray(GU_TRIANGLE_FAN, GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_2D, 4, 0, vertices);
		sceGuEnable(GU_TEXTURE_2D);
	}
	else
	{
		float x = destrect->x;
		float y = destrect->y;
		float width = destrect->w;
		float height = destrect->h;
		
		x += width * 0.5f;
		y += height * 0.5f;
		
		float c, s;
		
		pgeMathSincos(angle, &s, &c);
		
		width *= 0.5f;
		height *= 0.5f;
		
		float cw = c*width;
		float sw = s*width;
		float ch = c*height;
		float sh = s*height;

		pgeVertCV* vertices = (pgeVertCV*)sceGuGetMemory((sizeof(pgeVertCV))<<2);
	
		vertices[0].color = color1;
		vertices[0].x = x - cw + sh;
		vertices[0].y = y - sw - ch;
		vertices[0].z = 0;
				
		vertices[1].color = color2;
		vertices[1].x = x - cw - sh;
		vertices[1].y = y - sw + ch;
		vertices[1].z = 0;
		
		vertices[2].color = color3;
		vertices[2].x = x + cw - sh;
		vertices[2].y = y + sw + ch;
		vertices[2].z = 0;
		
		vertices[3].color = color4;
		vertices[3].x = x + cw + sh;
		vertices[3].y = y + sw - ch;
		vertices[3].z = 0;
	
		sceGuDisable(GU_TEXTURE_2D);
		sceGuDrawArray(GU_TRIANGLE_FAN, GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_2D, 4, 0, vertices);
		sceGuEnable(GU_TEXTURE_2D);	
	}
}

void pgeGfxDrawTextureEasy(pgeTexture *texture, float x, float y, float angle, unsigned char alpha)
{
	pgeRect sourcerect = {x, y, texture->width, texture->height};
	pgeRect destrect = {0, 0, texture->width, texture->height};
	
	pgeGfxDrawTexture(&sourcerect, &destrect, angle, alpha);
}

void pgeGfxDrawTexture(pgeRect *destrect, pgeRect *sourcerect, float angle, unsigned char alpha)
{
	if(destrect->w <= 0 || destrect->h <= 0)
		return;
	
	float x, y, width, height;
	float u0, v0, u1, v1;
	
	x = destrect->x;
	y = destrect->y;
	width = destrect->w;
	height = destrect->h;

	u0 = sourcerect->x;
	v0 = sourcerect->y;
	u1 = sourcerect->x + sourcerect->w;
	v1 = sourcerect->y + sourcerect->h;
	
	pgeGfxStartDrawing();
	
	if(alpha != 255)
	{
		sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
		sceGuColor(GU_RGBA(255, 255, 255, alpha));
	}
	
	if(angle == 0.0f)
	{
		if((pgeMathAbs(u1) - pgeMathAbs(u0)) < 64.0f)
		{
			pgeVertTV* vertices = (pgeVertTV*)sceGuGetMemory((sizeof(pgeVertTV))<<1);

			vertices[0].u = u0;
			vertices[0].v = v0;
			vertices[0].x = x;
			vertices[0].y = y; 
			vertices[0].z = 0;

			vertices[1].u = u1;
			vertices[1].v = v1;
			vertices[1].x = x + width;
			vertices[1].y = y + height;
			vertices[1].z = 0;

			sceGuDrawArray(GU_SPRITES, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_2D, 2, 0, vertices);
		}
		else
		{
			float start, end;
			float curU = u0;
			float curX = x;
			float endX = x + width;
			float slice = 64.0f;
			float ustep = (u1 - u0)/width * slice;
		
			if(ustep < 0.0f)
				ustep = -ustep;

			for(start = 0, end = width; start < end; start += slice)
			{
				pgeVertTV* vertices = (pgeVertTV*)sceGuGetMemory((sizeof(pgeVertTV))<<1);

				float polyWidth = ((curX + slice) > endX) ? (endX - curX) : slice;
				float sourceWidth = ((curU + ustep) > u1) ? (u1 - curU) : ustep;

				vertices[0].u = curU;
				vertices[0].v = v0;
				vertices[0].x = curX;
				vertices[0].y = y; 
				vertices[0].z = 0;

				curU += sourceWidth;
				curX += polyWidth;

				vertices[1].u = curU;
				vertices[1].v = v1;
				vertices[1].x = curX;
				vertices[1].y = (y + height);
				vertices[1].z = 0;

				sceGuDrawArray(GU_SPRITES, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_2D, 2, 0, vertices);
			}
		}
	}
	else
	{
		x += width * 0.5f;
		y += height * 0.5f;
		
		float c, s;
		
		pgeMathSincos(angle, &s, &c);
		
		width *= 0.5f;
		height *= 0.5f;
		
		float cw = c*width;
		float sw = s*width;
		float ch = c*height;
		float sh = s*height;

		pgeVertTV* vertices = (pgeVertTV*)sceGuGetMemory(sizeof(pgeVertTV)<<2);

		vertices[0].u = u0;
		vertices[0].v = v0;
		vertices[0].x = x - cw + sh;
		vertices[0].y = y - sw - ch;
		vertices[0].z = 0;
				
		vertices[1].u = u0;
		vertices[1].v = v1;
		vertices[1].x = x - cw - sh;
		vertices[1].y = y - sw + ch;
		vertices[1].z = 0;
		
		vertices[2].u = u1;
		vertices[2].v = v1;
		vertices[2].x = x + cw - sh;
		vertices[2].y = y + sw + ch;
		vertices[2].z = 0;
		
		vertices[3].u = u1;
		vertices[3].v = v0;
		vertices[3].x = x + cw + sh;
		vertices[3].y = y + sw - ch;
		vertices[3].z = 0;

		sceGuDrawArray(GU_TRIANGLE_FAN, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_2D, 4, 0, vertices);
	}
	
	if(alpha != 255)
		sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
}

void pgeGfxDrawTextureColored(pgeRect *destrect, pgeRect *sourcerect, float angle, unsigned int color)
{
	float x, y, width, height;
	float u0, v0, u1, v1;
	
	x = destrect->x;
	y = destrect->y;
	width = destrect->w;
	height = destrect->h;

	u0 = sourcerect->x;
	v0 = sourcerect->y;
	u1 = sourcerect->x + sourcerect->w;
	v1 = sourcerect->y + sourcerect->h;
	
	pgeGfxStartDrawing();
	
	pgeGfxSetBlendMode(PGE_BLEND_MODE_TRANSPARENT);
	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
	sceGuColor(color);
	
	if(angle == 0.0f)
	{
		float start, end;
		float curU = u0;
		float curX = x;
		float endX = x + width;
		float slice = 64.0f;
		float ustep = (u1 - u0)/width * slice;
		
		if(ustep < 0.0f)
			ustep = -ustep;

		// blit maximizing the use of the texture-cache
		for(start = 0, end = width; start < end; start += slice)
		{
			pgeVertTV* vertices = (pgeVertTV*)sceGuGetMemory((sizeof(pgeVertTV))<<1);

			float polyWidth = ((curX + slice) > endX) ? (endX - curX) : slice;
			float sourceWidth = ((curU + ustep) > u1) ? (u1 - curU) : ustep;

			vertices[0].u = curU;
			vertices[0].v = v0;
			vertices[0].x = curX;
			vertices[0].y = y; 
			vertices[0].z = 0;

			curU += sourceWidth;
			curX += polyWidth;

			vertices[1].u = curU;
			vertices[1].v = v1;
			vertices[1].x = curX;
			vertices[1].y = (y + height);
			vertices[1].z = 0;

			sceGuDrawArray(GU_SPRITES, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_2D, 2, 0, vertices);
		}
	}
	else
	{
		float realWidth = width/2.0f;
		float realHeight = height/2.0f;
	
		sceGumPushMatrix();
		sceGumLoadIdentity();
		ScePspFVector3 trans;
		trans.x = (x + realWidth);
		trans.y = (y + realHeight);
		trans.z = 0;
		sceGumTranslate(&trans);
		sceGumRotateZ(angle);

		pgeVertTV* vertices = (pgeVertTV*)sceGuGetMemory(sizeof(pgeVertTV)<<2);

		vertices[0].u = u0;
		vertices[0].v = v0;
		vertices[0].x = -(realWidth);
		vertices[0].y = -(realHeight);
		vertices[0].z = 0;

		vertices[1].u = u0;
		vertices[1].v = v1;
		vertices[1].x = -(realWidth);
		vertices[1].y = realHeight;
		vertices[1].z = 0;

		vertices[2].u = u1;
		vertices[2].v = v1;
		vertices[2].x = realWidth;
		vertices[2].y = realHeight;
		vertices[2].z = 0;

		vertices[3].u = u1;
		vertices[3].v = v0;
		vertices[3].x = realWidth;
		vertices[3].y = -(realHeight);
		vertices[3].z = 0;

		sceGumDrawArray(GU_TRIANGLE_FAN, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, vertices);
	
		sceGumPopMatrix();
	}
	
	sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
}

void *pgeGfxGetVertexMemory(int size)
{
	return sceGuGetMemory(size);
}

void pgeGfxDrawCustom2D(enum pgeGfxPrim prim, enum pgeGfxVertexType vtype, int count, const void *vertices)
{
	vtype |= GU_TRANSFORM_2D;
	
	if(vtype & GU_TEXTURE_32BITF)
	{
		// Nothing yet
	}
	else
		sceGuDisable(GU_TEXTURE_2D);
	
	sceGuDrawArray(prim, vtype, count, 0, vertices);
	
	sceGuEnable(GU_TEXTURE_2D);
}

void pgeGfxDrawCustom3D(ScePspFVector3 *position, ScePspFVector3 *rotation, enum pgeGfxPrim prim, enum pgeGfxVertexType vtype, int count, const void *vertices)
{
	vtype |= GU_TRANSFORM_3D;
	
	sceGumPushMatrix();
	
	sceGumLoadIdentity();
	
	sceGumTranslate(position);
	
	sceGumRotateXYZ(rotation);
	
	if(vtype & GU_TEXTURE_32BITF)
	{
		// No scaling
		sceGuTexScale(1.0f, 1.0f);
	}
	else
		sceGuDisable(GU_TEXTURE_2D);
		
	sceGuEnable(GU_DEPTH_TEST);
	
	sceGumDrawArray(prim, vtype, count, 0, vertices);
	
	sceGuEnable(GU_TEXTURE_2D);
	
	sceGuDisable(GU_DEPTH_TEST);
	
	sceGumPopMatrix();
}

void pgeGfxDrawTexture3D(ScePspFVector3 *position, ScePspFVector3 *rotation, float width, float height)
{
	pgeGfxStartDrawing();
	
	// No scaling
	sceGuTexScale(1.0f, 1.0f);
	
	sceGumPushMatrix();
	
	sceGumLoadIdentity();
	
	sceGumTranslate(position);
	
	sceGumRotateXYZ(rotation);
	
	float realWidth = width/2.0f;
	float realHeight = height/2.0f;
	
	pgeVertTV* vertices = (pgeVertTV*)sceGuGetMemory(sizeof(pgeVertTV) * 6);
	
	vertices[0].u = 0.0f;
	vertices[0].v = 1.0f;
	vertices[0].x = -(realWidth);
	vertices[0].y = -(realHeight);
	vertices[0].z = 0.0f;
	
	vertices[1].u = 1.0f;
	vertices[1].v = 0.0f;
	vertices[1].x = realWidth;
	vertices[1].y = realHeight;
	vertices[1].z = 0.0f;
	
	vertices[2].u = 0.0f;
	vertices[2].v = 0.0f;
	vertices[2].x = -(realWidth);
	vertices[2].y = realHeight;
	vertices[2].z = 0.0f;
	
	vertices[3].u = 0.0f;
	vertices[3].v = 1.0f;
	vertices[3].x = -(realWidth);
	vertices[3].y = -(realHeight);
	vertices[3].z = 0.0f;
	
	vertices[4].u = 1.0f;
	vertices[4].v = 1.0f;
	vertices[4].x = realWidth;
	vertices[4].y = -(realHeight);
	vertices[4].z = 0.0f;
	
	vertices[5].u = 1.0f;
	vertices[5].v = 0.0f;
	vertices[5].x = realWidth;
	vertices[5].y = realHeight;
	vertices[5].z = 0.0f;
	
	sceGuEnable(GU_DEPTH_TEST);
	
	sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 2*3, 0, vertices);
	
	sceGuDisable(GU_DEPTH_TEST);
	
	sceGumPopMatrix();
}

void pgeGfxDrawTexture3DColored(ScePspFVector3 *position, ScePspFVector3 *rotation, float width, float height, unsigned int color)
{
	pgeGfxStartDrawing();

	pgeGfxSetBlendMode(PGE_BLEND_MODE_TRANSPARENT);
	sceGuTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
	sceGuColor(color);

	// No scaling
	sceGuTexScale(1.0f, 1.0f);

	sceGumPushMatrix();

	sceGumLoadIdentity();

	sceGumTranslate(position);

	sceGumRotateXYZ(rotation);

	float realWidth = width/2.0f;
	float realHeight = height/2.0f;

	pgeVertTV* vertices = (pgeVertTV*)sceGuGetMemory(sizeof(pgeVertTV) * 6);

	vertices[0].u = 0.0f;
	vertices[0].v = 1.0f;
	vertices[0].x = -(realWidth);
	vertices[0].y = -(realHeight);
	vertices[0].z = 0.0f;

	vertices[1].u = 1.0f;
	vertices[1].v = 0.0f;
	vertices[1].x = realWidth;
	vertices[1].y = realHeight;
	vertices[1].z = 0.0f;

	vertices[2].u = 0.0f;
	vertices[2].v = 0.0f;
	vertices[2].x = -(realWidth);
	vertices[2].y = realHeight;
	vertices[2].z = 0.0f;

	vertices[3].u = 0.0f;
	vertices[3].v = 1.0f;
	vertices[3].x = -(realWidth);
	vertices[3].y = -(realHeight);
	vertices[3].z = 0.0f;

	vertices[4].u = 1.0f;
	vertices[4].v = 1.0f;
	vertices[4].x = realWidth;
	vertices[4].y = -(realHeight);
	vertices[4].z = 0.0f;

	vertices[5].u = 1.0f;
	vertices[5].v = 0.0f;
	vertices[5].x = realWidth;
	vertices[5].y = realHeight;
	vertices[5].z = 0.0f;

	sceGuEnable(GU_DEPTH_TEST);

	sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 2*3, 0, vertices);

	sceGuDisable(GU_DEPTH_TEST);

	sceGumPopMatrix();
}

void pgeGfxDrawCube(ScePspFVector3 *position, ScePspFVector3 *rotation, float width, float height, float depth, unsigned int color)
{
	pgeGfxStartDrawing();
	
	sceGumPushMatrix();
	
	sceGumLoadIdentity();
	
	sceGumTranslate(position);
	
	sceGumRotateXYZ(rotation);
	
	float realWidth = width/2.0f;
	float realDepth = depth/2.0f;
	float realHeight = height/2.0f;
	
	pgeVertV* vertices = (pgeVertV*)sceGuGetMemory(sizeof(pgeVertV) * 36);

	// Front
	vertices[0].x = -(realWidth);
	vertices[0].y = -(realHeight);
	vertices[0].z = realDepth;
	
	vertices[1].x = realWidth;
	vertices[1].y = realHeight;
	vertices[1].z = realDepth;
	
	vertices[2].x = -(realWidth);
	vertices[2].y = realHeight;
	vertices[2].z = realDepth;
	
	vertices[3].x = -(realWidth);
	vertices[3].y = -(realHeight);
	vertices[3].z = realDepth;
	
	vertices[4].x = realWidth;
	vertices[4].y = -(realHeight);
	vertices[4].z = realDepth;
	
	vertices[5].x = realWidth;
	vertices[5].y = realHeight;
	vertices[5].z = realDepth;
	
	// Back
	vertices[6].x = -(realWidth);
	vertices[6].y = -(realHeight);
	vertices[6].z = -(realDepth);
	
	vertices[7].x = realWidth;
	vertices[7].y = realHeight;
	vertices[7].z = -(realDepth);
	
	vertices[8].x = realWidth;
	vertices[8].y = -(realHeight);
	vertices[8].z = -(realDepth);
	
	vertices[9].x = -(realWidth);
	vertices[9].y = -(realHeight);
	vertices[9].z = -(realDepth);
	
	vertices[10].x = -(realWidth);
	vertices[10].y = realHeight;
	vertices[10].z = -(realDepth);
	
	vertices[11].x = realWidth;
	vertices[11].y = realHeight;
	vertices[11].z = -(realDepth);
	
	// Right
	vertices[12].x = realWidth;
	vertices[12].y = -(realHeight);
	vertices[12].z = -(realDepth);
	
	vertices[13].x = realWidth;
	vertices[13].y = realHeight;
	vertices[13].z = realDepth;
	
	vertices[14].x = realWidth;
	vertices[14].y = -(realHeight);
	vertices[14].z = realDepth;
	
	vertices[15].x = realWidth;
	vertices[15].y = -(realHeight);
	vertices[15].z = -(realDepth);
	
	vertices[16].x = realWidth;
	vertices[16].y = realHeight;
	vertices[16].z = -(realDepth);
	
	vertices[17].x = realWidth;
	vertices[17].y = realHeight;
	vertices[17].z = realDepth;
	
	// Left
	vertices[18].x = -(realWidth);
	vertices[18].y = -(realHeight);
	vertices[18].z = -(realDepth);
	
	vertices[19].x = -(realWidth);
	vertices[19].y = realHeight;
	vertices[19].z = realDepth;
	
	vertices[20].x = -(realWidth);
	vertices[20].y = realHeight;
	vertices[20].z = -(realDepth);
	
	vertices[21].x = -(realWidth);
	vertices[21].y = -(realHeight);
	vertices[21].z = -(realDepth);
	
	vertices[22].x = -(realWidth);
	vertices[22].y = -(realHeight);
	vertices[22].z = realDepth;
	
	vertices[23].x = -(realWidth);
	vertices[23].y = realHeight;
	vertices[23].z = realDepth;
	
	// Top
	vertices[24].x = -(realWidth);
	vertices[24].y = realHeight;
	vertices[24].z = -(realDepth);
	
	vertices[25].x = realWidth;
	vertices[25].y = realHeight;
	vertices[25].z = realDepth;
	
	vertices[26].x = realWidth;
	vertices[26].y = realHeight;
	vertices[26].z = -(realDepth);
	
	vertices[27].x = -(realWidth);
	vertices[27].y = realHeight;
	vertices[27].z = -(realDepth);
	
	vertices[28].x = -(realWidth);
	vertices[28].y = realHeight;
	vertices[28].z = realDepth;
	
	vertices[29].x = realWidth;
	vertices[29].y = realHeight;
	vertices[29].z = realDepth;
	
	// Bottom
	vertices[30].x = -(realWidth);
	vertices[30].y = -(realHeight);
	vertices[30].z = -(realDepth);
	
	vertices[31].x = realWidth;
	vertices[31].y = -(realHeight);
	vertices[31].z = realDepth;
	
	vertices[32].x = -(realWidth);
	vertices[32].y = -(realHeight);
	vertices[32].z = realDepth;
	
	vertices[33].x = -(realWidth);
	vertices[33].y = -(realHeight);
	vertices[33].z = -(realDepth);
	
	vertices[34].x = realWidth;
	vertices[34].y = -(realHeight);
	vertices[34].z = -(realDepth);
	
	vertices[35].x = realWidth;
	vertices[35].y = -(realHeight);
	vertices[35].z = realDepth;
	
	sceGuDisable(GU_TEXTURE_2D);
	
	sceGuShadeModel(GU_FLAT);
	
	sceGuColor(color);
	
	sceGuEnable(GU_DEPTH_TEST);
	
	sceGumDrawArray(GU_TRIANGLES, GU_VERTEX_32BITF|GU_TRANSFORM_3D, 12*3, 0, vertices);
	
	sceGuDisable(GU_DEPTH_TEST);
	
	sceGuShadeModel(GU_SMOOTH);
	
	sceGuEnable(GU_TEXTURE_2D);
	
	sceGumPopMatrix();
}

void pgeGfxDrawCubeTexture(ScePspFVector3 *position, ScePspFVector3 *rotation, float width, float height, float depth)
{
	pgeGfxStartDrawing();
	
	// No scaling
	sceGuTexScale(1.0f, 1.0f);
	
	sceGumPushMatrix();
	
	sceGumLoadIdentity();
	
	sceGumTranslate(position);
	
	sceGumRotateXYZ(rotation);
	
	float realWidth = width/2.0f;
	float realDepth = depth/2.0f;
	float realHeight = height/2.0f;
	
	pgeVertTV* vertices = (pgeVertTV*)sceGuGetMemory(sizeof(pgeVertTV) * 36);
	
	// Front
	vertices[0].u = 0.0f;
	vertices[0].v = 1.0f;
	vertices[0].x = -(realWidth);
	vertices[0].y = -(realHeight);
	vertices[0].z = realDepth;
	
	vertices[1].u = 1.0f;
	vertices[1].v = 0.0f;
	vertices[1].x = realWidth;
	vertices[1].y = realHeight;
	vertices[1].z = realDepth;
	
	vertices[2].u = 0.0f;
	vertices[2].v = 0.0f;
	vertices[2].x = -(realWidth);
	vertices[2].y = realHeight;
	vertices[2].z = realDepth;
	
	vertices[3].u = 0.0f;
	vertices[3].v = 1.0f;
	vertices[3].x = -(realWidth);
	vertices[3].y = -(realHeight);
	vertices[3].z = realDepth;
	
	vertices[4].u = 1.0f;
	vertices[4].v = 1.0f;
	vertices[4].x = realWidth;
	vertices[4].y = -(realHeight);
	vertices[4].z = realDepth;
	
	vertices[5].u = 1.0f;
	vertices[5].v = 0.0f;
	vertices[5].x = realWidth;
	vertices[5].y = realHeight;
	vertices[5].z = realDepth;
	
	// Back
	vertices[6].u = 1.0f;
	vertices[6].v = 1.0f;
	vertices[6].x = -(realWidth);
	vertices[6].y = -(realHeight);
	vertices[6].z = -(realDepth);
	
	vertices[7].u = 0.0f;
	vertices[7].v = 0.0f;
	vertices[7].x = realWidth;
	vertices[7].y = realHeight;
	vertices[7].z = -(realDepth);
	
	vertices[8].u = 0.0f;
	vertices[8].v = 1.0f;
	vertices[8].x = realWidth;
	vertices[8].y = -(realHeight);
	vertices[8].z = -(realDepth);
	
	vertices[9].u = 1.0f;
	vertices[9].v = 1.0f;
	vertices[9].x = -(realWidth);
	vertices[9].y = -(realHeight);
	vertices[9].z = -(realDepth);
	
	vertices[10].u = 1.0f;
	vertices[10].v = 0.0f;
	vertices[10].x = -(realWidth);
	vertices[10].y = realHeight;
	vertices[10].z = -(realDepth);
	
	vertices[11].u = 0.0f;
	vertices[11].v = 0.0f;
	vertices[11].x = realWidth;
	vertices[11].y = realHeight;
	vertices[11].z = -(realDepth);
	
	// Right
	vertices[12].u = 1.0f;
	vertices[12].v = 1.0f;
	vertices[12].x = realWidth;
	vertices[12].y = -(realHeight);
	vertices[12].z = -(realDepth);
	
	vertices[13].u = 0.0f;
	vertices[13].v = 0.0f;
	vertices[13].x = realWidth;
	vertices[13].y = realHeight;
	vertices[13].z = realDepth;
	
	vertices[14].u = 0.0f;
	vertices[14].v = 1.0f;
	vertices[14].x = realWidth;
	vertices[14].y = -(realHeight);
	vertices[14].z = realDepth;
	
	vertices[15].u = 1.0f;
	vertices[15].v = 1.0f;
	vertices[15].x = realWidth;
	vertices[15].y = -(realHeight);
	vertices[15].z = -(realDepth);
	
	vertices[16].u = 1.0f;
	vertices[16].v = 0.0f;
	vertices[16].x = realWidth;
	vertices[16].y = realHeight;
	vertices[16].z = -(realDepth);
	
	vertices[17].u = 0.0f;
	vertices[17].v = 0.0f;
	vertices[17].x = realWidth;
	vertices[17].y = realHeight;
	vertices[17].z = realDepth;
	
	// Left
	vertices[18].u = 0.0f;
	vertices[18].v = 1.0f;
	vertices[18].x = -(realWidth);
	vertices[18].y = -(realHeight);
	vertices[18].z = -(realDepth);
	
	vertices[19].u = 1.0f;
	vertices[19].v = 0.0f;
	vertices[19].x = -(realWidth);
	vertices[19].y = realHeight;
	vertices[19].z = realDepth;
	
	vertices[20].u = 0.0f;
	vertices[20].v = 0.0f;
	vertices[20].x = -(realWidth);
	vertices[20].y = realHeight;
	vertices[20].z = -(realDepth);
	
	vertices[21].u = 0.0f;
	vertices[21].v = 1.0f;
	vertices[21].x = -(realWidth);
	vertices[21].y = -(realHeight);
	vertices[21].z = -(realDepth);
	
	vertices[22].u = 1.0f;
	vertices[22].v = 1.0f;
	vertices[22].x = -(realWidth);
	vertices[22].y = -(realHeight);
	vertices[22].z = realDepth;
	
	vertices[23].u = 1.0f;
	vertices[23].v = 0.0f;
	vertices[23].x = -(realWidth);
	vertices[23].y = realHeight;
	vertices[23].z = realDepth;
	
	// Top
	vertices[24].u = 0.0f;
	vertices[24].v = 0.0f;
	vertices[24].x = -(realWidth);
	vertices[24].y = realHeight;
	vertices[24].z = -(realDepth);
	
	vertices[25].u = 1.0f;
	vertices[25].v = 1.0f;
	vertices[25].x = realWidth;
	vertices[25].y = realHeight;
	vertices[25].z = realDepth;
	
	vertices[26].u = 1.0f;
	vertices[26].v = 0.0f;
	vertices[26].x = realWidth;
	vertices[26].y = realHeight;
	vertices[26].z = -(realDepth);
	
	vertices[27].u = 0.0f;
	vertices[27].v = 0.0f;
	vertices[27].x = -(realWidth);
	vertices[27].y = realHeight;
	vertices[27].z = -(realDepth);
	
	vertices[28].u = 0.0f;
	vertices[28].v = 1.0f;
	vertices[28].x = -(realWidth);
	vertices[28].y = realHeight;
	vertices[28].z = realDepth;
	
	vertices[29].u = 1.0f;
	vertices[29].v = 1.0f;
	vertices[29].x = realWidth;
	vertices[29].y = realHeight;
	vertices[29].z = realDepth;
	
	// Bottom
	vertices[30].u = 0.0f;
	vertices[30].v = 1.0f;
	vertices[30].x = -(realWidth);
	vertices[30].y = -(realHeight);
	vertices[30].z = -(realDepth);
	
	vertices[31].u = 1.0f;
	vertices[31].v = 0.0f;
	vertices[31].x = realWidth;
	vertices[31].y = -(realHeight);
	vertices[31].z = realDepth;
	
	vertices[32].u = 0.0f;
	vertices[32].v = 0.0f;
	vertices[32].x = -(realWidth);
	vertices[32].y = -(realHeight);
	vertices[32].z = realDepth;
	
	vertices[33].u = 0.0f;
	vertices[33].v = 1.0f;
	vertices[33].x = -(realWidth);
	vertices[33].y = -(realHeight);
	vertices[33].z = -(realDepth);
	
	vertices[34].u = 1.0f;
	vertices[34].v = 1.0f;
	vertices[34].x = realWidth;
	vertices[34].y = -(realHeight);
	vertices[34].z = -(realDepth);
	
	vertices[35].u = 1.0f;
	vertices[35].v = 0.0f;
	vertices[35].x = realWidth;
	vertices[35].y = -(realHeight);
	vertices[35].z = realDepth;
	
	sceGuEnable(GU_DEPTH_TEST);
	
	sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 12*3, 0, vertices);
	
	sceGuDisable(GU_DEPTH_TEST);
	
	sceGumPopMatrix();
}

void pgeGfxDrawObj(pgeObj *obj, ScePspFVector3 *position, ScePspFVector3 *rotation)
{
	int texture = 0;
	
	if(obj->drawflag & GU_TEXTURE_32BITF)
	{
		texture = 1;
		// No scaling
		sceGuTexScale(1.0f, 1.0f);
	}
	
	pgeGfxStartDrawing();
	
	sceGumPushMatrix();
	
	sceGumLoadIdentity();
	
	sceGumTranslate(position);
	
	sceGumRotateXYZ(rotation);
	
	sceGuEnable(GU_DEPTH_TEST);
	
	if(!texture)
		sceGuDisable(GU_TEXTURE_2D);
	
	sceGumDrawArray(GU_TRIANGLES, obj->drawflag, obj->numvertices, 0, obj->vertices);
	
	if(!texture)
		sceGuEnable(GU_TEXTURE_2D);
	
	sceGuDisable(GU_DEPTH_TEST);
	
	sceGumPopMatrix();
}

unsigned int pgeGfxColorSub(unsigned int color1, unsigned int color2)
{
	unsigned int color = GU_RGBA(R(color1) - R(color2), G(color1) - G(color2), B(color1) - B(color2), A(color1) - A(color2));
	
	return color;
}

unsigned int pgeGfxColorAdd(unsigned int color1, unsigned int color2)
{
	unsigned int color = GU_RGBA(R(color1) + R(color2), G(color1) + G(color2), B(color1) + B(color2), A(color1) + A(color2));
	
	return color;
}

unsigned int pgeGfxColorMul(unsigned int color1, unsigned int color2)
{
	unsigned int color = GU_RGBA(R(color1) * R(color2), G(color1) * G(color2), B(color1) * B(color2), A(color1) * A(color2));
	
	return color;
}

unsigned int pgeGfxColorMulScalar(unsigned int color, float scalar)
{
	unsigned int retcolor = GU_RGBA((int)(R(color) * scalar), (int)(G(color) * scalar), (int)(B(color) * scalar), (int)(A(color) * scalar));

	return retcolor;
}

unsigned int pgeGfxColorDivScalar(unsigned int color, float scalar)
{
	if(scalar != 0.0f)
	{
		float mulscalar = 1.0f/scalar;
		
		unsigned int retcolor = GU_RGBA((int)(R(color) * mulscalar), (int)(G(color) * mulscalar), (int)(B(color) * mulscalar), (int)(A(color) * mulscalar));

		return retcolor;
	}
	
	return color;
}

int pgeGfxColorCompare(unsigned int color1, unsigned int color2)
{
	if(color1 == color2)
		return 1;
		
	return 0;
}

static int pgeGfxColorClampInt(int f)
{
	int x = f;
	
	if(x < 0)
	{
		x = 0;
	}
	else if(x > 255)
	{
		x = 255;
	}
	
	return x;
}

unsigned int pgeGfxColorClamp(unsigned int color)
{
	unsigned int color2 = GU_RGBA(pgeGfxColorClamp(R(color)), pgeGfxColorClamp(G(color)),  pgeGfxColorClamp(B(color)), pgeGfxColorClamp(A(color)));
	
	return color2;
}

//-------------------------------

#ifdef INCLUDE_HSV

pgeColorHSV pgeGfxColorHSVSub(pgeColorHSV col1, pgeColorHSV col2)
{
	pgeColorHSV col3;
	col3.h = col1.h - col2.h;
	col3.s = col1.s - col2.s;
	col3.v = col1.v - col2.v;
	col3.a = col1.a - col2.a;
	return col3;
}

pgeColorHSV pgeGfxColorHSVAdd(pgeColorHSV col1, pgeColorHSV col2)
{
	pgeColorHSV col3;
	col3.h = col1.h + col2.h;
	col3.s = col1.s + col2.s;
	col3.v = col1.v + col2.v;
	col3.a = col1.a + col2.a;
	return col3;
}

pgeColorHSV pgeGfxColorHSVMul(pgeColorHSV col1, pgeColorHSV col2)
{
	pgeColorHSV col3;
	col3.h = col1.h * col2.h;
	col3.s = col1.s * col2.s;
	col3.v = col1.v * col2.v;
	col3.a = col1.a * col2.a;
	return col3;
}

pgeColorHSV pgeGfxColorHSVDiv(pgeColorHSV color, float scalar)
{
	pgeColorHSV col2 = {0, 0, 0, 0};
	if(scalar != 0.0f)
	{
		col2.h = color.h / scalar;
		col2.s = color.s / scalar;
		col2.v = color.v / scalar;
		col2.a = color.a / scalar;
	}
	return col2;
}

int pgeGfxColorHSVCompare(pgeColorHSV col1, pgeColorHSV col2)
{
	if(col1.h == col2.h && col1.s == col1.s && col1.v == col2.v && col1.a == col2.a)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

pgeColorHSV pgeGfxColorHSVClamp(pgeColorHSV color)
{
	pgeColorHSV col2;
	col2.h = pgeGfxColorClampInt(color.h);
	col2.s = pgeGfxColorClampInt(color.s);
	col2.v = pgeGfxColorClampInt(color.v);
	col2.a = pgeGfxColorClampInt(color.a);
	return col2;
}

/*
void pgeGfxColorHSVSetHWColor(pgeColorHSV *color, u32 hwcol)
{
	float r, g, b;
	float minv, maxv, delta;
	float del_R, del_G, del_B;

	color->a = (hwcol>>24) / 255.0f;
	r = ((hwcol>>16) & 0xFF) / 255.0f;
	g = ((hwcol>>8) & 0xFF) / 255.0f;
	b = (hwcol & 0xFF) / 255.0f;

	minv = min(min(r, g), b);
	maxv = max(max(r, g), b);
	delta = maxv - minv;

	color->v = maxv;

	if (delta == 0)
	{
		color->h = 0;
		color->s = 0;
	}
	else
	{
		color->s = delta / maxv;
		del_R = (((maxv - r) / 6) + (delta / 2)) / delta;
		del_G = (((maxv - g) / 6) + (delta / 2)) / delta;
		del_B = (((maxv - b) / 6) + (delta / 2)) / delta;

		if      (r == maxv) {color->h = del_B - del_G;}
		else if (g == maxv) {color->h = (1 / 3) + del_R - del_B;}
		else if (b == maxv) {color->h = (2 / 3) + del_G - del_R;}

		if (color->h < 0) color->h += 1;
		if (color->h > 1) color->h -= 1;
	}
}
*/

u32 pgeGfxColorHSVGetHWColor(pgeColorHSV color)
{
	float r, g, b;
	float xh, i, p1, p2, p3;

	if (color.s == 0)
	{
		r = color.v;
		g = color.v;
		b = color.v;
	}
	else
	{
		xh = color.h * 6;
		if(xh == 6) xh=0;
		i = floorf(xh);
		p1 = color.v * (1 - color.s);
		p2 = color.v * (1 - color.s * (xh - i));
		p3 = color.v * (1 - color.s * (1 - (xh - i)));

		if      (i == 0) {r = color.v;  g = p3; b = p1;}
		else if (i == 1) {r = p2; g = color.v;  b = p1;}
		else if (i == 2) {r = p1; g = color.v;  b = p3;}
		else if (i == 3) {r = p1; g = p2; b = color.v; }
		else if (i == 4) {r = p3; g = p1; b = color.v; }
		else			 {r = color.v;  g = p1; b = p2;}
	}

	return (ARGB((int)(color.a*255.0f), (int)(r*255.0f), (int)(g*255.0f), (int)(b*255.0f)));
}

#endif
