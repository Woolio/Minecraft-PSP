#include "Creeper2.h"
#include <Aurora/Graphics/TextureManager.h>

#define PI 3.14159f
#define DEG_TO_RAD (PI / 180.0f)
#define PIXEL 1.0f / 16.0f
#define GRAVITY -6.8f
#define JUMPVELOCITY 4.6f
#include "CraftWorld2.h"

/// SIMPLE TRIGONOMETRY

/// Degree range = 0 - 360
/// Radian range = 0 - 2*PI

/// PI * 0.5 = 90 deg
/// PI * 1.0 = 180 deg
/// PI * 1.5 = 270 deg
/// PI * 2.0 = 360 deg (full circle)

/// Degree to radian = deg/180*PI
/// Radian to degree = rad/PI*180

/// atan2f(x-other.x,z-other.z) - function to find the angle between two points

/// sinf and cosf working only with radians
/// x = sinf(rad) * length; y = cosf(rad) * length

/// END

Creeper::Creeper(Vector3 position1, int number1) : LameMob(position1, number1)
{
    legs = Vector3(0,PIXEL*-6,0);

    SetCollideBox(Vector3(PIXEL*4,PIXEL*13,PIXEL*4),Vector3(PIXEL*4,PIXEL*14,PIXEL*4));
    bBox = BoundingBox(Vector3(0,0,0),Vector3(0,0,0));

    animLegAngle = 0.0f;
    animLegAngleT = 0.0f;

    rLegAngle = PI;
    lLegAngle = PI;

    SPD = 1.18f;
    HP = 18.0f;

    friendly = false;

    lastCommand = 0;

    UpdateBBox();

    mainTexture = TextureHelper::Instance()->GetTexture(TextureHelper::creeperTexture);

    playFuseSound = false;

    startExplosion = false;
    exploded = false;
    blowingScale = 0.0f;
    timeBeforeExplosion = 1.4f;

    boredom = false;
    boredomTime = 0.0f;
    animationTime = 0.0f;

    explosionRadius = 2.5f;

    freezedTimerMax = 120.0f;
}

Creeper::~Creeper()
{

}

void Creeper::BoxCollision()
{
    for(int x = 0; x < world->mCreepers.size(); x++)
    {
        Creeper *TestCreeper2 = world->mCreepers[x];
        if(TestCreeper2->number != number && (TestCreeper2->position.x != position.x || TestCreeper2->position.z != position.z || TestCreeper2->position.y != position.y))
        {
            if(TestCreeper2->bBox.intersect(bBox))
            {
                float angle1 = ATAN2_fast(position.x-TestCreeper2->position.x,position.z-TestCreeper2->position.z); // super secret function

                TryToMove(Vector3(sinf(angle1),0.0f,cosf(angle1)),SPD*0.05f);
            }
        }
    }
}

void Creeper::TakeDamage(float damage, float power, float dt)
{
    HP -= damage;

    kicked = true;
    damaged = true;
    damagedTimer = power*0.6f+0.15;
    velocity.y = (power+0.15f) * JUMPVELOCITY;
    walkingOnGround = false;

    position += velocity * dt;
    playHurtSound = true;

    lastKickType = 1; //by player
}

void Creeper::TakeDamageFromPointExplosion(float damage, float power, float dt, Vector3 point)
{
    explosionPos = point;

    HP -= damage;

    kicked = true;
    damaged = true;
    damagedTimer = power*0.6f+0.15;
    velocity.y = (power+0.25f) * JUMPVELOCITY;
    walkingOnGround = false;

    position += velocity * dt;
    playHurtSound = true;

    lastKickType = 2; //by explosion
}

