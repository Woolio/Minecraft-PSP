/*
 * pgeGfx.h: Header for Graphics
 *
 * This file is part of "Phoenix Game Engine".
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

#ifndef __PGEGFX_H__
#define __PGEGFX_H__

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup pgeGfx Graphics Library
 *  @{
 */

#include <pspgu.h>

#include "pgeTexture.h"
#include "pgeObj.h"

#define ARGB(a, r, g, b)		((a << 24) | (r << 16) | (g << 8) | b)
#define RGBA(r, g, b, a)		((a << 24) | (b << 16) | (g << 8) | r)
	
typedef struct
{
	float	x, y, z;
	
} pgeVertV;

typedef struct
{
	float	nx, ny, nz;
	float	x, y, z;
	
} pgeVertNV;

typedef struct
{
	unsigned int	color;
	float			x, y, z;
	
} pgeVertCV;

typedef struct
{
	unsigned int	color;
	float			nx, ny, nz;
	float			x, y, z;
	
} pgeVertCNV;

typedef struct
{
	float	u, v;
	float	x, y, z;
	
} pgeVertTV;

typedef struct
{
	float	u, v;
	float	nx, ny, nz;
	float	x, y, z;
	
} pgeVertTNV;

typedef struct
{
	float			u, v;
	unsigned int	color;
	float			x, y, z;
	
} pgeVertTCV;

typedef struct
{
	float			u, v;
	unsigned int	color;
	float			nx, ny, nz;
	float			x, y, z;
	
} pgeVertTCNV;

typedef struct
{
	float	x, y, w, h;
	
} pgeRect;

typedef struct
{
	ScePspFVector3 eye;
	ScePspFVector3 center;
	ScePspFVector3 up;
	
} pgeCamera;

/**
 * Enum for vertex types.
 */
enum pgeGfxVertexType
{
	PGE_VERT_V =	GU_VERTEX_32BITF,
	PGE_VERT_NV =	GU_NORMAL_32BITF|GU_VERTEX_32BITF,
	PGE_VERT_CV =	GU_COLOR_8888|GU_VERTEX_32BITF,
	PGE_VERT_CNV =	GU_COLOR_8888|GU_NORMAL_32BITF|GU_VERTEX_32BITF,
	PGE_VERT_TV =	GU_TEXTURE_32BITF|GU_VERTEX_32BITF,
	PGE_VERT_TNV =	GU_TEXTURE_32BITF|GU_NORMAL_32BITF|GU_VERTEX_32BITF,
	PGE_VERT_TCV =	GU_TEXTURE_32BITF|GU_COLOR_8888|GU_VERTEX_32BITF,
	PGE_VERT_TCNV =	GU_TEXTURE_32BITF|GU_COLOR_8888|GU_NORMAL_32BITF|GU_VERTEX_32BITF
};

/**
 * Enum for primitive types.
 */
enum pgeGfxPrim
{
	PGE_PRIM_POINTS,
	PGE_PRIM_LINES,
	PGE_PRIM_LINE_STRIP,
	PGE_PRIM_TRIANGLES,
	PGE_PRIM_TRIANGLE_STRIP,
	PGE_PRIM_TRIANGLE_FAN,
	PGE_PRIM_SPRITES
};

/**
 * Enum for blend modes.
 */
enum pgeBlendMode
{
	PGE_BLEND_MODE_TRANSPARENT,
	PGE_BLEND_MODE_REGULAR
};

/**
 * Enum for vertical sync.
 */
enum pgeWaitVsync
{
	PGE_NO_WAIT_VSYNC = 0,	/**< Don't wait for vblank. */
	PGE_WAIT_VSYNC			/**< Wait for vblank. */
};

#define PGE_GFX_PROJECTION	GU_PROJECTION
#define PGE_GFX_VIEW		GU_VIEW
#define PGE_GFX_MODEL		GU_MODEL

/**
 * Get current framebuffer
 *
 * @returns A pointer to the current framebuffer.
 */
unsigned int *pgeGfxGetFramebuffer(void);

/**
 * Init graphics.
 *
 * @param pixelformat - One of ::pgePixelFormat
 *
 * @returns 1 on success, 0 on error.
 */
