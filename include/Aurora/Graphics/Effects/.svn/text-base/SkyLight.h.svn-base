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

			float TimeToAngle(int seconds);
			void SetTexture(int texture);
			void UpdateLightSource(float angle);

			void Render();

		private:

			TexturesPSPVertex *skyVertices;
			int textureNumber;

			int time_sunrise, time_sunset;
			float sun_x, sun_y, sun_z;
			float sun_angle, sun_tilt;
			float dis, stepScale, lightHScale, lightVScale;
		};
	}
}

#endif

