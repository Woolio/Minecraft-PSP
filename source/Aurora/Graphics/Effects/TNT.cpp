#include <Aurora/Graphics/Effects/TNT.h>
#include <Aurora/Graphics/TextureManager.h>

#define PI 3.1415926535897f
#define DEG_TO_RAD (PI / 180.0f)
#define GRAVITY -6.8f

TNT::TNT(CraftWorld* world, Vector3 position2, Vector3 velocity2)
{
    position = Vector3(0,0,0);
    velocity = Vector3(0,0,0);

    SetPosition(position2);
    SetVelociy(velocity2);

    if(!world->BlockSolid(position2.x-1,position2.y,position2.z) && !world->BlockSolid(position2.x+1,position2.y,position2.z))
    {
        velocity.x = -0.6f+(rand()%2)*1.2f;
    }
    if(!world->BlockSolid(position2.x,position2.y,position2.z-1) && !world->BlockSolid(position2.x,position2.y,position2.z+1))
    {
        velocity.z = -0.6f+(rand()%2)*1.2f;
    }
    velocity.y = 2.2f;

    onGround = false;
    toDestroy = false;

    blockScale = 1.0f;
    animationTime = 0.0f;
    timeToExplode = 4.0f; // 4 secs to explode

    TNTid = 110;
    myWorld = world;
    num = 0;
    explodeRadius = 2.75f;
    ignitedByPlayer = false;
}

TNT::~TNT()
{

}

void TNT::ActivateOtherTNTs()
{
    if(myWorld->mTNTs.size() > 1)
    {
        for(unsigned int j = 0; j < myWorld->mTNTs.size(); j++)
        {
            if(j < myWorld->mTNTs.size())
            {
                TNT* TestTNT2 = myWorld->mTNTs[j];
                if(TestTNT2 != NULL)
                {
                    if(TestTNT2->num != num && TestTNT2->position.x != position.x && TestTNT2->position.z != position.z)
                    {
                        float distance = (myWorld->FastDistance2d(abs(position.x-TestTNT2->position.x)*10,abs(position.z-TestTNT2->position.z)*10))/10.0f+abs(position.y-TestTNT2->position.y);
                        if(distance <= 4.0f && distance >= 0.0f)
                        {
                            float angle2 = ATAN2_fast(position.x-(TestTNT2->position.x), position.z-(TestTNT2->position.z))+PI;

                            TestTNT2->velocity.x += sinf(angle2)*(7.5f-distance)*(7.5f-distance);
                            TestTNT2->velocity.z += cosf(angle2)*(7.5f-distance)*(7.5f-distance);
                            TestTNT2->velocity.y += ((TestTNT2->position.y-position.y)/2.5f)*(6.5f-distance)*(6.5f-distance);
                        }
                    }
                }
            }
        }
    }
}

void TNT::HurtNearbyMobs(float dt)
{
    if(myWorld->mZombies.empty() == false)
    {
        for(unsigned int j = 0; j < myWorld->mZombies.size(); j++)
        {
            if(j < myWorld->mZombies.size())
            {
                Zombie* TestMob = myWorld->mZombies[j];
                if(TestMob != NULL)
                {
                    float dist = myWorld->FastDistance2d(abs(position.x-TestMob->position.x)*10,abs(position.z-TestMob->position.z)*10)/10.0f + abs(position.y-(TestMob->position.y-0.7))/2.0f;

                    if(dist < 5.3f)
                    {
                        TestMob->TakeDamageFromPointExplosion(30-(dist*4.7),0.75,dt,position);
                    }
                }
            }
        }
    }
    if(myWorld->mCreepers.empty() == false)
    {
        for(unsigned int j = 0; j < myWorld->mCreepers.size(); j++)
        {
            if(j < myWorld->mCreepers.size())
            {
                Creeper* TestMob = myWorld->mCreepers[j];
                if(TestMob != NULL)
                {
                    float dist = myWorld->FastDistance2d(abs(position.x-TestMob->position.x)*10,abs(position.z-TestMob->position.z)*10)/10.0f + abs(position.y-(TestMob->position.y-0.7))/2.0f;

                    if(dist < 5.3f)
                    {
                        TestMob->TakeDamageFromPointExplosion(30-(dist*4.7),0.75,dt,position);
                    }
                }
            }
        }
    }
    if(myWorld->mCows.empty() == false)
    {
        for(unsigned int j = 0; j < myWorld->mCows.size(); j++)
        {
            if(j < myWorld->mCows.size())
            {
                Cow* TestMob = myWorld->mCows[j];
                if(TestMob != NULL)
                {
                    float dist = myWorld->FastDistance2d(abs(position.x-TestMob->position.x)*10,abs(position.z-TestMob->position.z)*10)/10.0f + abs(position.y-(TestMob->position.y-0.7))/2.0f;

                    if(dist < 5.3f)
                    {
                        TestMob->TakeDamageFromPointExplosion(30-(dist*4.7),0.75,dt,position);
                    }
                }
            }
        }
    }
    if(myWorld->mSheeps.empty() == false)
    {
        for(unsigned int j = 0; j < myWorld->mSheeps.size(); j++)
        {
            if(j < myWorld->mSheeps.size())
            {
                Sheep* TestMob = myWorld->mSheeps[j];
                if(TestMob != NULL)
                {
                    float dist = myWorld->FastDistance2d(abs(position.x-TestMob->position.x)*10,abs(position.z-TestMob->position.z)*10)/10.0f + abs(position.y-(TestMob->position.y-0.7))/2.0f;

                    if(dist < 5.3f)
                    {
                        TestMob->TakeDamageFromPointExplosion(30-(dist*4.7),0.75,dt,position);
                    }
                }
            }
        }
    }
}

