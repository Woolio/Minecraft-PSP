#include <Aurora/Graphics/Models/ObjModel.h>
#include <Aurora/Graphics/TextureManager.h>
#include <Aurora/Utils/Logger.h>

namespace Aurora
{
	namespace Graphics
	{
		void ObjModel::LoadMaterials(const char *Filname)
		{
			FILE *fp = fopen(Filname,"r");

			if (!fp)
			{
				return;
			}

			ObjMaterial *newMaterial = new ObjMaterial();

			int illum = 0;
			char buffer[255] = {0};

			bool newMat = false;

			while (fscanf(fp, "%s", buffer) != EOF)
			{
				switch(buffer[0])
				{
					case 'n': //new material
					{
						if (newMat)
						{
							mMaterials.push_back(newMaterial);
							newMaterial = new ObjMaterial();
							newMat = false;
						}

						fgets(buffer, sizeof(buffer), fp);
						sscanf(buffer, "%s %s", buffer, buffer);

						newMaterial->name = buffer;
					}
					break;

					case 'N': //Ns - shiness
					{
						 fscanf(fp, "%f", &newMaterial->shininess);

						 newMaterial->shininess /= 1000.0f;
					}
					break;

					case 'K':
					{
						switch(buffer[1])
						{
							case 'a'://Ka - ambient
							{
								fscanf(fp, "%f %f %f",&newMaterial->ambient[0],&newMaterial->ambient[1],&newMaterial->ambient[2]);
								newMaterial->ambient[3] = 1.0f;
							}
							break;
							case 'd'://Kd - diffuse
							{
								fscanf(fp, "%f %f %f",&newMaterial->diffuse[0],&newMaterial->diffuse[1],&newMaterial->diffuse[2]);
								newMaterial->diffuse[3] = 1.0f;
							}
							break;
							case 's'://Ks - specular
							{
								fscanf(fp, "%f %f %f",&newMaterial->specular[0],&newMaterial->specular[1],&newMaterial->specular[2]);
								newMaterial->specular[3] = 1.0f;
							}
							break;
							default:
							{
								fgets(buffer, sizeof(buffer), fp);
							}
							break;
						}
					}
					break;

					case 'T': // Tr
					{
						switch (buffer[1])
						{
							case 'r': // Tr
							{
								fscanf(fp, "%f", &newMaterial->alpha);
								newMaterial->alpha = 1.0f - newMaterial->alpha;
							}
							break;

							default:
							{
								fgets(buffer, sizeof(buffer), fp);
							}
							break;
						}
					}
					break;

					case 'd':
					{
						fscanf(fp, "%f", &newMaterial->alpha);
					}
					break;

					case 'i': // illum
					{
						fscanf(fp, "%d", &illum);

						if (illum == 1)
						{
							newMaterial->specular[0] = 0.0f;
							newMaterial->specular[1] = 0.0f;
							newMaterial->specular[2] = 0.0f;
							newMaterial->specular[3] = 1.0f;
						}
					}
					break;

					case 'm': // map_Kd, map_bump
					{
						if (strstr(buffer, "map_Kd") != 0)
						{
							fgets(buffer, sizeof(buffer), fp);
							sscanf(buffer, "%s %s", buffer, buffer);

							std::string pathName(Filname);
							size_t found = pathName.find_last_of("/");
							newMaterial->colorMapFilename = pathName.substr(0,found)+ "/" + buffer;

							TextureManager *tManager = TextureManager::Instance();

							if (!tManager->TextureExist(newMaterial->colorMapFilename))
							{
								tManager->LoadTexture(newMaterial->colorMapFilename);
							}

							newMaterial->texturID = tManager->GetTextureNumber(newMaterial->colorMapFilename);


							//try to load mipmap textures, currently only look for 3
							/*std::string baseName = newMaterial->colorMapFilename.substr(0,newMaterial->colorMapFilename.find_last_of("."));

							if(tManager->FileExist(baseName + "_mip1.png") == true)
							{
								newMaterial->mipmapping = true;

								if (!tManager->TextureExist(baseName + "_mip1.png"))
								{
									tManager->LoadTexture(baseName + "_mip1.png");
								}

								newMaterial->mipmap1 = tManager->GetTextureNumber(baseName + "_mip1.png");
							}

							if(tManager->FileExist(baseName + "_mip2.png") == true)
							{
								newMaterial->mipmapping = true;

								if (!tManager->TextureExist(baseName + "_mip2.png"))
								{
									tManager->LoadTexture(baseName + "_mip2.png");
								}

								newMaterial->mipmap2 = tManager->GetTextureNumber(baseName + "_mip2.png");
							}

							if(tManager->FileExist(baseName + "_mip3.png") == true)
							{
								newMaterial->mipmapping = true;

								if (!tManager->TextureExist(baseName + "_mip3.png"))
								{
									tManager->LoadTexture(baseName + "_mip3.png");
								}

								newMaterial->mipmap3 = tManager->GetTextureNumber(baseName + "_mip3.png");
							}*/

							newMat = true;

						}
						else if (strstr(buffer, "map_bump") != 0)
						{
							fgets(buffer, sizeof(buffer), fp);
							sscanf(buffer, "%s %s", buffer, buffer);

							newMaterial->bumpMapFilename =  "Assets/Obj/";
							newMaterial->bumpMapFilename += buffer;

							TextureManager *tManager = TextureManager::Instance();

							if (!tManager->TextureExist(newMaterial->bumpMapFilename))
							{
								tManager->LoadTexture(newMaterial->bumpMapFilename);
							}

							newMaterial->normaltextureID = tManager->GetTextureNumber(newMaterial->bumpMapFilename);
							newMat = true;
						}
						else
						{
							fgets(buffer, sizeof(buffer), fp);
						}
					}
					break;

					default:
						fgets(buffer, sizeof(buffer), fp);
					break;
				}
			}


			mMaterials.push_back(newMaterial);
			fclose(fp);
		}

