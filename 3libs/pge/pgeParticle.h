/*
 * pgeParticle.h: Header for Particle Engine
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

#ifndef __PGEPARTICLE_H__
#define __PGEPARTICLE_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup pgeParticle Particle Engine Library
 *  @{
 */

#include "pgeMath.h"
#include "pgeGfx.h"

#define MAX_PARTICLES	500
#define MAX_PSYSTEMS	100

/**
* A particle datatype
*/
typedef	struct
{
	ScePspFVector3 vecLocation;	/**< location vector */
	ScePspFVector3 vecVelocity;	/**< velocity vector */

	float fGravity;							/**< gravity */
	float fRadialAccel;					/**< radial acceleration */
	float fTangentialAccel;			/**< tangential acceleration */

	float fSpin;								/**< spin */
	float fSpinDelta;						/**< delta spin */

	float fSize;								/**< size */
	float fSizeDelta;						/**< delta size */

	unsigned int colColor;				/**< color */
	unsigned int colColorDelta;	/**< delta color */

	float fAge;									/**< time the particle is alive */
	float fTerminalAge;					/**< time at which the particle dies */
} pgeParticle;

/**
* A particle system information datatype
*/
typedef struct
{
	pgeRect			spriteRect;					/**<  ::pgeRect describing the region on the texture to be displayed */
	pgeTexture	*spriteTexture;			/**<  texture for particles */
	int					spriteBlendMode;		/**<  blend mode */

	int			nEmission;							/**< particles per sec (0..MAX_PARTICLES,1) */
	float		fLifetime;							/**< lifetime of the system float(0..100,0.1) */

	float		fParticleLifeMin;				/**< min. lifetime per particle (0..5,0.01) */
	float		fParticleLifeMax;				/**< max. lifetime per particle (0..5,0.01) */

	float		fDirection;							/**< part. movement direction (0..2*PI,0.1) */
	float		fSpread;								/**< part. movement spread (0..2*PI,0.1) */
	int			bRelative;							/**< part. relative movement (0..1,1) */

	float		fSpeedMin;							/**< min. speed (-300..300,0.1) */
	float		fSpeedMax;							/**< max. speed (-300..300,0.1) */

	float		fGravityMin;						/**< min. gravity (-100..100,0.1) */
	float		fGravityMax;						/**< max. gravity (-100..100,0.1) */

	float		fRadialAccelMin;				/**< min. rad. accel (-5..5,0.01) */
	float		fRadialAccelMax;				/**< max. rad. accel (-5..5,0.01) */

	float		fTangentialAccelMin;		/**< min. tangential accel (-100..100,0.1) */
	float		fTangentialAccelMax;		/**< max. tangential accel (-100..100,0.1) */

	float		fSizeStart;							/**< part. size at the beginning (0.01..2,0.01) */
	float		fSizeEnd;								/**< part. size at the end (0.01..2,0.01) */
	float		fSizeVar;								/**< variance in part. size (0..1,0.01) */

	float		fSpinStart;							/**< part. spin at the beginning (-50,50,1) */
	float		fSpinEnd;								/**< part. spin at the end (-50,50,1) */
	float		fSpinVar;								/**< part. spin variance (0..1,0.01) */

	unsigned int	colColorStart;			/**< rgb color at the beginning 4xfloat(0..1,.01) */
	unsigned int	colColorEnd;				/**< rgb color at the end 4xfloat(0..1,.01) */
	float		fColorVar;							/**< variance float(0..1,0.01) */
	float		fAlphaVar;							/**< variance float(0..1,0.01) */
} pgeParticleSystemInfo;

/**
* A particle system datatype
*/
typedef struct {
	pgeParticleSystemInfo info;								/**< particle system information */
	float						age;											/**< time the system emits new particles */
	float						emissionResidue;					/**< UNKNOWN */
	ScePspFVector3	prevLocation;							/**< previous location of the system */
	ScePspFVector3	location;									/**< location of the system */
	float						fTx, fTy;									/**< translation coordinates */
	int							numParticlesAlive;				/**< number of active particles */
	pgeRect2				boundingBox;							/**< bounding box of the whole system */
	int							bUpdateBoundingBox;				/**< update bounding box on runtime */
	pgeParticle			particles[MAX_PARTICLES];	/**< array of ::pgeParticle */
	float						timer;										/**< timer */
} pgeParticleSystem;

/**
* Populate a ::pgeParticleSystem with default values.
*
* @param spritetexture - Pointer to an existing ::pgeTexture.
*
* @param spriterect - a ::pgeRect describing the region on the spritetexture to be drawn.
*/
void pgeParticleSystemInit(pgeParticleSystem *ps, pgeTexture *spritetexture, pgeRect spriterect);

