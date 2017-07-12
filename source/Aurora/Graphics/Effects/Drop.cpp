#include <Aurora/Graphics/Effects/Drop.h>
#include <Aurora/Graphics/TextureManager.h>

#define PI 3.1415926535897f
#define DEG_TO_RAD (PI / 180.0f)
#define GRAVITY -6.8f

Drop::Drop(Vector3 position2, int id2, int amount2, bool stackable2, int num2)
{
    position = position2;
    velocity = Vector3((-15+rand()%30)*0.1f,3.5f,(-15+rand()%30)*0.1f);
    bBox =  BoundingBox(Vector3(position.x-0.25f,position.y-0.25f,position.z-0.25f),Vector3(position.x+0.25f,position.y+0.25f,position.z+0.25f));

    id = id2;
    amount = amount2;
    stackable = stackable2;
    num = num2;

    levit = 0.0f;
    existingTime = 0.0f;

    onGround = false;
    checkForOtherDrops = false;
    toDestroy = false;
    getMe = false;
    active = true;
    timeOut = 0.0f;

    size2 = 0.0f;
    angleBlock = 0.0f;
}

Drop::~Drop()
{

}

void Drop::BuildVerticesForItem()
{

}

void Drop::CheckOtherDrops()
{
    if(onGround == true && checkForOtherDrops == false)
    {
        for(int i = 0; i < myCrft->mDrops.size(); i++)
        {
            Drop *TestDrop = myCrft->mDrops[i];
            if(num != TestDrop->num)
            {
                float distance = 1.0f;
                if(id < 250)
                {
                    distance = 1.09f;
                }

                if(abs(position.x-TestDrop->position.x) < distance && abs(position.y-TestDrop->position.y) < distance && abs(position.z-TestDrop->position.z) < distance)
                {
                    if(amount+TestDrop->amount <= 64 && TestDrop->id == id)
                    {
                        amount += TestDrop->amount;

                        TestDrop->toDestroy = true;
                    }
                }
            }
        }
        checkForOtherDrops = true;
    }
}

void Drop::SetVelociy(Vector3 velocity2)
{
    velocity = velocity2;
}

int Drop::GetId()
{
    return id;
}

int Drop::GetAmount()
{
    return amount;
}

bool Drop::GetStackable()
{
    return stackable;
}

void Drop::Update(float dt)
{
    existingTime += dt;

    if(existingTime > 105.0f)
    {
        toDestroy = true;
        return;
    }

    if(myCrft->PlayerInLava(position)) // no player
    {
        toDestroy = true;
        return;
    }

    if(toDestroy == false && getMe == false && active == true)
    {
        if(myCrft->BlockSolid(position.x,position.y-0.5f,position.z) == false)
        {
            velocity.y += (GRAVITY*1.3 * dt)*1.6f;
            onGround = false;

           // velocity.x *= 0.95f;
          //  velocity.z *= 0.95f;
        }
        else
        {
            position.y = ((int)position.y)+0.45f;
            velocity.y = 0.0f;
            onGround = true;

            velocity.x *= 0.8f;
            velocity.z *= 0.8f;

            if(stackable == true)
            {
                 CheckOtherDrops();
            }

            bBox =  BoundingBox(Vector3(position.x-0.25f,position.y-0.25f,position.z-0.25f),Vector3(position.x+0.25f,position.y+0.25f,position.z+0.25f));
        }

        if(abs(velocity.x) < 0.1f)
        {
            velocity.x = 0.0f;
        }
        if(abs(velocity.z) < 0.1f)
        {
            velocity.z = 0.0f;
        }

        if(myCrft->BlockSolid(position.x,position.y+0.15f,position.z) == true)
        {
            velocity.y = 0;
            position.y -= 0.1f;
        }

        if(myCrft->BlockSolid(position.x+velocity.x*dt*2,position.y,position.z+velocity.z*dt*2) == true)
        {
            velocity.x *= -0.9f;
            velocity.z *= -0.9f;
        /*
            if(velocity.x >= 0)
            {
                velocity.x += 0.09f;
            }
            else
            {
                velocity.x -= 0.09f;
            }
            if(velocity.z >= 0)
            {
                velocity.z += 0.09f;
            }
            else
            {
                velocity.z -= 0.09f;
            }*/
        }

        if(myCrft->BlockSolid(position.x+0.1f,position.y,position.z) == true && myCrft->BlockSolid(position.x-0.1f,position.y,position.z) == true && myCrft->BlockSolid(position.x,position.y,position.z+0.1f) == true && myCrft->BlockSolid(position.x,position.y,position.z-0.1f) == true && myCrft->BlockSolid(position.x,position.y+0.1f,position.z) == true && myCrft->BlockSolid(position.x,position.y-0.1f,position.z) == true)
        {
            position.y += 1.1f;
            velocity.y = 2.0f;
        }


        position.x += velocity.x*dt;
        position.y += velocity.y*dt;
        position.z += velocity.z*dt;

        if(onGround)
        {
            levit += (PI/2.0f)*dt;
            if(levit >= PI*2)
            {
                levit = 0.0f;
            }

            angleBlock += (PI/2.5f)*dt;
            if(angleBlock >= PI*2)
            {
                angleBlock = 0.0f;
            }
        }
    }
}

