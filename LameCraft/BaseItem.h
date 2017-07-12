#ifndef BASEITEM_H
#define BASEITEM_H

#include <Aurora/Graphics/Vertex.h>
#include <stdlib.h>
#include <string>

using namespace Aurora::Graphics;

typedef unsigned short item_t;

class BaseItem
{
public:
	char textureRow;
	char plane;

	short furnItem;
    short durabilityPoints;

	short points;
	short addPoints;
	short add2Points;

	bool terrainTexture;
    bool stackable;

	char itemType; // "D" - default, "T" - tool, "F" - food, "H" - helmet, "C" - chestplate, "L" - leggings, "B" - boots "Y" - dye

	TexturesPSPVertex *vertices;
	TexturesPSPVertex *addVertices;
	TexturesPSPVertex *add2Vertices;

	TexturesPSPVertex *dropVertices;

	BaseItem();
	virtual ~BaseItem();

	static item_t getBaseID();
    std::string name;
};

#endif
