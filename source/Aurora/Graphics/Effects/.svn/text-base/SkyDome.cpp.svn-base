#include <Aurora/Graphics/Effects/SkyDome.h>
#include <Aurora/Graphics/TextureManager.h>

#define PI 3.1415926535897f
#define DEG_TO_RAD (PI / 180.0f)


namespace Aurora
{
	namespace Graphics
	{

		SkyDome::SkyDome()
		{
			dtheta = 5;
			dphi = 5;

			numVerts = 2556;
			domeVertices = NULL;

			timeOfDay = 0.0f;
		}

		SkyDome::~SkyDome()
		{
			if(domeVertices != NULL)
				free(domeVertices);
		}

		void SkyDome::CreateSkyDomeMesh()
		{
			domeVertices = (TexturesPSPVertex*)memalign(16,numVerts * sizeof(TexturesPSPVertex));

			//assign vertices
			int i = 0;
			int radius = 500;
			float textureScale = 1.0f / (90 / dphi);
			for (int phi = 0; phi < 90; phi += dphi)
			{
				for (int theta = 0; theta < 360 - dtheta; theta += dtheta)
				{
					/* vertex (phi + dphi, theta) */
					TexturesPSPVertex vert1;
					vert1.x = 320.0f + radius * vfpu_cosf((phi + dphi) * DEG_TO_RAD) *vfpu_cosf(theta * DEG_TO_RAD);
					vert1.y = radius * vfpu_sinf((phi + dphi) * DEG_TO_RAD);
					vert1.z = 320.0f + radius * vfpu_cosf((phi + dphi) * DEG_TO_RAD) * vfpu_sinf(theta * DEG_TO_RAD);
					vert1.u = 0;
					vert1.v = (1.0f / 64) * vert1.y * textureScale;
					memcpy(&domeVertices[i],&vert1,sizeof(TexturesPSPVertex));
					i++;

					/* vertex (phi, theta) */
					TexturesPSPVertex vert2;
					vert2.x = 320.0f + radius * vfpu_cosf(phi * DEG_TO_RAD) * vfpu_cosf(theta * DEG_TO_RAD);
					vert2.y = radius * vfpu_sinf(phi * DEG_TO_RAD);
					vert2.z = 320.0f + radius * vfpu_cosf(phi * DEG_TO_RAD) * vfpu_sinf(theta * DEG_TO_RAD);
					vert2.u = 0;
					vert2.v = (1.0f / 64) * vert2.y * textureScale;
					memcpy(&domeVertices[i],&vert2,sizeof(TexturesPSPVertex));
					i++;
				}
			}

			//sceKernelDcacheWritebackInvalidateRange(domeVertices,numVerts * sizeof(CraftPSPVertex));
			sceKernelDcacheWritebackInvalidateAll();
		}

		void SkyDome::SetTexture(int texture)
		{
			textureNumber = texture;
		}

		void SkyDome::Render()
		{
			sceGuColor(0xFFFFFFFF);
			sceGuEnable(GU_TEXTURE_2D);
			Image* image = TextureManager::Instance()->Images[textureNumber];

			sceGuTexMode(image->ColorMode,0,0,image->Swizzle);
			sceGuTexFunc( GU_TFX_REPLACE, GU_TCC_RGBA);
			sceGuTexFilter( GU_LINEAR, GU_LINEAR );
			sceGuTexScale(1.0f, 1.0f);
			sceGuTexOffset(timeOfDay, 0.0f );
			sceGuTexWrap(GU_REPEAT, GU_CLAMP);
			sceGuTexImage(0,image->power2Width,image->power2Height,image->power2Width,image->ImageData);

			sceGumDrawArray(GU_TRIANGLE_STRIP, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, numVerts, 0, domeVertices);


			sceGuDisable(GU_TEXTURE_2D);
		}

	}
}