void Creeper::Update(CraftWorld *myWorld, Vector3 playerPos, float dt)
{
    world = myWorld;
    BoundingBox playerHitBox = BoundingBox(Vector3(playerPos.x - 0.73f,playerPos.y - 1.65f,playerPos.z - 0.73f),Vector3(playerPos.x + 0.73f,playerPos.y + 0.2f,playerPos.z + 0.73f));

    distanceToPlayer = DistanceToPlayer();
    fastDistanceToPlayer = FastDistanceToPlayer();

    if(killed == true)
    {
        if(deathAngle < PI/2)
        {
            deathAngle += (PI/2)*dt*1.7+((PI/2-deathAngle)/24);
        }

        if(deathAngle > PI/2)
        {
            deathAngle = PI/2;
        }

        deathTimer -= dt;
    }

    if(toDestroy == false && exploded == false)
    {
        relPlayerPos = playerPos;

        if(fastDistanceToPlayer < world->mainOptions.horizontalViewDistance*14+5)
        {
            freezedTimer = 0.0f;
            if(HP <= 0.0f && killed == false)
            {
                killed = true;
                playDieSound = true;
                SetCollideBox(Vector3(PIXEL*4,PIXEL*5,PIXEL*4),Vector3(PIXEL*4,PIXEL*12,PIXEL*4));
                return;
            }

            UpdateBBox();


            /// TIMERS --- *
            if(dt < 0.1f)
            {
                jumpTimer += dt;
                if(jumpTimer > 0.2f)
                {
                    jumpReady = true;
                    jumpTimer = 0.0f;
                }

                if(damaged == true)
                {
                    if(damagedTimer > 0.0f)
                    {
                        damagedTimer -= dt;
                    }
                    else
                    {
                        damagedTimer = 0.0f;
                        damaged = false;
                    }
                }

                if(boredom == true)
                {
                    boredomTime -= dt;
                    if(boredomTime < -30.0f)
                    {
                        boredom = false;
                    }
                }

                float dist = world->FastDistance2d(abs(position.x-world->playerPos.x)*10,abs(position.z-world->playerPos.z)*10)/10.0f + abs(position.y-(world->playerPos.y-0.7))/2.0f;
                if(startExplosion)
                {
                    if(dist < 6.21f)
                    {
                        timeBeforeExplosion -= dt;
                        blowingScale += dt/7.0f;
                        animationTime += dt*4.5f;

                        if(timeBeforeExplosion < 0.0f)
                        {
                            if(dist < 4.6f)
                            {
                                world->kickedStart = true;
                                world->kickedAngle = ATAN2_fast(position.x-world->playerPos.x,position.z-world->playerPos.z);
                                world->kickedBy = 2;
                                world->kickedDamage = 26-(dist*4.7);
                            }

                            world->BuildExplodeSphere(explosionRadius,position.x,position.y,position.z);

                            world->AddChunkToFastUpdate(world->getChunkId(Vector3(position.x+4,position.y+4,position.z+4)));
                            world->AddChunkToFastUpdate(world->getChunkId(Vector3(position.x-4,position.y+4,position.z+4)));
                            world->AddChunkToFastUpdate(world->getChunkId(Vector3(position.x-4,position.y+4,position.z-4)));
                            world->AddChunkToFastUpdate(world->getChunkId(Vector3(position.x+4,position.y+4,position.z-4)));

                            world->AddChunkToFastUpdate(world->getChunkId(Vector3(position.x+4,position.y-4,position.z+4)));
                            world->AddChunkToFastUpdate(world->getChunkId(Vector3(position.x-4,position.y-4,position.z+4)));
                            world->AddChunkToFastUpdate(world->getChunkId(Vector3(position.x-4,position.y-4,position.z-4)));
                            world->AddChunkToFastUpdate(world->getChunkId(Vector3(position.x+4,position.y-4,position.z-4)));

                            exploded = true;
                            return;
                        }
                    }
                    else
                    {
                        timeBeforeExplosion += dt*1.33f;
                        blowingScale -= (dt/7.0f)*1.33;
                        animationTime += dt*4.5f;
                        if(timeBeforeExplosion > 1.4f)
                        {
                            animationTime = 0.0f;
                            blowingScale = 0.0f;
                            timeBeforeExplosion = 1.4f;
                            startExplosion = false;
                        }
                    }
                }

                /// * --- END



                /// BECOME ANGRY --- *
                if(killed == false)
                {
                    if(fastDistanceToPlayer < 11 && world->HP > 0 && boredom == false) // it sees you
                    {
                        haunting = false;
                        hauntingTimer = 0.0f;
                        hauntingTimerMax = 0.0f;
                        hauntingPos = Vector3(0,0,0);
                        hauntingAngle = 0.0f;

                        boredomTime += dt;
                        if(boredomTime > 30.0f)
                        {
                            boredom = true;
                        }

                        if(dist > 2.3f)
                        {
                            angred = true;
                        }
                        else
                        {
                            if(world->BlockSolid(position.x+(world->playerPos.x-position.x)/2.0f, position.y+0.4f, position.z+(world->playerPos.z-position.z)/2.0f) == false)
                            {
                                moving = false;
                                boredomTime = 0.0f;
                                if(startExplosion == false)
                                {
                                    playFuseSound = true;
                                }
                                startExplosion = true;
                            }
                        }
                    }
                    else // it doesn't see the player
                    {
                        headAngleZ = 0.0f;
                        headAngleY = 0.0f;
                        moving = false;
                        angred = false;
                        haunting = true;

                        if(hauntingTimerMax == 0.0f)
                        {
                            hauntingTimerMax = 5.0f + rand() % 6;
                        }

                        if(hauntingTimer < hauntingTimerMax)
                        {
                            hauntingTimer += dt;
                        }
                        if(hauntingTimer >= hauntingTimerMax)
                        {
                            hauntingTimer = 0.0f;

                            int percent = 50;
                            if(lastCommand == 0)
                            {
                                percent = 75+rand()%10;
                            }
                            else
                            {
                                percent = 30+rand()%20;
                            }
                            if(rand()%100 < percent) // walk to some point nearby
                            {

                                hauntingTimerMax = 4.0f + (rand() % 27)/5.0f;
                                if(rand() % 5 == 0)
                                {
                                    hauntingTimerMax = 1.0f+(rand() % 20)/10.0f;
                                }

                                hauntingPos.x = position.x - 6 + (rand() % 120)/10.0f;
                                hauntingPos.z = position.z - 6 + (rand() % 120)/10.0f;

                                if(hauntingPos.x > world->WORLD_SIZE-3)
                                {
                                    hauntingPos.x = world->WORLD_SIZE-3;
                                }
                                if(hauntingPos.x < 2)
                                {
                                    hauntingPos.x = 2;
                                }

                                if(hauntingPos.z > world->WORLD_SIZE-3)
                                {
                                    hauntingPos.z = world->WORLD_SIZE-3;
                                }
                                if(hauntingPos.z < 2)
                                {
                                    hauntingPos.z = 2;
                                }

                                hauntingAngle = ATAN2_fast(position.x-hauntingPos.x,position.z-hauntingPos.z)+PI;

                                lastCommand = 0;
                            }
                            else // just rotate about its own position
                            {
                                hauntingTimerMax = 2.0f+(rand()%13)/5.0f;
                                hauntingAngle = ((rand() % 360)/180.0f)*PI;

                                lastCommand = 1;

                                hauntingPos = Vector3(-1,-1,-1);
                            }
                        }
                    }
                    /// * --- END


                    /// SWIMING --- *
                    if(world->PlayerInWater(position) == true && jumpReady == true && killed == false)
                    {
                        TryToJump();
                        jumpReady = false;
                    }
                    /// * --- END



                    /// MOVING WHEN ANGRY ---- *
                    if(angred == true && kicked == false && startExplosion == false)
                    {
                        checkCollisionTimer += dt;
                        if(checkCollisionTimer >= 0.1f)
                        {
                            BoxCollision();
                            checkCollisionTimer = 0.0f;
                        }

                        float playerAngle = ATAN2_fast(position.x-relPlayerPos.x,position.z-relPlayerPos.z)+PI;
                        if(mainAngle != playerAngle)
                        {
                            mainAngle += sinf(playerAngle-mainAngle)*0.095f;
                        }
                        headAngleY = playerAngle-mainAngle;

                        if(distanceToPlayer > 1.5f)
                        {
                            TryToMove(Vector3(sinf(mainAngle),0.0f,cosf(mainAngle)),SPD*dt);
                        }

                        headAngleZ = (relPlayerPos.y-position.y-0.315f)*0.25f;
                        if(headAngleZ > PI/4.0f)
                        {
                            headAngleZ = PI/4.0f;
                        }
                        if(headAngleZ < -PI/4.0f)
                        {
                            headAngleZ = -PI/4.0f;
                        }
                    }
                    /// * --- END


                    /// HAUNTING --- *
                    if(haunting == true)
                    {
                        if(hauntingPos.x != 0 && hauntingPos.z != 0)
                        {
                            if(DistanceToHauntingPos() > 0.5f)
                            {
                                if(hauntingPos != Vector3(-1,-1,-1))
                                {
                                    //mainAngle = hauntingAngle;

                                    if(mainAngle != hauntingAngle)
                                    {
                                        mainAngle += sinf(hauntingAngle-mainAngle)*0.07f;
                                    }

                                    TryToMove(Vector3(sinf(mainAngle),0.0f,cosf(mainAngle)),SPD*dt);
                                }
                                else
                                {
                                    if(mainAngle != hauntingAngle)
                                    {
                                        mainAngle += sinf(hauntingAngle-mainAngle)*0.07f;
                                    }

                                    if(abs(mainAngle - hauntingAngle) < 0.02f)
                                    {
                                        mainAngle = hauntingAngle;
                                    }
                                }
                            }
                            else
                            {
                                hauntingPos.x = 0;
                                hauntingPos.z = 0;
                            }
                        }
                    }
                }
                /// * --- END


                /// MOVING WHEN KICKED ---- *
                if(kicked == true)
                {
                    boredom = false;
                    boredomTime = 0.0f;

                    mainAngle = ATAN2_fast(position.x-relPlayerPos.x,position.z-relPlayerPos.z)+PI; // super secret function

                    float angle;
                    if(lastKickType != 2)
                    {
                        angle = ATAN2_fast(position.x-relPlayerPos.x,position.z-relPlayerPos.z)+PI;
                    }
                    else
                    {
                        angle = ATAN2_fast(position.x-explosionPos.x,position.z-explosionPos.z)+PI;
                    }

                    TryToMoveWithoutJump(Vector3(sinf(angle+PI),0.0f,cosf(angle+PI)),(SPD+0.18f)*9.2f*(damagedTimer*damagedTimer)*dt);

                    if((walkingOnGround == true || footInLava || footInWater) && damagedTimer <= 0.0f)
                    {
                        kicked = false;
                    }
                }
                /// * --- END



                /// ANIMATIONS  --- *
                if(killed == false)
                {
                    if(moving == true && startExplosion == false)
                    {
                        animLegAngleT += PI*dt*1.8;
                        if(animLegAngleT >= PI*2)
                        {
                            animLegAngleT = 0.0f;
                        }
                    }
                    else
                    {
                        if (animLegAngleT > 0 && animLegAngleT < PI/2)
                        {
                            animLegAngleT -= animLegAngleT * 0.3f;
                            if (animLegAngleT < 0.05)
                            {
                                animLegAngleT = 0;
                            }
                        }
                        if (animLegAngleT >= PI/2 && animLegAngleT < PI)
                        {
                            animLegAngleT += (PI-animLegAngleT) * 0.3f;
                            if (animLegAngleT > PI - 0.05)
                            {
                                animLegAngleT = 0;
                            }
                        }
                        if (animLegAngleT > PI && animLegAngleT < PI*1.5)
                        {
                            animLegAngleT -= (animLegAngleT-PI) * 0.3f;
                            if (animLegAngleT < PI + 0.05)
                            {
                                animLegAngleT = 0;
                            }
                        }
                        if (animLegAngleT >= PI*1.5 && animLegAngleT < PI*2)
                        {
                            animLegAngleT += (2*PI-animLegAngleT) * 0.3f;
                            if (animLegAngleT > 2*PI - 0.05)
                            {
                                animLegAngleT = 0;
                            }
                        }
                    }
                    animLegAngle = sinf(animLegAngleT) * 0.7f;
                }
                /// * --- END


                /// DAMAGE FACTORS --- *
                if(touchingCactus)
                {
                    tickCactusDamage += dt;
                }
                else
                {
                    tickCactusDamage = 0.4f;
                }
                if(tickCactusDamage >= 0.5f)
                {
                    HP -= 1;
                    damaged = true;
                    damagedTimer = 0.55f;

                    tickCactusDamage -= 0.5f;
                }
                /// * --- END


                /// SIMEPLE MOB PHYSICS --- *

                Vector3 footPosition = position + foot;
                Vector3 legsPosition = position + legs;
                Vector3 headPosition = position + head;

                if(world->PlayerInWater(legsPosition))
                    footInWater = true;
                else
                    footInWater = false;

                if(world->PlayerInWater(headPosition))
                {
                    if (velocity.y > -3)
                    {
                        velocity.y += (GRAVITY/3.0f) * dt;
                    }
                    else
                    {
                        velocity.y = -3;
                    }
                    headInWater = true;
                }
                else
                {
                    headInWater = false;
                    velocity.y += (GRAVITY*1.3 * dt) * 1.6f;
                }

                if(world->PlayerInLava(legsPosition))
                {
                    footInLava = true;
                    HP -= 0.5f;
                    damaged = true;
                    damagedTimer = 0.55f;
                }
                else
                {
                    footInLava = false;
                }

                if(world->PlayerInLava(headPosition))
                {
                    velocity.y += (GRAVITY/3.0f) * dt;
                    headInLava = true;
                    HP -= 1;
                    damaged = true;
                    damagedTimer = 0.55f;
                }
                else
                {
                    headInLava = false;
                    velocity.y += GRAVITY * dt;
                }

                touchingCactus = false;
                if(world->PlayerCollisionWithCactus(headPosition) || world->PlayerCollisionWithCactus(legsPosition))
                {
                    touchingCactus = true;
                }

                if(world->SolidAtPointForEntity(footPosition,colliderD) || world->SolidAtPointForEntity(headPosition,colliderU))
                {
                    walkingOnGround = true;
                    if (world->SolidAtPointForEntity(headPosition,colliderU))
                    {
                        int blockIn = (int)(headPosition.y);
                        position.y = (float)(blockIn - colliderU.y - 0.05);
                    }
                    if (world->SolidAtPointForEntity(footPosition,colliderD))
                    {
                        int blockOn = (int)(footPosition.y);
                        position.y = (float)(blockOn + 1 + colliderD.y - 0.05);

                        if(world->GetBlock(footPosition.x,footPosition.y,footPosition.z) == CactusBlock::getID())
                        {
                            touchingCactus = true;
                        }
                    }

                    if(velocity.y != 0.0f) /// FALL DAMAGE
                    {
                        if (!world->PlayerInWater(footPosition))
                        {
                            if(velocity.y < -12 && velocity.y > -19)
                            {
                                HP -= (int)((velocity.y*-1) - 11) / 1.4;

                                damaged = true;
                                damagedTimer = 0.75f;
                                playHurtSound = true;
                            }
                            if(velocity.y < -19)
                            {
                                HP -= (int)((velocity.y*-1) - 11) * 1.3f;

                                damaged = true;
                                damagedTimer = 0.75f;
                                playHurtSound = true;
                            }
                        }
                    }
                    velocity.y = 0.0f;

                    if(jumping == true)
                    {
                        velocity.y = 1.55f * JUMPVELOCITY;
                        jumping = false;
                        walkingOnGround = false;
                    }
                }
                else
                {
                    walkingOnGround = false;
                }
                position += velocity * dt;
            }
            /// * --- END


        }
        else
        {
            if(dt < 0.1f)
            {
                freezedTimer += dt;
                if(freezedTimer >= freezedTimerMax)
                {
                    toDestroy = true;
                }
            }
        }
    }
}

