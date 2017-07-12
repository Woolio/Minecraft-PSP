#include <Aurora/Graphics/Effects/Destroyer.h>
#include <Aurora/Graphics/TextureManager.h>

#define PI 3.1415926535897f
#define DEG_TO_RAD (PI / 180.0f)

namespace Aurora
{
	namespace Graphics
	{
		Destroyer::Destroyer()
		{
			boxVertices = (TexturesPSPVertex*)memalign(16,4 * sizeof(TexturesPSPVertex));
			boxVertices2 = (TexturesPSPVertex*)memalign(16,4 * sizeof(TexturesPSPVertex));
			boxVertices3 = (TexturesPSPVertex*)memalign(16,4 * sizeof(TexturesPSPVertex));
			boxVertices4 = (TexturesPSPVertex*)memalign(16,4 * sizeof(TexturesPSPVertex));
			boxVertices5 = (TexturesPSPVertex*)memalign(16,4 * sizeof(TexturesPSPVertex));
			boxVertices6 = (TexturesPSPVertex*)memalign(16,4 * sizeof(TexturesPSPVertex));

			boxVertices11 = (SimplePSPVertex2*)memalign(16,4 * sizeof(SimplePSPVertex2));
			boxVertices12 = (SimplePSPVertex2*)memalign(16,4 * sizeof(SimplePSPVertex2));
			boxVertices13 = (SimplePSPVertex2*)memalign(16,4 * sizeof(SimplePSPVertex2));
			boxVertices14 = (SimplePSPVertex2*)memalign(16,4 * sizeof(SimplePSPVertex2));
			boxVertices15 = (SimplePSPVertex2*)memalign(16,4 * sizeof(SimplePSPVertex2));
			boxVertices16 = (SimplePSPVertex2*)memalign(16,4 * sizeof(SimplePSPVertex2));

			size = 0.0f;
		}

		Destroyer::~Destroyer()
		{
			free(boxVertices);
			free(boxVertices2);
			free(boxVertices3);
			free(boxVertices4);
			free(boxVertices5);
			free(boxVertices6);

			free(boxVertices11);
			free(boxVertices12);
			free(boxVertices13);
			free(boxVertices14);
			free(boxVertices15);
			free(boxVertices16);
		}

