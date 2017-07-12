#include <Aurora/Math/Ray.h>
#include <algorithm>

namespace Aurora
{
	namespace Math
	{
		Ray::Ray()
		{
		}

		Ray::Ray(const Vector3 &origin_, const Vector3 &direction_)
		{
			origin = origin_;
			direction = direction_;
		}

		Ray::~Ray()
		{
		}

		bool Ray::hasIntersected(const BoundingSphere &sphere) const
		{
			Vector3 w(sphere.center - origin);
			float wsq = Vector3::dot(w, w);
			float proj = Vector3::dot(w, direction);
			float rsq = sphere.radius * sphere.radius;

			// Early out: if sphere is behind the ray then there's no intersection.
			if (proj < 0.0f && wsq > rsq)
				return false;

			float vsq = Vector3::dot(direction, direction);

			// Test length of difference vs. radius.
			return vsq * wsq - proj * proj <= vsq * rsq;
		}

		void Ray::hasIntersected(BoundingBox box,float &distance)
		{
			distance = -1.0f;
			float num = 0.0f;
			float num2 = 3.40282347E+38f;
			if (fabs(direction.x) < 1E-06f)
			{
				if (origin.x < box.min.x || origin.x > box.max.x)
				{
					return;
				}
			}
			else
			{
				float num3 = 1.0f / direction.x;
				float num4 = (box.min.x - origin.x) * num3;
				float num5 = (box.max.x - origin.x) * num3;
				if (num4 > num5)
				{
					float num6 = num4;
					num4 = num5;
					num5 = num6;
				}
				num = std::max(num4, num);
				num2 = std::min(num5, num2);
				if (num > num2)
				{
					return;
				}
			}
			if (fabs(direction.y) < 1E-06f)
			{
				if (origin.y < box.min.y || origin.y > box.max.y)
				{
					return;
				}
			}
			else
			{
				float num7 = 1.0f / direction.y;
				float num8 = (box.min.y - origin.y) * num7;
				float num9 = (box.max.y - origin.y) * num7;
				if (num8 > num9)
				{
					float num10 = num8;
					num8 = num9;
					num9 = num10;
				}
				num = std::max(num8, num);
				num2 = std::min(num9, num2);
				if (num > num2)
				{
					return;
				}
			}
			if (fabs(direction.z) < 1E-06f)
			{
				if (origin.z < box.min.z || origin.z > box.max.z)
				{
					return;
				}
			}
			else
			{
				float num11 = 1.0f / direction.z;
				float num12 = (box.min.z - origin.z) * num11;
				float num13 = (box.max.z - origin.z) * num11;
				if (num12 > num13)
				{
					float num14 = num12;
					num12 = num13;
					num13 = num14;
				}
				num = std::max(num12, num);
				num2 = std::min(num13, num2);
				if (num > num2)
				{
					return;
				}
			}
			distance = num;
		}



