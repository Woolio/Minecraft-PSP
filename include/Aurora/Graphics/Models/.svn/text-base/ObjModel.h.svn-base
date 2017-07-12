#ifndef AURORA_OBJMODEL_H
#define AURORA_OBJMODEL_H

#include <string>
#include <vector>

#include <Aurora/Math/Vector3.h>
#include <Aurora/Math/BoundingBox.h>
#include <Aurora/Math/Polygon.h>
#include <Aurora/Math/Ray.h>
#include <Aurora/Graphics/Vertex.h>

using namespace Aurora::Math;

namespace Aurora
{
	namespace Graphics
	{

		class ObjFace
		{
		public:
			int x,y,z;

			ObjFace()
			{
				x = 0;
				y = 0;
				z = 0;
			}
		};

		class ObjUV
		{
		public:

			float u,v;

			ObjUV()
			{
				u = 0.0f;
				v = 0.0f;
			}
		};



		class ObjMaterial
		{
		public:

			float ambient[4];
			float diffuse[4];
			float specular[4];
			float shininess;        // [0 = min shininess, 1 = max shininess]
			float alpha;            // [0 = fully transparent, 1 = fully opaque]

			std::string name;
			std::string colorMapFilename;
			std::string bumpMapFilename;

			int texturID;
			int lightMapID;
			int normaltextureID;

			int mipmap1;
			int mipmap2;
			int mipmap3;

			//mipmaps
			bool mipmapping;
			bool lightmapping;

			ObjMaterial()
			{
				texturID = 0;
				normaltextureID = 0;
				lightMapID = 0;

				ambient[0] = 0.2f;
				ambient[1] = 0.2f;
				ambient[2] = 0.2f;
				ambient[3] = 1.0f;
				diffuse[0] = 0.8f;
				diffuse[1] = 0.8f;
				diffuse[2] = 0.8f;
				diffuse[3] = 1.0f;
				specular[0] = 0.0f;
				specular[1] = 0.0f;
				specular[2] = 0.0f;
				specular[3] = 1.0f;
				shininess = 0.0f;
				alpha = 1.0f;

				mipmapping = false;
				lightmapping = false;
			}

		};

		class ObjMesh
		{
		public:

			std::vector<ObjFace> mFace;
			std::vector<ObjFace> mUVFace;
			std::vector<ObjFace> mNormalFace;

			TexturesPSPVertex * meshVertices;

			int triangleCount;
			int vertexCount;
			int indicesCount;

			int mMaterial;

			//name
			char name[30];

			bool uvMapping;
			bool isNormals;
			int listCreated;

			BoundingBox aabb;

			bool triangles;
			bool bbverts;

			ObjMesh()
			{
				triangleCount = 0;
				vertexCount = 0;
				mMaterial = -1;
				triangles = true;
				bbverts = false;
				meshVertices = NULL;
			}

			~ObjMesh()
			{
				if(meshVertices != NULL)
					free(meshVertices);
			}
		};

		class ObjModel
		{
		public:
			std::vector<ObjMesh*>	mMeshes;

			std::vector<Vector3>	allVertex;
			std::vector<Vector3>	allNormal;
			std::vector<ObjUV>		allUVMap;

			//materials
			std::vector<ObjMaterial*>	mMaterials;

			//meshes count
			int meshCount;


			ObjModel()
			{
				//meshCount = 0;
			}

			~ObjModel()
			{
				//clear materials
				for(unsigned int i = 0;i < mMaterials.size();i++)
				{
					delete mMaterials[i];
				}
				mMaterials.clear();

				//clean meshes
				for(unsigned int i = 0;i < mMeshes.size();i++)
				{
					delete mMeshes[i];
				}
				mMeshes.clear();
			}

			void LoadMaterials(const char *Filname);
			void LoadObj(const char *FileName);
			void Optimize();

			void SaveOptimized(const char *FileName);
			void LoadOptimized(const char *FileName);

			void FindLightMaps();

			void BuildAABB();
			void BuildBBoxVertices();

			bool RayIntersection();

			bool Collide(Ray &ray,float &t,Vector3 &point);
			bool Collide(Ray &ray,float rayLenght,float &t,Vector3 &point,Vector3 &normal);

		private:

			std::vector<Vector3>	tempVertex;
			std::vector<Vector3>	tempNormal;
			std::vector<ObjUV>		tempUVMap;

			std::string				pathName;

			int getTempVerts(Vector3 v)
			{
				for (unsigned int i = 0; i < tempVertex.size();i++)
				{
					if(v == tempVertex[i])
						return i;
				}
				return -1;
			}

			int getTempNormal(Vector3 n)
			{
				for (unsigned int i = 0; i < tempNormal.size();i++)
				{
					if(n == tempNormal[i])
						return i;
				}
				return -1;
			}

			int getTempUVmap(ObjUV u)
			{
				for (unsigned int i = 0; i < tempUVMap.size();i++)
				{
					if(u.u == tempUVMap[i].u && u.v == tempUVMap[i].v)
						return i;
				}
				return -1;
			}


		};


	}
}


#endif
