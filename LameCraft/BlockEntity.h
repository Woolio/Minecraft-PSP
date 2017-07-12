#ifndef BLOCKENTITY_H
#define BLOCKENTITY_H

#include <Aurora/Graphics/Vertex.h>
#include <Aurora/Math/BoundingBox.h>
#include <stdlib.h>
#include <string>
#include <Aurora/Graphics/RenderManager.h>
#include <pspmath.h>
#include <psptypes.h>

class BlockEntity
{
public:
	BlockEntity(int create_x, int create_y, int create_z);
	virtual ~BlockEntity();

    int GetX();
    int GetY();
    int GetZ();

    Vector3 GetPosition();

private:
	int x;
	int y;
	int z;
};

#endif