void TNT::SetVelociy(Vector3 velocity2)
{
    velocity = velocity2;
}

void TNT::SetPosition(Vector3 position2)
{
    position = position2;
}

void TNT::Update(float dt)
{
    if(position.x < 1 || position.x >= myWorld->WORLD_SIZE-1 || position.y < 1 || position.y >= myWorld->WORLD_HEIGHT-1 || position.z < 1 || position.z >= myWorld->WORLD_SIZE-1)
    {
        toDestroy = true;
    }

    if(toDestroy)
    {
        return;
    }

    velocity.x -= velocity.x*dt*3;
    velocity.z -= velocity.z*dt*3;

    if(abs(velocity.x) < 0.04f && abs(velocity.z) < 0.04f)
    {
        velocity = Vector3(0,velocity.y,0);
    }
    position += velocity*dt;

    // gravitation
    if(myWorld->BlockSolid(position.x-0.45f,position.y-0.5f,position.z-0.45f) == false &&
       myWorld->BlockSolid(position.x-0.45f,position.y-0.5f,position.z+0.45f) == false &&
       myWorld->BlockSolid(position.x+0.45f,position.y-0.5f,position.z-0.45f) == false &&
       myWorld->BlockSolid(position.x+0.45f,position.y-0.5f,position.z+0.45f) == false) // check solid blocks in 4 points
    {
        velocity.y += (GRAVITY*1.3 * dt)*1.6f;
        onGround = false;
    }
    else
    {
        position.y = ((int)position.y)+0.45f;
        velocity.y = 0.0f;
        onGround = true;
    }

    //collisions
    if(myWorld->BlockSolid(position.x-0.45f,position.y-0.2f,position.z-0.45f) == true ||
       myWorld->BlockSolid(position.x-0.45f,position.y-0.2f,position.z+0.45f) == true ||
       myWorld->BlockSolid(position.x-0.45f,position.y+0.2f,position.z+0.45f) == true ||
       myWorld->BlockSolid(position.x-0.45f,position.y+0.2f,position.z-0.45f) == true) // check solid blocks in 4 points
    {
        if(velocity.x < 0)
        {
            velocity.x *= -0.65f;
        }
    }
    if(myWorld->BlockSolid(position.x+0.45f,position.y-0.2f,position.z-0.45f) == true ||
       myWorld->BlockSolid(position.x+0.45f,position.y-0.2f,position.z+0.45f) == true ||
       myWorld->BlockSolid(position.x+0.45f,position.y+0.2f,position.z+0.45f) == true ||
       myWorld->BlockSolid(position.x+0.45f,position.y+0.2f,position.z-0.45f) == true) // check solid blocks in 4 points
    {
        if(velocity.x > 0)
        {
            velocity.x *= -0.65f;
        }
    }
    if(myWorld->BlockSolid(position.x-0.45f,position.y-0.2f,position.z-0.45f) == true ||
       myWorld->BlockSolid(position.x+0.45f,position.y-0.2f,position.z-0.45f) == true ||
       myWorld->BlockSolid(position.x+0.45f,position.y+0.2f,position.z-0.45f) == true ||
       myWorld->BlockSolid(position.x-0.45f,position.y+0.2f,position.z-0.45f) == true) // check solid blocks in 4 points
    {
        if(velocity.z < 0)
        {
            velocity.z *= -0.65f;
        }
    }
    if(myWorld->BlockSolid(position.x-0.45f,position.y-0.2f,position.z+0.45f) == true ||
       myWorld->BlockSolid(position.x+0.45f,position.y-0.2f,position.z+0.45f) == true ||
       myWorld->BlockSolid(position.x+0.45f,position.y+0.2f,position.z+0.45f) == true ||
       myWorld->BlockSolid(position.x-0.45f,position.y+0.2f,position.z+0.45f) == true) // check solid blocks in 4 points
    {
        if(velocity.z > 0)
        {
            velocity.z *= -0.65f;
        }
    }

    // timers
    timeToExplode -= dt;
    animationTime += dt*2.55f;

    if(timeToExplode < 0.25f) // if time before explosion less than 1/4 of second
    {
        blockScale += dt*0.5f; // our tnt block begin to grow
    }

    if(timeToExplode <= 0.0f && toDestroy == false) // if our time has ended
    {
        ActivateOtherTNTs();
        HurtNearbyMobs(dt);

        float dist = myWorld->FastDistance2d(abs(position.x-myWorld->playerPos.x)*10,abs(position.z-myWorld->playerPos.z)*10)/10.0f + abs(position.y-(myWorld->playerPos.y-0.7))/2.0f;

        if(dist < 5.3f)
        {
            myWorld->kickedStart = true;
            myWorld->kickedAngle = ATAN2_fast(position.x-myWorld->playerPos.x,position.z-myWorld->playerPos.z);
            myWorld->kickedBy = 2;
            myWorld->kickedDamage = 32-(dist*5.6);
        }
        if(myWorld->GetBlock(position.x,position.y,position.z) != WaterBlock::getID() && myWorld->GetBlock(position.x,position.y,position.z) != Lava::getID())
        {
            myWorld->BuildExplodeSphere(explodeRadius,position.x,position.y,position.z); // destroy blocks in radius of 3 meters

            myWorld->AddChunkToFastUpdate(myWorld->getChunkId(Vector3(position.x+4,position.y+4,position.z+4)));
            myWorld->AddChunkToFastUpdate(myWorld->getChunkId(Vector3(position.x-4,position.y+4,position.z+4)));
            myWorld->AddChunkToFastUpdate(myWorld->getChunkId(Vector3(position.x-4,position.y+4,position.z-4)));
            myWorld->AddChunkToFastUpdate(myWorld->getChunkId(Vector3(position.x+4,position.y+4,position.z-4)));

            myWorld->AddChunkToFastUpdate(myWorld->getChunkId(Vector3(position.x+4,position.y-4,position.z+4)));
            myWorld->AddChunkToFastUpdate(myWorld->getChunkId(Vector3(position.x-4,position.y-4,position.z+4)));
            myWorld->AddChunkToFastUpdate(myWorld->getChunkId(Vector3(position.x-4,position.y-4,position.z-4)));
            myWorld->AddChunkToFastUpdate(myWorld->getChunkId(Vector3(position.x+4,position.y-4,position.z-4)));
        }
        /*for(int x = position.x-explodeRadius; x <= position.x+explodeRadius; x++)
        {
            for(int z = position.z-explodeRadius; z <= position.z+explodeRadius; z++)
            {
                myWorld->initPutBlocksLight(x,position.y+4,z);
            }
        }*/

        toDestroy = true; // destroy TNT entity

    }
}

void TNT::Render()
{
    if(toDestroy)
    {
        return;
    }
    float myLight = myWorld->BlockFinalLight(position.x,position.y,position.z) * 1.35f; // fetching the light of position

    if(myLight > 1.0f)
    {
        myLight = 1.0f;
    }

    sceGuEnable(GU_DEPTH_TEST);
    sceGuEnable(GU_ALPHA_TEST);
    sceGuEnable(GU_BLEND);

    sceGumPushMatrix();

    ScePspFVector3 loc = {position.x,position.y,position.z};
    sceGumTranslate(&loc);

    ScePspFVector3 sca = {blockScale,blockScale,blockScale};
    sceGumScale(&sca);

    sceGumRotateX(0.0f);
    sceGumRotateY(0.0f);
    sceGumRotateZ(0.0f);

    (int)animationTime % 2 == 0 ? myWorld->drawCubes(TNTid,myLight) : myWorld->drawWhiteCube(TNTid,myLight); // our animation magic

    sceGumPopMatrix();

    sceGuDisable(GU_BLEND);
    sceGuDisable(GU_ALPHA_TEST);
    sceGuDisable(GU_DEPTH_TEST);
}

