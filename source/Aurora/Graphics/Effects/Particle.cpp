#include <Aurora/Graphics/Effects/Particle.h>
#include <Aurora/Graphics/TextureManager.h>

#define PI 3.1415926535897f
#define DEG_TO_RAD (PI / 180.0f)

namespace Aurora
{
	namespace Graphics
	{
		Particle::Particle(CraftWorld* world, std::string _name, Vector3 _position)
		{
		    particleVertices = NULL;
		    texture = -1;

			position = _position;
			name = _name;

			horizontalScale = 0.3f;
			verticalScale = 0.3f;

			gravity = 0.9;
			maxTimeToLive = 0.0f;

			color = Vector3(1,1,1);

            SetMaxLiveTime(0.0f);
			SetLiveTime(0.0f);
			SetScale(0.3f);
			SetGravity(-0.3f);
			SetFramesAmount(0);
			SetBrightness(1.0f);
			frame = 0;
			totalFrames = 0;


            var1 = -1;
            var2 = -1;
            var3 = -1;
            varf1 = -1.0f;
            varf2 = -1.0f;

			toDestroy = false;
			rederMe = true;
			friction = 0.0f;

			myLight = 1.0f;

			if(name == "smoke")
            {
                texture = TextureHelper::Instance()->GetTexture(TextureHelper::smokeTexture);

                SetMaxLiveTime(3000);
                switch(rand() % 8)
                {
                    case 0 : SetLiveTime(1500,2000); break;
                    case 1 : SetLiveTime(2500,3000); break;
                    case 2 : SetLiveTime(2500,3000); break;
                    case 3 : SetLiveTime(3000,3500); break;
                    case 4 : SetLiveTime(3000,3500); break;
                    default: SetLiveTime(2000,2500); break;
                }

                SetScale(0.2f,0.4f);
                SetGravity(0.0f);
                SetFramesAmount(8);
                SetBrightness(0.8f, 1.01f);

                friction = 2.0f;

                float veloc_x_shift = 0.0f;
                float veloc_z_shift = 0.0f;

                switch(rand() % 4)
                {
                    case 0: veloc_x_shift = -1.2f; veloc_z_shift = -1.2f; break;
                    case 1: veloc_x_shift = 1.2f; veloc_z_shift = -1.2f; break;
                    case 2: veloc_x_shift = -1.2f; veloc_z_shift = 1.2f; break;
                    case 3: veloc_x_shift = 1.2f; veloc_z_shift = 1.2f; break;
                }

                velocity = Vector3(veloc_x_shift,0.0f,veloc_z_shift);
            }

			if(name == "black smoke")
            {
                texture = TextureHelper::Instance()->GetTexture(TextureHelper::smokeTexture);

                SetMaxLiveTime(900);
                SetLiveTime(600,900);

                SetScale(0.10f,0.18f);
                SetGravity(0.0f);
                SetFramesAmount(8);
                SetBrightness(0.2f, 0.3f);

                friction = 2.0f;
                varf2 = (rand()%6)/5.0f;

                velocity = Vector3(0,0.0f,0);
            }
            if(name == "note")
            {
                texture = TextureHelper::Instance()->GetTexture(TextureHelper::noteTexture);

                SetMaxLiveTime(370);
                SetLiveTime(370);

                SetScale(0.19f,0.26f);
                SetGravity(0.0f);
                friction = 4.0f;
                myLight = 1.0f;

                velocity = Vector3(0.0f,3.0f,0.0f);

                totalFrames = 1;
            }
			if(name == "explosion")
            {
                texture = TextureHelper::Instance()->GetTexture(TextureHelper::explosionTexture);

                SetMaxLiveTime(600);
                SetLiveTime(500,600);

                SetScale(1.1f,1.3f);
                SetGravity(0.0f);
                SetFramesAmount(16);
                SetBrightness(0.8f, 1.01f);

                velocity = Vector3(0,0.0f,0);
            }
            if(name == "flame")
            {
                texture = TextureHelper::Instance()->GetTexture(TextureHelper::flameTexture);

                SetMaxLiveTime(900);
                SetLiveTime(700,900);

                SetScale(0.13f,0.18f);
                SetGravity(0.0f);
                SetBrightness(1.0f);

                velocity = Vector3(0,0.0f,0);

                totalFrames = 1;
            }
            if(name == "bubble")
            {
                texture = TextureHelper::Instance()->GetTexture(TextureHelper::bubbleTexture);
                timeToLive = 0.6f + (rand() % 6) / 10.0f;
                gravity = 0.1f;
                totalFrames = 1;
            }
            if(name == "block crack")
            {
                texture = TextureHelper::Instance()->GetTexture(TextureHelper::Terrain1);

                SetLiveTime(600,1200);
                SetScale(0.05f, 0.11f);
                SetFramesAmount(1);
                SetBrightness(1.0f);
                SetGravity(-6.8f);

                friction = 0.0f;

                velocity = Vector3((-15+rand()%30)*0.1f,2.5f,(-15+rand()%30)*0.1f);
            }

            if(name == "item crack")
            {
                texture = TextureHelper::Instance()->GetTexture(TextureHelper::Items1);

                SetLiveTime(600,1200);
                SetScale(0.04f, 0.09f);
                SetFramesAmount(1);
                SetBrightness(1.0f);
                SetGravity(-6.8f);

                friction = 0.0f;

                velocity = Vector3((-15+rand()%30)*0.1f,2.5f,(-15+rand()%30)*0.1f);
            }


            if(world->TaxicabDistance2d(position.x,position.y,world->playerPos.x,world->playerPos.y) >= 12.0f)
            {
                toDestroy = true;
            }

            if(toDestroy == false)
            {
                particleVertices = (TexturesPSPVertex*)memalign(16,4 * sizeof(TexturesPSPVertex));
            }
            bBox =  BoundingBox(Vector3(position.x-0.33f,position.y-0.33f,position.z-0.33f),Vector3(position.x+0.33f,position.y+0.33f,position.z+0.33f));

			//sceKernelDcacheWritebackInvalidateRange(particleVertices,4 * sizeof(TexturesPSPVertex));
			//particleVertices = (TexturesPSPVertex*)malloc(14 * sizeof(TexturesPSPVertex));
		}

