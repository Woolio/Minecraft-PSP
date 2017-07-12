#ifndef TNT_H
#define TNT_H

#include <Aurora/Graphics/Vertex.h>
#include <Aurora/Graphics/RenderManager.h>
#include <pspmath.h>
#include <Aurora/Math/Frustum.h>
#include <LameCraft/CraftWorld2.h>
#include <Aurora/Math/BoundingBox.h>

class CraftWorld;

class TNT
{
public:
    TNT(CraftWorld* world, Vector3 position2, Vector3 velocity2);
    ~TNT();

    void Update(float dt);
    void Render();
    void SetVelociy(Vector3 velocity2);
    void SetPosition(Vector3 position2);
    void ActivateOtherTNTs();
    void HurtNearbyMobs(float dt);

    float timeToExplode;
    float explodeRadius;
    Vector3 position;
    Vector3 velocity;
    bool toDestroy;
    int num;
    bool ignitedByPlayer;

private:

    float animationTime;
    int TNTid;

    float blockScale;
    bool onGround;

    CraftWorld* myWorld;
};

#endif