void Drop::Render(Frustum &camFrustum, Vector3 playerPos, float camAngle)
{
    if(abs(position.x-playerPos.x) < 10.0f && abs(position.y-playerPos.y) < 11.0f && abs(position.z-playerPos.z) < 10.0f)
    {
        active = true;
    }
    else
    {
        active = false;
    }

    if(toDestroy == false && getMe == false && active == true)
    {
        if(abs(position.x-playerPos.x) < 0.8f && abs(position.y-playerPos.y) < 1.8f && abs(position.z-playerPos.z) < 0.8f && existingTime >= 0.5f && myCrft->InventoryIsFull(id,amount,stackable) == false && myCrft->HP > 0.0f)
        {
            getMe = true;
            return;
        }

        if(camFrustum.BoxInFrustum(bBox) == Frustum::Intersects)
        {
            float myLight = myCrft->BlockFinalLight(position.x,position.y,position.z);
            float angle = ATAN2_fast(position.x-(playerPos.x+(cosf(camAngle+PI)*4)),position.z-(playerPos.z+(sinf(camAngle+PI)*4)))-PI*0.5f;

            sceGuColor(GU_COLOR(myLight,myLight,myLight,1.0f));
            sceGuEnable(GU_DEPTH_TEST);

            sceGuEnable(GU_ALPHA_TEST);
            sceGuEnable(GU_BLEND);


            if(id < 250)
            {
                sceGumPushMatrix();

                ScePspFVector3 loc = {position.x,position.y+sinf(levit)*0.09f,position.z};
                sceGumTranslate(&loc);

                ScePspFVector3 sca = {0.25f,0.25f,0.25f};
                sceGumScale(&sca);

                sceGumRotateX(0.0f);
                sceGumRotateY(angleBlock);
                sceGumRotateZ(0.0f);

                myCrft->drawCubes(id,myCrft->BlockFinalLight(position.x,position.y,position.z));

                if(amount >= 4 && stackable == true)
                {
                    sceGumPushMatrix();

                    ScePspFVector3 loc2 = {0.21f,-0.21f,0.21f};
                    sceGumTranslate(&loc2);

                    myCrft->drawCubes(id,myCrft->BlockFinalLight(position.x,position.y,position.z));

                    sceGumPopMatrix();
                }

                sceGumPopMatrix();
            }
            else
            {
                sceGumPushMatrix();

                ScePspFVector3 loc = {position.x,position.y+sinf(levit)*0.09f,position.z};
                sceGumTranslate(&loc);

                sceGumRotateX(0.0f);
                sceGumRotateY(angle);
                sceGumRotateZ(0.0f);

                myCrft->drawDropItems(id);

                sceGumPopMatrix();

                if(amount >= 4 && stackable == true)
                {
                    sceGumPushMatrix();

                    ScePspFVector3 loc3 = {position.x+0.09f,position.y+sinf(levit)*0.09f+0.09f,position.z};
                    sceGumTranslate(&loc3);

                    sceGumRotateX(0.0f);
                    sceGumRotateY(angle);
                    sceGumRotateZ(0.0f);

                    myCrft->drawDropItems(id);

                    sceGumPopMatrix();
                }
            }

            sceGuDisable(GU_BLEND);
            sceGuDisable(GU_ALPHA_TEST);
            sceGuDisable(GU_DEPTH_TEST);
        }
    }
}

