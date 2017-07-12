#include "ItemFrameEntity.h"
#include "CraftWorld2.h"

#define PIXEL 1.0f / 16.0f
#define PI 3.14159f

ItemFrameEntity::ItemFrameEntity(int create_x, int create_y, int create_z, unsigned int create_facing) : BlockEntity(create_x, create_y, create_z)
{
    facing = create_facing;
    itemID = -1;
    itemAmount = -1;

    bBox =  BoundingBox(Vector3(GetX(),GetY(),GetZ()),Vector3(GetX()+1,GetY()+1,GetZ()+1));

    updateLightTimer = 0.6f;
    light = 0.0f;
}

unsigned int ItemFrameEntity::GetFacing()
{
    return facing;
}

unsigned int ItemFrameEntity::GetItem()
{
    return itemID;
}

int ItemFrameEntity::GetAmount()
{
    return itemAmount;
}

void ItemFrameEntity::SetFacing(unsigned int new_facing)
{
    facing = new_facing;
}

void ItemFrameEntity::SetItem(unsigned int new_itemID)
{
    itemID = new_itemID;
}

void ItemFrameEntity::SetAmount(int new_amount)
{
    itemAmount = new_amount;
}

void ItemFrameEntity::ResetItem()
{
    itemID = -1;
}

void ItemFrameEntity::Render(CraftWorld *mWorld, Frustum &camFrustum, float dt)
{
    if(camFrustum.BoxInFrustum(bBox) == Frustum::Intersects && ( ((GetZ() - mWorld->playerPos.z) <= 19) && ((GetY() - mWorld->playerPos.y) <= 12) && ((GetX() - mWorld->playerPos.x) <= 19) ))
    {
        sceGumPushMatrix();
        sceGuEnable(GU_DEPTH_TEST);
        sceGuEnable(GU_ALPHA_TEST);

        float x_offset = 0.0f;
        float z_offset = 0.0f;
        float rotate_y_offset;

        updateLightTimer += dt;
        if(updateLightTimer > 0.4f)
        {
            updateLightTimer = 0.0f;
            light = mWorld->BlockFinalLight(GetX(),GetY(),GetZ());
        }

        if(GetItem() < 250) // block
        {
            TextureManager::Instance()->SetTextureModeulate(mWorld->textureTerrainId);

            switch(GetFacing())
            {
                case 1: x_offset = -PIXEL*7.5f; rotate_y_offset = -PI*0.5f; break;
                case 2: x_offset = PIXEL*7.5f;  rotate_y_offset = -PI*1.5f; break;
                case 3: z_offset = -PIXEL*7.5f; rotate_y_offset = -PI; break;
                case 4: z_offset = PIXEL*7.5f;  rotate_y_offset = 0.0f; break;
            }
            if(mWorld->blockTypes[GetItem()].blockModel == 2 || mWorld->blockTypes[GetItem()].blockModel == 6) // if it is fence
            {
                rotate_y_offset += PI/2.0f; // make their model render correct
            }

            ScePspFVector3 loc = {GetX()+0.5f+x_offset,GetY()+0.5f,GetZ()+0.5f+z_offset};
            sceGumTranslate(&loc);

            sceGumRotateX(0.0f);
            sceGumRotateY(rotate_y_offset);
            sceGumRotateZ(0.0f);

            ScePspFVector3 scale = {0.3f,0.3f,0.3f};
            sceGumScale(&scale);

            mWorld->drawCubes(GetItem(),light); // should optimize final light params in future
        }
        else // item
        {
            if(mWorld->ItemHaveTerrainTexture(GetItem()))
            {
                TextureManager::Instance()->SetTextureModeulate(mWorld->textureTerrainId);
            }
            else
            {
                TextureManager::Instance()->SetTextureModeulate(mWorld->textureItemsId);
            }


            switch(GetFacing())
            {
                case 1: x_offset = -PIXEL*6.9f; rotate_y_offset = -PI*0.5f; break;
                case 2: x_offset = PIXEL*6.9f;  rotate_y_offset = -PI*1.5f; break;
                case 3: z_offset = -PIXEL*6.9f; rotate_y_offset = -PI; break;
                case 4: z_offset = PIXEL*6.9f;  rotate_y_offset = 0.0f; break;
            }
            rotate_y_offset -= PI/2.0f;

            sceGuColor(GU_COLOR(light,light,light,1.0f));

            ScePspFVector3 loc = {GetX()+0.5f+x_offset,GetY()+0.5f,GetZ()+0.5f+z_offset};
            sceGumTranslate(&loc);

            sceGumRotateX(0.0f);
            sceGumRotateY(rotate_y_offset);
            sceGumRotateZ(0.0f);

            ScePspFVector3 scale = {1.0f,1.0f,1.0f};
            sceGumScale(&scale);

            mWorld->drawDropItems(GetItem());
        }

        sceGuDisable(GU_ALPHA_TEST);
        sceGuDisable(GU_DEPTH_TEST);
        sceGumPopMatrix();
    }
}

ItemFrameEntity::~ItemFrameEntity()
{

}

