/*
 * pgeMath.h: Header for VFPU math operations
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

#ifndef __PGEMATH_H__
#define __PGEMATH_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup pgeMath VFPU Math Library
 *  @{
 */
#include <psptypes.h>

/**
* Another rect datatype
*/
typedef struct
{
	float	x1, y1, x2, y2;
	int clean;
} pgeRect2;

/**
 * Calculate absolute value
 *
 * @param x - Input
 */
float pgeMathAbs(float x);

/**
 * Round value up
 *
 * @param x - Input
 */
float pgeMathCeil(float x);

/**
 * Round value down
 *
 * @param x - Input
 */
float pgeMathFloor(float x);

/**
 * Calculate inverse tangent (arctan)
 *
 * @param x - Input in radians
 */
float pgeMathAtan(float x);

/**
 * Calculate inverse tangent, with quadrant fix-up
 *
 * @param y - Input in radians
 *
 * @param x - Input in radians
 */
float pgeMathAtan2(float y, float x);

/**
 * Calculate square root
 *
 * @param x - Input
 */
float pgeMathSqrt(float x);

/**
 * Calculate cosine
 *
 * @param x - Input in radians
 */
float pgeMathCos(float rad);

/**
 * Calculate sine
 *
 * @param x - Input in radians
 */
float pgeMathSin(float rad);

/**
 * Calculate inverse cosine (arccos)
 *
 * @param x - Input in radians
 */
float pgeMathAcos(float x);

/**
 * Calculate inverse sine (arcsin)
 *
 * @param x - Input in radians
 */
float pgeMathAsin(float x);

/**
 * Calculate hyperbolic cosine
 *
 * @param x - Input in radians
 */
float pgeMathCosh(float x);

/**
 * Calculate exponent
 *
 * @param x - Input in radians
 */
float pgeMathExp(float x);

/**
 * Calculate maximum numeric value
 *
 * @param x - Input
 *
 * @param y - Input
 */
float pgeMathFmax(float x, float y);

/**
 * Calculate minimum numeric value
 *
 * @param x - Input
 *
 * @param y - Input
 */
float pgeMathFmin(float x, float y);

/**
 * Calculate floating point remainder of x/y
 *
 * @param x - Input
 *
 * @param y - Input
 */
float pgeMathFmod(float x, float y);

/**
 * Calculate natural logarithm
 *
 * @param x - Input in radians
 */
float pgeMathLog(float x);

/**
 * Calculate base 2 logarithm
 *
 * @param x - Input in radians
 */
float pgeMathLog2(float x);

/**
 * Calculate base 10 logarithm
 *
 * @param x - Input in radians
 */
float pgeMathLog10(float x);

/**
 * Calculate x raised to the power of y
 *
 * @param x - Number to raise power of
 *
 * @param y - Power to raise x by
 */
float pgeMathPow(float x, float y);

/**
 * Calculate 2 raised to the power of x
 *
 * @param x - Input
 */
float pgeMathPow2(float x);

/**
 * Round to nearest value
 *
 * @param x - Input
 */
float pgeMathRound(float x);

/**
 * Round towards 0
 *
 * @param x - Input
 */
float pgeMathTrunc(float x);

/**
 * Set random generator seed
 *
 * @param x - Seed value
 */
void pgeMathSrand(unsigned int x);

/**
 * Generate random float value
 *
 * @param min - Minimum value to return
 *
 * @param max - Maximum value to return
 *
 * @returns A value between min and max
 */
float pgeMathRandFloat(float min, float max);

/**
 * Generate random int value
 *
 * @param min - Minimum value to return
 *
 * @param max - Maximum value to return
 *
 * @returns A value between min and max
 */
int pgeMathRandInt(float min, float max);

/**
 * Calculate sine and cosine
 *
 * @param r - Input in radians
 *
 * @param s - Pointer to a float for sin result
 *
 * @param c - pointer to a float for cos result
*/
void pgeMathSincos(float r, float *s, float *c);

/**
 * Calculate hyperbolic sine
 *
 * @param x - Input in radians
 */
float pgeMathSinh(float x);

/**
 * Calculate tangent
 *
 * @param x - Input in radians
 */
float pgeMathTan(float x);

/**
 * Calculate hyperbolic tangent
 *
 * @param x - Input in radians
 */
float pgeMathTanh(float x);

/**
* Calculate inverse square root (1/sqrt(x))
*
* @param x - Input value
*/
float pgeMathInvSqrt(float x);

/**
* Calculate radian angle from euler angle
*
* @param x - Input value in degrees
*/
float pgeMathDegToRad(float x);

/**
* Calculate euler angle from radian angle
*
* @param x - Input value in degrees
*/
float pgeMathRadToDeg(float x);

/**
* Normalize an ::ScePSPFVector3
*
* @param result - pointer to an ::ScePSPFVector3 the result gets stored in
*
* @param vec - pointer to an ::ScePSPFVector3 to be normalized
*
* @returns a pointer to an ::ScePSPFVector3 as result
*/
ScePspFVector3 *pgeMathVecNormalize3f(ScePspFVector3 *result, const ScePspFVector3 *vec);

