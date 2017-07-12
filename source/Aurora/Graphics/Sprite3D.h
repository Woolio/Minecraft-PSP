#ifndef SPRITE3D_H_
#define SPRITE3D_H_

#include <Aurora/Graphics/Vertex.h>
#include <string>

namespace Aurora
{
	namespace Graphics
	{

		class Sprite3D
		{
		public:
			std::string imageName;

			float posX,posY,posZ;
			float scaleX,scaleY,scaleZ;
			float rotationX,rotationY,rotationZ;

			unsigned int* colours;
						
			int width,height;

			Sprite3D(const char* filename,int startW,int startH,int endW,int endH);
			~Sprite3D();

			void setPosition(float x,float y,float z);
			void setscale(float x,float y,float z);
			void setRotation(float x,float y,float z);
			void Draw();
		};
	}
}

#endif
