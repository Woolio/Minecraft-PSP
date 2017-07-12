#include "BaseEntity.h"


BaseEntity::BaseEntity()
{
	vertices = NULL;

	position = Vector3(0,0,0);
	scale = Vector3(0,0,0);
	angle = Vector3(0,0,0);
	collisionBox = Vector3(0,0,0);

	colour = Vector3(0,0,0);
	alpha = 1.0f;

	ortho = false;
}

BaseEntity::~BaseEntity()
{
	if(vertices != NULL)
	{
		free(vertices);
	}
}

void BaseEntity::SetPosition(Vector3 position2)
{
    position = position2;
}

void BaseEntity::SetScale(Vector3 scale2)
{
    scale = scale2;
}

void BaseEntity::SetAngle(Vector3 angle2)
{
    angle = angle2;
}

void BaseEntity::SetCollisionBox(Vector3 colB2)
{
    collisionBox = colB2;
}

void BaseEntity::SetColour(float red, float green, float blue, float alpha2)
{
    colour = Vector3(red, green, blue);
    alpha = alpha2;
}

bool BaseEntity::CheckCollisionPoint(CraftWorld* crtf)
{
    if(crtf->GetBlock(position.x, position.y, position.z) != 0)
    {
        return true;
    }
    return false;
}