/**
* Add two ::ScePSPFVector3
*
* @param result - pointer to an ::ScePSPFVector3 the result gets stored in
*
* @param vec1 - pointer to an ::ScePSPFVector3 to be added
*
* @param vec2 - pointer to an ::ScePSPFVector3 to be added
*
* @returns a pointer to an ::ScePSPFVector3 as result
*/
ScePspFVector3 *pgeMathVecAdd3f(ScePspFVector3 *result, const ScePspFVector3 *vec1, const ScePspFVector3 *vec2);

/**
* Subtract a ::ScePSPFVector3 from another ::ScePSPFVector3
*
* @param result - pointer to an ::ScePSPFVector3 the result gets stored in
*
* @param vec1 - pointer to an ::ScePSPFVector3 as minuend
*
* @param vec2 - pointer to an ::ScePSPFVector3 as subtrahend
*
* @returns a pointer to an ::ScePSPFVector3 as result
*/
ScePspFVector3 *pgeMathVecSub3f(ScePspFVector3 *result, const ScePspFVector3 *vec1, const ScePspFVector3 *vec2);

/**
* Compare two ::ScePSPFVector3
*
* @param vec1 - pointer to an ::ScePSPFVector3 for comparison
*
* @param vec2 - pointer to an ::ScePSPFVector3 for comparison
*
* @returns 1 if vec1 == vec2, else 0
*/
int pgeMathVecCompare3f(const ScePspFVector3 *vec1, const ScePspFVector3 *vec2);

/**
* Scale a ::ScePSPFVector3 by a scalar
*
* @param result - pointer to an ::ScePSPFVector3 the result gets stored in
*
* @param vec - pointer to an ::ScePSPFVector3 for multiplication
*
* @param scalar - float for multiplication
*
* @returns a pointer to an ::ScePSPFVector3 as result
*/
ScePspFVector3 *pgeMathVecScale3f(ScePspFVector3 *result, const ScePspFVector3 *vec, float scalar);

/**
* Calculate the dot product of two ::ScePSPFVector3
*
* @param vec1 - pointer to an ::ScePSPFVector3 as Input
*
* @param vec2 - pointer to an ::ScePSPFVector3 as Input
*
* @returns float result of the dot product of vec1 and vec2
*/
float pgeMathVecDot3f(const ScePspFVector3 *vec1, const ScePspFVector3 *vec2);

/**
* Calculate the length of a ::ScePSPFVector3
*
* @param vec - pointer to an ::ScePSPFVector3 as Input
*
* @returns float result of the length calculation of vec
*/
float pgeMathVecLength3f(const ScePspFVector3 *vec);

/**
* Calculate the angle between two ::ScePSPFVector3
*
* @param vec1 - pointer to an ::ScePSPFVector3 as Input
*
* @param vec2 - pointer to an ::ScePSPFVector3 as Input
*
* @returns float result of the dot product of vec1 and vec2
*/
float pgeMathVecAngle3f(const ScePspFVector3 *vec1, const ScePspFVector3 *vec2);

/**
* Rotate a ::ScePSPFVector3 about the Z-axis
*
* @param vec - pointer to an ::ScePSPFVector3 as Input
*
* @param a - float Input angle in radians
*
* @param result - pointer to an ::ScePSPFVector3 the result gets stored in
*
* @returns pointer to the ::ScePSPFVector3 result
*/
ScePspFVector3 *pgeMathVecRotateZ3f(const ScePspFVector3 *vec, float a, ScePspFVector3 *result);

/**
* Clear a ::pgeRect2
*
* @param rect - pointer to a ::pgeRect2 as Input
*/
void pgeMathRectClear(pgeRect2 *rect);

/**
* Set the coordinates of a ::pgeRect2
*
* @param rect - pointer to a ::pgeRect2 to be processed
*
* @param x - x coordinate of the center
*
* @param y - y coordinate of the center
*
* @param r - radius of the rect's inner circle
*/
void pgeMathRectSetRadius(pgeRect2 *rect, float x, float y, float r);

/**
* Encapsulate a point into a ::pgeRect2
*
* @param rect - pointer to a ::pgeRect2 to be processed
*
* @param x - float x coordinate
*
* @param y - float y coordinate
*/
void pgeMathRectEncapsulate(pgeRect2 *rect, float x, float y);

/**
* Test if a point lies within a ::pgeRect2
*
* @param rect - pointer to a ::pgeRect2 as Input
*
* @param x - float x coordinate
*
* @param y - float y coordinate
*
* @returns int 1 if x,y is withing rect, else 0
*/
int pgeMathRectTestPoint(pgeRect2 *rect, float x, float y);

/**
* Test if two ::pgeRect2 intersect
*
* @param rect1 - pointer to a ::pgeRect2 as Input
*
* @param rect2 - pointer to a ::pgeRect2 as Input
*
* @returns int 1 if rect1 intersects rect2, else 0
*/
int pgeMathRectIntersect(pgeRect2 *rect1, pgeRect2 *rect2);

/** @} */

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __PGEMATH_H__