		void ObjModel::LoadObj(const char *FileName)
		{
			FILE *fp = fopen(FileName,"r");

			if (!fp)
			{
				return;
			}

			pathName = FileName;

			ObjMesh *newMesh = new ObjMesh();

			char ch				= 0;
			char ch2			= 0;
			char strLine[255]	= {0};
			char buff[512];

			bool faceLoad = false;
			bool uvFace = false;
			bool normal = false;

			while(!feof(fp))
			{
				//get first character
				ch = fgetc(fp);

				switch(ch)
				{
				case 'm': //material info
					{
						char text[100];
						char materialName[100];

						//read material info
						fscanf(fp,"%s %s", &text, &materialName);

						std::string pathmat(FileName);
						size_t found = pathmat.find_last_of("/");
						pathmat = pathmat.substr(0,found)+ "/" + materialName;

						LoadMaterials(pathmat.c_str());

						//read line to the end
						fgets(strLine, 100, fp);
					}
					break;

				case 'u'://use material
				{
					if (faceLoad)
					{
						mMeshes.push_back(newMesh);
						meshCount++;

						//new newmesh
						newMesh = new ObjMesh();
					}
					char name[50];
					char name2[50];

					//read material name
					fscanf(fp,"%s %s", &name, &name2);

					newMesh->mMaterial = 0;

					for(unsigned int i = 0;i < mMaterials.size();i++)
					{
						if(strcmp(mMaterials[i]->name.c_str(),name2) == 0)
						{
							newMesh->mMaterial = i;
						}
					}

					//read line to the end
					fgets(strLine, 100, fp);
				}
				break;

				case 'g': //mesh name
					{
						//read mesh name
						fscanf(fp," %s", newMesh->name);

						//read line to the end
						fgets(strLine, 100, fp);
					}
					break;



				case 'v': //vertex found
					{
						//get second character
						ch2 = fgetc(fp);

						switch(ch2)
						{
						case ' ':
							{
								if (faceLoad)
								{
									mMeshes.push_back(newMesh);
									meshCount++;

									//new newmesh
									newMesh = new ObjMesh();

									faceLoad = false;
									uvFace = false;
									normal = false;
								}

								//new vertex position
								Vector3 vPosition;

								//read vertex info
								fscanf(fp,"%f %f %f", &vPosition.x, &vPosition.y, &vPosition.z);

								allVertex.push_back(vPosition);

								//read line to the end
								fgets(strLine, 100, fp);
							}
							break;

						case 't': //texture info
							{
								uvFace = true;

								//new uv map
								ObjUV nObjUV;

								//read uv map
								fscanf(fp,"%f %f", &nObjUV.u, &nObjUV.v);

								allUVMap.push_back(nObjUV);

								//read line to the end
								fgets(strLine, 100, fp);
							}
							break;

						case 'n': //normal info
							{
								normal = true;

								//new normal position
								Vector3 nNormal;

								//read normal
								fscanf(fp,"%f %f %f", &nNormal.x, &nNormal.y ,&nNormal.z);

								allNormal.push_back(nNormal);

								//read line to the end
								fgets(strLine, 100, fp);
							}
							break;

						default:
							break;
						}
					}
					break;

				case 'f': //face info
					{
						faceLoad = true;

						//new face
						ObjFace nFace;
						ObjFace nUVFace;
						ObjFace nNorm;

						/* Read whole line */
						fgets (buff, sizeof (buff), fp);

						if (sscanf(buff,"%d/%d/%d %d/%d/%d %d/%d/%d",&nFace.x,&nUVFace.x,&nNorm.x,&nFace.y,&nUVFace.y,&nNorm.y,&nFace.z,&nUVFace.z,&nNorm.z) == 9)
						{
							//push UV face to mesh
							nUVFace.x -=1;nUVFace.y -=1;nUVFace.z -=1;

							newMesh->mUVFace.push_back(nUVFace);

							//we must -1 to all indicies
							nNorm.x -=1;nNorm.y -=1;nNorm.z -=1;

							//push
							newMesh->mNormalFace.push_back(nNorm);
							uvFace = true;
							normal = true;
						}else
						if (sscanf(buff,"%d/%d %d/%d %d/%d",&nFace.x,&nUVFace.x,&nFace.y,&nUVFace.y,&nFace.z,&nUVFace.z) == 6)
						{
							//push UV face to mesh
							nUVFace.x -=1;nUVFace.y -=1;nUVFace.z -=1;
							newMesh->mUVFace.push_back(nUVFace);
							uvFace = true;
						}else
						{
							sscanf(buff,"%d %d %d",&nFace.x,&nFace.y,&nFace.z);
						}

						//we must -1 to all indicies
						nFace.x -=1;nFace.y -=1;nFace.z -=1;

						//push face to mesh
						newMesh->mFace.push_back(nFace);
					}
					break;

				case '\n':

					break;

				default:
					//read line to the end
					fgets(strLine, 100, fp);
					break;
				}
			}

			//ad last submesh or if thhere is only one
			mMeshes.push_back(newMesh);

			BuildAABB();

			fclose(fp);
		}

