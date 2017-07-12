#ifndef BASEENTITY_H
#define BASEENTITY_H

#include <Aurora/Graphics/Vertex.h>
#include <Aurora/Math/Vector3.h>
#include <pspmath.h>
#include <LameCraft/CraftWorld2.h>
#include <math.h>
#include <psptypes.h>

using namespace Aurora::Graphics;

class BaseEntity
{
public:

	BaseEntity();
	virtual ~BaseEntity();

    void SetPosition(Vector3 position2);
    void SetScale(Vector3 scale2);
    void SetAngle(Vector3 angle2);
    void SetCollisionBox(Vector3 colB2);
    void SetColour(float red, float green, float blue, float alpha2);

    void BuildEntity();
    void RenderEntity();

    bool CheckCollisionPoint(CraftWorld* crtf);

private:

    Vector3 position;
    Vector3 scale;
    Vector3 angle;
    Vector3 collisionBox;

    Vector3 colour;
    float alpha;

    CraftPSPVertex *vertices;

};

#endif
