#ifndef COW_H_
#define COW_H_

#include <Aurora/Graphics/Vertex.h>
#include <Aurora/Graphics/RenderManager.h>
#include <pspmath.h>
#include <psptypes.h>
#include <Aurora/Math/Frustum.h>

#include <Aurora/Math/BoundingBox.h>
#include <Aurora/Math/Frustum.h>
#include "LameMob.h"
#include "Blocks.h"
#include "CraftWorld2.h"

class CraftWorld;

class Cow : public LameMob
{
public:

    Cow(Vector3 position1, int number1);
    ~Cow();

    void BoxCollision();
    void Update(CraftWorld *myWorld, Vector3 playerPos, float dt);
    void Render(Frustum &camFrustum, float dt);
    void TakeDamage(float damage, float power, float dt);
    void TakeDamageFromPointExplosion(float damage, float power, float dt, Vector3 point);

    bool rotate1;
    int handItemId;

private:

    int lastCommand;

    float rLegAngle;
    float lLegAngle;

    float headAngleY;

    float animLegAngle;
    float animLegAngleT;

    Vector3 hauntingPos;

    bool senselessJumps;
    float senselessJumpsTimer;
    float senselessJumpsTimerMax;
};

#endif

