#include <Aurora/Graphics/Effects/WeatherSystem.h>
#include <Aurora/Graphics/TextureManager.h>
#include <LameCraft/CraftWorld2.h>
#include <Aurora/Graphics/TextureManager.h>

#define PI 3.1415926535897f
#define DEG_TO_RAD (PI / 180.0f)

#define CLEAR 0
#define RAIN 1

WeatherSystem::WeatherSystem()
{
    plane = (TexturesPSPVertex*)memalign(16,4 * sizeof(TexturesPSPVertex));

    size = 0.0f;

    rainTextureOffset = 0.0f;
    snowTextureOffset = 0.0f;
    textureAlpha = 1.0f;

    raindropsCount = 0;
    playerTempPosition = Vector3(-1,-1,-1);
    weatherType = CLEAR;
    weatherDuration = 0.0f;

    for(int i = 0; i <= 27; i++)
    {
        planeY[i] = 0;
    }

    BuildPlanesVertex();
    SetWeatherDuration(300+rand()%2400);

    itIsRaining = false;
    itIsSnowing = false;
}

WeatherSystem::~WeatherSystem()
{
    free(plane);
}

void WeatherSystem::BuildPlanesVertex()
{
    size = 4.0f;

    int i = 0;

    // right
    plane[i].x = 0.0f;
    plane[i].y = 0.0f;
    plane[i].z = 0.5f;
    plane[i].u = 1.0;
    plane[i].v = 0;
    i++;

    plane[i].x = 0.0f;
    plane[i].y = size;
    plane[i].z = 0.5f;
    plane[i].u = 1.0;
    plane[i].v = 1;
    i++;

    plane[i].x = 0.0f;
    plane[i].y = size;
    plane[i].z = -0.5f;
    plane[i].u = 0.0;
    plane[i].v = 1;
    i++;

    plane[i].x = 0.0f;
    plane[i].y = 0.0f;
    plane[i].z = -0.5f;
    plane[i].u = 0.0;
    plane[i].v = 0;
    i++;

    sceKernelDcacheWritebackInvalidateRange(plane,4 * sizeof(TexturesPSPVertex));
}

void WeatherSystem::SetWeatherDuration(float duration)
{
    weatherDuration = duration;
}

void WeatherSystem::SetWeatherType(int type)
{
    weatherType = type;
}

float WeatherSystem::GetWeatherDuration()
{
    return weatherDuration;
}

int WeatherSystem::GetWeatherType()
{
    return weatherType;
}

Vector3 WeatherSystem::GetPlaneCoord(int planeID)
{
    if(planeID >= 1 && planeID <= 5)
    {
        return Vector3(-2,0,-2+(planeID-1));
    }
    if(planeID >= 6 && planeID <= 10)
    {
        return Vector3(-1,0,-2+(planeID-6));
    }
    if(planeID >= 11 && planeID <= 13)
    {
        return Vector3(0,0,-3+(planeID-11));
    }
    if(planeID >= 14 && planeID <= 16)
    {
        return Vector3(0,0,1+(planeID-14));
    }
    if(planeID >= 17 && planeID <= 21)
    {
        return Vector3(1,0,-2+(planeID-17));
    }
    if(planeID >= 22 && planeID <= 26)
    {
        return Vector3(2,0,-2+(planeID-22));
    }
    if(planeID == 28)
    {
        return Vector3(3,0,0);
    }
    return Vector3(-3,0,0);
}

void WeatherSystem::Update(CraftWorld* mWorld, float dt)
{
    if(dt < 0.15)
    {
        weatherDuration -= dt;

        if(weatherDuration < 0)
        {
            if(GetWeatherType() == CLEAR)
            {
                SetWeatherType(RAIN);
                SetWeatherDuration(240+rand()%360);

                if(rand() % 10)
                {
                    SetWeatherDuration(400+rand()%400);
                }
            }
            else
            {
                SetWeatherType(CLEAR);
                SetWeatherDuration(500+rand()%5500);
            }
        }
    }
}