		void ObjModel::BuildAABB()
		{
			for (unsigned int i = 0; i < mMeshes.size();i++)
			{
				ObjMesh *mesh = mMeshes[i];
				mesh->aabb = BoundingBox();

				int j =0;

				for(unsigned int f = 0;f < mesh->mFace.size();f++)
				{

					mesh->aabb.expandToInclude(allVertex[mesh->mFace[f].x]);
					j++;

					mesh->aabb.expandToInclude(allVertex[mesh->mFace[f].y]);
					j++;

					mesh->aabb.expandToInclude(allVertex[mesh->mFace[f].z]);
					j++;

				}

			}
		}

		void ObjModel::Optimize()
		{
			for (unsigned int i = 0; i < mMeshes.size();i++)
			{
				ObjMesh *mesh = mMeshes[i];

				//build mesh vertices
				mesh->meshVertices =  (TexturesPSPVertex*)memalign(16,( mesh->mFace.size() * 3) * sizeof(TexturesPSPVertex));
				mesh->triangleCount = mesh->mFace.size();
				mesh->vertexCount = mesh->mFace.size() * 3;

				int j =0;

				for(unsigned int f = 0;f < mesh->mFace.size();f++)
				{
					//vertices[j].color = GU_RGBA((int)(material->diffuse[0] * 255),(int)(material->diffuse[1] * 255),(int)(material->diffuse[2] * 255),255);
					TexturesPSPVertex vert1;
					vert1.u = allUVMap[mesh->mUVFace[f].x].u;// * 32767;//32767 127
					vert1.v = allUVMap[mesh->mUVFace[f].x].v;// * 32767;;
					vert1.x = allVertex[mesh->mFace[f].x].x;// * 32767;
					vert1.y = allVertex[mesh->mFace[f].x].y;// * 32767;
					vert1.z = allVertex[mesh->mFace[f].x].z;// * 32767;
					memcpy(&mesh->meshVertices[j],&vert1,sizeof(TexturesPSPVertex));

					/*mesh->meshVertices[j].u = allUVMap[mesh->mUVFace[f].x].u;
					mesh->meshVertices[j].v = allUVMap[mesh->mUVFace[f].x].v;
					mesh->meshVertices[j].x = allVertex[mesh->mFace[f].x].x;
					mesh->meshVertices[j].y = allVertex[mesh->mFace[f].x].y;
					mesh->meshVertices[j].z = allVertex[mesh->mFace[f].x].z;*/
					mesh->aabb.expandToInclude(allVertex[mesh->mFace[f].x]);
					j++;

					//vertices[j].color = GU_RGBA((int)(material->diffuse[0] * 255),(int)(material->diffuse[1] * 255),(int)(material->diffuse[2] * 255),255);
					TexturesPSPVertex vert2;
					vert2.u = allUVMap[mesh->mUVFace[f].y].u;//  * 32767;
					vert2.v = allUVMap[mesh->mUVFace[f].y].v;//  * 32767;
					vert2.x = allVertex[mesh->mFace[f].y].x;// * 32767;
					vert2.y = allVertex[mesh->mFace[f].y].y;// * 32767;
					vert2.z = allVertex[mesh->mFace[f].y].z;//  * 32767;
					memcpy(&mesh->meshVertices[j],&vert2,sizeof(TexturesPSPVertex));

					/*mesh->meshVertices[j].u = allUVMap[mesh->mUVFace[f].y].u;
					mesh->meshVertices[j].v = allUVMap[mesh->mUVFace[f].y].v;
					mesh->meshVertices[j].x = allVertex[mesh->mFace[f].y].x;
					mesh->meshVertices[j].y = allVertex[mesh->mFace[f].y].y;
					mesh->meshVertices[j].z = allVertex[mesh->mFace[f].y].z;*/
					mesh->aabb.expandToInclude(allVertex[mesh->mFace[f].y]);
					j++;

					//vertices[j].color = GU_RGBA((int)(material->diffuse[0] * 255),(int)(material->diffuse[1] * 255),(int)(material->diffuse[2] * 255),255);
					TexturesPSPVertex vert3;
					vert3.u = allUVMap[mesh->mUVFace[f].z].u;//  * 32767;
					vert3.v = allUVMap[mesh->mUVFace[f].z].v;//  * 32767;
					vert3.x = allVertex[mesh->mFace[f].z].x;//  * 32767;
					vert3.y = allVertex[mesh->mFace[f].z].y;//  * 32767;
					vert3.z = allVertex[mesh->mFace[f].z].z;//  * 32767;
					memcpy(&mesh->meshVertices[j],&vert3,sizeof(TexturesPSPVertex));

					/*mesh->meshVertices[j].u = allUVMap[mesh->mUVFace[f].z].u;
					mesh->meshVertices[j].v = allUVMap[mesh->mUVFace[f].z].v;
					mesh->meshVertices[j].x = allVertex[mesh->mFace[f].z].x;
					mesh->meshVertices[j].y = allVertex[mesh->mFace[f].z].y;
					mesh->meshVertices[j].z = allVertex[mesh->mFace[f].z].z;*/
					mesh->aabb.expandToInclude(allVertex[mesh->mFace[f].z]);
					j++;

				}
				mesh->mUVFace.clear();
				mesh->mFace.clear();
				mesh->mNormalFace.clear();

				//clear the cache or there will be some errors
				sceKernelDcacheWritebackInvalidateRange(mesh->meshVertices,( mesh->mFace.size() * 3) * sizeof(TexturesPSPVertex));
				//sceKernelDcacheWritebackInvalidateAll();
			}

			allVertex.clear();
			allNormal.clear();
			allUVMap.clear();
		}

