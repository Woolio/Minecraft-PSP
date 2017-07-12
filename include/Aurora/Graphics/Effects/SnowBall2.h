#ifndef SNOWBALL2_H_
#define SNOWBALL2_H_

#include <Aurora/Math/Vector3.h>
#include <Aurora/Graphics/Vertex.h>
#include <pspmath.h>
#include <LameCraft/CraftWorld2.h>
#include <math.h>
#include <psptypes.h>

namespace Aurora
{
	namespace Graphics
	{
		class SnowBall2
		{
		public:
			SnowBall2(float pos2x, float pos2y, float pos2z);
			~SnowBall2();

			void SetTexture(int texture);
			void SetVeloc(float verAngle, float horAngle);
			void Update(CraftWorld* crtf, float dt);
			bool CheckCollision(CraftWorld* crtf);

			void Render();

			Vector3 position;
			Vector3 velocity;

		private:

			CraftPSPVertex *ballVertices;
			int textureNumber;
			float snowHScale;
			float snowVScale;

			float startSpeed;
		};
	}
}

#endif

