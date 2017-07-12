#include <Aurora/Graphics/RenderManager.h>

namespace Aurora
{
	namespace Graphics
	{


		void RenderManager::Init()
		{
			mVerticalSync = false;
			listNum = 0;
			cleanColor = 0xFFF5B783;
			fov = 54.0f;
			aspect =  480.0f / 272.0f;
			znear = 0.1f;
			zfar = 200.0f;
			screenNumber = 0;

			//initialize GU engine
			sceGuInit();
			sceGuStart(GU_DIRECT,list);

			sceGuDrawBuffer( GU_PSM_8888, SCEGU_VRAM_BP32_0, BUF_WIDTH );
			sceGuDispBuffer( SCR_WIDTH, SCR_HEIGHT, SCEGU_VRAM_BP32_1, BUF_WIDTH);
			sceGuDepthBuffer( SCEGU_VRAM_BP32_2, BUF_WIDTH);

			sceGuOffset(2048 - (SCR_WIDTH/2),2048 - (SCR_HEIGHT/2));
			sceGuViewport(2048,2048,SCR_WIDTH,SCR_HEIGHT);

			sceGuEnable(GU_SCISSOR_TEST);
			sceGuScissor(0,0,SCR_WIDTH,SCR_HEIGHT);
			sceGuDepthRange(50000, 10000);
			sceGuDepthFunc(GU_GEQUAL);
			sceGuEnable(GU_DEPTH_TEST);
			sceGuShadeModel(GU_SMOOTH);
			sceGuEnable(GU_CLIP_PLANES);
			sceGuEnable(GU_CULL_FACE);
			sceGuFrontFace(GU_CCW);

			sceGuEnable(GU_BLEND);
			sceGuBlendFunc(GU_ADD, GU_SRC_ALPHA, GU_ONE_MINUS_SRC_ALPHA, 0, 0);

			sceGuStencilFunc(GU_ALWAYS, 1, 1); // always set 1 bit in 1 bit mask
			sceGuStencilOp(GU_KEEP, GU_KEEP, GU_REPLACE); // keep value on failed test (fail and zfail) and replace on pass

			sceGuTexFilter(GU_LINEAR,GU_LINEAR);

			sceGuFinish();
			sceGuSync(0,0);

			sceDisplayWaitVblankStart();
			sceGuDisplay(GU_TRUE);

			sceCtrlSetSamplingCycle(0);
			sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

			//initialize variables
			mCam = 0;

			g_vbl_count = -1;
			g_vbl_step = 1;
			g_vbl_time = 0;
			g_cpu_load = 0;
			g_gpu_load = 0;
			g_frame_count = 0;
			performanceCounter = false;
		}

		void RenderManager::InitDebugFont()
		{
			intraFontInit();
			debugFont = intraFontLoad("Assets/Lamecraft/lamecraft.pgf",INTRAFONT_CACHE_ASCII);
			intraFontSetStyle(debugFont, 0.5f,WHITE,BLACK,INTRAFONT_ALIGN_CENTER);
		}

		void RenderManager::SetFontStyle(float size, unsigned int color, unsigned int shadowColor, unsigned int options)
		{
			intraFontSetStyle(debugFont, size,color,shadowColor,options);
		}

		void RenderManager::DebugPrint(int x,int y,const char *message, ...)
		{
			va_list argList;
			char cbuffer[512];
			va_start(argList, message);
			vsnprintf(cbuffer, 512, message, argList);
			va_end(argList);

			sceGuEnable(GU_BLEND);
			sceGuEnable(GU_TEXTURE_2D);
			intraFontPrint(debugFont, x, y, cbuffer);
			sceGuDisable(GU_BLEND);
			sceGuDisable(GU_TEXTURE_2D);
		}

		void RenderManager::Start()
		{
			sceGuStart(GU_DIRECT,list);
		}