		void ObjModel::SaveOptimized(const char *FileName)
		{

		}

		void ObjModel::FindLightMaps()
		{
			TextureManager *tManager = TextureManager::Instance();

			for (unsigned int i = 0; i < mMeshes.size();i++)
			{
				ObjMesh *mesh = mMeshes[i];

				//for each mesh we need to check if there is material
				if (mesh->mMaterial != -1)
				{
					//there is material :) so we are in half way
					//now check if there is already lightmap
					if (mMaterials[mesh->mMaterial]->lightmapping)
					{
						//everything is ok - we don't do anything
					}else
					{
						//lest see if there is a lightmap with the same name as object
						size_t found = pathName.find_last_of("/");
						pathName = pathName.substr(0,found) + "/" + mesh->name + "VRayTotalLightingMap.png";//"LightingMap.png";

						if (!tManager->TextureExist(pathName))
						{
							tManager->LoadTexture(pathName);
						}

						int lightMap = tManager->GetTextureNumber(pathName);

						if (lightMap != -1)
						{
							mMaterials[mesh->mMaterial]->lightmapping = true;
							mMaterials[mesh->mMaterial]->lightMapID = lightMap;
						}
					}
				}else
				{
					//if there is no material then make one :>
					ObjMaterial *newMaterial = new ObjMaterial();

					//lest see if there is a lightmap with the same name as object
					size_t found = pathName.find_last_of("/");
					pathName = pathName.substr(0,found) + "/" + mesh->name + "VRayTotalLightingMap.png";//"LightingMap.png";

					if (!tManager->TextureExist(pathName))
					{
						tManager->LoadTexture(pathName);
					}

					int lightMap = tManager->GetTextureNumber(pathName);
					if (lightMap != -1)
					{
						//set it as a main texture
						newMaterial->texturID = lightMap;

						//add to container
						mMaterials.push_back(newMaterial);
						//assign this material to mesh
						mesh->mMaterial = mMaterials.size() - 1;
					}
				}
			}
		}

