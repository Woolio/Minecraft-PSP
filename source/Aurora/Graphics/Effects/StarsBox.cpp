#include <Aurora/Graphics/Effects/StarsBox.h>
#include <Aurora/Graphics/TextureManager.h>

#define PI 3.1415926535897f
#define DEG_TO_RAD (PI / 180.0f)

namespace Aurora
{
	namespace Graphics
	{
		StarsBox::StarsBox()
		{
			boxVertices = (TexturesPSPVertex*)memalign(16,4 * sizeof(TexturesPSPVertex));
			boxVertices2 = (TexturesPSPVertex*)memalign(16,4 * sizeof(TexturesPSPVertex));
			boxVertices3 = (TexturesPSPVertex*)memalign(16,4 * sizeof(TexturesPSPVertex));
			boxVertices4 = (TexturesPSPVertex*)memalign(16,4 * sizeof(TexturesPSPVertex));
			boxVertices5 = (TexturesPSPVertex*)memalign(16,4 * sizeof(TexturesPSPVertex));
			boxVertices6 = (TexturesPSPVertex*)memalign(16,4 * sizeof(TexturesPSPVertex));

			size = 0.95f;

			Build();
		}

		StarsBox::~StarsBox()
		{
			free(boxVertices);
			free(boxVertices2);
			free(boxVertices3);
			free(boxVertices4);
			free(boxVertices5);
			free(boxVertices6);
		}

		void StarsBox::Build()
		{
		    int i = 0;

			boxVertices[i].x = size;
			boxVertices[i].y = -size;
			boxVertices[i].z = size;
			boxVertices[i].u = 1;
			boxVertices[i].v = 1;
			i++;

			boxVertices[i].x = size;
			boxVertices[i].y = size;
			boxVertices[i].z = size;
            boxVertices[i].u = 1;
			boxVertices[i].v = 0;
			i++;

			boxVertices[i].x = size;
			boxVertices[i].y = size;
			boxVertices[i].z = -size;
            boxVertices[i].u = 0;
			boxVertices[i].v = 0;
			i++;

			boxVertices[i].x = size;
			boxVertices[i].y = -size;
			boxVertices[i].z = -size;
			boxVertices[i].u = 0;
			boxVertices[i].v = 1;
			i++;

			sceKernelDcacheWritebackInvalidateRange(boxVertices,4 * sizeof(TexturesPSPVertex));

            i = 0;

			boxVertices2[i].x = -size;
			boxVertices2[i].y = -size;
			boxVertices2[i].z = -size;
			boxVertices2[i].u = 1;
			boxVertices2[i].v = 1;
			i++;

			boxVertices2[i].x = -size;
			boxVertices2[i].y = size;
			boxVertices2[i].z = -size;
            boxVertices2[i].u = 1;
			boxVertices2[i].v = 0;
			i++;

			boxVertices2[i].x = -size;
			boxVertices2[i].y = size;
			boxVertices2[i].z = size;
            boxVertices2[i].u = 0;
			boxVertices2[i].v = 0;
			i++;

			boxVertices2[i].x = -size;
			boxVertices2[i].y = -size;
			boxVertices2[i].z = size;
			boxVertices2[i].u = 0;
			boxVertices2[i].v = 1;
			i++;

			sceKernelDcacheWritebackInvalidateRange(boxVertices2,4 * sizeof(TexturesPSPVertex));

            i = 0;

			boxVertices3[i].x = -size;
			boxVertices3[i].y = size;
			boxVertices3[i].z = size;
			boxVertices3[i].u = 1;
			boxVertices3[i].v = 0;
			i++;

			boxVertices3[i].x = size;
			boxVertices3[i].y = size;
			boxVertices3[i].z = size;
			boxVertices3[i].u = 0;
			boxVertices3[i].v = 0;
			i++;

			boxVertices3[i].x = size;
			boxVertices3[i].y = -size;
			boxVertices3[i].z = size;
			boxVertices3[i].u = 0;
			boxVertices3[i].v = 1;
			i++;

			boxVertices3[i].x = -size;
			boxVertices3[i].y = -size;
			boxVertices3[i].z = size;
			boxVertices3[i].u = 1;
			boxVertices3[i].v = 1;
			i++;

			sceKernelDcacheWritebackInvalidateRange(boxVertices3,4 * sizeof(TexturesPSPVertex));

            i = 0;

			boxVertices4[i].x = -size;
			boxVertices4[i].y = -size;
			boxVertices4[i].z = -size;
			boxVertices4[i].u = 0;
			boxVertices4[i].v = 1;
			i++;

			boxVertices4[i].x = size;
			boxVertices4[i].y = -size;
			boxVertices4[i].z = -size;
            boxVertices4[i].u = 1;
			boxVertices4[i].v = 1;
			i++;

			boxVertices4[i].x = size;
			boxVertices4[i].y = size;
			boxVertices4[i].z = -size;
            boxVertices4[i].u = 1;
			boxVertices4[i].v = 0;
			i++;

			boxVertices4[i].x = -size;
			boxVertices4[i].y = size;
			boxVertices4[i].z = -size;
			boxVertices4[i].u = 0;
			boxVertices4[i].v = 0;
			i++;

			sceKernelDcacheWritebackInvalidateRange(boxVertices4,4 * sizeof(TexturesPSPVertex));

            i = 0;

			boxVertices5[i].x = -size;
			boxVertices5[i].y = size;
			boxVertices5[i].z = -size;
			boxVertices5[i].u = 0;
			boxVertices5[i].v = 0;
			i++;

			boxVertices5[i].x = size;
			boxVertices5[i].y = size;
			boxVertices5[i].z = -size;
			boxVertices5[i].u = 1;
			boxVertices5[i].v = 0;
			i++;

			boxVertices5[i].x = size;
			boxVertices5[i].y = size;
			boxVertices5[i].z = size;
			boxVertices5[i].u = 1;
			boxVertices5[i].v = 1;
			i++;

			boxVertices5[i].x = -size;
			boxVertices5[i].y = size;
			boxVertices5[i].z = size;
			boxVertices5[i].u = 0;
			boxVertices5[i].v = 1;
			i++;

			sceKernelDcacheWritebackInvalidateRange(boxVertices5,4 * sizeof(TexturesPSPVertex));

            i = 0;

			boxVertices6[i].x = -size;
			boxVertices6[i].y = -size;
			boxVertices6[i].z = size;
			boxVertices6[i].u = 0;
			boxVertices6[i].v = 0;
			i++;

			boxVertices6[i].x = size;
			boxVertices6[i].y = -size;
			boxVertices6[i].z = size;
			boxVertices6[i].u = 1;
			boxVertices6[i].v = 0;
			i++;

			boxVertices6[i].x = size;
			boxVertices6[i].y = -size;
			boxVertices6[i].z = -size;
			boxVertices6[i].u = 1;
			boxVertices6[i].v = 1;
			i++;

			boxVertices6[i].x = -size;
			boxVertices6[i].y = -size;
			boxVertices6[i].z = -size;
			boxVertices6[i].u = 0;
			boxVertices6[i].v = 1;
			i++;

			sceKernelDcacheWritebackInvalidateRange(boxVertices6,4 * sizeof(TexturesPSPVertex));
		}

