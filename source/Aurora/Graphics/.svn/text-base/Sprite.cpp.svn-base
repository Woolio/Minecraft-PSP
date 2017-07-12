#include <Aurora/Graphics/Sprite.h>
#include <Aurora/Graphics/TextureManager.h>

namespace Aurora
{

	namespace Graphics
	{

		Sprite::Sprite()
		{
			imageNumber = 0;
		}

		TexturesPSPVertex getVertex(float u,float v,float x, float y,float z)
		{
			TexturesPSPVertex vert;
			vert.u = u;
			vert.v = v;
			vert.x = x;
			vert.y = y;
			vert.z = z;
			return vert;
		}

		Sprite::Sprite(const char* filename)
		{
			TextureManager::Instance()->LoadTexture(filename);

			imageNumber = TextureManager::Instance()->GetTextureNumber(filename);

			//generate wertices
			vertices = (TexturesPSPVertex*)memalign(16, 4 * sizeof(TexturesPSPVertex) );

			Image *img = TextureManager::Instance()->Images[imageNumber];

			width = img->Width;
			height = img->Height;

			float hPercent = (float)img->Height / (float)img->power2Height;
			float wPercent = (float)img->Width / (float)img->power2Width;

			if( vertices )
			{
				vertices[0] = getVertex(0.0f,0.0f,-img->Width/2,-img->Height/2,0.0f);
				vertices[1] = getVertex(0.0f,hPercent,-img->Width/2, img->Height/2,0.0f);
				vertices[2] = getVertex(wPercent,0.0f,img->Width/2,-img->Height/2,0.0f);
				vertices[3] = getVertex(wPercent,hPercent,img->Width/2, img->Height/2,0.0f);
			}

			sceKernelDcacheWritebackInvalidateRange(vertices, 4 * sizeof(TexturesPSPVertex));
		}

		Sprite::Sprite(int textureNumer)
		{
			imageNumber = textureNumer;

			//generate wertices
			vertices = (TexturesPSPVertex*)memalign(16, 4 * sizeof(TexturesPSPVertex) );

			Image *img = TextureManager::Instance()->Images[imageNumber];

			width = img->Width;
			height = img->Height;

			float hPercent = (float)img->Height / (float)img->power2Height;
			float wPercent = (float)img->Width / (float)img->power2Width;

			if( vertices )
			{
				vertices[0] = getVertex(0.0f,0.0f,-img->Width/2,-img->Height/2,0.0f);
				vertices[1] = getVertex(0.0f,hPercent,-img->Width/2, img->Height/2,0.0f);
				vertices[2] = getVertex(wPercent,0.0f,img->Width/2,-img->Height/2,0.0f);
				vertices[3] = getVertex(wPercent,hPercent,img->Width/2, img->Height/2,0.0f);
			}

			sceKernelDcacheWritebackInvalidateRange(vertices, 4 * sizeof(TexturesPSPVertex));
		}

		Sprite::Sprite(const char* filename,int startW,int startH,int endW,int endH)
		{
			TextureManager::Instance()->LoadTexture(filename);

			imageNumber = TextureManager::Instance()->GetTextureNumber(filename);

			//generate wertices
			vertices = (TexturesPSPVertex*)memalign(16, 4 * sizeof(TexturesPSPVertex) );

			Image *img = TextureManager::Instance()->Images[imageNumber];

			width = endW;
			height = endH;

			float hstart = (float)startH / (float)img->power2Height;
			float wstart = (float)startW / (float)img->power2Width;
			float hPercent = (float)(startH + endH) / (float)img->power2Height;
			float wPercent = (float)(startW + endW) / (float)img->power2Width;

			if( vertices )
			{
				vertices[0] = getVertex(wstart,hstart,-width/2,-height/2,0.0f);
				vertices[1] = getVertex(wstart,hPercent,-width/2, height/2,0.0f);
				vertices[2] = getVertex(wPercent,hstart,width/2,-height/2,0.0f);
				vertices[3] = getVertex(wPercent,hPercent,width/2, height/2,0.0f);
			}

			//sceKernelDcacheWritebackInvalidateAll();
			sceKernelDcacheWritebackInvalidateRange(vertices, 4 * sizeof(TexturesPSPVertex));
		}

		Sprite::Sprite(int textureNumer,int startW,int startH,int endW,int endH)
		{
			imageNumber = textureNumer;

			//generate wertices
			vertices = (TexturesPSPVertex*)memalign(16, 4 * sizeof(TexturesPSPVertex) );

			Image *img = TextureManager::Instance()->Images[imageNumber];

			width = endW;
			height = endH;

			float hstart = (float)startH / (float)img->power2Height;
			float wstart = (float)startW / (float)img->power2Width;
			float hPercent = (float)(startH + endH) / (float)img->power2Height;
			float wPercent = (float)(startW + endW) / (float)img->power2Width;

			if( vertices )
			{
				vertices[0] = getVertex(wstart,hstart,-width/2,-height/2,0.0f);
				vertices[1] = getVertex(wstart,hPercent,-width/2, height/2,0.0f);
				vertices[2] = getVertex(wPercent,hstart,width/2,-height/2,0.0f);
				vertices[3] = getVertex(wPercent,hPercent,width/2, height/2,0.0f);
			}

			//sceKernelDcacheWritebackInvalidateAll();
			sceKernelDcacheWritebackInvalidateRange(vertices, 4 * sizeof(TexturesPSPVertex));
		}

		Sprite::~Sprite()
		{
			free(vertices);
		}

		void Sprite::SetPosition(float x,float y)
		{
			posX = x;
			posY = y;
		}

		void Sprite::Scale(float x,float y)
		{
			scaleX = x;
			scaleY = y;

			for(int i=0; i<4; i++)
			{
				vertices[i].x = vertices[i].x * scaleX;
				vertices[i].y = vertices[i].y * scaleY;
			}

		}

		void Sprite::Draw()
		{
			sceGumPushMatrix();

			ScePspFVector3 loc = {posX,posY,0.0f};
			sceGumTranslate(&loc);

			sceGuEnable(GU_TEXTURE_2D);
			TextureManager::Instance()->SetTexture(imageNumber,GU_NEAREST,GU_NEAREST);

			sceGumDrawArray(GU_TRIANGLE_STRIP,GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D, 4, 0, vertices);

			sceGuDisable(GU_TEXTURE_2D);
			sceGumPopMatrix();
		}

		void Sprite::RemoveImage()
		{
			TextureManager::Instance()->RemoveTexture(imageNumber);
		}

	}

}