        Particle::~Particle()
		{
		    if(particleVertices != NULL)
            {
                free(particleVertices);
            }
		}

		void Particle::SetVelocity(Vector3 _velocity)
		{
		    velocity = _velocity;
		}

        void Particle::SetScale(float scale)
		{
		    horizontalScale = scale;
		    verticalScale = scale;
		}

        void Particle::SetScale(float minScale, float maxScale)
		{
		    float scale = (minScale*100 + rand() % (int)((maxScale-minScale)*100)) / 100.0f;
		    horizontalScale = scale;
		    verticalScale = scale;
		}

        void Particle::SetColor(Vector3 _color)
		{
		    color = _color;
		}

        void Particle::SetBrightness(float brightness)
		{
		    color = Vector3(brightness, brightness, brightness);
		}

        void Particle::SetBrightness(float minBrightness, float maxBrightness)
		{
		    float brightness = (minBrightness*100 + rand() % (int)((maxBrightness-minBrightness)*100)) / 100.0f;
		    color = Vector3(brightness, brightness, brightness);
		}

        void Particle::SetMaxLiveTime(int _maxLiveTime)
		{
		    maxTimeToLive = _maxLiveTime / 1000.0f;
		}

        void Particle::SetLiveTime(int _liveTime)
		{
		    timeToLive = _liveTime / 1000.0f;
		}

        void Particle::SetLiveTime(int minLiveTime, int maxLiveTime)
		{
		    int _liveTime = minLiveTime/10 + rand() % ((maxLiveTime-minLiveTime)/10);
		    timeToLive = _liveTime / 100.0f;
		}

		void Particle::SetFramesAmount(int _totalFrames)
		{
		    frame = 0;
		    totalFrames = _totalFrames;
		}

        void Particle::SetGravity(float _gravity)
		{
            gravity = _gravity;
		}

