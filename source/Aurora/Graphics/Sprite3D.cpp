#include <Aurora/Graphics/Sprite3D.h>
#include <Aurora/Graphics/TextureManager.h>


namespace Aurora
{
	namespace Graphics
	{
		Sprite3D::Sprite3D(const char* filename,int startW,int startH,int endW,int endH)
		{
			imageName = filename;
			TextureManager::Instance()->loadImageFromFile(filename);

			width = endW - startW;
			height = endH - startH;

			colours = new unsigned int[width * height];

			Image *img = TextureManager::Instance()->getImage(filename);

			for (int i = 0;i < height;i++)
			{
				for (int j = 0;j<width;j++)
				{
					unsigned int colour = img->getColour(startW + j,startH + i);
					colours[j + i*width] = colour;
				}
			}

			posX=posY=posZ=0.0f;
			scaleX=scaleY=scaleZ = 1.0f;
			rotationX=rotationY=rotationZ = 0.0f;
		}

		Sprite3D::~Sprite3D()
		{

		}

		void Sprite3D::setPosition(float x,float y,float z)
		{
			posX = x;
			posY = y;
			posZ = z;
		}

		void Sprite3D::setscale(float x,float y,float z)
		{
			scaleX = x;
			scaleY = y;
			scaleZ = z;
		}

		void Sprite3D::setRotation(float x,float y,float z)
		{
			rotationX = x;
			rotationY = y;
			rotationZ = z;
		}


	}
}

