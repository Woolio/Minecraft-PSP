#ifndef AURORA_PLANE_H
#define AURORA_PLANE_H

#include <Aurora/Math/Vector3.h>
#include <pspmath.h>

namespace Aurora
{
	namespace Math
	{

		class Plane
		{
		public:
			Vector3 n;
			float d;

			static float dot(const Plane &p, const Vector3 &pt);

			Plane();
			Plane(float a_, float b_, float c_, float d_);
			Plane(const Vector3 &pt, const Vector3 &normal);
			Plane(const Vector3 &pt1, const Vector3 &pt2, const Vector3 &pt3);
			~Plane();

			bool operator==(const Plane &rhs) const;
			bool operator!=(const Plane &rhs) const;

			void fromPointNormal(const Vector3 &pt, const Vector3 &normal);
			void fromPoints(const Vector3 &pt1, const Vector3 &pt2, const Vector3 &pt3);
			const Vector3 &normal() const;
			Vector3 &normal();
			void normalize();
			void set(float a_, float b_, float c_, float d_);
		};
	}
}

#endif
