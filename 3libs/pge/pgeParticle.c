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

#include <string.h>
#include <malloc.h>
#include "pgeGfx.h"
#include "pgeFile.h"
#include "pgeParticle.h"

#define PI   3.14159265358979f
#define PI_2 1.57079632679489f

void pgeParticleSystemInit(pgeParticleSystem *ps, pgeTexture *spritetexture, pgeRect spriterect)
{
	memset(ps, 0, sizeof(pgeParticleSystem));

	ps->info.spriteRect = spriterect;
	ps->info.spriteTexture = spritetexture;
	ps->info.spriteBlendMode = GU_TFX_MODULATE; // GU_TFX_MODULATE, GU_TFX_DECAL, GU_TFX_BLEND, GU_TFX_REPLACE, GU_TFX_ADD

	ps->location = (ScePspFVector3){0.0f, 0.0f, 0.0f};
	ps->prevLocation = (ScePspFVector3){0.0f, 0.0f, 0.0f};

	ps->fTx = ps->fTy = 0;

	ps->emissionResidue = 0.0f;
	ps->numParticlesAlive = 0;
	ps->age = -2.0;

	ps->bUpdateBoundingBox = 0;
}

pgeParticleSystem *pgeParticleSystemCreate(pgeTexture *spritetexture, pgeRect spriterect)
{
	if(!spritetexture)
	{
		return NULL;
	}
	pgeParticleSystem *ps = (pgeParticleSystem *)pgeMalloc(sizeof(pgeParticleSystem));
	if(!ps)
	{
		return NULL;
	}

	pgeParticleSystemInit(ps, spritetexture, spriterect);

	return ps;
}

void pgeParticleSystemSetInfoFromMemory(pgeParticleSystem *ps, const char *psi, int sizePsi, pgeTexture *spritetexture)
{
	pgeRect rect = {0, 0, 32, 32};
	pgeParticleSystemInit(ps, spritetexture, rect);

	memcpy(&ps->info, psi, sizePsi);
}

pgeParticleSystem *pgeParticleSystemLoadMemory(const char *psi, int sizePsi, pgeTexture *spritetexture)
{
	pgeRect rect = {0, 0, 32, 32};
	pgeParticleSystem *ps = pgeParticleSystemCreate(spritetexture, rect);
	if(!ps)
	{
		return NULL;
	}
	pgeParticleSystemSetInfoFromMemory(ps, psi, sizePsi, spritetexture);
	return ps;
}

int pgeParticleSystemSetInfoFromFile(pgeParticleSystem *ps, const char *filename, pgeTexture *spritetexture)
{
	pgeRect rect = {0, 0, 32, 32};
	pgeParticleSystemInit(ps, spritetexture, rect);

	pgeFile *file = pgeFileOpen((char *)filename, PGE_FILE_RDONLY);
	if(file)
	{
		pgeFileRead(file, &ps->info, sizeof(pgeParticleSystemInfo));
		pgeFileClose(file);
		ps->info.spriteTexture = spritetexture;
		return 1;
	}
	return 0;
}

pgeParticleSystem *pgeParticleSystemLoad(const char *filename, pgeTexture *spritetexture)
{
	pgeRect rect = {0, 0, 32, 32};
	pgeParticleSystem *ps = pgeParticleSystemCreate(spritetexture, rect);
	if(!ps)
	{
		return NULL;
	}
	if(!pgeParticleSystemSetInfoFromFile(ps, filename, spritetexture))
	{
		pgeParticleSystemDestroy(ps);
		return NULL;
	}
	return ps;
}

void pgeParticleSystemDestroy(pgeParticleSystem *ps)
{
	if(!ps)
	{
		return;
	}
	pgeFree(ps);
	ps = NULL;
}

