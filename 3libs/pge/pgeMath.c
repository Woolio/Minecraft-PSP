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

#include <math.h>

#include "pgeMath.h"

#define PI   3.14159265358979f
#define PI_2 1.57079632679489f

#define radToDeg(x) ((x)*180.f/PI)
#define degToRad(x) ((x)*PI/180.f)

float pgeMathAbs(float x)
{
	float result;

	__asm__ volatile (
		"mtv      %1, S000\n"
		"vabs.s   S000, S000\n"
		"mfv      %0, S000\n"
	: "=r"(result) : "r"(x));

	return result;
}

float pgeMathCeil(float x)
{
	float result;
	
	__asm__ volatile (
		"mtv      %1, S000\n"
		"vf2iu.s  S000, S000, 0\n"
		"vi2f.s	  S000, S000, 0\n"
		"mfv      %0, S000\n"
	: "=r"(result) : "r"(x));
	
	return result;
}

float pgeMathFloor(float x)
{
	float result;
	
	__asm__ volatile (
		"mtv      %1, S000\n"
		"vf2id.s  S000, S000, 0\n"
		"vi2f.s	  S000, S000, 0\n"
		"mfv      %0, S000\n"
	: "=r"(result) : "r"(x));
	
	return result;
}

float pgeMathAtan(float x)
{
	float result;

	__asm__ volatile (
		"mtv      %1, S000\n"
		"vmul.s   S001, S000, S000\n"
		"vadd.s   S001, S001, S001[1]\n"
		"vrsq.s   S001, S001\n"
		"vmul.s   S000, S000, S001\n"
		"vasin.s  S000, S000\n"
		"vcst.s   S001, VFPU_PI_2\n"
		"vmul.s   S000, S000, S001\n"
		"mfv      %0, S000\n"
	: "=r"(result) : "r"(x));

	return result;
}

float pgeMathAtan2(float y, float x)
{
	float r;

	if (pgeMathAbs(x) >= pgeMathAbs(y))
	{
		r = pgeMathAtan(y/x);

		if (x < 0.0f)
			r += (y >= 0.0f ? PI : -PI);
	}
	else
	{
		r = -pgeMathAtan(x/y);
		r += (y < 0.0f ? -PI_2 : PI_2);
	}

	return r;
}

float pgeMathSqrt(float x)
{
	float result;
	__asm__ volatile (
		"mtv     %1, S000\n"
		"vsqrt.s S000, S000\n"
		"mfv     %0, S000\n"
	: "=r"(result) : "r"(x));
	return result;
}

float pgeMathCos(float rad)
{
    float result;
    __asm__ volatile (
        "mtv     %1, S000\n"
        "vcst.s  S001, VFPU_2_PI\n"
        "vmul.s  S000, S000, S001\n"
        "vcos.s  S000, S000\n"
        "mfv     %0, S000\n"
        : "=r"(result) : "r"(rad));
    return result;
}

float pgeMathSin(float rad)
{
    float result;
    __asm__ volatile (
        "mtv     %1, S000\n"
        "vcst.s  S001, VFPU_2_PI\n"
        "vmul.s  S000, S000, S001\n"
        "vsin.s  S000, S000\n"
        "mfv     %0, S000\n"
        : "=r"(result) : "r"(rad));
    return result;
}

float pgeMathAcos(float x)
{
    float result;
    __asm__ volatile (
        "mtv     %1, S000\n"
        "vcst.s  S001, VFPU_PI_2\n"
        "vasin.s S000, S000\n"
        "vocp.s  S000, S000\n"
        "vmul.s  S000, S000, S001\n"
        "mfv     %0, S000\n"
        : "=r"(result) : "r"(x));
    return result;
}

float pgeMathAsin(float x)
{
    float result;
    __asm__ volatile (
        "mtv     %1, S000\n"
        "vcst.s  S001, VFPU_PI_2\n"
        "vasin.s S000, S000\n"
        "vmul.s  S000, S000, S001\n"
        "mfv     %0, S000\n"
        : "=r"(result) : "r"(x));
    return result;
}

