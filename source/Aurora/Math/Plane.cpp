#include <Aurora/Math/Plane.h>

namespace Aurora
{
	namespace Math
	{
		Plane::Plane()
		{
		}

		Plane::Plane(float a_, float b_, float c_, float d_) : n(a_, b_, c_), d(d_)
		{
		}

		Plane::Plane(const Vector3 &pt, const Vector3 &normal)
		{
			fromPointNormal(pt, normal);
		}

		Plane::Plane(const Vector3 &pt1, const Vector3 &pt2, const Vector3 &pt3)
		{
			fromPoints(pt1, pt2, pt3);
		}

		Plane::~Plane()
		{
		}

		float Plane::dot(const Plane &p, const Vector3 &pt)
		{
			// Returns:
			//  > 0 if the point 'pt' lies in front of the plane 'p'
			//  < 0 if the point 'pt' lies behind the plane 'p'
			//    0 if the point 'pt' lies on the plane 'p'
			//
			// The signed distance from the point 'pt' to the plane 'p' is returned.

			return Vector3::dot(p.n, pt) + p.d;
		}

		bool Plane::operator==(const Plane &rhs) const
		{
			return (n == rhs.n) && Math::closeEnough(d, rhs.d);
		}

		bool Plane::operator!=(const Plane &rhs) const
		{
			return !(*this == rhs);
		}

		void Plane::fromPointNormal(const Vector3 &pt, const Vector3 &normal)
		{
			set(normal.x, normal.y, normal.z, -Vector3::dot(normal, pt));
			normalize();
		}

		void Plane::fromPoints(const Vector3 &pt1, const Vector3 &pt2, const Vector3 &pt3)
		{
			n = Vector3::cross(pt2 - pt1, pt3 - pt1);
			d = -Vector3::dot(n, pt1);
			normalize();
		}

		const Vector3 &Plane::normal() const
		{
			return n;
		}

		Vector3 &Plane::normal()
		{
			return n;
		}

		void Plane::normalize()
		{
			float length = 1.0f / n.magnitude();
			n *= length;
			d *= length;
		}

		void Plane::set(float a_, float b_, float c_, float d_)
		{
			n.set(a_, b_, c_);
			d = d_;
		}
	}
}