void WeatherSystem::Render(CraftWorld* mWorld, Frustum &camFrustum, float camera_angle, Vector3 playerPosition, float dt)
{
    raindropsCount = 0;
    if(GetWeatherType() == RAIN && (mWorld->desertBiomeRadius == -1 || mWorld->FastDistance2d( abs(playerPosition.x-mWorld->desertBiomePosition.x), abs(playerPosition.z-mWorld->desertBiomePosition.z)) > mWorld->desertBiomeRadius))
    {
        bool raindrop = false;
        bool snowflake = false;

        float playerGridX = ((int)playerPosition.x)+0.5f;
        float playerGridY = ((int)playerPosition.y);
        float playerGridZ = ((int)playerPosition.z)+0.5f;

        /*if(playerGridX == playerTempPosition.x && playerGridY == playerTempPosition.y && playerGridZ == playerTempPosition.z)
        {
            needUpdate = true;
        }
        else
        {
            playerTempPosition.x = playerGridX;
            playerTempPosition.y = playerGridY;
            playerTempPosition.z = playerGridZ;
        }*/

        rainTextureOffset += 1.7*dt;
        if(rainTextureOffset >= 1.0f)
        {
            rainTextureOffset -= 1.0f;
        }
        snowTextureOffset += 0.15*dt;
        if(snowTextureOffset >= 2.0f)
        {
            snowTextureOffset -= 2.0f;
        }

        sceGuEnable(GU_DEPTH_TEST);
        sceGuEnable(GU_ALPHA_TEST);
        sceGuEnable(GU_BLEND);

        if(mWorld->snowBiomeRadius != -1 && (mWorld->FastDistance2d(abs(playerPosition.x-mWorld->snowBiomePosition.x), abs(playerPosition.z-mWorld->snowBiomePosition.z)) <= mWorld->snowBiomeRadius))
        {
            raindrop = itIsRaining = false;
            snowflake = itIsSnowing = true;

            TextureManager::Instance()->SetTextureModeulate(TextureHelper::Instance()->GetTexture(TextureHelper::snowTexture));
        }
        else
        {
            raindrop = itIsRaining = true;
            snowflake = itIsSnowing = false;

            TextureManager::Instance()->SetTextureModeulate(TextureHelper::Instance()->GetTexture(TextureHelper::rainTexture));
        }

        for(int i = 0; i <= 27; i++)
        {
            Vector3 planeCoord = GetPlaneCoord(i);
            float shiftX = planeCoord.x;
            float shiftY = planeCoord.y;
            float shiftZ = planeCoord.z;

            float x = playerGridX + shiftX;
            float y = playerGridY + shiftY;
            float z = playerGridZ + shiftZ;


            BoundingBox testbBox =  BoundingBox(Vector3(x-0.4f,y-6,z-0.4f),Vector3(x+0.4f,y+6,z+0.4f));

            if((camFrustum.BoxInFrustum(testbBox) == Frustum::Intersects))
            {
                bool needRender = true;
                //if(needUpdate)
                //{
                if(mWorld->GetShadowLevel(x,y,z) != 0)
                {
                    needRender = false;
                    for(int yy = y+1; yy <= (int)y + 4; yy++)
                    {
                        if(mWorld->GetShadowLevel(x,yy,z) == 0)
                        {
                            y = yy+1;
                            needRender = true;
                            break;
                        }
                    }
                }
                else
                {
                    for(int yy = y; yy >= y - 6; yy--)
                    {
                        if(mWorld->GetShadowLevel(x,yy-1,z) != 0)
                        {
                            y = yy+1;
                            break;
                        }
                    }
                }
                //}

                if(needRender)
                {
                    sceGuEnable(GU_TEXTURE_2D);
                    sceGuTexWrap(GU_REPEAT,GU_REPEAT);
                    if(raindrop)
                    {
                        sceGuTexOffset(0.0f,rainTextureOffset);
                        raindropsCount ++;
                    }
                    if(snowflake)
                    {
                        TextureManager::Instance()->SetTextureModeulate(TextureHelper::Instance()->GetTexture(TextureHelper::snowTexture));
                        if(i%2 == 0 || shiftX == 0 || shiftZ == 0)
                        {
                            sceGuTexOffset(snowTextureOffset,snowTextureOffset*0.5f);
                        }
                        else
                        {
                            sceGuTexOffset(-snowTextureOffset,snowTextureOffset*0.5f);
                        }
                    }

                    float light = mWorld->BlockFinalLight(x,y+2,z);
                    MatrixColor(GU_COLOR(light,light,light,textureAlpha));

                    if(camera_angle < 1.35)
                    {
                        MatrixPush();
                        MatrixTranslation(Vector3(x,y,z));
                        RenderPlane(shiftX, shiftY, shiftZ);
                        MatrixPop();
                    }

                    if(camera_angle > 0.9 || y+4 < playerPosition.y)
                    {
                        MatrixPush();
                        MatrixTranslation(Vector3(x,y+4.0f,z));
                        RenderPlane(shiftX, shiftY, shiftZ);
                        MatrixPop();
                    }

                    sceGuTexOffset(0.0f,0.0f);
                    sceGuTexWrap(GU_CLAMP,GU_CLAMP);
                    sceGuDisable(GU_TEXTURE_2D);
                    /*MatrixPush();
                    MatrixTranslation(Vector3(x,y+8.0f,z));
                    RenderPlane(shiftX, shiftY, shiftZ);
                    MatrixPop();*/

                }
            }
        }


        sceGuDisable(GU_BLEND);
        sceGuDisable(GU_ALPHA_TEST);
        sceGuDisable(GU_DEPTH_TEST);
    }
}