int pgeGfxInit(enum pgePixelFormat pixelformat);

/**
 * Shutdown graphics.
 */
void pgeGfxShutdown(void);

/**
 * Start drawing.
 */
void pgeGfxStartDrawing(void);

/**
 * End drawing.
 */
void pgeGfxEndDrawing(void);

/**
 * Swap the framebuffers.
 *
 * @param vsync - One of ::pgeWaitVsync
 */
void pgeGfxSwapBuffers(enum pgeWaitVsync vsync);

/**
 * Clear the screen.
 *
 * @param color - The color to clear the screen to.
 */
void pgeGfxClearScreen(unsigned int color);

/**
 * Create a 32-bit color.
 *
 * @param r - The red value (0-255).
 *
 * @param g - The green value (0-255).
 *
 * @param b - The blue value (0-255).
 *
 * @param a - The alpha value (0-255).
 *
 * @returns The color in 0xAABBGGRR format.
 */
unsigned int pgeGfxCreateColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

/**
 * Render to a texture.
 *
 * @param texture - The texture to render to (must reside in VRAM).
 */
void pgeGfxRenderToTexture(pgeTexture *texture);

/**
 * Render to screen.
 *
 * @param color - The color to clear the screen to.
 */
void pgeGfxRenderToScreen(void);

/**
 * Set the blend mode.
 *
 * @param mode - One of ::pgeBlendMode.
 *
 * @returns The previous blend mode.
 */
int pgeGfxSetBlendMode(enum pgeBlendMode mode);

/**
 * Set 3D camera drawing mode.
 *
 * @param fov - The field of view to set.
 *
 * @param cam - Pointer to a ::pgeCamera.
 */
void pgeGfxSetMode3DCamera(float fov, pgeCamera *cam);

/**
 * Draw a line.
 *
 * @param startx - The start x position of the line.
 *
 * @param starty - The start y position of the line.
 *
 * @param endx - The end x position of the line.
 *
 * @param endy - The end y position of the line.
 *
 * @param color - The color of the line.
 */
void pgeGfxDrawLine(float startx, float starty, float endx, float endy, unsigned int color);

/**
 * Draw a star.
 *
 * @param x - The central position of the star on the x axis.
 *
 * @param y - The central position of the star on the y axis.
 *
 * @param outerradius - The outer radius of the star.
 *
 * @param innerradius - The inner radius of the star.
 *
 * @param numpoints - Number of points of the star.
 *
 * @param color - The color of the star.
 *
 * @param angle - The rotation of the star.
 */
void pgeGfxDrawStar(float x, float y, float outerradius, float innerradius, unsigned int numpoints, unsigned int color, float angle);

/**
 * Draw an outline star.
 *
 * @param x - The central position of the star on the x axis.
 *
 * @param y - The central position of the star on the y axis.
 *
 * @param outerradius - The outer radius of the star.
 *
 * @param innerradius - The inner radius of the star.
 *
 * @param numpoints - Number of points of the star.
 *
 * @param color - The color of the star.
 *
 * @param angle - The rotation of the star.
 */
void pgeGfxDrawStarOutline(float x, float y, float outerradius, float innerradius, unsigned int numpoints, unsigned int color, float angle);

/**
 * Draw a circle.
 *
 * @param x - The central position of the circle on the x axis.
 *
 * @param y - The central position of the circle on the y axis.
 *
 * @param radius - The radius of the circle.
 *
 * @param numsteps - Number of steps to take when generating the circle (higher is better quality).
 *
 * @param color - The color of the circle.
 */
void pgeGfxDrawCircle(float x, float y, float radius, unsigned int numsteps, unsigned int color);

/**
 * Draw an outline circle.
 *
 * @param x - The central position of the circle on the x axis.
 *
 * @param y - The central position of the circle on the y axis.
 *
 * @param radius - The radius of the circle.
 *
 * @param numsteps - Number of steps to take when generating the circle (higher is better quality).
 *
 * @param color - The color of the circle.
 */
void pgeGfxDrawCircleOutline(float x, float y, float radius, unsigned int numsteps, unsigned int color);

