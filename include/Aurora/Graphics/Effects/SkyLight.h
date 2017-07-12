#ifndef SKYLIGHT_H_
#define SKYLIGHT_H_

#include <Aurora/Math/Vector3.h>
#include <Aurora/Graphics/Vertex.h>
#include <pspmath.h>

namespace Aurora
{
	namespace Graphics
	{
		class SkyLight
		{
		public:
			SkyLight();
			~SkyLight();

			void UpdateLightSource(float sun_angle);
			void Render();

		private:

			TexturesPSPVertex *skyVertices;

			float sun_angle;
		};
	}
}

#endif