		void RenderManager::CleanBuffers()
		{
			sceGuClearColor(cleanColor);
			sceGuClearStencil(0);
			sceGuClearDepth(0);
			sceGuClear(GU_COLOR_BUFFER_BIT | GU_STENCIL_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);

			//reset textures
			TextureManager::Instance()->currentTexture = -1;
		}

		void RenderManager::StartFrame()
		{
			sceGuStart(GU_DIRECT,list);
		
			sceGuClearColor(cleanColor);
			sceGuClearStencil(0);
			sceGuClearDepth(0);
			sceGuClear(GU_COLOR_BUFFER_BIT | GU_STENCIL_BUFFER_BIT | GU_DEPTH_BUFFER_BIT);

			//set camera look at -- later maybe i will move this code somewhere else
			if(mCam != NULL)
			{
				sceGumMatrixMode(GU_PROJECTION);
				sceGumLoadIdentity();
				sceGumLoadMatrix(&proj);

				sceGumMatrixMode(GU_VIEW);
				sceGumLoadIdentity();

				ScePspFVector3 pos = {mCam->m_vPosition.x + mCam->m_vOffset.x,mCam->m_vPosition.y + mCam->m_vOffset.y,mCam->m_vPosition.z + mCam->m_vOffset.z};
				ScePspFVector3 eye = {mCam->m_vView.x,mCam->m_vView.y,mCam->m_vView.z};
				ScePspFVector3 up = {mCam->m_vUpVector.x,mCam->m_vUpVector.y,mCam->m_vUpVector.z};

				sceGumLookAt(&pos, &eye, &up);

				sceGumStoreMatrix(&view);

				sceGumMatrixMode(GU_MODEL);
				sceGumLoadIdentity();

				if(mCam->needUpdate)
				{
					UpdateFrustumMatrix();
					mCam->mFrustum.ExtractPlanes(projection_view_matrix);
					mCam->needUpdate = false;
				}
			}

			TextureManager::Instance()->currentTexture = -1;
		}

		void RenderManager::LookAt()
		{
			//set camera look at -- later maybe i will move this code somewhere else
			if(mCam != NULL)
			{
				sceGumMatrixMode(GU_PROJECTION);
				sceGumLoadIdentity();
				sceGumLoadMatrix(&proj);

				sceGumMatrixMode(GU_VIEW);
				sceGumLoadIdentity();

				ScePspFVector3 pos = {mCam->m_vPosition.x + mCam->m_vOffset.x,mCam->m_vPosition.y + mCam->m_vOffset.y,mCam->m_vPosition.z + mCam->m_vOffset.z};
				ScePspFVector3 eye = {mCam->m_vView.x,mCam->m_vView.y,mCam->m_vView.z};
				ScePspFVector3 up = {mCam->m_vUpVector.x,mCam->m_vUpVector.y,mCam->m_vUpVector.z};

				sceGumLookAt(&pos, &eye, &up);
				sceGumUpdateMatrix();

				sceGumStoreMatrix(&view);

				sceGumMatrixMode(GU_MODEL);
				sceGumLoadIdentity();

				if(mCam->needUpdate)
				{
					UpdateFrustumMatrix();
					mCam->mFrustum.ExtractPlanes(projection_view_matrix);
					mCam->needUpdate = false;
				}
			}
		}

		void RenderManager::EndFrame()
		{
			if(!performanceCounter)
			{
				sceGuFinish();
				sceGuSync(0,0);

				sceDisplayWaitVblankStart();
				sceGuSwapBuffers();
			}else
			{
				sceGuFinish();
				int cpu_time = sceKernelGetSystemTimeLow();
				sceGuSync(0,0);
				int gpu_time = sceKernelGetSystemTimeLow();
				sceDisplayWaitVblankStart();
				sceGuSwapBuffers();

				g_frame_count ++ ;

				//performance
				int vbl_time = sceKernelGetSystemTimeLow();
				int vbl_count = sceDisplayGetVcount();

				if ( g_vbl_count >= 0 )
				{
					g_vbl_step = vbl_count - g_vbl_count ;
					if ( g_frame_count % 6 == 0 )
					{
						g_cpu_load = ( cpu_time - g_vbl_time ) * 100 / 16667 ;
						g_gpu_load = ( gpu_time - g_vbl_time ) * 100 / 16667 ;
					}
				}
				g_vbl_time = vbl_time ;
				g_vbl_count = vbl_count ;
			}
		}