float pgeMathCosh(float x)
{
	float result;
	__asm__ volatile (
		"mtv      %1, S000\n"
		"vcst.s   S001, VFPU_LN2\n"
		"vrcp.s   S001, S001\n"
		"vmov.s   S002, S000[|x|]\n"
		"vmul.s   S002, S001, S002\n"
		"vexp2.s  S002, S002\n"
		"vrcp.s   S003, S002\n"
		"vadd.s   S002, S002, S003\n"
		"vmul.s   S002, S002, S002[1/2]\n"
		"mfv      %0, S002\n"
	: "=r"(result) : "r"(x));
	return result;
}

float pgeMathExp(float x)
{
    float result;
    __asm__ volatile (
        "mtv     %1, S000\n"
        "vcst.s  S001, VFPU_LN2\n"
        "vrcp.s  S001, S001\n"
        "vmul.s  S000, S000, S001\n"
        "vexp2.s S000, S000\n"
        "mfv     %0, S000\n"
        : "=r"(result) : "r"(x));
    return result;
}

float pgeMathFmax(float x, float y)
{
	float result;
	__asm__ volatile (
		"mtv      %1, S000\n"
		"mtv      %2, S001\n"
		"vmax.s   S002, S000, S001\n"
		"mfv      %0, S002\n"
	: "=r"(result) : "r"(x), "r"(y));
	return result;
}

float pgeMathFmin(float x, float y)
{
	float result;
	__asm__ volatile (
		"mtv      %1, S000\n"
		"mtv      %2, S001\n"
		"vmin.s   S002, S000, S001\n"
		"mfv      %0, S002\n"
	: "=r"(result) : "r"(x), "r"(y));
	return result;
}

float pgeMathFmod(float x, float y)
{
	float result;
	// return x-y*((int)(x/y));
	__asm__ volatile (
		"mtv       %2, S001\n"
		"mtv       %1, S000\n"
		"vrcp.s    S002, S001\n"
		"vmul.s    S003, S000, S002\n"
		"vf2iz.s   S002, S003, 0\n"
		"vi2f.s    S003, S002, 0\n"
		"vmul.s    S003, S003, S001\n"
		"vsub.s    S000, S000, S003\n"
		"mfv       %0, S000\n"
	: "=r"(result) : "r"(x), "r"(y));
	return result;
}

float pgeMathLog(float x)
{
    float result;
    __asm__ volatile (
        "mtv     %1, S000\n"
        "vcst.s  S001, VFPU_LOG2E\n"
        "vrcp.s  S001, S001\n"
        "vlog2.s S000, S000\n"
        "vmul.s  S000, S000, S001\n"
        "mfv     %0, S000\n"
        : "=r"(result) : "r"(x));
    return result;
}

float pgeMathLog2(float x)
{
    float result;
    __asm__ volatile (
        "mtv     %1, S000\n"
        "vlog2.s S000, S000\n"
        "mfv     %0, S000\n"
        : "=r"(result) : "r"(x));
	
    return result;
}

float pgeMathLog10(float x)
{
    float result;
    __asm__ volatile (
        "mtv     %1, S000\n"
        "vcst.s  S001, VFPU_LOG2TEN\n"
        "vrcp.s  S001, S001\n"
        "vlog2.s S000, S000\n"
        "vmul.s  S000, S000, S001\n"
        "mfv     %0, S000\n"
        : "=r"(result) : "r"(x));
    return result;
}

float pgeMathPow(float x, float y)
{
	float result;
	// result = exp2f(y * log2f(x));
	__asm__ volatile (
		"mtv      %1, S000\n"
		"mtv      %2, S001\n"
		"vlog2.s  S001, S001\n"
		"vmul.s   S000, S000, S001\n"
		"vexp2.s  S000, S000\n"
		"mfv      %0, S000\n"
	: "=r"(result) : "r"(x), "r"(y));
	return result;
}

float pgeMathPow2(float x)
{
	float result;

	__asm__ volatile (
		"mtv      %1, S000\n"
		"vexp2.s  S000, S000\n"
		"mfv      %0, S000\n"
	: "=r"(result) : "r"(x));
	
	return result;
}

