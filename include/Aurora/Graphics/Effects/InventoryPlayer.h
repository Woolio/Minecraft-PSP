#ifndef INVENTORYPLAYER_H_
#define INVENTORYPLAYER_H_

#include <Aurora/Math/Vector3.h>
#include <Aurora/Graphics/Vertex.h>
#include <pspmath.h>
#include <Lamecraft/CraftWorld2.h>
#include <Lamecraft/TextureHelper.h>

namespace Aurora
{
	namespace Graphics
	{
		class InventoryPlayer
		{
		public:
			InventoryPlayer();
			~InventoryPlayer();

			void Update();
			void Render(CraftWorld *world, float dt, float angle, int texNum, int handItemId);

			float mainAngle;
			float scale;
			float animT;

		private:

            float rHandAngle;
            float lHandAngle;
            float rLegAngle;
            float lLegAngle;

            Vector3 position;
		};
	}
}

#endif