		void ObjModel::LoadOptimized(const char *FileName)
		{
			FILE *binaryFile;
			binaryFile = fopen (FileName, "rb");
			if (!binaryFile)
			{
				Utils::Logger::Instance()->LogMessage( "Error: couldn't open \"%s\"!\n", FileName);
				return ;
			}

			pathName = FileName;

			//materials count
			int count = 0;
			fread(&count,sizeof(int),1,binaryFile);

			TextureManager *tManager = TextureManager::Instance();

			//read all materials
			for(int i = 0; i < count;i++)
			{
				char textureName[50];
				fread(textureName,sizeof(textureName),1,binaryFile);

				ObjMaterial *newMaterial = new ObjMaterial();

				newMaterial->colorMapFilename = textureName;

				std::string pathName(FileName);
				size_t found = pathName.find_last_of("/");
				pathName = pathName.substr(0,found)+ "/" + textureName;



				if (!tManager->TextureExist(pathName))
				{
					tManager->LoadTexture(pathName);
				}

				newMaterial->texturID = tManager->GetTextureNumber(pathName);

				//try to load mipmap textures, currently only look for 3
				/*std::string baseName = pathName.substr(0,pathName.find_last_of("."));

				if(tManager->FileExist(baseName + "_mip1.png") == true)
				{
					newMaterial->mipmapping = true;

					if (!tManager->TextureExist(baseName + "_mip1.png"))
					{
						tManager->LoadTexture(baseName + "_mip1.png");
					}

					newMaterial->mipmap1 = tManager->GetTextureNumber(baseName + "_mip1.png");
				}

				if(tManager->FileExist(baseName + "_mip2.png") == true)
				{
					newMaterial->mipmapping = true;

					if (!tManager->TextureExist(baseName + "_mip2.png"))
					{
						tManager->LoadTexture(baseName + "_mip2.png");
					}

					newMaterial->mipmap2 = tManager->GetTextureNumber(baseName + "_mip2.png");
				}

				if(tManager->FileExist(baseName + "_mip3.png") == true)
				{
					newMaterial->mipmapping = true;

					if (!tManager->TextureExist(baseName + "_mip3.png"))
					{
						tManager->LoadTexture(baseName + "_mip3.png");
					}

					newMaterial->mipmap3 = tManager->GetTextureNumber(baseName + "_mip3.png");
				}*/

				mMaterials.push_back(newMaterial);
			}

			//meshes
			count = 0;
			fread(&count,sizeof(int),1,binaryFile);

			//read all meshes
			for(int i = 0; i < count;i++)
			{
				ObjMesh *mesh = new ObjMesh();

				mesh->triangles = false;

				//name
				fread(mesh->name,sizeof(mesh->name),1,binaryFile);

				//material number
				fread(&mesh->mMaterial,sizeof(int),1,binaryFile);

				//aabb min
				fread(&mesh->aabb.min,sizeof(Vector3),1,binaryFile);

				//aabb max
				fread(&mesh->aabb.max,sizeof(Vector3),1,binaryFile);

				//indices count
				fread(&mesh->indicesCount,sizeof(int),1,binaryFile);

				//write all vertices
				mesh->meshVertices = (TexturesPSPVertex*)memalign(16,mesh->indicesCount * sizeof(TexturesPSPVertex));
				fread(mesh->meshVertices,sizeof(TexturesPSPVertex),mesh->indicesCount,binaryFile);

				mMeshes.push_back(mesh);
			}

			//clear the cache or there will be some errors
			sceKernelDcacheWritebackInvalidateAll();


			fclose(binaryFile);
		}

