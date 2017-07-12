#ifndef CREEPER_H_
#define CREEPER_H_

#include <Aurora/Graphics/Vertex.h>
#include <Aurora/Graphics/RenderManager.h>
#include <pspmath.h>
#include <psptypes.h>
#include <Aurora/Math/Frustum.h>

#include <Aurora/Math/BoundingBox.h>
#include <Aurora/Math/Frustum.h>
#include "LameMob.h"

class CraftWorld;

class Creeper : public LameMob
{
public:

    Creeper(Vector3 position1, int number1);
    ~Creeper();

    void BoxCollision();
    void Update(CraftWorld *myWorld, Vector3 playerPos, float dt);
    void Render(Frustum &camFrustum, float dt);
    void TakeDamage(float damage, float power, float dt);
    void TakeDamageFromPointExplosion(float damage, float power, float dt, Vector3 point);

    bool exploded;
    bool playFuseSound;

private:

    bool startExplosion;
    float timeBeforeExplosion;
    float blowingScale;
    float animationTime;



    bool boredom;
    float boredomTime;

    int lastCommand;

    float rLegAngle;
    float lLegAngle;

    float headAngleZ;
    float headAngleY;

    float animLegAngle;
    float animLegAngleT;

    float explosionRadius;
};

#endif