		void Destroyer::Update(float stad, CraftWorld* crft, int blockId)
		{
			int i = 0;
			float block = 1/16.0f;
			float block1 = 15.0f / 16.0f;
			float pixel = 1.0f/16.0f;

			BoundingBox modelBox = crft->blockTypes[blockId].collideBox;

            int temp = stad;
            if(stad == -1)
            {
                stad = 10;
            }
            // left

            boxVertices[i].x = modelBox.min.x -size;
            boxVertices[i].y = modelBox.min.y -size;
            boxVertices[i].z = modelBox.max.z +size;
            boxVertices[i].u = stad/16.0f + (0.5+modelBox.max.z)*pixel;
            boxVertices[i].v = 1-(0.5+modelBox.min.y)*pixel;
            i++;

            boxVertices[i].x = modelBox.min.x -size;
            boxVertices[i].y = modelBox.max.y +size;
            boxVertices[i].z = modelBox.max.z +size;
            boxVertices[i].u = stad/16.0f + (0.5+modelBox.max.z)*pixel;
            boxVertices[i].v = 1-(0.5+modelBox.max.y)*pixel;
            i++;

            boxVertices[i].x = modelBox.min.x -size;
            boxVertices[i].y = modelBox.max.y +size;
            boxVertices[i].z = modelBox.min.z -size;
            boxVertices[i].u = stad/16.0f + (0.5+modelBox.min.z)*pixel;
            boxVertices[i].v = 1-(0.5+modelBox.max.y)*pixel;
            i++;

            boxVertices[i].x = modelBox.min.x -size;
            boxVertices[i].y = modelBox.min.y -size;
            boxVertices[i].z = modelBox.min.z -size;
            boxVertices[i].u = stad/16.0f + (0.5+modelBox.min.z)*pixel;
            boxVertices[i].v = 1-(0.5+modelBox.min.y)*pixel;
            i++;

            sceKernelDcacheWritebackInvalidateRange(boxVertices,4 * sizeof(TexturesPSPVertex));

            // right
            i = 0;

            boxVertices2[i].x = modelBox.max.x +size;
            boxVertices2[i].y = modelBox.min.y -size;
            boxVertices2[i].z = modelBox.min.z -size;
            boxVertices2[i].u = stad/16.0f + (0.5+modelBox.max.z)*pixel;
            boxVertices2[i].v = 1-(0.5+modelBox.min.y)*pixel;
            i++;

            boxVertices2[i].x = modelBox.max.x +size;
            boxVertices2[i].y = modelBox.max.y +size;
            boxVertices2[i].z = modelBox.min.z -size;
            boxVertices2[i].u = stad/16.0f + (0.5+modelBox.max.z)*pixel;
            boxVertices2[i].v = 1-(0.5+modelBox.max.y)*pixel;
            i++;

            boxVertices2[i].x = modelBox.max.x +size;
            boxVertices2[i].y = modelBox.max.y +size;
            boxVertices2[i].z = modelBox.max.z +size;
            boxVertices2[i].u = stad/16.0f + (0.5+modelBox.min.z)*pixel;
            boxVertices2[i].v = 1-(0.5+modelBox.max.y)*pixel;
            i++;

            boxVertices2[i].x = modelBox.max.x +size;
            boxVertices2[i].y = modelBox.min.y -size;
            boxVertices2[i].z = modelBox.max.z +size;
            boxVertices2[i].u = stad/16.0f + (0.5+modelBox.min.z)*pixel;
            boxVertices2[i].v = 1-(0.5+modelBox.min.y)*pixel;
            i++;

            sceKernelDcacheWritebackInvalidateRange(boxVertices2,4 * sizeof(TexturesPSPVertex));

            // back
            i = 0;

            boxVertices3[i].x = modelBox.min.x -size;
            boxVertices3[i].y = modelBox.max.y +size;
            boxVertices3[i].z = modelBox.min.z -size;
            boxVertices3[i].u = stad/16.0f + (0.5+modelBox.max.x)*pixel;
            boxVertices3[i].v = 1-(0.5+modelBox.max.y)*pixel;
            i++;

            boxVertices3[i].x = modelBox.max.x +size;
            boxVertices3[i].y = modelBox.max.y +size;
            boxVertices3[i].z = modelBox.min.z -size;
            boxVertices3[i].u = stad/16.0f + (0.5+modelBox.min.x)*pixel;
            boxVertices3[i].v = 1-(0.5+modelBox.max.y)*pixel;
            i++;

            boxVertices3[i].x = modelBox.max.x +size;
            boxVertices3[i].y = modelBox.min.y -size;
            boxVertices3[i].z = modelBox.min.z -size;
            boxVertices3[i].u = stad/16.0f + (0.5+modelBox.min.x)*pixel;
            boxVertices3[i].v = 1-(0.5+modelBox.min.y)*pixel;
            i++;

            boxVertices3[i].x = modelBox.min.x -size;
            boxVertices3[i].y = modelBox.min.y -size;
            boxVertices3[i].z = modelBox.min.z -size;
            boxVertices3[i].u = stad/16.0f + (0.5+modelBox.max.x)*pixel;
            boxVertices3[i].v = 1-(0.5+modelBox.min.y)*pixel;
            i++;

            sceKernelDcacheWritebackInvalidateRange(boxVertices3,4 * sizeof(TexturesPSPVertex));

            //front
            i = 0;

            boxVertices4[i].x = modelBox.min.x -size;
            boxVertices4[i].y = modelBox.min.y -size;
            boxVertices4[i].z = modelBox.max.z +size;
            boxVertices4[i].u = stad/16.0f + (0.5+modelBox.min.x)*pixel;
            boxVertices4[i].v = 1-(0.5+modelBox.min.y)*pixel;
            i++;

            boxVertices4[i].x = modelBox.max.x +size;
            boxVertices4[i].y = modelBox.min.y -size;
            boxVertices4[i].z = modelBox.max.z +size;
            boxVertices4[i].u = stad/16.0f + (0.5+modelBox.max.x)*pixel;
            boxVertices4[i].v = 1-(0.5+modelBox.min.y)*pixel;
            i++;

            boxVertices4[i].x = modelBox.max.x +size;
            boxVertices4[i].y = modelBox.max.y +size;
            boxVertices4[i].z = modelBox.max.z +size;
            boxVertices4[i].u = stad/16.0f + (0.5+modelBox.max.x)*pixel;
            boxVertices4[i].v = 1-(0.5+modelBox.max.y)*pixel;
            i++;

            boxVertices4[i].x = modelBox.min.x -size;
            boxVertices4[i].y = modelBox.max.y +size;
            boxVertices4[i].z = modelBox.max.z +size;
            boxVertices4[i].u = stad/16.0f + (0.5+modelBox.min.x)*pixel;
            boxVertices4[i].v = 1-(0.5+modelBox.max.y)*pixel;
            i++;

            sceKernelDcacheWritebackInvalidateRange(boxVertices4,4 * sizeof(TexturesPSPVertex));

            // bot
            i = 0;

            boxVertices5[i].x = modelBox.min.x -size;
            boxVertices5[i].y = modelBox.min.y -size;
            boxVertices5[i].z = modelBox.min.z -size;
            boxVertices5[i].u = stad/16.0f + (0.5+modelBox.min.x)*pixel;
            boxVertices5[i].v = 1-(0.5+modelBox.max.z)*pixel;
            i++;

            boxVertices5[i].x = modelBox.max.x +size;
            boxVertices5[i].y = modelBox.min.y -size;
            boxVertices5[i].z = modelBox.min.z -size;
            boxVertices5[i].u = stad/16.0f + (0.5+modelBox.max.x)*pixel;
            boxVertices5[i].v = 1-(0.5+modelBox.max.z)*pixel;
            i++;

            boxVertices5[i].x = modelBox.max.x +size;
            boxVertices5[i].y = modelBox.min.y -size;
            boxVertices5[i].z = modelBox.max.z +size;
            boxVertices5[i].u = stad/16.0f + (0.5+modelBox.max.x)*pixel;
            boxVertices5[i].v = 1-(0.5+modelBox.min.z)*pixel;
            i++;

            boxVertices5[i].x = modelBox.min.x -size;
            boxVertices5[i].y = modelBox.min.y -size;
            boxVertices5[i].z = modelBox.max.z +size;
            boxVertices5[i].u = stad/16.0f + (0.5+modelBox.min.x)*pixel;
            boxVertices5[i].v = 1-(0.5+modelBox.min.z)*pixel;
            i++;

            sceKernelDcacheWritebackInvalidateRange(boxVertices5,4 * sizeof(TexturesPSPVertex));

            // top
            i = 0;

            boxVertices6[i].x = modelBox.min.x -size;
            boxVertices6[i].y = modelBox.max.y +size;
            boxVertices6[i].z = modelBox.max.z +size;
            boxVertices6[i].u = stad/16.0f + (0.5+modelBox.min.x)*pixel;
            boxVertices6[i].v = 1-(0.5+modelBox.max.z)*pixel;
            i++;

            boxVertices6[i].x = modelBox.max.x +size;
            boxVertices6[i].y = modelBox.max.y +size;
            boxVertices6[i].z = modelBox.max.z +size;
            boxVertices6[i].u = stad/16.0f + (0.5+modelBox.max.x)*pixel;
            boxVertices6[i].v = 1-(0.5+modelBox.max.z)*pixel;
            i++;

            boxVertices6[i].x = modelBox.max.x +size;
            boxVertices6[i].y = modelBox.max.y +size;
            boxVertices6[i].z = modelBox.min.z -size;
            boxVertices6[i].u = stad/16.0f + (0.5+modelBox.max.x)*pixel;
            boxVertices6[i].v = 1-(0.5+modelBox.min.z)*pixel;
            i++;

            boxVertices6[i].x = modelBox.min.x -size;
            boxVertices6[i].y = modelBox.max.y +size;
            boxVertices6[i].z = modelBox.min.z -size;
            boxVertices6[i].u = stad/16.0f + (0.5+modelBox.min.x)*pixel;
            boxVertices6[i].v = 1-(0.5+modelBox.min.z)*pixel;
            i++;

            sceKernelDcacheWritebackInvalidateRange(boxVertices6,4 * sizeof(TexturesPSPVertex));

            stad = temp;

           /* else
            {
                // left
                i = 0;
                boxVertices11[i].x = modelBox.min.x -size;
                boxVertices11[i].y = modelBox.min.y -size;
                boxVertices11[i].z = modelBox.max.z +size;
                i++;

                boxVertices11[i].x = modelBox.min.x -size;
                boxVertices11[i].y = modelBox.max.y +size;
                boxVertices11[i].z = modelBox.max.z +size;
                i++;

                boxVertices11[i].x = modelBox.min.x -size;
                boxVertices11[i].y = modelBox.max.y +size;
                boxVertices11[i].z = modelBox.min.z -size;
                i++;

                boxVertices11[i].x = modelBox.min.x -size;
                boxVertices11[i].y = modelBox.min.y -size;
                boxVertices11[i].z = modelBox.min.z -size;
                i++;

                sceKernelDcacheWritebackInvalidateRange(boxVertices11,4 * sizeof(SimplePSPVertex2));

                // right
                i = 0;

                boxVertices12[i].x = modelBox.max.x +size;
                boxVertices12[i].y = modelBox.max.y +size;
                boxVertices12[i].z = modelBox.min.z -size;
                i++;

                boxVertices12[i].x = modelBox.max.x +size;
                boxVertices12[i].y = modelBox.min.y -size;
                boxVertices12[i].z = modelBox.min.z -size;
                i++;

                boxVertices12[i].x = modelBox.max.x +size;
                boxVertices12[i].y = modelBox.min.y -size;
                boxVertices12[i].z = modelBox.max.z +size;
                i++;

                boxVertices12[i].x = modelBox.max.x +size;
                boxVertices12[i].y = modelBox.max.y +size;
                boxVertices12[i].z = modelBox.max.z +size;
                i++;

                sceKernelDcacheWritebackInvalidateRange(boxVertices12,4 * sizeof(SimplePSPVertex2));

                // bot
                i = 0;

                boxVertices15[i].x = modelBox.max.x +size;
                boxVertices15[i].y = modelBox.min.y -size;
                boxVertices15[i].z = modelBox.min.z -size;
                i++;

                boxVertices15[i].x = modelBox.min.x -size;
                boxVertices15[i].y = modelBox.min.y -size;
                boxVertices15[i].z = modelBox.min.z -size;
                i++;

                boxVertices15[i].x = modelBox.min.x -size;
                boxVertices15[i].y = modelBox.min.y -size;
                boxVertices15[i].z = modelBox.max.z +size;
                i++;

                boxVertices15[i].x = modelBox.max.x +size;
                boxVertices15[i].y = modelBox.min.y -size;
                boxVertices15[i].z = modelBox.max.z +size;
                i++;


                sceKernelDcacheWritebackInvalidateRange(boxVertices15,4 * sizeof(SimplePSPVertex2));

                // top
                i = 0;

                boxVertices16[i].x = modelBox.min.x -size;
                boxVertices16[i].y = modelBox.max.y +size;
                boxVertices16[i].z = modelBox.max.z +size;
                i++;

                boxVertices16[i].x = modelBox.max.x +size;
                boxVertices16[i].y = modelBox.max.y +size;
                boxVertices16[i].z = modelBox.max.z +size;
                i++;

                boxVertices16[i].x = modelBox.max.x +size;
                boxVertices16[i].y = modelBox.max.y +size;
                boxVertices16[i].z = modelBox.min.z -size;
                i++;

                boxVertices16[i].x = modelBox.min.x -size;
                boxVertices16[i].y = modelBox.max.y +size;
                boxVertices16[i].z = modelBox.min.z -size;
                i++;

                sceKernelDcacheWritebackInvalidateRange(boxVertices16,4 * sizeof(SimplePSPVertex2));
            }*/
		}

