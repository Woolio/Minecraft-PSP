#ifndef SKYDOME_H_
#define SKYDOME_H_

#include <Aurora/Math/Vector3.h>
#include <Aurora/Graphics/Vertex.h>
#include <pspmath.h>

namespace Aurora
{
	namespace Graphics
	{
		class SkyDome
		{
		public:
			SkyDome();
			~SkyDome();

			void CreateSkyDomeMesh();
			void SetTexture(int texture);
			void Render();

			int dphi;
			int dtheta;
			int numVerts;

			float timeOfDay;


		private:

			TexturesPSPVertex *domeVertices;
			int textureNumber;
		};
	}
}

#endif