		void RenderManager::StartDialog()
		{
			sceGuStart(GU_DIRECT,list);
			sceGuClear(GU_COLOR_BUFFER_BIT|GU_DEPTH_BUFFER_BIT);
			sceGuFinish();
			sceGuSync(0,0);
		}

		void RenderManager::EndDialog()
		{
			sceDisplayWaitVblankStart();
			sceGuSwapBuffers();
		}

		void RenderManager::UseVerticalSync(bool Enabled)
		{
			mVerticalSync = Enabled;
		}

		void RenderManager::SetClearColor(float r,float g,float b,float a)
		{
			cleanColor = GU_RGBA((int)r * 255,(int)g * 255,(int)b * 255,(int)a * 255);
		}

		void RenderManager::SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
		{
			//some lame stuff XD
			sceGumMatrixMode(GU_PROJECTION);
			sceGumLoadIdentity();
			sceGumOrtho(0, 480, 272, 0, -30, 30);

			sceGumMatrixMode(GU_VIEW);
			sceGumLoadIdentity();

			sceGumMatrixMode(GU_MODEL);
			sceGumLoadIdentity();
		}

		void RenderManager::SetPerspective(float _fov,float _aspect,float _znear,float _zfar)
		{
			fov = _fov;
			aspect = _aspect;
			znear = _znear;
			zfar = _zfar;

			sceGumMatrixMode(GU_PROJECTION);
			sceGumLoadIdentity();

			sceGumPerspective(fov,aspect,znear,zfar);
			sceGumStoreMatrix(&proj);

			sceGumMatrixMode(GU_VIEW);
			sceGumLoadIdentity();

			sceGumStoreMatrix(&view);

			sceGumMatrixMode(GU_MODEL);
			sceGumLoadIdentity();
		}



		void  RenderManager::DrawToTexture(Image* offscreenTexture )
		{
			//draw to texture
			sceGuDrawBufferList(GU_PSM_8888,(void*)offscreenTexture->ImageData,offscreenTexture->Width);

			// setup viewport
			sceGuOffset(2048 - (offscreenTexture->Width/2),2048 - (offscreenTexture->Height/2));
			sceGuViewport(2048,2048,offscreenTexture->Width,offscreenTexture->Height);

			//CleanBuffers();
		}

		void RenderManager::SetRTT()
		{
			// set frame buffer
			sceGuDrawBufferList(GU_PSM_8888,SCEGU_VRAM_BP32_0,BUF_WIDTH);

			// setup viewport
			sceGuOffset(2048 - (SCR_WIDTH/2),2048 - (SCR_HEIGHT/2));
			sceGuViewport(2048,2048,SCR_WIDTH,SCR_HEIGHT);

			CleanBuffers();
		}

		void RenderManager::Draw(ObjModel *model)
		{
			sceGuEnable(GU_TEXTURE_2D);

			unsigned int count = model->mMeshes.size();

			for(unsigned int i = 0;i < count;i++)
			{
				ObjMesh *mesh = model->mMeshes[i];

				if(mesh->mMaterial != -1)
				{
					ObjMaterial *material = model->mMaterials[mesh->mMaterial];

					if(material->mipmapping)
					{
						TextureManager::Instance()->SetMipMapsTextures(material->texturID,material->mipmap1,material->mipmap2,material->mipmap3);

					}else
					{
						TextureManager::Instance()->SetTexture(material->texturID,GU_NEAREST,GU_NEAREST);
					}
				}

				if (mesh->mMaterial != -1 && model->mMaterials[mesh->mMaterial]->lightmapping)
				{
					ObjMaterial *material = model->mMaterials[mesh->mMaterial];
					TextureManager::Instance()->SetTexture(material->lightMapID);
				}

				if(mesh->triangles)
					sceGumDrawArray(GU_TRIANGLES,GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D,mesh->vertexCount,0,mesh->meshVertices);
				else
					sceGumDrawArray(GU_TRIANGLE_STRIP,GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D,mesh->indicesCount,0,mesh->meshVertices);
			}

			sceGuDisable(GU_TEXTURE_2D);
		}