		void Particle::BuildVerticies()
		{
		    float frameSize = 1.0f/(totalFrames);

		    ScePspFMatrix4 t;
			sceGumMatrixMode(GU_VIEW);
			sceGumStoreMatrix(&t);
			sceGumMatrixMode(GU_MODEL);

            ScePspFVector3 up, right;

			right.x = t.x.x * horizontalScale * 1;
			right.y = t.y.x * horizontalScale * 1;
			right.z = t.z.x * horizontalScale * 1;
			up.x = t.x.y * verticalScale * 1;
			up.y = t.y.y * verticalScale * 1;
			up.z = t.z.y * verticalScale * 1;

			int i = 0;

			particleVertices[i].x = position.x - right.x + up.x;// * stepScale;
			particleVertices[i].y = position.y - right.y + up.y;// * stepScale;
			particleVertices[i].z = position.z - right.z + up.z;// * stepScale;
            particleVertices[i].u = frame*frameSize;// * textureScale;
			particleVertices[i].v = 0.f;// * textureScale;
			i++;

			// (x, y - 1, z)
			particleVertices[i].x = position.x - right.x - up.x;// * stepScale;
			particleVertices[i].y = position.y - right.y - up.y;// * stepScale;
			particleVertices[i].z = position.z - right.z - up.z;// * stepScale;
			particleVertices[i].u = frame*frameSize;// * textureScale;
			particleVertices[i].v = 1.f;// * textureScale;
			i++;

			// (x + 1, y, z)
			particleVertices[i].x = position.x + right.x + up.x;// * stepScale;
			particleVertices[i].y = position.y + right.y + up.y;// * stepScale;
			particleVertices[i].z = position.z + right.z + up.z;// * stepScale;
			particleVertices[i].u = frame*frameSize+frameSize;// * textureScale;
			particleVertices[i].v = 0.f;// * textureScale;
			i++;

			// (x + 1, y - 1, z)
			particleVertices[i].x = position.x + right.x - up.x;// * stepScale;
			particleVertices[i].y = position.y + right.y - up.y;//* stepScale;
			particleVertices[i].z = position.z + right.z - up.z;// * stepScale;
			particleVertices[i].u = frame*frameSize+frameSize;// * textureScale;
			particleVertices[i].v = 1.f;// * textureScale;

            sceKernelDcacheWritebackInvalidateRange(particleVertices,4 * sizeof(TexturesPSPVertex));
		}

		void Particle::BuildVerticiesForBlockCrack(CraftWorld* crtf, int blockId)
		{
		    float texturePixel = (float)1.0f/256.0f;
            float percent = (float)1.0f/16.0f;

            int tex_x, tex_y;
            if(crtf->mainOptions.fastRendering == true && (blockId == LeavesBlock::getID() || blockId == BirchLeaves::getID() || blockId == SpruceLeaves::getID()))
            {
                tex_x = crtf->blockTypes[blockId].downPlane_x+1;
                tex_y = crtf->blockTypes[blockId].downPlane_y;
            }
            else
            {
                tex_x = crtf->blockTypes[blockId].downPlane_x;
                tex_y = crtf->blockTypes[blockId].downPlane_y;
            }

            if(varf1 == -1.0f)
            {
                varf1 = tex_y * percent + (rand() % 12)*texturePixel + texturePixel*0.5f;
                varf2 = tex_x * percent + (rand() % 12)*texturePixel + texturePixel*0.5f;
            }
            float texCordY = varf1;
            float texScaleY = texturePixel*4 + texturePixel*0.5f;

            float texCordX = varf2;
            float texScaleX = texturePixel*4 + texturePixel*0.5f;

		    ScePspFMatrix4 t;
			sceGumMatrixMode(GU_VIEW);
			sceGumStoreMatrix(&t);
			sceGumMatrixMode(GU_MODEL);

            ScePspFVector3 up, right;

			right.x = t.x.x * horizontalScale * 1;
			right.y = t.y.x * horizontalScale * 1;
			right.z = t.z.x * horizontalScale * 1;
			up.x = t.x.y * verticalScale * 1;
			up.y = t.y.y * verticalScale * 1;
			up.z = t.z.y * verticalScale * 1;

			int i = 0;

			particleVertices[i].x = position.x - right.x + up.x;// * stepScale;
			particleVertices[i].y = position.y - right.y + up.y;// * stepScale;
			particleVertices[i].z = position.z - right.z + up.z;// * stepScale;
            particleVertices[i].u = texCordX;// * textureScale;
			particleVertices[i].v = texCordY;// * textureScale;
			i++;

			// (x, y - 1, z)
			particleVertices[i].x = position.x - right.x - up.x;// * stepScale;
			particleVertices[i].y = position.y - right.y - up.y;// * stepScale;
			particleVertices[i].z = position.z - right.z - up.z;// * stepScale;
			particleVertices[i].u = texCordX;// * textureScale;
			particleVertices[i].v = texCordY+texScaleY;// * textureScale;
			i++;

			// (x + 1, y, z)
			particleVertices[i].x = position.x + right.x + up.x;// * stepScale;
			particleVertices[i].y = position.y + right.y + up.y;// * stepScale;
			particleVertices[i].z = position.z + right.z + up.z;// * stepScale;
			particleVertices[i].u = texCordX+texScaleX;// * textureScale;
			particleVertices[i].v = texCordY;// * textureScale;
			i++;

			// (x + 1, y - 1, z)
			particleVertices[i].x = position.x + right.x - up.x;// * stepScale;
			particleVertices[i].y = position.y + right.y - up.y;//* stepScale;
			particleVertices[i].z = position.z + right.z - up.z;// * stepScale;
			particleVertices[i].u = texCordX+texScaleX;// * textureScale;
			particleVertices[i].v = texCordY+texScaleY;// * textureScale;

            //sceKernelDcacheWritebackInvalidateRange(particleVertices,4 * sizeof(TexturesPSPVertex));
            sceKernelDcacheWritebackInvalidateAll();
		}