void WeatherSystem::RenderPlane(int shiftX, int shiftY, int shiftZ)
{
    if(shiftZ == 0)
    {
        if(shiftX < 0)
        {
            RenderLeftPlane();
        }
        if(shiftX > 0)
        {
            RenderRightPlane();
        }
    }
    if(shiftX == 0)
    {
        if(shiftZ < 0)
        {
            RenderBackPlane();
        }
        if(shiftZ > 0)
        {
            RenderFrontPlane();
        }
    }

    if(shiftX != 0 && shiftZ != 0)
    {
        if(shiftX > 0 && shiftZ > 0)
        {
            RenderDiagonalePlane(0);
        }
        if(shiftX > 0 && shiftZ < 0)
        {
            RenderDiagonalePlane(1);
        }
        if(shiftX < 0 && shiftZ > 0)
        {
            RenderDiagonalePlane(2);
        }
        if(shiftX < 0 && shiftZ < 0)
        {
            RenderDiagonalePlane(3);
        }
    }
}

void WeatherSystem::RenderRightPlane()
{
    MatrixScale(Vector3(1.0f,1.0f,1.0f));

    sceGumDrawArray(GU_TRIANGLE_FAN, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, plane);
}

void WeatherSystem::RenderLeftPlane()
{
    MatrixRadianAngle(Vector3(0.0f,PI,0.0f));
    MatrixScale(Vector3(1.0f,1.0f,1.0f));

    sceGumDrawArray(GU_TRIANGLE_FAN, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, plane);
}

void WeatherSystem::RenderBackPlane()
{
    MatrixRadianAngle(Vector3(0.0f,PI/2.0f,0.0f));
    MatrixScale(Vector3(1.0f,1.0f,1.0f));

    sceGumDrawArray(GU_TRIANGLE_FAN, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, plane);
}

void WeatherSystem::RenderFrontPlane()
{
    MatrixRadianAngle(Vector3(0.0f,-PI/2.0f,0.0f));
    MatrixScale(Vector3(1.0f,1.0f,1.0f));

    sceGumDrawArray(GU_TRIANGLE_FAN, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, plane);
}

void WeatherSystem::RenderDiagonalePlane(int type)
{
    switch(type)
    {
        case 0:
        MatrixRadianAngle(Vector3(0.0f,-PI/4.0f,0.0f));
        MatrixScale(Vector3(1.0f,1.0f,1.0f));

        sceGumDrawArray(GU_TRIANGLE_FAN, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, plane);
        break;

        case 1:
        MatrixRadianAngle(Vector3(0.0f,PI/4.0f,0.0f));
        MatrixScale(Vector3(1.0f,1.0f,1.0f));

        sceGumDrawArray(GU_TRIANGLE_FAN, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, plane);
        break;

        case 2:
        MatrixRadianAngle(Vector3(0.0f,PI+PI/4.0f,0.0f));
        MatrixScale(Vector3(1.0f,1.0f,1.0f));

        sceGumDrawArray(GU_TRIANGLE_FAN, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, plane);
        break;

        case 3:
        MatrixRadianAngle(Vector3(0.0f,-PI-PI/4.0f,0.0f));
        MatrixScale(Vector3(1.0f,1.0f,1.0f));

        sceGumDrawArray(GU_TRIANGLE_FAN, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, plane);
        break;
    }
}

