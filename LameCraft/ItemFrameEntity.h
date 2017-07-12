#ifndef ITEMFRAMEENTITY_H
#define ITEMFRAMEENTITY_H

#include "BlockEntity.h"

#include <string>
#include <pspmath.h>
#include <psptypes.h>
#include <Aurora/Graphics/Vertex.h>
#include <Aurora/Math/BoundingBox.h>
#include <Lamecraft/TextureHelper.h>

class CraftWorld;

class ItemFrameEntity : public BlockEntity
{
public:
	ItemFrameEntity(int create_x, int create_y, int create_z, unsigned int create_facing);
	virtual ~ItemFrameEntity();

    void SetFacing(unsigned int new_facing);
    unsigned int GetFacing();

    void SetItem(unsigned int new_itemID);
    void ResetItem();
    unsigned int GetItem();

    void SetAmount(int new_amount);
    int GetAmount();

    void Render(CraftWorld *mWorld, Frustum &camFrustum, float dt);

private:
    unsigned int facing;
    unsigned int itemID;

    int itemAmount;

    float updateLightTimer;
    float light;

    BoundingBox bBox;
};

#endif
