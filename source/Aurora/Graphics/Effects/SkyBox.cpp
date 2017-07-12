#include <Aurora/Graphics/Effects/SkyBox.h>
#include <Aurora/Graphics/TextureManager.h>

#define PI 3.1415926535897f
#define DEG_TO_RAD (PI / 180.0f)

namespace Aurora
{
	namespace Graphics
	{
		SkyBox::SkyBox()
		{
		    vertsNum = 0;
            BuildVertexObject();
		}

		SkyBox::~SkyBox()
		{
			free(skyBoxVertices);
		}

		void SkyBox::BuildVertexObject()
		{
		    bool skyBoxMatrix[11*14] = {0,0,0,0,1,1,1,1,1,1,0,0,0,0,
                                        0,0,0,1,1,1,1,1,1,1,1,0,0,0,
                                        0,0,1,1,1,1,1,1,1,1,1,1,0,0,
                                        0,1,1,1,1,1,1,1,1,1,1,1,1,0,
                                        0,1,1,1,1,1,1,1,1,1,1,1,1,0,
                                        1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                                        1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                                        1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                                        1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                                        1,1,0,0,0,0,0,0,0,0,0,0,1,1,
                                        0,0,0,0,0,0,0,0,0,0,0,0,0,0};

            size = 36.0f;
            std::vector<Vector3*> mPosition;
            std::vector<Vector3*> mTriangle;

            int iVertex = 0;
            int points = 0;

            float x;
            float y;
            float z;

            int vert = 0;

            x = -size * 1 -size/2.0f;
            y = 0;
            z = -size * 7 -size/2.0f;

            vert = 0;
            iVertex = 0;
            points = 0;

            for(int i = 0; i < 11; i++)
            {
                for(int j = 0; j < 14; j++)
                {
                    if(skyBoxMatrix[i*14+j] == true)
                    {
                        mPosition.push_back(new Vector3(x+i*size,   y, z+(j+1)*size));
                        mPosition.push_back(new Vector3(x+(i+1)*size, y, z+(j+1)*size));
                        mPosition.push_back(new Vector3(x+(i+1)*size, y, z+j*size));
                        mPosition.push_back(new Vector3(x+i*size,   y, z+j*size));

                        mTriangle.push_back(new Vector3(iVertex, iVertex+1, iVertex+2));
                        mTriangle.push_back(new Vector3(iVertex+2, iVertex+3, iVertex));

                        iVertex += 4;
                        points += 6;
                    }
                }
            }
            vertsNum = points;

            skyBoxVertices = (SimplePSPVertex2*)memalign(16,points * sizeof(SimplePSPVertex2));

            size =  mTriangle.size();
            for(unsigned int j = 0; j < size; j++)
            {
                skyBoxVertices[vert].x = mPosition[mTriangle[j]->x]->x;
                skyBoxVertices[vert].y = mPosition[mTriangle[j]->x]->y;
                skyBoxVertices[vert].z = mPosition[mTriangle[j]->x]->z;
                vert++;

                skyBoxVertices[vert].x = mPosition[mTriangle[j]->y]->x;
                skyBoxVertices[vert].y = mPosition[mTriangle[j]->y]->y;
                skyBoxVertices[vert].z = mPosition[mTriangle[j]->y]->z;
                vert++;

                skyBoxVertices[vert].x = mPosition[mTriangle[j]->z]->x;
                skyBoxVertices[vert].y = mPosition[mTriangle[j]->z]->y;
                skyBoxVertices[vert].z = mPosition[mTriangle[j]->z]->z;
                vert++;
            }

            sceKernelDcacheWritebackInvalidateRange(skyBoxVertices,( mTriangle.size() * 3) * sizeof(SimplePSPVertex2));

            for(unsigned int aa = 0; aa < mPosition.size(); aa++)
            {
                delete mPosition[aa];
            }
            mPosition.clear();

            for(unsigned int aa = 0; aa < mTriangle.size(); aa++)
            {
                delete 	mTriangle[aa];
            }
            mTriangle.clear();
		}

		void SkyBox::Update()
		{

		}

		void SkyBox::Render(Vector3 color, Vector3 playerPos, float camAngle)
		{
            sceGumRotateX(0.0f);
            sceGumRotateY(-camAngle);
            sceGumRotateZ(0.0f);

		    sceGuColor(GU_COLOR(color.x,color.y,color.z,1.0f));
		    sceGuFrontFace(GU_CW);

			sceGuEnable(GU_BLEND);
            sceGuDisable(GU_DEPTH_TEST);
			//sceGuDepthMask(GU_TRUE);

			sceGumDrawArray(GU_TRIANGLES, GU_VERTEX_32BITF | GU_TRANSFORM_3D, vertsNum, 0, skyBoxVertices);

           // sceGuDepthMask(GU_FALSE);
            sceGuEnable(GU_DEPTH_TEST);
			sceGuDisable(GU_BLEND);

			sceGuFrontFace(GU_CCW);
		}
	}
}

