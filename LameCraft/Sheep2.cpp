#include "Sheep2.h"
#include <Aurora/Graphics/TextureManager.h>

#define PI 3.14159f
#define DEG_TO_RAD (PI / 180.0f)
#define PIXEL 1.0f / 16.0f
#define GRAVITY -6.8f
#define JUMPVELOCITY 4.6f

Sheep::Sheep(Vector3 position1, int number1) : LameMob(position1, number1)
{
    SPD = 1.0f;
    HP = 8.0f;

    legs = Vector3(0,-4*PIXEL,0);

    SetCollideBox(Vector3(PIXEL*7,PIXEL*16,PIXEL*7),Vector3(PIXEL*7,PIXEL*9,PIXEL*7));
    bBox = BoundingBox(Vector3(0,0,0),Vector3(0,0,0));

    animLegAngle = 0.0f;
    animLegAngleT = 0.0f;

    headAngleY = 0.0f;

    rLegAngle = PI;
    lLegAngle = PI;

    mainAngle = (rand()%360)/180.0f*PI;
    deathAngle = 0.0f;

    senselessJumpsTimer = 0.0f;
    senselessJumpsTimerMax = 20.0f + (rand() % 11) * 3;
    senselessJumps = false;
    rotate1 = false;
    sheared = false;

    handItemId = -1;
    myLight = 1.0f;
    furTime = 60+(rand()%5)*30;

    color = 14;
    int colorRandom = rand() % 100;
    if(colorRandom > 80)
    {
        switch(rand() % 3)
        {
            case 0 : color = 1; break;
            case 1 : color = 13; break;
            case 2 : color = 0; break;
        }
        if(colorRandom  > 93)
        {
            color = 6;
        }
        if(colorRandom  > 97)
        {
            color = 3;
        }
    }

    SetFurColor(color);
    UpdateBBox();

    lastCommand = 0;
    freezedTimerMax = 600.0f;

    mainTexture = TextureHelper::Instance()->GetTexture(TextureHelper::sheepTexture);
    furTexture = TextureHelper::Instance()->GetTexture(TextureHelper::sheepFurTexture);
}

Sheep::~Sheep()
{

}

void Sheep::FetchFurColor()
{
    switch(color)
    {
        case 0 :  furColor = Vector3(0.19f,0.16f,0.16f); break;  // black
        case 1 :  furColor = Vector3(0.38f,0.38f,0.38f); break;  // gray
        case 2 :  furColor = Vector3(0.73f,0.2f,0.2f); break; // red
        case 3 :  furColor = Vector3(1,0.67f,0.67f); break; // pink
        case 4 :  furColor = Vector3(0.19f,0.38f,0.1f); break; // dark green
        case 5 :  furColor = Vector3(0.19f,0.77f,0.13f); break; // green
        case 6 :  furColor = Vector3(0.35f*1.1f,0.20f*1.1f,0.13f*1.1f); break; // brown
        case 7 :  furColor = Vector3(0.73f,0.73f,0.1f); break; // yellow
        case 8 :  furColor = Vector3(0.24f,0.24f,0.68f); break; // blue
        case 9 :  furColor = Vector3(0.45f,0.52f,0.9f); break; // light blue
        case 10 :  furColor = Vector3(0.63f,0.29f,0.73f); break; // purple
        case 11 :  furColor = Vector3(0.73f,0.42f,0.73f); break; // magneta
        case 12 :  furColor = Vector3(0.8f,0.4f,0.1f); break; // orange
        case 13 :  furColor = Vector3(0.73f,0.73f,0.73f); break; // light gray
        case 14 : furColor = Vector3(1,1,1); break;
        default : furColor = Vector3(1,1,1); break;
    }
}

void Sheep::SetFurColor(int newColor)
{
    color = newColor;

    FetchFurColor();
}

int Sheep::GetFurColor()
{
    return color;
}

void Sheep::BoxCollision()
{
    for(int x = 0; x < world->mSheeps.size(); x++)
    {
        Sheep *TestSheep2 = world->mSheeps[x];
        if(TestSheep2->number != number && (TestSheep2->position.x != position.x || TestSheep2->position.z != position.z || TestSheep2->position.y != position.y))
        {
            if(TestSheep2->bBox.intersect(bBox))
            {
                float angle1 = ATAN2_fast(position.x-TestSheep2->position.x,position.z-TestSheep2->position.z); // super secret function

                TryToMove(Vector3(sinf(angle1),0.0f,cosf(angle1)),SPD*0.05f);
            }
        }
    }
}