		bool ObjModel::RayIntersection()
		{
			return false;
		}

		bool ObjModel::Collide(Ray &ray,float &t,Vector3 &point)
		{
			float tempT = 100.0f;
			bool tempCollide = false;
			Vector3 normal;

			for (unsigned int i = 0;i < mMeshes.size();i++)
			{
				//check aabb
				if(ray.hasIntersected(mMeshes[i]->aabb))
				{
					//start checking faces
					for (unsigned int j = 0;j < mMeshes[i]->mFace.size();j++)
					{
						ObjFace *tFace = &mMeshes[i]->mFace[j];
						Vector3 pointX = Vector3(allVertex[tFace->x].x,allVertex[tFace->x].y, allVertex[tFace->x].z);
						Vector3 pointY = Vector3(allVertex[tFace->y].x,allVertex[tFace->y].y, allVertex[tFace->y].z);
						Vector3 pointZ = Vector3(allVertex[tFace->z].x,allVertex[tFace->z].y, allVertex[tFace->z].z);

						Vector3 vTriangle[3] = { pointX,pointY,pointZ };
						Vector3 vLine[2]     = { Vector3(ray.origin.x,ray.origin.y - 3,ray.origin.z),  ray.origin };

						if(Polygon::IntersectedPolygon(vTriangle, vLine, 3,point,normal))
						{
							tempCollide = true;
							float tt = Vector3::fromVectors(ray.origin,point).magnitude();
							if(tt < tempT)
								tempT = tt;
						}

					}

					t = tempT;
				}
			}



			return tempCollide;
		}

		bool ObjModel::Collide(Ray &ray,float rayLenght,float &t,Vector3 &point,Vector3 &normal)
		{
			float tempT = 100.0f;
			Vector3 tempV;
			Vector3 tempN;
			bool tempCollide = false;

			for (unsigned int i = 0;i < mMeshes.size();i++)
			{
				//check aabb
				if(ray.hasIntersected(mMeshes[i]->aabb))
				{
					//start checking faces
					for (unsigned int j = 0;j < mMeshes[i]->mFace.size();j++)
					{
						ObjFace *tFace = &mMeshes[i]->mFace[j];
						Vector3 pointX = Vector3(allVertex[tFace->x].x,allVertex[tFace->x].y, allVertex[tFace->x].z);
						Vector3 pointY = Vector3(allVertex[tFace->y].x,allVertex[tFace->y].y, allVertex[tFace->y].z);
						Vector3 pointZ = Vector3(allVertex[tFace->z].x,allVertex[tFace->z].y, allVertex[tFace->z].z);

						Vector3 vTriangle[3] = { pointX,pointY,pointZ };
						Vector3 vLine[2]     = { Vector3((ray.direction.x * rayLenght) + ray.origin.x,(ray.direction.y  * rayLenght) + ray.origin.y,(ray.direction.z * rayLenght) + ray.origin.z),  ray.origin };

						if(Polygon::IntersectedPolygon(vTriangle, vLine, 3,point,normal))
						{
							tempCollide = true;
							float tt = Vector3::fromVectors(ray.origin,point).magnitude();
							if(tt < tempT)
							{
								tempT = tt;
								tempV = point;
								tempN = normal;
							}
						}

					}

					t = tempT;
					point = tempV;
					normal = tempN * (-1.0f);
				}
			}



			return tempCollide;
		}
	}
}
