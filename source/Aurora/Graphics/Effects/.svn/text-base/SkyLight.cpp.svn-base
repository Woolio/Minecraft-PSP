#include <Aurora/Graphics/Effects/SkyLight.h>
#include <Aurora/Graphics/TextureManager.h>

#define PI 3.1415926535897f
#define DEG_TO_RAD (PI / 180.0f)

namespace Aurora
{
	namespace Graphics
	{
		SkyLight::SkyLight()
		{
			time_sunrise = 21600;	// time of sunrise in seconds -- 06.00
			time_sunset = 75600;		// time of sunset in seconds -- 21.00
			sun_tilt = 45.0f * DEG_TO_RAD;
			dis = ((((time_sunrise + time_sunset) - 86400.0f) / 3600.0f) / 24.f) * 2.0f * PI;
			stepScale = 10.0f;
			lightHScale = 100.0f;
			lightVScale = 100.0f;

			skyVertices = (TexturesPSPVertex*)memalign(16,4 * sizeof(TexturesPSPVertex));
		}

		SkyLight::~SkyLight()
		{
			free(skyVertices);
		}

		float SkyLight::TimeToAngle(int seconds)
		{
			return seconds / 240.0f;
		}

		void SkyLight::SetTexture(int texture)
		{
			textureNumber = texture;
		}

		void  SkyLight::UpdateLightSource(float angle)
		{
			ScePspFMatrix4 t;
			sceGumMatrixMode(GU_VIEW);
			sceGumStoreMatrix(&t);
			sceGumMatrixMode(GU_MODEL);

			ScePspFVector3 up, right;

			right.x = t.x.x * lightHScale * 0.5f;
			right.y = t.y.x * lightHScale * 0.5f;
			right.z = t.z.x * lightHScale * 0.5f;
			up.x = t.x.y * lightVScale * 0.5f;
			up.y = t.y.y * lightVScale * 0.5f;
			up.z = t.z.y * lightVScale * 0.5f;

			//printf("angle: %f\n", angle);
			float r = 450.0f;
			sun_x = 320.0f + (r * vfpu_sinf((angle - dis) * DEG_TO_RAD));
			sun_y = -r * vfpu_cosf((angle - dis) * DEG_TO_RAD) * vfpu_cosf(sun_tilt);
			sun_z = 320.0f + (r * vfpu_cosf((angle - dis) * DEG_TO_RAD) * vfpu_sinf(sun_tilt));

			//float textureScale = 1.0f / stepScale;
			int i = 0;

			skyVertices[i].x = sun_x - right.x + up.x;// * stepScale;
			skyVertices[i].y = sun_y - right.y + up.y;// * stepScale;
			skyVertices[i].z = sun_z - right.z + up.z;// * stepScale;
			skyVertices[i].u = 0.f;// * textureScale;
			skyVertices[i].v = 0.f;// * textureScale;
			i++;

			// (x, y - 1, z)
			skyVertices[i].x = sun_x - right.x - up.x;// * stepScale;
			skyVertices[i].y = sun_y - right.y - up.y;// * stepScale;
			skyVertices[i].z = sun_z - right.z - up.z;// * stepScale;
			skyVertices[i].u = 0.f;// * textureScale;
			skyVertices[i].v = 1.f;// * textureScale;
			i++;

			// (x + 1, y, z)
			skyVertices[i].x = sun_x + right.x + up.x;// * stepScale;
			skyVertices[i].y = sun_y + right.y + up.y;// * stepScale;
			skyVertices[i].z = sun_z + right.z + up.z;// * stepScale;
			skyVertices[i].u = 1.f;// * textureScale;
			skyVertices[i].v = 0.f;// * textureScale;
			i++;

			// (x + 1, y - 1, z)
			skyVertices[i].x = sun_x + right.x - up.x;// * stepScale;
			skyVertices[i].y = sun_y + right.y - up.y;//* stepScale;
			skyVertices[i].z = sun_z + right.z - up.z;// * stepScale;
			skyVertices[i].u = 1.f;// * textureScale;
			skyVertices[i].v = 1.f;// * textureScale;

			sceKernelDcacheWritebackInvalidateRange(skyVertices,4 * sizeof(CraftPSPVertex));
		}

		void SkyLight::Render()
		{
			sceGuColor(0xFFFFFFFF);
			sceGuEnable(GU_TEXTURE_2D);
			sceGuEnable(GU_BLEND);

			sceGuBlendFunc(GU_ADD, GU_FIX,GU_FIX, 0xFFFFFFFF, 0xFFFFFFFF);

			Image* image = TextureManager::Instance()->Images[textureNumber];

			sceGuTexMode(image->ColorMode,0,0,image->Swizzle);
			sceGuTexFunc( GU_TFX_MODULATE, GU_TCC_RGBA);
			sceGuTexFilter(GU_LINEAR, GU_LINEAR);
			sceGuTexScale(1.0f, 1.0f);                // No scaling
			sceGuTexOffset(0.0f, 0.0f);
			sceGuTexImage(0,image->power2Width,image->power2Height,image->power2Width,image->ImageData);

			sceGumDrawArray(GU_TRIANGLE_STRIP, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, skyVertices);

			sceGuDisable(GU_TEXTURE_2D);
			sceGuDisable(GU_BLEND);

			//make default blend function
			sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
		}
	}
}