		void Particle::BuildVerticiesForItemCrack(CraftWorld* crtf, int itemId)
		{
		    float texturePixel = (float)1.0f/256.0f;
            float percent = (float)1.0f/16.0f;

            if(varf1 == -1.0f)
            {
                varf1 = (1.0f) - (crtf->itemTypes[itemId].textureRow * percent + percent) + ((rand() % 12)*texturePixel + texturePixel*0.5f);
                varf2 = crtf->itemTypes[itemId].plane * percent + (rand() % 12)*texturePixel + texturePixel*0.5f;
            }
            float texCordY = varf1;
            float texScaleY = texturePixel*4 + texturePixel*0.5f;

            float texCordX = varf2;
            float texScaleX = texturePixel*4 + texturePixel*0.5f;

		    ScePspFMatrix4 t;
			sceGumMatrixMode(GU_VIEW);
			sceGumStoreMatrix(&t);
			sceGumMatrixMode(GU_MODEL);

            ScePspFVector3 up, right;

			right.x = t.x.x * horizontalScale * 1;
			right.y = t.y.x * horizontalScale * 1;
			right.z = t.z.x * horizontalScale * 1;
			up.x = t.x.y * verticalScale * 1;
			up.y = t.y.y * verticalScale * 1;
			up.z = t.z.y * verticalScale * 1;

			int i = 0;

			particleVertices[i].x = position.x - right.x + up.x;// * stepScale;
			particleVertices[i].y = position.y - right.y + up.y;// * stepScale;
			particleVertices[i].z = position.z - right.z + up.z;// * stepScale;
            particleVertices[i].u = texCordX;// * textureScale;
			particleVertices[i].v = texCordY;// * textureScale;
			i++;

			// (x, y - 1, z)
			particleVertices[i].x = position.x - right.x - up.x;// * stepScale;
			particleVertices[i].y = position.y - right.y - up.y;// * stepScale;
			particleVertices[i].z = position.z - right.z - up.z;// * stepScale;
			particleVertices[i].u = texCordX;// * textureScale;
			particleVertices[i].v = texCordY+texScaleY;// * textureScale;
			i++;

			// (x + 1, y, z)
			particleVertices[i].x = position.x + right.x + up.x;// * stepScale;
			particleVertices[i].y = position.y + right.y + up.y;// * stepScale;
			particleVertices[i].z = position.z + right.z + up.z;// * stepScale;
			particleVertices[i].u = texCordX+texScaleX;// * textureScale;
			particleVertices[i].v = texCordY;// * textureScale;
			i++;

			// (x + 1, y - 1, z)
			particleVertices[i].x = position.x + right.x - up.x;// * stepScale;
			particleVertices[i].y = position.y + right.y - up.y;//* stepScale;
			particleVertices[i].z = position.z + right.z - up.z;// * stepScale;
			particleVertices[i].u = texCordX+texScaleX;// * textureScale;
			particleVertices[i].v = texCordY+texScaleY;// * textureScale;

            //sceKernelDcacheWritebackInvalidateRange(particleVertices,4 * sizeof(TexturesPSPVertex));
            sceKernelDcacheWritebackInvalidateAll();
		}

