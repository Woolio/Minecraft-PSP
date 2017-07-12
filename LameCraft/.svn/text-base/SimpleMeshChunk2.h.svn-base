#ifndef SIMPLEMESHCHUNK_H
#define SIMPLEMESHCHUNK_H

#include <vector>

//framework
#include <Aurora/Math/BoundingBox.h>
#include <Aurora/Math/Vector2.h>
#include <Aurora/Graphics/Vertex.h>

using namespace Aurora::Math;
using namespace Aurora::Graphics;

class SimpleMeshChunk
{
public:
	
	int id;

	int chunkStartZ;
	int chunkStartY;
	int chunkStartX;

	BoundingBox bBox;

	SimpleMeshChunk();
	~SimpleMeshChunk();

	int trienglesCount;
	bool created;
	bool needUpdate;

	void position(float x,float y,float z);
	void textureCoord(float u,float v);
	void triangle(int x,int y,int z);
	void colour(float x,float y,float z);

	void info(float x,float y,float z,float u,float v,float r,float g,float b);

	void start();
	void end();

	void drawChunk();
	void reset();

private:

	std::vector<Vector3*> mPosition;
	std::vector<Vector3*> mColour;
	std::vector<Vector2*> mtextures;
	std::vector<Vector3*> mTriangle;

	CraftPSPVertex * meshVertices;
};

#endif

