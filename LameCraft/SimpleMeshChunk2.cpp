#include "SimpleMeshChunk2.h"
#include <Aurora/Graphics/RenderManager.h>

template <typename T>
void FreeAll( T & t ) {
	T tmp;
	t.swap( tmp );
}


SimpleMeshChunk::SimpleMeshChunk()
{
	trienglesCount = 0;
	created = true;
	needUpdate = false;
	periodicallyUpadted = false;

	inFrustum = false;

	meshVertices = NULL;
}

SimpleMeshChunk::SimpleMeshChunk(int _chunkSize)
{
	trienglesCount = 0;
	created = true;
	needUpdate = false;
	periodicallyUpadted = false;

	inFrustum = false;

	meshVertices = NULL;
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

}

void SimpleMeshChunk::vert(float x,float y,float z,float u,float v,float r,float g,float b)
{
    CraftPSPVertex vertex;

	vertex.u = u;
	vertex.v = v;
	vertex.color = GU_COLOR(r,g,b,1);
	vertex.x = x;
	vertex.y = y;
	vertex.z = z;

	mVertices.push_back(vertex);
}

void SimpleMeshChunk::position(float x,float y,float z)
{
	CraftPSPVertex vertex;

	vertex.x = x;
	vertex.y = y;
	vertex.z = z;

	mVertices.push_back(vertex);
}

void SimpleMeshChunk::textureCoord(float u,float v)
{
	mVertices[mVertices.size()-1].u = u;
	mVertices[mVertices.size()-1].v = v;
}

void SimpleMeshChunk::colour(float x,float y,float z)
{
	mVertices[mVertices.size()-1].color = GU_COLOR(x,y,z,1);
}

void SimpleMeshChunk::triangle(int x,int y,int z)
{
    Vector3 triangle;
    triangle.x = x;
    triangle.y = y;
    triangle.z = z;

	mTriangle.push_back(triangle);
}

void SimpleMeshChunk::end()
{
	if(mTriangle.size() > 0)
	{
		//optimize here
		meshVertices = (CraftPSPVertex*)memalign(16,( mTriangle.size() * 3) * sizeof(CraftPSPVertex));

		//vertices
		int vert = 0;
		unsigned int size0 =  mTriangle.size();
		for(unsigned int i = 0;i < size0;i++)
		{
			meshVertices[vert].u = mVertices[mTriangle[i].x].u;
			meshVertices[vert].v = mVertices[mTriangle[i].x].v;
			meshVertices[vert].color = mVertices[mTriangle[i].x].color;
			meshVertices[vert].x = mVertices[mTriangle[i].x].x;
			meshVertices[vert].y = mVertices[mTriangle[i].x].y;
			meshVertices[vert].z = mVertices[mTriangle[i].x].z;
			vert++;

			meshVertices[vert].u = mVertices[mTriangle[i].y].u;
			meshVertices[vert].v = mVertices[mTriangle[i].y].v;
			meshVertices[vert].color = mVertices[mTriangle[i].y].color;
			meshVertices[vert].x = mVertices[mTriangle[i].y].x;
			meshVertices[vert].y = mVertices[mTriangle[i].y].y;
			meshVertices[vert].z = mVertices[mTriangle[i].y].z;
			vert++;

			meshVertices[vert].u = mVertices[mTriangle[i].z].u;
			meshVertices[vert].v = mVertices[mTriangle[i].z].v;
			meshVertices[vert].color = mVertices[mTriangle[i].z].color;
			meshVertices[vert].x = mVertices[mTriangle[i].z].x;
			meshVertices[vert].y = mVertices[mTriangle[i].z].y;
			meshVertices[vert].z = mVertices[mTriangle[i].z].z;
			vert++;
		}
		//clear the cache or there will be some errors
		sceKernelDcacheWritebackInvalidateRange(meshVertices,( mTriangle.size() * 3) * sizeof(CraftPSPVertex));
		//sceKernelDcacheWritebackInvalidateAll();

		trienglesCount = size0 * 3;

		//clean buffers
		FreeAll(mVertices);
		FreeAll(mTriangle);
	}else
	{
		trienglesCount = 0;
	}

	created = true;
	needUpdate = false;
}

void SimpleMeshChunk::drawChunk()
{
	if(trienglesCount > 0 && meshVertices != NULL)
    {
		sceGumDrawArray(GU_TRIANGLES, GU_TEXTURE_32BITF |GU_COLOR_8888|GU_VERTEX_32BITF| GU_TRANSFORM_3D, trienglesCount, 0, meshVertices);
    }
}

void SimpleMeshChunk::reset()
{
	if(trienglesCount > 0 && meshVertices != NULL)
	{
		free(meshVertices);
		trienglesCount = 0;
	}
	created = false;
}