/**
 * Draw a rectangle.
 *
 * @param destrect - A ::pgeRect that contains the x, y, width & height.
 *
 * @param color - The color of the rectangle.
 *
 * @param angle - The rotation of the rectangle.
 */
void pgeGfxDrawRect(pgeRect *destrect, unsigned int color, float angle);

/**
 * Draw an outline rectangle.
 *
 * @param destrect - A ::pgeRect that contains the x, y, width & height.
 *
 * @param color - The color of the rectangle.
 *
 * @param angle - The rotation of the rectangle.
 */
void pgeGfxDrawRectOutline(pgeRect *destrect, unsigned int color, float angle);

/**
 * Draw a gradient rectangle.
 *
 * @param destrect - A ::pgeRect that contains the x, y, width & height.
 *
 * @param color1 - The color of the rectangle's XXX XXX point.
 *
 * @param color2 - The color of the rectangle's XXX XXX point.
 *
 * @param color3 - The color of the rectangle's XXX XXX point.
 *
 * @param color4 - The color of the rectangle's XXX XXX point.
 *
 * @param angle - The rotation of the rectangle.
 */
void pgeGfxDrawRectGrad(pgeRect *destrect, unsigned int color1, unsigned int color2, unsigned int color3, unsigned int color4, float angle);

/**
 * Draw a texture (easy).
 *
 * @param texture - A ::pgeTexture.
 *
 * @param x - The x position of the texture.
 *
 * @param y - The y position of the texture.
 *
 * @param angle - The rotation of the texture.
 *
 * @param alpha - The transparency of the texture (0-255).
 */
void pgeGfxDrawTextureEasy(pgeTexture *texture, float x, float y, float angle, unsigned char alpha);

/**
 * Draw a texture.
 *
 * @param destrect - A ::pgeRect that contains the x, y, width & height.
 *
 * @param sourcerect - A ::pgeRect that contains the u, v, width & height.
 *
 * @param angle - The rotation of the texture.
 *
 * @param alpha - The transparency of the texture (0-255).
 */
void pgeGfxDrawTexture(pgeRect *destrect, pgeRect *sourcerect, float angle, unsigned char alpha);

/**
* Draw a texture in a given color.
*
* @param destrect - A ::pgeRect that contains the x, y, width & height.
*
* @param sourcerect - A ::pgeRect that contains the u, v, width & height.
*
* @param angle - The rotation of the texture.
*
* @param color - A ::pgeColorRGB specifying the color in which the texture should be drawn
*/
void pgeGfxDrawTextureColored(pgeRect *destrect, pgeRect *sourcerect, float angle, unsigned int color);

/**
 * Get display list memory.
 *
 * @param size - Display list size required (in bytes).
 *
 * @returns A pointer to the memory.
 */
void *pgeGfxGetVertexMemory(int size);

/**
 * Draw custom (2D).
 *
 * @param prim - One of ::pgeGfxPrim.
 *
 * @param vtype - One of ::pgeGfxVertexType.
 *
 * @param count - Number of vertices.
 *
 * @param vertices - A pointer to the vertex data.
 */
void pgeGfxDrawCustom2D(enum pgeGfxPrim prim, enum pgeGfxVertexType vtype, int count, const void *vertices);

/**
 * Draw custom (3D).
 *
 * @param position - Vector holding the position.
 *
 * @param rotation - Vector holding the rotation.
 *
 * @param prim - One of ::pgeGfxPrim.
 *
 * @param vtype - One of ::pgeGfxVertexType.
 *
 * @param count - Number of vertices.
 *
 * @param vertices - A pointer to the vertex data.
 */
void pgeGfxDrawCustom3D(ScePspFVector3 *position, ScePspFVector3 *rotation, enum pgeGfxPrim prim, enum pgeGfxVertexType vtype, int count, const void *vertices);

/**
 * Draw texture (3D).
 *
 * @param position - Vector holding the position.
 *
 * @param rotation - Vector holding the rotation.
 *
 * @param width - Width to be drawn.
 *
 * @param height - Height to be drawn.
 */
