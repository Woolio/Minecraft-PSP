#include <Aurora/Math/BoundingSphere.h>

namespace Aurora
{
	namespace Math
	{
		BoundingSphere::BoundingSphere()
		{
			center.set(0.0f, 0.0f, 0.0f);
			radius = 0.0f;
		}

		BoundingSphere::BoundingSphere(const Vector3 &center_, float radius_)
		{
			center = center_;
			radius = radius_;
		}

		BoundingSphere::~BoundingSphere()
		{
		}

		bool BoundingSphere::hasCollided(const BoundingSphere &other) const
		{
			Vector3 disp(other.center - center);
			float lengthSq = (disp.x * disp.x) + (disp.y * disp.y) + (disp.z * disp.z);
			float radiiSq = (other.radius + radius) * (other.radius + radius);

			return (lengthSq < radiiSq) ? true : false;
		}

		bool BoundingSphere::intersect(BoundingBox &box)
		{
			Vector3 vector = Vector3::clamp(center, box.min, box.max);
			float num = Vector3::distanceSq(center, vector);
			return (num <= (radius * radius));
		}

		bool BoundingSphere::inside(BoundingBox &box,Vector3 &axis, float &distance)
		{
			if(axis.x == 0.0f)
			{
				if(center.x - radius <= box.min.x || center.x + radius >= box.max.x)
					return false;
			}
			if(axis.y == 0.0f)
			{
				if(center.y - radius <= box.min.y || center.y + radius >= box.max.y)
					return false;
			}
			if(axis.z == 0.0f)
			{
				if(center.z - radius <= box.min.z || center.z + radius >= box.max.z)
					return false;
			}

			return true;
			/*float r2 = radius * radius;
			float xmin,ymin,zmin;

			distance = 0.0f;

			if(axis.x ==  0.0f)
			{
				xmin = center.x;
				xmin = (xmin > box.max.x) ? box.max.x : xmin;
				xmin = (xmin < box.min.x) ? box.min.x : xmin;
				distance += (center.x - xmin) * (center.x - xmin);
			}

			if(axis.y ==  0.0f)
			{
				ymin = center.y;
				ymin = (ymin > box.max.y) ? box.max.y : ymin;
				ymin = (ymin < box.min.y) ? box.min.y : ymin;
				distance += (center.y - ymin) * (center.y - ymin);
			}

			if(axis.z ==  0.0f)
			{
				zmin = center.z;
				zmin = (zmin > box.max.z) ? box.max.z : zmin;
				zmin = (zmin < box.min.z) ? box.min.z : zmin;
				distance += (center.z - zmin) * (center.z - zmin);
			}

			return distance <= r2;*/
		}

		bool BoundingSphere::PointCollided(Vector3 &point)
		{
			Vector3 disp(point - center);
			float lengthSq = (disp.x * disp.x) + (disp.y * disp.y) + (disp.z * disp.z);
			float radiiSq = (radius) * ( radius);

			return (lengthSq < radiiSq) ? true : false;
		}
	}
}