/**
* Create a ::pgeParticleSystem with default values.
*
* @param spritetexture - Pointer to an existing ::pgeTexture.
*
* @param spriterect - a ::pgeRect describing the region on the spritetexture to be drawn.
*
* @returns Pointer to a pgeParticleSystem
*/
pgeParticleSystem *pgeParticleSystemCreate(pgeTexture *spritetexture, pgeRect spriterect);

/**
* Populate a ::pgeParticleSystem with values from memory.
*
* @param ps - Pointer to an existing ::pgeParticleSystem.
*
* @param psi - Pointer to a char array containing the byte representation of ::pgeParticleSystemInfo.
*
* @param sizePsi - size of the psi char array.
*
* @param spritetexture - Pointer to an existing ::pgeTexture.
*/
void pgeParticleSystemSetInfoFromMemory(pgeParticleSystem *ps, const char *psi, int sizePsi, pgeTexture *spritetexture);

/**
* Create a ::pgeParticleSystem with values from memory.
*
* @param psi - Pointer to a char array containing the byte representation of ::pgeParticleSystemInfo.
*
* @param sizePsi - size of the psi char array.
*
* @param spritetexture - Pointer to an existing ::pgeTexture.
*
* @returns Pointer to a pgeParticleSystem
*/
pgeParticleSystem *pgeParticleSystemLoadMemory(const char *psi, int sizePsi, pgeTexture *spritetexture);

/**
* Populate a ::pgeParticleSystem with values read from file.
*
* @param ps - Pointer to an existing ::pgeParticleSystem.
*
* @param filename - the name of the file.
*
* @param spritetexture - Pointer to an existing ::pgeTexture.
*
* @returns 1 if file loaded OK, else 0 (the ::pgeParticleSystem ps is populated with default values in that case).
*/
int pgeParticleSystemSetInfoFromFile(pgeParticleSystem *ps, const char *filename, pgeTexture *spritetexture);

/**
* Create a ::pgeParticleSystem with values read from file.
*
* @param filename - the name of the file.
*
* @param spritetexture - Pointer to an existing ::pgeTexture.
*
* @returns Pointer to a pgeParticleSystem
*/
pgeParticleSystem *pgeParticleSystemLoad(const char *filename, pgeTexture *spritetexture);

/**
* Destroy a ::pgeParticleSystem
*
* @param ps - Pointer to an existing ::pgeParticleSystem.
*/
void pgeParticleSystemDestroy(pgeParticleSystem *ps);

/**
* Render the current state of a ::pgeParticleSystem
*
* @param ps - Pointer to an existing ::pgeParticleSystem.
*/
void pgeParticleSystemRender(pgeParticleSystem *ps);

/**
* Fire a ::pgeParticleSystem at its current position
*
* @param ps - Pointer to an existing ::pgeParticleSystem.
*/
void pgeParticleSystemFire(pgeParticleSystem *ps);

/**
* Fire a ::pgeParticleSystem at a given coordinate
*
* @param ps - Pointer to an existing ::pgeParticleSystem.
*
* @param x - x coordinate.
*
* @param y - y coordinate
*/
void pgeParticleSystemFireAt(pgeParticleSystem *ps, float x, float y);

/**
* Stop a ::pgeParticleSystem from emitting new particles
*
* @param ps - Pointer to an existing ::pgeParticleSystem.
*
* @param killParticles - 1: already fired particles get killed 0: already fired particles stay.
*/
void pgeParticleSystemStop(pgeParticleSystem *ps, int killParticles);

/**
* Update a ::pgeParticleSystem
*
* @param ps - Pointer to an existing ::pgeParticleSystem.
*
* @param deltatime - time from last update.
*/
void pgeParticleSystemUpdate(pgeParticleSystem *ps, float deltatime);

/**
* Move a ::pgeParticleSystem to a new position
*
* @param ps - Pointer to an existing ::pgeParticleSystem.
*
* @param x - x coordinate.
*
* @param y - y coordinate
*/
void pgeParticleSystemMoveTo(pgeParticleSystem *ps, float x, float y, int moveParticles);

/**
* Transpose a ::pgeParticleSystem
*
* @param ps - Pointer to an existing ::pgeParticleSystem.
*
* @param x - x coordinate.
*
* @param y - y coordinate
*/
void pgeParticleSystemTranspose(pgeParticleSystem *ps, float x, float y);

/** @} */

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __PGEPARTICLE_H__