float pgeMathTrunc(float x)
{
	float result;

	__asm__ volatile (
		"mtv      %1, S000\n"
		"vf2iz.s  S000, S000, 0\n"
		"vi2f.s	  S000, S000, 0\n"
		"mfv      %0, S000\n"
	: "=r"(result) : "r"(x));
	
	return result;
}

float pgeMathRound(float x)
{
	float result;

	__asm__ volatile (
		"mtv      %1, S000\n"
		"vf2in.s  S000, S000, 0\n"
		"vi2f.s	  S000, S000, 0\n"
		"mfv      %0, S000\n"
	: "=r"(result) : "r"(x));
	
	return result;
}

void pgeMathSrand(unsigned int x)
{
	__asm__ volatile (
		"mtv %0, S000\n"
		"vrnds.s S000"
		: "=r"(x));
}

float pgeMathRandFloat(float min, float max)
{
    float result;
    __asm__ volatile (
		"mtv      %1, S000\n"
        "mtv      %2, S001\n"
        "vsub.s   S001, S001, S000\n"
        "vrndf1.s S002\n"
        "vone.s	  S003\n"
        "vsub.s   S002, S002, S003\n"
        "vmul.s   S001, S002, S001\n"
        "vadd.s   S000, S000, S001\n"
        "mfv      %0, S000\n"
        : "=r"(result) : "r"(min), "r"(max));
    return result;
}

int pgeMathRandInt(float min, float max)
{
    float result = pgeMathRandFloat(min, max);
	
	return (int)result;
}

void pgeMathSincos(float r, float *s, float *c)
{
	__asm__ volatile (
		"mtv      %2, S002\n"
		"vcst.s   S003, VFPU_2_PI\n"
		"vmul.s   S002, S002, S003\n"
		"vrot.p   C000, S002, [s, c]\n"
		"mfv      %0, S000\n"
		"mfv      %1, S001\n"
	: "=r"(*s), "=r"(*c): "r"(r));
}

float pgeMathSinh(float x)
{
	float result;
	__asm__ volatile (
		"mtv      %1, S000\n"
		"vcst.s   S001, VFPU_LN2\n"
		"vrcp.s   S001, S001\n"
		"vmov.s   S002, S000[|x|]\n"
		"vcmp.s   NE, S000, S002\n"
        "vmul.s   S002, S001, S002\n"
        "vexp2.s  S002, S002\n"
        "vrcp.s   S003, S002\n"
        "vsub.s   S002, S002, S003\n"
        "vmul.s   S002, S002, S002[1/2]\n"
        "vcmov.s  S002, S002[-x], 0\n"
        "mfv      %0, S002\n"
	: "=r"(result) : "r"(x));
	return result;
}

float pgeMathTan(float x)
{
	float result;
	// result = sin(x)/cos(x);
	__asm__ volatile (
		"mtv      %1, S000\n"
		"vcst.s   S001, VFPU_2_PI\n"
        "vmul.s   S000, S000, S001\n"
        "vrot.p   C002, S000, [s, c]\n"
        "vdiv.s   S000, S002, S003\n"
        "mfv      %0, S000\n"
	: "=r"(result) : "r"(x));
	return result;
}

float pgeMathTanh(float x)
{
	float result;
	//y = exp(x+x);
	//return (y-1)/(y+1);
	__asm__ volatile (
		"mtv      %0, S000\n"
		"vadd.s   S000, S000, S000\n"
		"vcst.s   S001, VFPU_LN2\n"
		"vrcp.s   S001, S001\n"
		"vmul.s   S000, S000, S001\n"
        "vexp2.s  S000, S000\n"
        "vone.s   S001\n"
        "vbfy1.p  C002, C000\n"
        "vdiv.s   S000, S003, S002\n"
        "mfv      %0, S000\n"
	: "=r"(result): "r"(x));
	return result;
}

float pgeMathInvSqrt(float x)
{
	float result;
	
	// return 1.0f/sqrtf(x);
	
	__asm__ volatile (
		"mtv		%0, S000\n"
		"vrsq.s		S000, S000\n"
		"mfv		%0, S000\n"
	: "=r"(result): "r"(x));
	return result;
}

float pgeMathDegToRad(float x)
{
     return degToRad(x);
}

float pgeMathRadToDeg(float x)
{
     return radToDeg(x);
}

