#include "BlockEntity.h"

BlockEntity::BlockEntity(int create_x, int create_y,  int create_z)
{
    x = create_x;
    y = create_y;
    z = create_z;
}

int BlockEntity::GetX()
{
    return x;
}

int BlockEntity::GetY()
{
    return y;
}

int BlockEntity::GetZ()
{
    return z;
}

Vector3 BlockEntity::GetPosition()
{
    return Vector3(x,y,z);
}

BlockEntity::~BlockEntity()
{

}

