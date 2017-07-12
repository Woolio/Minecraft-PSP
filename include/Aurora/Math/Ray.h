#ifndef AURORA_RAY_H
#define AURORA_RAY_H

#include <Aurora/Math/Vector3.h>
#include <Aurora/Math/BoundingBox.h>
#include <Aurora/Math/BoundingSphere.h>
#include <Aurora/Math/Plane.h>
#include <pspmath.h>

namespace Aurora
{
	namespace Math
	{

		class Ray
		{
		public:
			Vector3 origin;
			Vector3 direction;

			Ray();
			Ray(const Vector3 &origin_, const Vector3 &direction_);
			~Ray();

			bool hasIntersected(const BoundingSphere &sphere) const;
			bool hasIntersected(const BoundingBox &box) const;
			void hasIntersected(BoundingBox box,float &distance);
			bool hasIntersected(const Plane &plane) const;
			bool hasIntersected(const Plane &plane, float &t, Vector3 &intersection) const;
		};

	}
}

#endif