		void Destroyer::Render(float stad)
		{
		    //if(stad != -1)
            //{
                sceGuEnable(GU_TEXTURE_2D);
                sceGuBlendFunc(GU_ADD, GU_DST_COLOR, GU_SRC_COLOR, 0xFFFFFFFF,0xFFFFFFFF);
                sceGuDepthMask(1);
                sceGuDepthOffset(40);

                sceGumDrawArray(GU_TRIANGLE_FAN, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, boxVertices);
                sceGumDrawArray(GU_TRIANGLE_FAN, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, boxVertices2);
                sceGumDrawArray(GU_TRIANGLE_FAN, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, boxVertices3);
                sceGumDrawArray(GU_TRIANGLE_FAN, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, boxVertices4);
                sceGumDrawArray(GU_TRIANGLE_FAN, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, boxVertices5);
                sceGumDrawArray(GU_TRIANGLE_FAN, GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, boxVertices6);

                sceGuDepthOffset(0);
                sceGuDepthMask(0);
                sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);
                sceGuDisable(GU_TEXTURE_2D);
            //}
            //else
            //{
                /*sceGuColor(GU_COLOR(0.0,0.0,0.0,0.5f));
                sceGuDisable(GU_TEXTURE_2D);
                sceGuShadeModel(GU_FLAT);

                sceGuDepthMask(1);
                sceGuDepthOffset(40);

                sceGumDrawArray(GU_LINE_STRIP, GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, boxVertices11);
                sceGumDrawArray(GU_LINE_STRIP, GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, boxVertices12);
                sceGumDrawArray(GU_LINE_STRIP, GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, boxVertices15);
                sceGumDrawArray(GU_LINE_STRIP, GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, boxVertices16);

                sceGuShadeModel(GU_SMOOTH);
                sceGuDepthOffset(0);
                sceGuDepthMask(0);*/
            //}
		}
	}
}

