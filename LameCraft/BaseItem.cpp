#include "BaseItem.h"


BaseItem::BaseItem()
{
	textureRow = 0;
	plane = 0;

	points = 0;
	addPoints = 0;

	furnItem = -1;
	durabilityPoints = -1;
	itemType = 'D';

	vertices = NULL;
	addVertices = NULL;

	terrainTexture = false;
	stackable = true;

	name = "00";
}

BaseItem::~BaseItem()
{
	if(vertices != NULL)
	{
		free(vertices);
		free(addVertices);
		free(add2Vertices);
		free(dropVertices);
	}
}

item_t BaseItem::getBaseID(){	return 0;}