void pgeParticleSystemUpdate(pgeParticleSystem *ps, float deltatime)
{
	int i;
	float ang;
	pgeParticle *par;
	ScePspFVector3 vecAccel, vecAccel2, temp, temp2;

	if(ps->age >= 0)
	{
		ps->age += deltatime;
		if(ps->age >= ps->info.fLifetime) ps->age = -2.0f;
	}

	ps->timer += deltatime;
	if (ps->timer < 0.01f)
		return;

	deltatime = ps->timer;
	ps->timer = 0.0f;

	// update all alive particles

	//if(ps->bUpdateBoundingBox) rectBoundingBox.Clear();
	par=ps->particles;
	
	for(i=0; i<ps->numParticlesAlive; i++)
	{
		par->fAge += deltatime;
		if(par->fAge >= par->fTerminalAge)
		{
			ps->numParticlesAlive--;
			memcpy(par, &ps->particles[ps->numParticlesAlive], sizeof(pgeParticle));
			i--;
			continue;
		}

		pgeMathVecSub3f(&vecAccel, &par->vecLocation, &ps->location);
		pgeMathVecNormalize3f(&vecAccel, &vecAccel);
		vecAccel2 = vecAccel;
		pgeMathVecScale3f(&vecAccel, &vecAccel, par->fRadialAccel);

		ang = vecAccel2.x;
		vecAccel2.x = -vecAccel2.y;
		vecAccel2.y = ang;
		
		pgeMathVecScale3f(&vecAccel2, &vecAccel2, par->fTangentialAccel);
		//par->vecVelocity = pgeMathVecAdd3f(par->vecVelocity, pgeMathVecMul3f(pgeMathVecAdd3f(vecAccel, vecAccel2), deltatime));
		pgeMathVecAdd3f(&temp, &vecAccel, &vecAccel2);
		pgeMathVecScale3f(&temp, &temp, deltatime);
		pgeMathVecAdd3f(&par->vecVelocity, &par->vecVelocity, &temp);
		par->vecVelocity.y += par->fGravity*deltatime;

		//par->vecVelocity.y = 0.1f;
		pgeMathVecAdd3f(&par->vecLocation, &par->vecLocation, &par->vecVelocity);

		par->fSpin += par->fSpinDelta*deltatime;
		par->fSize += par->fSizeDelta*deltatime;
		par->colColor = pgeGfxColorRGBAdd(par->colColor, pgeGfxColorRGBMulScalar(par->colColorDelta, deltatime));

		//if(bUpdateBoundingBox) rectBoundingBox.Encapsulate(par->vecLocation.x, par->vecLocation.y);

		par++;
	}

	// generate new particles

	if(ps->age != -2.0f)
	{
		float fParticlesNeeded = ps->info.nEmission*deltatime + ps->emissionResidue;
		int nParticlesCreated = (unsigned int)fParticlesNeeded;
		ps->emissionResidue=fParticlesNeeded-nParticlesCreated;

		par=&ps->particles[ps->numParticlesAlive];

		for(i=0; i<nParticlesCreated; i++)
		{
			if(ps->numParticlesAlive>=MAX_PARTICLES) break;

			par->fAge = 0.0f;
			par->fTerminalAge = pgeMathRandFloat(ps->info.fParticleLifeMin, ps->info.fParticleLifeMax);
			
			pgeMathVecSub3f(&temp, &ps->location, &ps->prevLocation);
			pgeMathVecScale3f(&temp, &temp, pgeMathRandFloat(0.0f, 1.0f));

			pgeMathVecAdd3f(&par->vecLocation, &ps->prevLocation, &temp);
			par->vecLocation.x += pgeMathRandFloat(-2.0f, 2.0f);
			par->vecLocation.y += pgeMathRandFloat(-2.0f, 2.0f);

			ang=ps->info.fDirection-PI_2+pgeMathRandFloat(0,ps->info.fSpread)-ps->info.fSpread/2.0f;
			
			pgeMathVecSub3f(&temp, &ps->prevLocation, &ps->location);
			
			pgeMathVecSub3f(&temp2, &ps->prevLocation, &ps->location);
			
			//if(ps->info.bRelative)
			//	ang += pgeMathAtan2(temp.y, temp.x) + PI_2;
				
			par->vecVelocity.x = pgeMathCos(ang);
			par->vecVelocity.y = pgeMathSin(ang);
			pgeMathVecScale3f(&par->vecVelocity, &par->vecVelocity, pgeMathRandFloat(ps->info.fSpeedMin, ps->info.fSpeedMax));

			par->fGravity = pgeMathRandFloat(ps->info.fGravityMin, ps->info.fGravityMax);
			par->fRadialAccel = pgeMathRandFloat(ps->info.fRadialAccelMin, ps->info.fRadialAccelMax);
			par->fTangentialAccel = pgeMathRandFloat(ps->info.fTangentialAccelMin, ps->info.fTangentialAccelMax);

			par->fSize = pgeMathRandFloat(ps->info.fSizeStart, ps->info.fSizeStart+(ps->info.fSizeEnd-ps->info.fSizeStart)*ps->info.fSizeVar);
			par->fSizeDelta = (ps->info.fSizeEnd-par->fSize) / par->fTerminalAge;

			par->fSpin = pgeMathRandFloat(ps->info.fSpinStart, ps->info.fSpinStart+(ps->info.fSpinEnd-ps->info.fSpinStart)*ps->info.fSpinVar);
			par->fSpinDelta = (ps->info.fSpinEnd-par->fSpin) / par->fTerminalAge;

			par->colColor.r = pgeMathRandFloat(ps->info.colColorStart.r, ps->info.colColorStart.r+(ps->info.colColorEnd.r-ps->info.colColorStart.r)*ps->info.fColorVar);
			par->colColor.g = pgeMathRandFloat(ps->info.colColorStart.g, ps->info.colColorStart.g+(ps->info.colColorEnd.g-ps->info.colColorStart.g)*ps->info.fColorVar);
			par->colColor.b = pgeMathRandFloat(ps->info.colColorStart.b, ps->info.colColorStart.b+(ps->info.colColorEnd.b-ps->info.colColorStart.b)*ps->info.fColorVar);
			par->colColor.a = pgeMathRandFloat(ps->info.colColorStart.a, ps->info.colColorStart.a+(ps->info.colColorEnd.a-ps->info.colColorStart.a)*ps->info.fAlphaVar);

			par->colColorDelta.r = (ps->info.colColorEnd.r-par->colColor.r) / par->fTerminalAge;
			par->colColorDelta.g = (ps->info.colColorEnd.g-par->colColor.g) / par->fTerminalAge;
			par->colColorDelta.b = (ps->info.colColorEnd.b-par->colColor.b) / par->fTerminalAge;
			par->colColorDelta.a = (ps->info.colColorEnd.a-par->colColor.a) / par->fTerminalAge;

			//if(bUpdateBoundingBox) rectBoundingBox.Encapsulate(par->vecLocation.x, par->vecLocation.y);

			ps->numParticlesAlive++;
			par++;
		}
	}

	ps->prevLocation=ps->location;
}