		void RenderManager::SetActiveCamera(Camera *camera)
		{
			mCam = camera;
			mCam->needUpdate = true;
		}

		void RenderManager::UpdateFrustumMatrix()
		{
			gumMultMatrix(&projection_view_matrix,&proj,&view);
		}

		void RenderManager::SetClearColour(unsigned int color)
		{
			cleanColor = color;
		}

		void RenderManager::TakeNextScreenshot()
		{
			char name[20];
			sprintf(name,"screen%d.png",screenNumber);
			TakeScreenshot(name);
			screenNumber++;
		}

		void RenderManager::TakeScreenshot(const char* filename)
		{
			u32* vram32;
			u16* vram16;
			int bufferwidth;
			int pixelformat;
			int unknown = 0;
			int i, x, y;
			png_structp png_ptr;
			png_infop info_ptr;
			FILE* fp;
			u8* line;

			fp = fopen(filename, "wb");
			if (!fp) return;
			png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			if (!png_ptr) return;
			info_ptr = png_create_info_struct(png_ptr);
			if (!info_ptr)
			{
					png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
					fclose(fp);
					return;
			}
			png_init_io(png_ptr, fp);
			png_set_IHDR(png_ptr, info_ptr, 480, 272,8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
			png_write_info(png_ptr, info_ptr);
			line = (u8*) malloc(480 * 3);
			sceDisplayWaitVblankStart();  // if framebuf was set with PSP_DISPLAY_SETBUF_NEXTFRAME, wait until it is changed
			sceDisplayGetFrameBuf((void**)&vram32, &bufferwidth, &pixelformat, unknown);
			vram16 = (u16*) vram32;
			for (y = 0; y < 272; y++)
			{
					for (i = 0, x = 0; x < 480; x++)
					{
							u32 color = 0;
							u8 r = 0, g = 0, b = 0;
							switch (pixelformat)
							{
									case PSP_DISPLAY_PIXEL_FORMAT_565:
											color = vram16[x + y * bufferwidth];
											r = (color & 0x1f) << 3;
											g = ((color >> 5) & 0x3f) << 2 ;
											b = ((color >> 11) & 0x1f) << 3 ;
											break;
									case PSP_DISPLAY_PIXEL_FORMAT_5551:
											color = vram16[x + y * bufferwidth];
											r = (color & 0x1f) << 3;
											g = ((color >> 5) & 0x1f) << 3 ;
											b = ((color >> 10) & 0x1f) << 3 ;
											break;
									case PSP_DISPLAY_PIXEL_FORMAT_4444:
											color = vram16[x + y * bufferwidth];
											r = (color & 0xf) << 4;
											g = ((color >> 4) & 0xf) << 4 ;
											b = ((color >> 8) & 0xf) << 4 ;
											break;
									case PSP_DISPLAY_PIXEL_FORMAT_8888:
											color = vram32[x + y * bufferwidth];
											r = color & 0xff;
											g = (color >> 8) & 0xff;
											b = (color >> 16) & 0xff;
											break;
							}
							line[i++] = r;
							line[i++] = g;
							line[i++] = b;
					}
					png_write_row(png_ptr, line);
			}
			free(line);
			png_write_end(png_ptr, info_ptr);
			png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
			fclose(fp);
		}

		RenderManager RenderManager::m_RenderManager;

		RenderManager * RenderManager::InstancePtr()
		{
			return &m_RenderManager;
		}

		

	}
}