ScePspFVector3 *pgeMathVecAdd3f(ScePspFVector3 *result, const ScePspFVector3 *vec1, const ScePspFVector3 *vec2)
{	
	/*result->x = vec1->x + vec2->x;
	result->y = vec1->y + vec2->y;
	result->z = vec1->z + vec2->z;*/
	
	__asm__ volatile(
		".set			push\n"
		".set			noreorder\n"
		"lv.s			S000, 0 + %1\n"
		"lv.s			S001, 4 + %1\n"
		"lv.s			S002, 8 + %1\n"
		"lv.s			S010, 0 + %2\n"
		"lv.s			S011, 4 + %2\n"
		"lv.s			S012, 8 + %2\n"
		"vadd.t			C000, C000, C010\n"
		"sv.s			S000, 0 + %0\n"
		"sv.s			S001, 4 + %0\n"
		"sv.s			S002, 8 + %0\n"
		".set			pop\n"
		: "=m"(*result) : "m"(*vec1), "m"(*vec2));
	
	return result;
}

ScePspFVector3 *pgeMathVecSub3f(ScePspFVector3 *result, const ScePspFVector3 *vec1, const ScePspFVector3 *vec2)
{	
	/*result->x = vec1->x - vec2->x;
	result->y = vec1->y - vec2->y;
	result->z = vec1->z - vec2->z;*/
	
	__asm__ volatile(
		".set			push\n"
		".set			noreorder\n"
		"lv.s			S000, 0 + %1\n"
		"lv.s			S001, 4 + %1\n"
		"lv.s			S002, 8 + %1\n"
		"lv.s			S010, 0 + %2\n"
		"lv.s			S011, 4 + %2\n"
		"lv.s			S012, 8 + %2\n"
		"vsub.t			C000, C000, C010\n"
		"sv.s			S000, 0 + %0\n"
		"sv.s			S001, 4 + %0\n"
		"sv.s			S002, 8 + %0\n"
		".set			pop\n"
		: "=m"(*result) : "m"(*vec1), "m"(*vec2));
	
	return result;
}

int pgeMathVecCompare3f(const ScePspFVector3 *vec1, const ScePspFVector3 *vec2)
{
	return(vec1->x == vec2->x && vec1->y == vec2->y && vec1->z == vec2->z);
}

ScePspFVector3 *pgeMathVecScale3f(ScePspFVector3 *result, const ScePspFVector3 *vec, float scalar)
{
	/*ScePspFVector3 w;
	w.x = u.x*scalar;
	w.y = u.y*scalar;
	//w.z = u.z*scalar;
	w.z = 0.0f;
	return w;*/
	
	__asm__ volatile (
		".set			push\n"
		".set			noreorder\n"
		"mfc1			$8,   %2\n"
		"mtv			$8,   S010\n"
		"lv.s			S000, 0 + %1\n"
		"lv.s			S001, 4 + %1\n"
		"lv.s			S002, 8 + %1\n"
		"vscl.t			C000, C000, S010\n"
		"sv.s			S000, 0 + %0\n"
		"sv.s			S001, 4 + %0\n"
		"sv.s			S002, 8 + %0\n"
		".set			pop\n"
		: "=m"(*result) : "m"(*vec), "f"(scalar): "$8");
		
	return result;
}

float pgeMathVecDot3f(const ScePspFVector3 *vec1, const ScePspFVector3 *vec2)
{
	//return(vec1.x*vec2.x + vec1.y*vec2.y + vec1.z*vec2.z);
	
	float result;
	
	__asm__ volatile (
		".set			push\n"
		".set			noreorder\n"
		"lv.s			S000, 0 + %1\n"
		"lv.s			S001, 4 + %1\n"
		"lv.s			S002, 8 + %1\n"
		"lv.s			s010, 0 + %2\n"
		"lv.s			s011, 4 + %2\n"
		"lv.s			s012, 8 + %2\n"
		"vdot.t			S000, C000, C010\n"
		"sv.s			S000, %0\n"
		".set			pop\n"
		: "=m"(result) : "m"(*vec1), "m"(*vec2));

	return result;
	
}