void pgeParticleSystemStop(pgeParticleSystem *ps, int killParticles)
{
	ps->age=-2.0f;
	if(killParticles) 
	{
		ps->numParticlesAlive=0;
		//rectBoundingBox.Clear();
	}
}

void pgeParticleSystemMoveTo(pgeParticleSystem *ps, float x, float y, int moveParticles)
{
	int i;
	float dx,dy;

	if(moveParticles)
	{
		dx=x-ps->location.x;
		dy=y-ps->location.y;

		for(i=0;i<ps->numParticlesAlive;i++)
		{
			ps->particles[i].vecLocation.x += dx;
			ps->particles[i].vecLocation.y += dy;
		}

		ps->prevLocation.x += dx;
		ps->prevLocation.y += dy;
	}
	else
	{
		if(ps->age == -2.0)
		{
			ps->prevLocation.x = x;
			ps->prevLocation.y = y;
		}
		else
		{
			ps->prevLocation.x = ps->location.x;
			ps->prevLocation.y = ps->location.y;
		}
	}

	ps->location.x = x;
	ps->location.y = y;
}

void pgeParticleSystemFire(pgeParticleSystem *ps)
{
	ps->timer = 0.0f;

	if(ps->info.fLifetime==-1.0f) ps->age=-1.0f;
	else ps->age=0.0f;
}

void pgeParticleSystemFireAt(pgeParticleSystem *ps, float x, float y)
{
	pgeParticleSystemStop(ps, 0);
	pgeParticleSystemMoveTo(ps, x, y, 0);
	pgeParticleSystemFire(ps);
}

void pgeParticleSystemTranspose(pgeParticleSystem *ps, float x, float y)
{
	ps->fTx = x;
	ps->fTy = y;
}

void pgeParticleSystemRender(pgeParticleSystem *ps)
{
	int i;
	pgeRect destrect;

	pgeTextureModeSet(PGE_TEX_MODE_ADD);
	pgeTextureActivate(ps->info.spriteTexture);
	
	pgeParticle *par = ps->particles;
	for(i=0; i < ps->numParticlesAlive; i++)
	{
		destrect.x = -ps->info.spriteRect.w * par->fSize * 0.5f + par->vecLocation.x+ps->fTx;
		destrect.y = -ps->info.spriteRect.h * par->fSize * 0.5f + par->vecLocation.y+ps->fTy;
		destrect.w = ps->info.spriteRect.w * par->fSize;
		destrect.h = ps->info.spriteRect.h * par->fSize;
		pgeGfxDrawTextureColored(&destrect, &ps->info.spriteRect, par->fSpin*par->fAge, par->colColor);
		par++;
	}
}