void pgeGfxDrawTexture3D(ScePspFVector3 *position, ScePspFVector3 *rotation, float width, float height);

/**
* Draw texture (3D) in a given color.
*
* @param position - Vector holding the position.
*
* @param rotation - Vector holding the rotation.
*
* @param width - Width to be drawn.
*
* @param height - Height to be drawn.
*
* @param color - pgeColorRGB containing the color and alpha the texture has to be drawn in.
*/
void pgeGfxDrawTexture3DColored(ScePspFVector3 *position, ScePspFVector3 *rotation, float width, float height, unsigned int color);

/**
 * Draw a cube.
 *
 * @param position - Vector holding the position.
 *
 * @param rotation - Vector holding the rotation.
 *
 * @param width - The width of the cube.
 *
 * @param height - The height of the cube.
 *
 * @param depth - The depth of the cube.
 *
 * @param color - The color of the cube.
 */
void pgeGfxDrawCube(ScePspFVector3 *position, ScePspFVector3 *rotation, float width, float height, float depth, unsigned int color);

/**
 * Draw a textured cube.
 *
 * @param position - Vector holding the position.
 *
 * @param rotation - Vector holding the rotation.
 *
 * @param width - The width of the cube.
 *
 * @param height - The height of the cube.
 *
 * @param depth - The depth of the cube.
 */
void pgeGfxDrawCubeTexture(ScePspFVector3 *position, ScePspFVector3 *rotation, float width, float height, float depth);

/**
 * Draw an OBJ file.
 *
 * @param obj - Pointer to a ::pgeObj.
 *
 * @param position - Vector holding the position.
 *
 * @param rotation - Vector holding the rotation.
 */
void pgeGfxDrawObj(pgeObj *obj, ScePspFVector3 *position, ScePspFVector3 *rotation);

void pgeGfxDrawTextureColored(pgeRect *destrect, pgeRect *sourcerect, float angle, unsigned int color);

int pgeGfxGetMatrix(ScePspFMatrix4 *m, int mode);

void pgeGfxRotateY(float angle);

void pgeGfxRotateX(float angle);

void pgeGfxLookAt(ScePspFVector3* eye, ScePspFVector3* center, ScePspFVector3* up);

void pgeGfxPerspective(float fovy, float aspect, float near, float far);

void pgeGfxMatrixMode(int mode);

void pgeGfxLoadIdentity(void);

int pgeGfxProject(ScePspFVector3 *object, ScePspFVector3 *screen);

int pgeGfxUnproject(ScePspFVector3 *screen, ScePspFVector3 *object);

void pgeGfxSetTransparent(unsigned int color);

void pgeGfxUnsetTransparent(void);

unsigned int pgeGfxColorSub(unsigned int color1, unsigned int color2);
unsigned int pgeGfxColorAdd(unsigned int color1, unsigned int color2);
unsigned int pgeGfxColorMul(unsigned int color1, unsigned int color2);
unsigned int pgeGfxColorMulScalar(unsigned int color, float scalar);
unsigned int pgeGfxColorDivScalar(unsigned int color, float scalar);
int pgeGfxColorCompare(unsigned int color1, unsigned int color2);
unsigned int pgeGfxColorClamp(unsigned int color);

#ifdef INCLUDE_HSV

//pgeColorHSV pgeGfxColorHSVSub(pgeColorHSV col1, pgeColorHSV col2);
//pgeColorHSV pgeGfxColorHSVAdd(pgeColorHSV col1, pgeColorHSV col2);
//pgeColorHSV pgeGfxColorHSVMul(pgeColorHSV col1, pgeColorHSV col2);
//pgeColorHSV pgeGfxColorHSVDiv(pgeColorHSV color, float scalar);
//int pgeGfxColorHSVCompare(pgeColorHSV col1, pgeColorHSV col2);
//pgeColorHSV pgeGfxColorHSVClamp(pgeColorHSV color);
//void pgeGfxColorHSVSetHWColor(pgeColorHSV *color, u32 hwcol);
//u32 pgeGfxColorHSVGetHWColor(pgeColorHSV color);

#endif


/** @} */

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __PGEGFX_H__
