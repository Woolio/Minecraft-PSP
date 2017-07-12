#include "SimpleMeshChunk2.h"
#include <Aurora/Graphics/RenderManager.h>

SimpleMeshChunk::SimpleMeshChunk()
{
	trienglesCount = 0;
	created = true;
	needUpdate = false;
}

SimpleMeshChunk::~SimpleMeshChunk()
{
	if(trienglesCount > 0)
	{
		free(meshVertices);
	}
}

void SimpleMeshChunk::info(float x,float y,float z,float u,float v,float r,float g,float b)
{
	mPosition.push_back(new Vector3(x,y,z));
	mtextures.push_back(new Vector2(u,v));
	mColour.push_back(new Vector3(r,g,b));
}

void SimpleMeshChunk::position(float x,float y,float z)
{
	mPosition.push_back(new Vector3(x,y,z));
}

void SimpleMeshChunk::textureCoord(float u,float v)
{
	mtextures.push_back(new Vector2(u,v));
}

void SimpleMeshChunk::triangle(int x,int y,int z)
{
	mTriangle.push_back(new Vector3(x,y,z));
}

void SimpleMeshChunk::colour(float x,float y,float z)
{
	mColour.push_back(new Vector3(x,y,z));
}

void SimpleMeshChunk::end()
{
	if(mTriangle.size() > 0)
	{
		//optimize here
		meshVertices = (CraftPSPVertex*)memalign(16,( mTriangle.size() * 3) * sizeof(CraftPSPVertex));

		//vertices
		int vert = 0;
		unsigned int size =  mTriangle.size();
		for(unsigned int i = 0;i < size;i++)
		{
			meshVertices[vert].u = mtextures[mTriangle[i]->x]->x;
			meshVertices[vert].v = mtextures[mTriangle[i]->x]->y;
			meshVertices[vert].color = GU_COLOR( mColour[mTriangle[i]->x]->x,mColour[mTriangle[i]->x]->y,mColour[mTriangle[i]->x]->z,1.0f);
			meshVertices[vert].x = mPosition[mTriangle[i]->x]->x;
			meshVertices[vert].y = mPosition[mTriangle[i]->x]->y;
			meshVertices[vert].z = mPosition[mTriangle[i]->x]->z;
			vert++;

			meshVertices[vert].u = mtextures[mTriangle[i]->y]->x;
			meshVertices[vert].v = mtextures[mTriangle[i]->y]->y;
			meshVertices[vert].color = GU_COLOR( mColour[mTriangle[i]->y]->x,mColour[mTriangle[i]->y]->y,mColour[mTriangle[i]->y]->z,1.0f);
			meshVertices[vert].x = mPosition[mTriangle[i]->y]->x;
			meshVertices[vert].y = mPosition[mTriangle[i]->y]->y;
			meshVertices[vert].z = mPosition[mTriangle[i]->y]->z;
			vert++;

			meshVertices[vert].u = mtextures[mTriangle[i]->z]->x;
			meshVertices[vert].v = mtextures[mTriangle[i]->z]->y;
			meshVertices[vert].color = GU_COLOR( mColour[mTriangle[i]->z]->x,mColour[mTriangle[i]->z]->y,mColour[mTriangle[i]->z]->z,1.0f);
			meshVertices[vert].x = mPosition[mTriangle[i]->z]->x;
			meshVertices[vert].y = mPosition[mTriangle[i]->z]->y;
			meshVertices[vert].z = mPosition[mTriangle[i]->z]->z;
			vert++;
		}

		//clear the cache or there will be some errors
		sceKernelDcacheWritebackInvalidateRange(meshVertices,( mTriangle.size() * 3) * sizeof(CraftPSPVertex));
		//sceKernelDcacheWritebackInvalidateAll();

		//indices
		trienglesCount = size * 3;

		for(unsigned int aa = 0;aa < mPosition.size();aa++)
		{
			delete mPosition[aa];
			delete mtextures[aa];
			delete mColour[aa];
		}
		mPosition.clear();
		mtextures.clear();
		mColour.clear();

		for(unsigned int aa = 0;aa < mTriangle.size();aa++)
			delete 		mTriangle[aa];
		mTriangle.clear();
	}else
	{
		trienglesCount = 0;
	}

	created = true;
	needUpdate = false;
}

void SimpleMeshChunk::drawChunk()
{
	if(trienglesCount > 0)
		sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF |GU_COLOR_8888| GU_VERTEX_32BITF | GU_TRANSFORM_3D, trienglesCount, 0, meshVertices);
}

void SimpleMeshChunk::reset()
{
	if(trienglesCount > 0)
	{
		free(meshVertices);
		trienglesCount = 0;
	}
	created = false;
}