		void StarsBox::Update()
		{

		}

		void StarsBox::Render(float alpha)
		{
		    if(alpha > 1.0f)
            {
                alpha = 1.0f;
            }
            if(alpha < 0.0f)
            {
                alpha = 0.0f;
            }

		    sceGuColor(GU_COLOR(alpha,alpha,alpha,alpha));
			sceGuEnable(GU_TEXTURE_2D);
			sceGuTexScale(0.85f,0.85f);
            sceGuEnable(GU_BLEND);
			sceGuDisable(GU_DEPTH_TEST);
			sceGuDepthMask(GU_TRUE);
            sceGuBlendFunc(GU_ADD, GU_FIX,GU_FIX, 0xFFFFFFFF, 0xFFFFFFFF);
            sceGuEnable(GU_ALPHA_TEST);

			sceGumDrawArray(GU_TRIANGLE_FAN, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, boxVertices);
			sceGumDrawArray(GU_TRIANGLE_FAN, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, boxVertices2);
			sceGumDrawArray(GU_TRIANGLE_FAN, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, boxVertices3);
			sceGumDrawArray(GU_TRIANGLE_FAN, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, boxVertices4);
			sceGumDrawArray(GU_TRIANGLE_FAN, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, boxVertices5);
			sceGumDrawArray(GU_TRIANGLE_FAN, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, boxVertices6);

            sceGuDisable(GU_ALPHA_TEST);
            sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
            sceGuDepthMask(GU_FALSE);
            sceGuEnable(GU_DEPTH_TEST);
			//sceGuDisable(GU_TEXTURE_2D);
			sceGuDisable(GU_BLEND);
			sceGuTexScale(1.0f,1.0f);
			sceGuDisable(GU_TEXTURE_2D);
			sceGuColor(GU_COLOR(1.0,1.0,1.0,1.0));
		}
	}
}