		void Particle::Update(CraftWorld* crtf, float dt)
		{
            if(position.x < 0 || position.x >= crtf->WORLD_SIZE || position.y < 0 || position.y >= crtf->WORLD_HEIGHT || position.z < 0 || position.z >= crtf->WORLD_SIZE)
            {
                toDestroy = true;
            }

            if(timeToLive <= 0.0f)
            {
                toDestroy = true;
            }

		    if(toDestroy == true)
            {
                return;
            }

            bBox =  BoundingBox(Vector3(position.x-0.33f,position.y-0.33f,position.z-0.33f),Vector3(position.x+0.33f,position.y+0.33f,position.z+0.33f));

		    myLight = crtf->BlockFinalLight(position.x,position.y,position.z);
		    timeToLive -= dt;

            position += velocity*dt;
           // velocity.y += gravity*dt;

            if(crtf->TaxicabDistance2d(position.x,position.y,crtf->playerPos.x,crtf->playerPos.y) < 12.0f)
            {
                rederMe = true;
            }
            else
            {
                rederMe = false;
            }

            if(name == "smoke")
            {
                if(CheckCollision(crtf) == true)
                {
                    velocity.y = 0.0f;
                }
                BuildVerticies();

                frame = (timeToLive/maxTimeToLive)*8;
                if(frame > 7)
                {
                    frame = 7;
                }

                position.y += 0.2*dt;
                velocity.x -= velocity.x*dt*friction;
                velocity.y -= velocity.y*dt*friction;
                velocity.z -= velocity.z*dt*friction;

                if(abs(velocity.x) < 0.01f && abs(velocity.y) < 0.01f && abs(velocity.z) < 0.01f)
                {
                    velocity = Vector3(0,0,0);
                }
            }

            if(name == "black smoke")
            {
                BuildVerticies();

                frame = (timeToLive/maxTimeToLive)*8;
                if(frame > 7)
                {
                    frame = 7;
                }

                position.y += (0.2+varf2)*dt;
                velocity.x -= velocity.x*dt*friction;
                velocity.y -= velocity.y*dt*friction;
                velocity.z -= velocity.z*dt*friction;

                if(abs(velocity.x) < 0.01f && abs(velocity.y) < 0.01f && abs(velocity.z) < 0.01f)
                {
                    velocity = Vector3(0,0,0);
                }
            }

            if(name == "flame")
            {
                frame = 0;
                BuildVerticies();

                horizontalScale -= dt*0.09f;
                verticalScale = horizontalScale;
            }

            if(name == "explosion")
            {
                BuildVerticies();

                frame = (timeToLive/maxTimeToLive)*16;
                if(frame > 15)
                {
                    frame = 15;
                }
            }

            if(name == "note")
            {
                BuildVerticies();

                velocity.y -= velocity.y*dt*friction;

                if(abs(velocity.y) < 0.01f)
                {
                    velocity = Vector3(0,0,0);
                }
            }

            if(name == "block crack" || name == "item crack")
            {
                if(CheckCollision(crtf) == true)
                {
                    velocity.y = 0.0f;
                }
                if(name == "block crack")
                {
                    BuildVerticiesForBlockCrack(crtf,var1);
                }

                if(name == "item crack")
                {
                    BuildVerticiesForItemCrack(crtf,var1);
                }

                if(crtf->BlockSolid(position.x,position.y-0.2f,position.z) == false)
                {
                    velocity.y += (gravity*1.3 * dt)*1.6f;
                }
                else
                {
                    position.y = ((int)position.y)+0.15f;
                    velocity.y = 0.0f;

                    velocity.x *= 0.8f;
                    velocity.z *= 0.8f;
                }

                if(abs(velocity.x) < 0.1f)
                {
                    velocity.x = 0.0f;
                }
                if(abs(velocity.z) < 0.1f)
                {
                    velocity.z = 0.0f;
                }

                if(crtf->BlockSolid(position.x,position.y+0.15f,position.z) == true)
                {
                    velocity.y = 0;
                    position.y -= 0.1f;
                }

                if(crtf->BlockSolid(position.x+velocity.x*dt*2,position.y,position.z+velocity.z*dt*2) == true)
                {
                    velocity.x *= -0.9f;
                    velocity.z *= -0.9f;
                }
            }
		}

		bool Particle::CheckCollision(CraftWorld* crtf)
		{
            if (crtf->BlockSolid(position.x,position.y+horizontalScale*1.5f,position.z) != 0)
            {
                return 1;
            }
            else
            {
                return 0;
            }
		}

		void Particle::Render(Frustum &camFrustum)
		{
		    if(rederMe == true && toDestroy == false && particleVertices != NULL && camFrustum.BoxInFrustum(bBox) == Frustum::Intersects)
            {
                sceGumPushMatrix();
                sceGuColor(GU_COLOR(color.x*myLight,color.y*myLight,color.z*myLight,1));
                if(name == "flame")
                {
                    sceGuColor(GU_COLOR(1,1,1,1));
                }
                TextureManager::Instance()->SetTextureModeulate(texture);

                sceGuEnable(GU_TEXTURE_2D);
                sceGuEnable(GU_DEPTH_TEST);
                sceGuEnable(GU_ALPHA_TEST);
                sceGuEnable(GU_BLEND);

                sceGumDrawArray(GU_TRIANGLE_STRIP, GU_TEXTURE_32BITF | GU_VERTEX_32BITF | GU_TRANSFORM_3D, 4, 0, particleVertices);

                sceGuDisable(GU_BLEND);
                sceGuDisable(GU_ALPHA_TEST);
                sceGuDisable(GU_DEPTH_TEST);
                sceGuDisable(GU_TEXTURE_2D);
                sceGuColor(0xFFFFFFFF);
                sceGumPopMatrix();
            }
		}
	}
}

