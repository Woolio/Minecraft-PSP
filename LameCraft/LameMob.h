#ifndef LAMEMOB_H_
#define LAMEMOB_H_

#include <math.h>
#include <psptypes.h>

#include <Aurora/Graphics/RenderManager.h>
#include <Aurora/Utils/Logger.h>
#include <Aurora/Utils/Timer.h>
#include <Aurora/System/SystemManager.h>
#include "TextureHelper.h"

class CraftWorld;

class LameMob
{
public:
	LameMob(Vector3 mobPosition, int mobNumber);
    virtual ~LameMob();

    float DistanceToPlayer();
    unsigned int FastDistanceToPlayer();
    float PreciseDistanceToPlayer();
    float DistanceToHauntingPos();
    void TryToJump();
    void TryToMove(Vector3 movePosition1, float speed);
    void TryToMoveWithoutJump(Vector3 movePosition1, float speed);
    void SetPosition(Vector3 newPosition);
    void SetCollideBox(Vector3 collider1, Vector3 collider2);
    void UpdateBBox();

    virtual void BoxCollision();
    virtual void TakeDamage(float damage, float power, float dt);
    virtual void TakeDamageFromPointExplosion(float damage, float power, float dt, Vector3 point);
    virtual void Update(CraftWorld *myWorld, Vector3 playerPos, float dt);
    virtual void Render(Frustum &camFrustum, float dt);

    float mainAngle;

    Vector3 position;
    Vector3 relPlayerPos;
    BoundingBox bBox;

    bool walkingOnGround;
    bool moving;
    bool angred;
    bool toDestroy;
    bool killed;
    bool kicked;
    bool nearToPlayer;
    bool damaged;

    int lastKickType;

    float HP;
    float SPD;

    float damagedTimer;
    float kickTimer;
    float deathTimer;
    float sayTimer;

    float deathAngle;

    int number;

    bool playHurtSound;
    bool playDieSound;
    bool playSaySound;

    CraftWorld *world;

    float fastDistanceToPlayer;
    float distanceToPlayer;

    Vector3 hauntingPos;

    Vector3 velocity;
    Vector3 colliderD;
    Vector3 colliderU;

    Vector3 legs, foot, body, head;

    Vector3 explosionPos;

    bool jumping;
    bool freezed;
    bool footInWater;
    bool footInLava;
    bool headInWater;
    bool headInLava;
    bool jumpReady;
    bool haunting;

    float jumpTimer;
    float freezedTimer;
    float freezedTimerMax;
    float hauntingTimer;
    float hauntingTimerMax;
    float checkCollisionTimer;
    float updateLightTimer;

    float tickCactusDamage;
    bool touchingCactus;

    float hauntingAngle;

    float myLight;

    int mainTexture;

    bool friendly;

private:

};

#endif

