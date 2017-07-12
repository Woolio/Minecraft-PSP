#include "BaseBlock.h"


BaseBlock::BaseBlock()
{
	textureRow = 0;
	upPlane = 0;
	downPlane = 0;
	sidePlane = 0;

	vertices = NULL;

	soundType = -1;

	editable = false;
	transparent = true;
	lightSource = false;
	blockSpecial = false;
}

BaseBlock::~BaseBlock()
{
	if(vertices != NULL)
	{
		free(vertices);
	}
}

block_t BaseBlock::getBaseID(){	return 0;}