void Sheep::TakeDamage(float damage, float power, float dt)
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

void Sheep::TakeDamageFromPointExplosion(float damage, float power, float dt, Vector3 point)
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

void Sheep::Update(CraftWorld *myWorld, Vector3 playerPos, float dt)
{
    world = myWorld;
    relPlayerPos = playerPos;
    BoundingBox playerHitBox = BoundingBox(Vector3(playerPos.x - 0.35f,playerPos.y - 1.65f,playerPos.z - 0.35f),Vector3(playerPos.x + 0.35f,playerPos.y + 0.2f,playerPos.z + 0.35f));

    distanceToPlayer = DistanceToPlayer();
    fastDistanceToPlayer = FastDistanceToPlayer();

    if(killed == true)
    {
        if(deathAngle < PI/2)
        {
            deathAngle += (PI/2)*dt*1.75+((PI/2-deathAngle)/24);
        }

        if(deathAngle > PI/2)
        {
            deathAngle = PI/2;
        }

        deathTimer -= dt;
    }

    if(sheared == true)
    {
        furTime -= dt;
        if(furTime < 0.0f)
        {
            furTime = 60+(rand()%5)*30;
            sheared = false;
        }
    }

    if(toDestroy == false)
    {
        HP += dt/10.0f;
        if(HP > 8.0f)
        {
            HP = 8.0f;
        }

        relPlayerPos = playerPos;
        if(fastDistanceToPlayer < world->mainOptions.horizontalViewDistance*14+5)
        {
            freezedTimer = 0.0f;
            if(HP <= 0.0f && killed == false)
            {
                killed = true;
                SetCollideBox(Vector3(PIXEL*7,PIXEL*6,PIXEL*7),Vector3(PIXEL*7,PIXEL*10,PIXEL*7));
                return;
            }

            UpdateBBox();


            /// TIMERS --- *
            if(dt < 0.1f)
            {
                sayTimer += dt;
                if(sayTimer > 10.0f)
                {
                    if(rand() % 2 == 0)
                    {
                        playSaySound = true;
                    }
                    sayTimer = 0.0f;
                }

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

                if(playerHitBox.intersect(bBox))
                {
                    float angle2 = ATAN2_fast(position.x-relPlayerPos.x,position.z-relPlayerPos.z);
                    TryToMove(Vector3(sinf(angle2),0.0f,cosf(angle2)),1.0*dt);
                }
                if(distanceToPlayer < 1.5)
                {
                    nearToPlayer = true;
                }
                else
                {
                    nearToPlayer = false;
                }
                /// * --- END


                /// SWIMING --- *
                if(world->PlayerInWater(position) == true && jumpReady == true && killed == false)
                {
                    TryToJump();
                    jumpReady = false;
                }
                /// * --- END


                /// HAUNTING --- *
                moving = false;

                if(killed == false)
                {
                    senselessJumpsTimer += dt;
                    if(senselessJumpsTimer >= senselessJumpsTimerMax)
                    {
                        senselessJumpsTimer = 0.0f;
                        if(senselessJumps == true)
                        {
                            senselessJumps = false;
                            senselessJumpsTimerMax = 15.0f + (rand() % 21) * 1.5f;

                            if(rand() % 3 == 0)
                            {
                                senselessJumpsTimerMax = 60.0f + rand() % 80;
                            }
                        }
                        else
                        {
                            senselessJumps = true;
                            senselessJumpsTimerMax = 2.0f + (rand() % 21) / 10.0f;
                        }
                    }

                    if(senselessJumps && jumpReady)
                    {
                        TryToJump();
                        jumpReady = false;
                    }

                    if(handItemId != Wheat::getID() || fastDistanceToPlayer >= 10)
                    {
                        headAngleY = 0.0f;
                        if(hauntingTimer < hauntingTimerMax)
                        {
                            hauntingTimer += dt;
                        }
                        if(hauntingTimer >= hauntingTimerMax)
                        {
                            rotate1 = false;
                            hauntingTimer = 0.0f;

                            int percent = 50;
                            if(lastCommand == 0)
                            {
                                percent = 60+rand()%10;
                            }
                            else
                            {
                                percent = 45+rand()%20;
                            }

                            if(rand()%100 < percent)
                            {
                                hauntingTimerMax = 4.0f + rand() % 4;

                                hauntingPos.x = position.x - 6 + ((rand() % 120)/10.0f);
                                hauntingPos.z = position.z - 6 + ((rand() % 120)/10.0f);

                                if(hauntingPos.x > 232)
                                {
                                    hauntingPos.x = 232-rand()%12;
                                }
                                if(hauntingPos.x < 22)
                                {
                                    hauntingPos.x = 22+rand()%12;
                                }

                                if(hauntingPos.z > 232)
                                {
                                    hauntingPos.z = 232-rand()%12;
                                }
                                if(hauntingPos.z < 22)
                                {
                                    hauntingPos.z = 22+rand()%12;
                                }

                                hauntingAngle = ATAN2_fast(position.x-hauntingPos.x,position.z-hauntingPos.z)+PI;

                                lastCommand = 0;
                            }
                            else // just rotate about its own position
                            {
                                hauntingTimerMax = 2.0f+(rand()%30)/10.0f;
                                hauntingAngle = (rand() % 360)/180.0f*PI;

                                hauntingPos = Vector3(-1,-1,-1);

                                lastCommand = 1;
                            }
                        }


                        if(hauntingPos.x != 0 && hauntingPos.z != 0)
                        {
                            if(DistanceToHauntingPos() > 0.5f)
                            {
                                if(hauntingPos != Vector3(-1,-1,-1))
                                {
                                    if(mainAngle != hauntingAngle)
                                    {
                                        mainAngle += sinf(hauntingAngle-mainAngle)*0.07f;
                                    }

                                   // mainAngle = hauntingAngle;
                                    TryToMove(Vector3(sinf(mainAngle),0.0f,cosf(mainAngle)),SPD*dt);
                                }
                                else
                                {
                                    if(mainAngle != hauntingAngle)
                                    {
                                        mainAngle += sinf(hauntingAngle-mainAngle)*0.07f;
                                    }

                                   /* if(abs(mainAngle - hauntingAngle) < 0.02f)
                                    {
                                        mainAngle = hauntingAngle;
                                    } */
                                }
                            }
                            else
                            {
                                hauntingPos.x = 0;
                                hauntingPos.z = 0;
                            }
                        }
                    }
                    else
                    {
                        hauntingTimer = 0.0f;

                        float playerAngle = ATAN2_fast(position.x-playerPos.x,position.z-playerPos.z)+PI;
                        if(mainAngle != playerAngle)
                        {
                            mainAngle += sinf(playerAngle-mainAngle)*0.07f;
                        }
                        headAngleY = playerAngle-mainAngle;

                        if(distanceToPlayer > 2.3f)
                        {
                            TryToMove(Vector3(sinf(mainAngle),0.0f,cosf(mainAngle)),SPD*dt);
                        }
                    }
                }

                if(kicked == true || handItemId == Wheat::getID())
                {
                    checkCollisionTimer += dt;
                    if(checkCollisionTimer >= 0.1f)
                    {
                        BoxCollision();
                        checkCollisionTimer = 0.0f;
                    }
                }
                /// * --- END



                /// MOVING WHEN KICKED ---- *
                if(kicked == true)
                {
                    haunting = false;
                    hauntingTimer = 0.0f;
                    hauntingPos.x = 0;
                    hauntingPos.z = 0;

                    float mainAngle2; // super secret function
                    if(lastKickType != 2)
                    {
                        mainAngle2 = ATAN2_fast(position.x-relPlayerPos.x,position.z-relPlayerPos.z)+PI;
                    }
                    else
                    {
                        mainAngle2 = ATAN2_fast(position.x-explosionPos.x,position.z-explosionPos.z)+PI;
                    }
                    TryToMoveWithoutJump(Vector3(sinf(mainAngle2+PI),0.0f,cosf(mainAngle2+PI)),(SPD+0.18f)*9.2f*(damagedTimer*damagedTimer)*dt);

                    if((walkingOnGround == true || footInLava || footInWater) && damagedTimer <= 0.17f)
                    {
                        kicked = false;

                        hauntingTimerMax = 3.0f + rand() % 3;

                        hauntingPos.x = position.x - 5 + (rand() % 100)/10.0f;
                        hauntingPos.z = position.z - 5 + (rand() % 100)/10.0f;

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
                    }
                }
                /// * --- END



                /// ANIMATIONS  --- *
                if(moving == true && kicked == false)
                {
                    animLegAngleT += PI*dt*1.5;
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
                animLegAngle = sinf(animLegAngleT) * 0.655f;
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
                    }

                    if(velocity.y != 0.0f) /// FALL DAMAGE
                    {
                        if (!world->PlayerInWater(footPosition))
                        {
                            if(velocity.y < -12 && velocity.y > -19)
                            {
                                HP -= (int)((velocity.y*-1) - 11) / 1.4;

                                damaged = true;
                                damagedTimer = 0.5f;
                                playHurtSound = true;
                            }
                            if(velocity.y < -19)
                            {
                                HP -= (int)((velocity.y*-1) - 11) * 1.3f;

                                damaged = true;
                                damagedTimer = 0.5f;
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

                /// * --- END
            }
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

void Sheep::Render(Frustum &camFrustum, float dt)
{
    if(toDestroy == false)
    {
        if(fastDistanceToPlayer < world->mainOptions.horizontalViewDistance*14 && camFrustum.BoxInFrustum(bBox) == Frustum::Intersects)
        {
            sceGumPushMatrix();

            ScePspFVector3 loc22 = {position.x,position.y,position.z};
            sceGumTranslate(&loc22);

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
            }

            if((damaged == true && damagedTimer > 0.25f) || killed == true)
            {
                sceGuColor(GU_COLOR(0.8f,0.1,0.1,1.0f));
            }
            else
            {
                sceGuColor(GU_COLOR(myLight*furColor.x,myLight*furColor.y,myLight*furColor.z,1.0f));
            }

            TextureManager::Instance()->SetTextureModeulate(mainTexture);

            sceGuEnable(GU_TEXTURE_2D);
            sceGuEnable(GU_DEPTH_TEST);
            sceGuDisable(GU_ALPHA_TEST);
            sceGuDisable(GU_BLEND);



            if(sheared)
            {
                if((damaged == true && damagedTimer > 0.25f) || killed == true)
                {
                    sceGuColor(GU_COLOR(0.8f,0.1,0.1,1.0f));
                }
                else
                {
                    sceGuColor(GU_COLOR(myLight,myLight,myLight,1.0f));
                }

                sceGumPushMatrix();

                ScePspFVector3 loc = {0,0,0};
                sceGumTranslate(&loc);

                ScePspFVector3 sca = {1,1,1};
                sceGumScale(&sca);

                sceGumRotateX(0.0f);
                sceGumRotateY(0.0f);
                sceGumRotateZ(0.0f);

                sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 36, 0, world->sheepBodyVertices);

                sceGumPopMatrix();
            }
            else
            {
                if((damaged == true && damagedTimer > 0.25f) || killed == true)
                {
                    sceGuColor(GU_COLOR(myLight*furColor.x*0.8f,myLight*furColor.y*0.1,myLight*furColor.z*0.1,1.0f));
                }
                else
                {
                    sceGuColor(GU_COLOR(myLight*furColor.x,myLight*furColor.y,myLight*furColor.z,1.0f));
                }

                TextureManager::Instance()->SetTextureModeulate(furTexture);
                // body fur
                sceGumPushMatrix();

                ScePspFVector3 loc = {-PIXEL*0.2,-PIXEL*0.2,0};
                sceGumTranslate(&loc);

                ScePspFVector3 sca = {1.225,1.6,1.4};
                sceGumScale(&sca);

                sceGumRotateX(0.0f);
                sceGumRotateY(0.0f);
                sceGumRotateZ(0.0f);

                sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 36, 0, world->sheepBodyVertices);

                sceGumPopMatrix();

                // head fur
                sceGumPushMatrix();
                ScePspFVector3 loc2 = {PIXEL*9,PIXEL*4,0};
                sceGumTranslate(&loc2);

                ScePspFVector3 sca2 = {1.24,1.24,1.24};
                sceGumScale(&sca2);

                sceGumRotateX(0.0f);
                sceGumRotateY(headAngleY);
                sceGumRotateZ(0.0f);

                sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 36, 0, world->sheepHeadFurVertices);

                sceGumPopMatrix();
            }



            if((damaged == true && damagedTimer > 0.25f) || killed == true)
            {
                sceGuColor(GU_COLOR(0.8f,0.1,0.1,1.0f));
            }
            else
            {
                sceGuColor(GU_COLOR(myLight,myLight,myLight,1.0f));
            }

            TextureManager::Instance()->SetTextureModeulate(mainTexture);

            sceGumPushMatrix();
            ScePspFVector3 loc2 = {PIXEL*10,PIXEL*4,0};
            sceGumTranslate(&loc2);

            sceGumRotateX(0.0f);
            sceGumRotateY(headAngleY);
            sceGumRotateZ(0.0f);

            sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 30+sheared*6, 0, world->sheepHeadVertices);

            sceGumPopMatrix();

            if(!sheared)
            {
                TextureManager::Instance()->SetTextureModeulate(furTexture);
                if((damaged == true && damagedTimer > 0.25f) || killed == true)
                {
                    sceGuColor(GU_COLOR(myLight*furColor.x*0.8f,myLight*furColor.y*0.1,myLight*furColor.z*0.1,1.0f));
                }
                else
                {
                    sceGuColor(GU_COLOR(myLight*furColor.x,myLight*furColor.y,myLight*furColor.z,1.0f));
                }

                ScePspFVector3 sca = {1.23f,1.23f,1.23f};

                sceGumPushMatrix();
                ScePspFVector3 loc3 = {-PIXEL*7,-PIXEL*5,PIXEL*3};
                sceGumTranslate(&loc3);

                sceGumScale(&sca);

                sceGumRotateX(0.0f);
                sceGumRotateY(PI);
                sceGumRotateZ(rLegAngle-animLegAngle);

                sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 30, 0, world->sheepLegFurVertices);
                sceGumPopMatrix();




                sceGumPushMatrix();
                ScePspFVector3 loc4 = {PIXEL*5,-PIXEL*5,PIXEL*3};
                sceGumTranslate(&loc4);

                sceGumScale(&sca);

                sceGumRotateX(0.0f);
                sceGumRotateY(PI);
                sceGumRotateZ(lLegAngle+animLegAngle);

                sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 30, 0, world->sheepLegFurVertices);
                sceGumPopMatrix();



                sceGumPushMatrix();
                ScePspFVector3 loc5 = {-PIXEL*7,-PIXEL*5,-PIXEL*3};
                sceGumTranslate(&loc5);

                sceGumScale(&sca);

                sceGumRotateX(0.0f);
                sceGumRotateY(PI);
                sceGumRotateZ(rLegAngle+animLegAngle);

                sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 30, 0, world->sheepLegFurVertices);
                sceGumPopMatrix();




                sceGumPushMatrix();
                ScePspFVector3 loc6 = {PIXEL*5,-PIXEL*5,-PIXEL*3};
                sceGumTranslate(&loc6);

                sceGumScale(&sca);

                sceGumRotateX(0.0f);
                sceGumRotateY(PI);
                sceGumRotateZ(lLegAngle-animLegAngle);

                sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 30, 0, world->sheepLegFurVertices);
                sceGumPopMatrix();
            }

            if((damaged == true && damagedTimer > 0.25f) || killed == true)
            {
                sceGuColor(GU_COLOR(0.8f,0.1,0.1,1.0f));
            }
            else
            {
                sceGuColor(GU_COLOR(myLight,myLight,myLight,1.0f));
            }
            TextureManager::Instance()->SetTextureModeulate(mainTexture);

            sceGumPushMatrix();
            ScePspFVector3 loc3 = {-PIXEL*7,-PIXEL*5,PIXEL*3};
            sceGumTranslate(&loc3);

            sceGumRotateX(0.0f);
            sceGumRotateY(PI);
            sceGumRotateZ(rLegAngle-animLegAngle);

            sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 30+sheared*6, 0, world->cowLegVertices);
            sceGumPopMatrix();



            sceGumPushMatrix();
            ScePspFVector3 loc4 = {PIXEL*5,-PIXEL*5,PIXEL*3};
            sceGumTranslate(&loc4);

            sceGumRotateX(0.0f);
            sceGumRotateY(PI);
            sceGumRotateZ(lLegAngle+animLegAngle);

            sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 30+sheared*6, 0, world->cowLegVertices);
            sceGumPopMatrix();



            sceGumPushMatrix();
            ScePspFVector3 loc5 = {-PIXEL*7,-PIXEL*5,-PIXEL*3};
            sceGumTranslate(&loc5);

            sceGumRotateX(0.0f);
            sceGumRotateY(PI);
            sceGumRotateZ(rLegAngle+animLegAngle);

            sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 30+sheared*6, 0, world->cowLegVertices);
            sceGumPopMatrix();



            sceGumPushMatrix();
            ScePspFVector3 loc6 = {PIXEL*5,-PIXEL*5,-PIXEL*3};
            sceGumTranslate(&loc6);

            sceGumRotateX(0.0f);
            sceGumRotateY(PI);
            sceGumRotateZ(lLegAngle-animLegAngle);

            sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 30+sheared*6, 0, world->cowLegVertices);
            sceGumPopMatrix();


            //sceGuDisable(GU_BLEND);
            //sceGuDisable(GU_ALPHA_TEST);
            sceGuDisable(GU_DEPTH_TEST);
            sceGuDisable(GU_TEXTURE_2D);
            sceGuColor(0xFFFFFFFF);

            sceGumPopMatrix();
        }
    }
}

