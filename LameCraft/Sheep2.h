#ifndef SHEEP_H_
#define SHEEP_H_

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

class Sheep : public LameMob
{
public:

    Sheep(Vector3 position1, int number1);
    ~Sheep();

    void BoxCollision();
    void Update(CraftWorld *myWorld, Vector3 playerPos, float dt);
    void Render(Frustum &camFrustum, float dt);
    void TakeDamage(float damage, float power, float dt);
    void TakeDamageFromPointExplosion(float damage, float power, float dt, Vector3 point);

    void SetFurColor(int newColor);
    int GetFurColor();

    void FetchFurColor();

    bool rotate1;
    int handItemId;

    bool sheared;

private:

    float furTime;

    int furTexture;
    int color;
    Vector3 furColor;

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