void Creeper::Render(Frustum &camFrustum, float dt)
{
    if(toDestroy == false && exploded == false)
    {
        if(fastDistanceToPlayer < world->mainOptions.horizontalViewDistance*14 && camFrustum.BoxInFrustum(bBox) == Frustum::Intersects)
        {
            sceGumPushMatrix();

            ScePspFVector3 loc22 = {position.x,position.y,position.z};
            sceGumTranslate(&loc22);

            ScePspFVector3 sca = {1.0+blowingScale*2,1.0+blowingScale,1.0+blowingScale};
            sceGumScale(&sca);


            sceGumRotateZ(0.0f);
            sceGumRotateY(mainAngle-PI/2);

            if(killed == true)
            {
                sceGumRotateX(deathAngle);
            }
            else
            {
                sceGumRotateX(0.0f);
            }


            updateLightTimer += dt;
            if(updateLightTimer > 0.25f)
            {
                updateLightTimer = 0.0f;
                myLight = world->BlockFinalLight(position.x,position.y+1,position.z);
                if(myLight <= 0.1f)
                {
                    myLight = 0.1f;
                }
            }

            if((damaged == true && damagedTimer > 0.25f) || killed == true)
            {
                //sceGuColor(GU_COLOR(0.8f,0.1,0.1,0.8f));
                sceGuColor(GU_COLOR(myLight,myLight,myLight,1.0f));
            }
            else
            {
                sceGuColor(GU_COLOR(myLight,myLight,myLight,1.0f));
            }

            TextureManager::Instance()->SetTextureModeulate(mainTexture);


            if(startExplosion == false)
            {
                sceGuEnable(GU_TEXTURE_2D);
            }
            else
            {
                (int)animationTime % 2 == 0 ? sceGuEnable(GU_TEXTURE_2D) : sceGuDisable(GU_TEXTURE_2D);
            }

            sceGuEnable(GU_DEPTH_TEST);
            sceGuDisable(GU_BLEND);
            sceGuDisable(GU_ALPHA_TEST);

            sceGumPushMatrix();
            ScePspFVector3 loc = {0,0,0};
            sceGumTranslate(&loc);

            sceGumRotateX(0.0f);
            sceGumRotateY(0.0f);
            sceGumRotateZ(0.0f);

            sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 36, 0, world->bodyVertices);
            sceGumPopMatrix();



            sceGumPushMatrix();
            ScePspFVector3 loc2 = {0,PIXEL*10,0};
            sceGumTranslate(&loc2);

            sceGumRotateX(0.0f);
            sceGumRotateY(headAngleY);
            sceGumRotateZ(headAngleZ);

            sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 36, 0, world->headVertices);
            sceGumPopMatrix();



            sceGumPushMatrix();
            ScePspFVector3 loc5 = {PIXEL*4,-PIXEL*6,PIXEL*2};
            sceGumTranslate(&loc5);

            sceGumRotateX(0.0f);
            sceGumRotateY(PI);
            sceGumRotateZ(rLegAngle+animLegAngle);

            sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 36, 0, world->creeperLegVertices);
            sceGumPopMatrix();



            sceGumPushMatrix();
            ScePspFVector3 loc15 = {-PIXEL*4,-PIXEL*6,-PIXEL*2};
            sceGumTranslate(&loc15);

            sceGumRotateX(0.0f);
            sceGumRotateY(PI);
            sceGumRotateZ(rLegAngle+animLegAngle);

            sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 36, 0, world->creeperLegVertices);
            sceGumPopMatrix();



            sceGumPushMatrix();
            ScePspFVector3 loc6 = {PIXEL*4,-PIXEL*6,-PIXEL*2};
            sceGumTranslate(&loc6);

            sceGumRotateX(0.0f);
            sceGumRotateY(PI);
            sceGumRotateZ(lLegAngle-animLegAngle);

            sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 36, 0, world->creeperLegVertices);
            sceGumPopMatrix();



            sceGumPushMatrix();
            ScePspFVector3 loc16 = {-PIXEL*4,-PIXEL*6,PIXEL*2};
            sceGumTranslate(&loc16);

            sceGumRotateX(0.0f);
            sceGumRotateY(PI);
            sceGumRotateZ(lLegAngle-animLegAngle);

            sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 36, 0, world->creeperLegVertices);
            sceGumPopMatrix();



            sceGuDisable(GU_DEPTH_TEST);
            sceGuDisable(GU_TEXTURE_2D);
            sceGuColor(0xFFFFFFFF);

            sceGumPopMatrix();
        }
    }
}