float pgeMathVecLength3f(const ScePspFVector3 *vec)
{
	//return(pgeMathSqrtf(pgeMathVecDot3f(u, u)));
	
	float result;
	
	__asm__ volatile (
		".set			push\n"
		".set			noreorder\n"
		"lv.s			S000, 0 + %1\n"
		"lv.s			S001, 4 + %1\n"
		"lv.s			S002, 8 + %1\n"
		"vdot.t			S000, C000, C000\n"
		"vsqrt.s		S000, S000\n"
		"sv.s			S000, %0\n"
		".set			pop\n"
		: "=m"(result) : "m"(*vec));

	return result;
}

ScePspFVector3 *pgeMathVecNormalize3f(ScePspFVector3 *result, const ScePspFVector3 *vec)
{
/*
	ScePspFVector3 w;
	float leninv;

	if(!(u->x == 0.0f && u->y == 0.0f && u->z == 0.0f))
	{
		leninv = pgeMathVecLengthInv3f(*u);

		u->x *= leninv;
		u->y *= leninv;
		u->z *= leninv;
	}
*/
	__asm__ volatile(
		".set			push\n"
		".set			noreorder\n"
		"lv.s			S000, 0 + %1\n"
		"lv.s			S001, 4 + %1\n"
		"lv.s			S002, 8 + %1\n"
		"vdot.t			S010, C000, C000\n"
		"vzero.s		S011\n"
		"vcmp.s			EZ, S010\n"
		"vrsq.s			S010, S010\n"
		"vcmovt.s		S010, S011, 0\n"
		"vscl.t			C000[-1:1,-1:1,-1:1], C000, S010\n"
		"sv.s			S000, 0 + %0\n"
		"sv.s			S001, 4 + %0\n"
		"sv.s			S002, 8 + %0\n"
		".set			pop\n"
		: "=m"(*result) : "m"(*vec));
		
	return result;
}

float pgeMathVecAngle3f(const ScePspFVector3 *vec1, const ScePspFVector3 *vec2)
{
	ScePspFVector3 s;
	ScePspFVector3 t;
	
	pgeMathVecNormalize3f(&s, vec1);
	
	pgeMathVecNormalize3f(&t, vec2);
	
	return pgeMathAcos(pgeMathVecDot3f(&s, &t));
}

ScePspFVector3 *pgeMathVecRotateZ3f(const ScePspFVector3 *vec, float a, ScePspFVector3 *result)
{
	result->x = vec->x * pgeMathCos(a) - vec->y * pgeMathSin(a);
	result->y = vec->x * pgeMathSin(a) + vec->y * pgeMathCos(a);
	result->z = vec->z;
	
	return result;
}

void pgeMathRectClear(pgeRect2 *rect)
{
	rect->clean = 1;
}

void pgeMathRectSetRadius(pgeRect2 *rect, float x, float y, float r)
{
	rect->x1 = x-r;
	rect->x2 = x+r;
	rect->y1 = y-r;
	rect->y2 = y+r;
	rect->clean = 0;
}

void pgeMathRectEncapsulate(pgeRect2 *rect, float x, float y)
{
	if(rect->clean)
	{
		rect->x1 = rect->x2 = x;
		rect->y1 = rect->y2 = y;
		rect->clean = 0;
	}
	else
	{
		if(x < rect->x1) rect->x1 = x;
		if(x > rect->x2) rect->x2 = x;
		if(y < rect->y1) rect->y1 = y;
		if(y > rect->y2) rect->y2 = y;
	}
}

int pgeMathRectTestPoint(pgeRect2 *rect, float x, float y)
{
	if(x>=rect->x1 && x<rect->x2 && y>=rect->y1 && y<rect->y2)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int pgeMathRectIntersect(pgeRect2 *rect1, pgeRect2 *rect2)
{
	if(fabsf(rect1->x1 + rect1->x2 - rect2->x1 - rect2->x2) < (rect1->x2 - rect1->x1 + rect2->x2 - rect2->x1))
	{
		if(fabsf(rect1->y1 + rect1->y2 - rect2->y1 - rect2->y2) < (rect1->y2 - rect1->y1 + rect2->y2 - rect2->y1))
			return 1;
	}
	
	return 0;
}
