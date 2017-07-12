#ifndef AURORA_BOUNDINGSPHERE_H
#define AURORA_BOUNDINGSPHERE_H

#include <Aurora/Math/Vector3.h>
#include <Aurora/Math/BoundingBox.h>
#include <pspmath.h>

namespace Aurora
{
	namespace Math
	{
		class BoundingSphere
		{
		public:
			Vector3 center;
			float radius;

			BoundingSphere();
			BoundingSphere(const Vector3 &center_, float radius_);
			~BoundingSphere();

			bool hasCollided(const BoundingSphere &other) const;
			bool intersect(BoundingBox &box);
			bool inside(BoundingBox &box,Vector3 &axis, float &distance);
			bool PointCollided(Vector3 &point);
		};
	}
}

#endif
