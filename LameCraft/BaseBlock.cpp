#include "BaseBlock.h"
#include "CraftWorld2.h"

BaseBlock::BaseBlock()
{
	upPlane_x = 0;
	upPlane_y = 0;

	downPlane_x = 0;
	downPlane_y = 0;

	leftPlane_x = 0;
	leftPlane_y = 0;

    rightPlane_x = 0;
    rightPlane_y = 0;

    botPlane_x = 0;
    botPlane_y = 0;

    forPlane_x = 0;
    forPlane_y = 0;

    verticesLeftRight = NULL;
    pointsLeftRight = 0;

    verticesBackFront = NULL;
    pointsBackFront = 0;

    verticesBottomTop = NULL;
    pointsBottomTop = 0;

    material = 0;
	furnItem = -1;
    loot = -1;
	soundType = -1;
	blockModel = 0;
	refraction = 1;

	editable = false;
	transparent = true;
	lightSource = false;
	blockModel = 0;
	solid = false;
	update = false;
	animated = false;
	allowLight = true;

	collideBox = BoundingBox(Vector3(-0.5f,-0.5f,-0.5f),Vector3(0.5f,0.5f,0.5f));

	name = " ";
}

BaseBlock::~BaseBlock()
{
	if(verticesLeftRight != NULL)
	{
		free(verticesLeftRight);
	}
	if(verticesBackFront != NULL)
	{
		free(verticesBackFront);
	}
	if(verticesBottomTop != NULL)
	{
		free(verticesBottomTop);
	}
}

bool BaseBlock::MatchWithRecipe2x2(std::string recipe_string[2], int& return_id, int& return_am, bool& return_st) // may be in future
{
    return_id = -1;
    return_am = -1;
    return_st = false;
    return false;
}

bool BaseBlock::MatchWithRecipe3x3(std::string recipe_string[3], int& return_id, int& return_am, bool& return_st)
{
    return_id = -1;
    return_am = -1;
    return_st = false;
    return false;
}

block_t BaseBlock::getBaseID(){return 0;}
