#ifndef LAMEFUNCTIONS_H
#define LAMEFUNCTIONS_H

#define PI 3.1415926535897f

#include <stdio.h>
#include <string>
#include <math.h>

#include <psptypes.h>
#include <pspgu.h>
#include <pspgum.h>
#include <pspdisplay.h>
#include <pspkernel.h>

inline float ATAN2(float dx, float dz)
{
    if(dx == 0 && dz == 0)
    {
        return 0.0f;
    }
    return atan2f(dx,dz);
}

#define PI_FLOAT     3.14159265f
#define PIBY2_FLOAT  1.5707963f
inline float ATAN2_fast(float y, float x)
{
	if ( x == 0.0f )
	{
		if ( y > 0.0f ) return PIBY2_FLOAT;
		if ( y == 0.0f ) return 0.0f;
		return -PIBY2_FLOAT;
	}
	float atan;
	float z = y/x;
	if ( fabs( z ) < 1.0f )
	{
		atan = z/(1.0f + 0.28f*z*z);
		if ( x < 0.0f )
		{
			if ( y < 0.0f ) return atan - PI_FLOAT;
			return atan + PI_FLOAT;
		}
	}
	else
	{
		atan = PIBY2_FLOAT - z/(z*z + 0.28f);
		if ( y < 0.0f ) return atan - PI_FLOAT;
	}
	return atan;
}

inline void MatrixAngle(Vector3 angle)
{
    sceGumRotateX(angle.x/180*PI);
    sceGumRotateY(angle.y/180*PI);
    sceGumRotateZ(angle.z/180*PI);
}

inline void MatrixRadianAngle(Vector3 angle)
{
    sceGumRotateX(angle.x);
    sceGumRotateY(angle.y);
    sceGumRotateZ(angle.z);
}

inline void MatrixScale(Vector3 scale)
{
    ScePspFVector3 sca = {scale.x,scale.y,scale.z};
    sceGumScale(&sca);
}

inline void MatrixTranslation(Vector3 pos)
{
    ScePspFVector3 loc = {pos.x,pos.y,pos.z};
    sceGumTranslate(&loc);
}

inline void MatrixPush()
{
    sceGumPushMatrix();
}

inline void MatrixPop()
{
    sceGumPopMatrix();
}

inline void MatrixColor(int color)
{
    sceGuColor(color);
}

inline void DrawSetInverseColors()
{
    sceGuBlendFunc(GU_ABS, GU_ONE_MINUS_SRC_ALPHA, GU_SRC_ALPHA, 0, 0);
}

inline void DrawSetDefaultColors()
{
    sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
}

inline void DrawSetCulling(bool enable)
{
    if(enable)
    {
        sceGuFrontFace(GU_CW);
    }
    else
    {
        sceGuFrontFace(GU_CCW);
    }
}

inline void DrawSetBlend(bool enable)
{
    if(enable)
    {
        sceGuEnable(GU_BLEND);
    }
    else
    {
        sceGuDisable(GU_BLEND);
    }
}

inline void DrawSetDepthTest(bool enable)
{
    if(enable)
    {
        sceGuEnable(GU_DEPTH_TEST);
    }
    else
    {
        sceGuDisable(GU_DEPTH_TEST);
    }
}

inline void DrawSetDepthMask(bool enable)
{
    sceGuDepthMask(enable);
}

inline void DrawSetAlphaTest(bool enable)
{
    if(enable)
    {
        sceGuEnable(GU_ALPHA_TEST);
    }
    else
    {
        sceGuDisable(GU_ALPHA_TEST);
    }
}

inline void DrawPlaceTexture(bool enable)
{
    if(enable)
    {
        sceGuEnable(GU_TEXTURE_2D);
    }
    else
    {
        sceGuDisable(GU_TEXTURE_2D);
    }
}

#endif