		bool Ray::hasIntersected(const BoundingBox &box) const
		{
			// References:
			//  Jeffrey Mahovsky and Brian Wyvill, "Fast Ray-Axis Aligned Bounding Box
			//  Overlap Tests with Plücker Coordinates", Journal of Graphics Tools,
			//  9(1):35-46.

			if (direction.x < 0.0f)
			{
				if (direction.y < 0.0f)
				{
					if (direction.z < 0.0f)
					{
						// case MMM: side(R,HD) < 0 or side(R,FB) > 0 or side(R,EF) > 0 or side(R,DC) < 0 or side(R,CB) < 0 or side(R,HE) > 0 to miss

						if ((origin.x < box.min.x) || (origin.y < box.min.y) || (origin.z < box.min.z))
							return false;

						float xa = box.min.x - origin.x;
						float ya = box.min.y - origin.y;
						float za = box.min.z - origin.z;
						float xb = box.max.x - origin.x;
						float yb = box.max.y - origin.y;
						float zb = box.max.z - origin.z;

						if ((direction.x * ya - direction.y * xb < 0)
								|| (direction.x * yb - direction.y * xa > 0)
								|| (direction.x * zb - direction.z * xa > 0)
								|| (direction.x * za - direction.z * xb < 0)
								|| (direction.y * za - direction.z * yb < 0)
								|| (direction.y * zb - direction.z * ya > 0))
							return false;

						return true;
					}
					else
					{
						// case MMP: side(R,HD) < 0 or side(R,FB) > 0 or side(R,HG) > 0 or side(R,AB) < 0 or side(R,DA) < 0 or side(R,GF) > 0 to miss

						if ((origin.x < box.min.x) || (origin.y < box.min.y) || (origin.z > box.max.z))
							return false;

						float xa = box.min.x - origin.x;
						float ya = box.min.y - origin.y;
						float za = box.min.z - origin.z;
						float xb = box.max.x - origin.x;
						float yb = box.max.y - origin.y;
						float zb = box.max.z - origin.z;

						if ((direction.x * ya - direction.y * xb < 0.0f)
								|| (direction.x * yb - direction.y * xa > 0.0f)
								|| (direction.x * zb - direction.z * xb > 0.0f)
								|| (direction.x * za - direction.z * xa < 0.0f)
								|| (direction.y * za - direction.z * ya < 0.0f)
								|| (direction.y * zb - direction.z * yb > 0.0f))
							return false;

						return true;
					}
				}
				else
				{
					if (direction.z < 0.0f)
					{
						// case MPM: side(R,EA) < 0 or side(R,GC) > 0 or side(R,EF) > 0 or side(R,DC) < 0 or side(R,GF) < 0 or side(R,DA) > 0 to miss

						if ((origin.x < box.min.x) || (origin.y > box.max.y) || (origin.z < box.min.z))
							return false;

						float xa = box.min.x - origin.x;
						float ya = box.min.y - origin.y;
						float za = box.min.z - origin.z;
						float xb = box.max.x - origin.x;
						float yb = box.max.y - origin.y;
						float zb = box.max.z - origin.z;

						if ((direction.x * ya - direction.y * xa < 0.0f)
								|| (direction.x * yb - direction.y * xb > 0.0f)
								|| (direction.x * zb - direction.z * xa > 0.0f)
								|| (direction.x * za - direction.z * xb < 0.0f)
								|| (direction.y * zb - direction.z * yb < 0.0f)
								|| (direction.y * za - direction.z * ya > 0.0f))
							return false;

						return true;
					}
					else
					{
						// case MPP: side(R,EA) < 0 or side(R,GC) > 0 or side(R,HG) > 0 or side(R,AB) < 0 or side(R,HE) < 0 or side(R,CB) > 0 to miss

						if ((origin.x < box.min.x) || (origin.y > box.max.y) || (origin.z > box.max.z))
							return false;

						float xa = box.min.x - origin.x;
						float ya = box.min.y - origin.y;
						float za = box.min.z - origin.z;
						float xb = box.max.x - origin.x;
						float yb = box.max.y - origin.y;
						float zb = box.max.z - origin.z;

						if ((direction.x * ya - direction.y * xa < 0.0f)
								|| (direction.x * yb - direction.y * xb > 0.0f)
								|| (direction.x * zb - direction.z * xb > 0.0f)
								|| (direction.x * za - direction.z * xa < 0.0f)
								|| (direction.y * zb - direction.z * ya < 0.0f)
								|| (direction.y * za - direction.z * yb > 0.0f))
							return false;

						return true;
					}
				}
			}
			else
			{
				if (direction.y < 0.0f)
				{
					if (direction.z < 0.0f)
					{
						// case PMM: side(R,GC) < 0 or side(R,EA) > 0 or side(R,AB) > 0 or side(R,HG) < 0 or side(R,CB) < 0 or side(R,HE) > 0 to miss

						if ((origin.x > box.max.x) || (origin.y < box.min.y) || (origin.z < box.min.z))
							return false;

						float xa = box.min.x - origin.x;
						float ya = box.min.y - origin.y;
						float za = box.min.z - origin.z;
						float xb = box.max.x - origin.x;
						float yb = box.max.y - origin.y;
						float zb = box.max.z - origin.z;

						if ((direction.x * yb - direction.y * xb < 0.0f)
								|| (direction.x * ya - direction.y * xa > 0.0f)
								|| (direction.x * za - direction.z * xa > 0.0f)
								|| (direction.x * zb - direction.z * xb < 0.0f)
								|| (direction.y * za - direction.z * yb < 0.0f)
								|| (direction.y * zb - direction.z * ya > 0.0f))
							return false;

						return true;
					}
					else
					{
						// case PMP: side(R,GC) < 0 or side(R,EA) > 0 or side(R,DC) > 0 or side(R,EF) < 0 or side(R,DA) < 0 or side(R,GF) > 0 to miss

						if ((origin.x > box.max.x) || (origin.y < box.min.y) || (origin.z > box.max.z))
							return false;

						float xa = box.min.x - origin.x;
						float ya = box.min.y - origin.y;
						float za = box.min.z - origin.z;
						float xb = box.max.x - origin.x;
						float yb = box.max.y - origin.y;
						float zb = box.max.z - origin.z;

						if ((direction.x * yb - direction.y * xb < 0.0f)
								|| (direction.x * ya - direction.y * xa > 0.0f)
								|| (direction.x * za - direction.z * xb > 0.0f)
								|| (direction.x * zb - direction.z * xa < 0.0f)
								|| (direction.y * za - direction.z * ya < 0.0f)
								|| (direction.y * zb - direction.z * yb > 0.0f))
							return false;

						return true;
					}
				}
				else
				{
					if (direction.z < 0.0f)
					{
						// case PPM: side(R,FB) < 0 or side(R,HD) > 0 or side(R,AB) > 0 or side(R,HG) < 0 or side(R,GF) < 0 or side(R,DA) > 0 to miss

						if ((origin.x > box.max.x) || (origin.y > box.max.y) || (origin.z < box.min.z))
							return false;

						float xa = box.min.x - origin.x;
						float ya = box.min.y - origin.y;
						float za = box.min.z - origin.z;
						float xb = box.max.x - origin.x;
						float yb = box.max.y - origin.y;
						float zb = box.max.z - origin.z;

						if ((direction.x * yb - direction.y * xa < 0.0f)
								|| (direction.x * ya - direction.y * xb > 0.0f)
								|| (direction.x * za - direction.z * xa > 0.0f)
								|| (direction.x * zb - direction.z * xb < 0.0f)
								|| (direction.y * zb - direction.z * yb < 0.0f)
								|| (direction.y * za - direction.z * ya > 0.0f))
							return false;

						return true;
					}
					else
					{
						// case PPP: side(R,FB) < 0 or side(R,HD) > 0 or side(R,DC) > 0 or side(R,EF) < 0 or side(R,HE) < 0 or side(R,CB) > 0 to miss

						if ((origin.x > box.max.x) || (origin.y > box.max.y) || (origin.z > box.max.z))
							return false;

						float xa = box.min.x - origin.x;
						float ya = box.min.y - origin.y;
						float za = box.min.z - origin.z;
						float xb = box.max.x - origin.x;
						float yb = box.max.y - origin.y;
						float zb = box.max.z - origin.z;

						if ((direction.x * yb - direction.y * xa < 0.0f)
								|| (direction.x * ya - direction.y * xb > 0.0f)
								|| (direction.x * za - direction.z * xb > 0.0f)
								|| (direction.x * zb - direction.z * xa < 0.0f)
								|| (direction.y * zb - direction.z * ya < 0.0f)
								|| (direction.y * za - direction.z * yb > 0.0f))
							return false;

						return true;
					}
				}
			}

			return false;
		}

		bool Ray::hasIntersected(const Plane &plane) const
		{
			float t;
			Vector3 intersection;

			return hasIntersected(plane, t, intersection);
		}

		bool Ray::hasIntersected(const Plane &plane, float &t, Vector3 &intersection) const
		{
			float denominator = Vector3::dot(direction, plane.n);

			// Early out: if ray is parallel to the plane then no intersection.
			if (Math::closeEnough(fabs(denominator), 0.0f))
			{
				float rayOriginToPlane = Plane::dot(plane, origin);

				// The ray lies in the plane if the ray origin is in the plane.
				// In such a case there's an infinite number of intersection points.
				if (Math::closeEnough(rayOriginToPlane, 0.0f))
				{
					t = 0.0f;
					return true;
				}
				else
				{
					return false;
				}
			}

			// Ray isn't parallel to plane. That means there's an intersection.
			t = -Plane::dot(plane, origin) / denominator;

			// Late out: rays and line segments are only defined for 't' >= 0.
			if (t < 0.0f)
				return false;

			intersection = origin + (direction * t);
			return true;
		}
	}
}
