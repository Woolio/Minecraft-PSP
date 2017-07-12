#ifndef SPRITE_H_
#define SPRITE_H_

#include <Aurora/Graphics/Vertex.h>

namespace Aurora
{
	namespace Graphics
	{

		class Sprite
		{
		public:
			Sprite();
			Sprite(const char* filename);
			Sprite(int textureNumer);
			Sprite(const char* filename,int startW,int startH,int endW,int endH);
			Sprite(int textureNumer,int startW,int startH,int endW,int endH);
			~Sprite();


			void SetPosition(float x,float y);
			void Scale(float x,float y);
			void Draw();
			void RemoveImage();

		private:
			int imageNumber;

			TexturesPSPVertex *vertices;

			float scaleX,scaleY;
			float posX,posY;
			int width,height;

		};

	}

}

#endif
