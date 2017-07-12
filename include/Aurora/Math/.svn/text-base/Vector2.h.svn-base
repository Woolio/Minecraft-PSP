#ifndef AURORA_VECTOR2_H
#define AURORA_VECTOR2_H

#include <Aurora/Math/MathLib.h>
#include <pspmath.h>

namespace Aurora
{
	namespace Math
	{
		class Vector2
		{
			friend Vector2 operator*(float lhs, const Vector2 &rhs);
			friend Vector2 operator-(const Vector2 &v);

		public:
			float x, y;

			static float distance(const Vector2 &pt1, const Vector2 &pt2);
			static float distanceSq(const Vector2 &pt1, const Vector2 &pt2);  
			static float dot(const Vector2 &p, const Vector2 &q);
			static Vector2 lerp(const Vector2 &p, const Vector2 &q, float t);
			static void orthogonalize(Vector2 &v1, Vector2 &v2);
			static Vector2 proj(const Vector2 &p, const Vector2 &q);
			static Vector2 perp(const Vector2 &p, const Vector2 &q);
			static Vector2 reflect(const Vector2 &i, const Vector2 &n);

			Vector2() {}
			Vector2(float x_, float y_);
			~Vector2() {}

			bool operator==(const Vector2 &rhs) const;
			bool operator!=(const Vector2 &rhs) const;

			Vector2 &operator+=(const Vector2 &rhs);
			Vector2 &operator-=(const Vector2 &rhs);
			Vector2 &operator*=(float scalar);
			Vector2 &operator/=(float scalar);

			Vector2 operator+(const Vector2 &rhs) const;
			Vector2 operator-(const Vector2 &rhs) const;
			Vector2 operator*(float scalar) const;
			Vector2 operator/(float scalar) const;

			float magnitude() const;
			float magnitudeSq() const;
			Vector2 inverse() const;
			void normalize();
			void set(float x_, float y_);
		};

		inline Vector2 operator*(float lhs, const Vector2 &rhs)
		{
			return Vector2(lhs * rhs.x, lhs * rhs.y);
		}

		inline Vector2 operator-(const Vector2 &v)
		{
			return Vector2(-v.x, -v.y);
		}

		inline float Vector2::distance(const Vector2 &pt1, const Vector2 &pt2)
		{
			// Calculates the distance between 2 points.
			return sqrtf(distanceSq(pt1, pt2));
		}

		inline float Vector2::distanceSq(const Vector2 &pt1, const Vector2 &pt2)
		{
			// Calculates the squared distance between 2 points.
			return ((pt1.x - pt2.x) * (pt1.x - pt2.x)) + ((pt1.y - pt2.y) * (pt1.y - pt2.y));
		}

		inline float Vector2::dot(const Vector2 &p, const Vector2 &q)
		{
			return (p.x * q.x) + (p.y * q.y);
		}

		inline Vector2 Vector2::lerp(const Vector2 &p, const Vector2 &q, float t)
		{
			// Linearly interpolates from 'p' to 'q' as t varies from 0 to 1.
			return p + t * (q - p);
		}

		inline void Vector2::orthogonalize(Vector2 &v1, Vector2 &v2)
		{
			// Performs Gram-Schmidt Orthogonalization on the 2 basis vectors to
			// turn them into orthonormal basis vectors.
			v2 = v2 - proj(v2, v1);
			v2.normalize();
		}

		inline Vector2 Vector2::proj(const Vector2 &p, const Vector2 &q)
		{
			// Calculates the projection of 'p' onto 'q'.
			float length =  q.magnitude();
			return (Vector2::dot(p, q) / (length * length)) * q;
		}

		inline Vector2 Vector2::perp(const Vector2 &p, const Vector2 &q)
		{
			// Calculates the component of 'p' perpendicular to 'q'.
			float length = q.magnitude();
			return p - ((Vector2::dot(p, q) / (length * length)) * q);
		}

		inline Vector2 Vector2::reflect(const Vector2 &i, const Vector2 &n)
		{
			// Calculates reflection vector from entering ray direction 'i'
			// and surface normal 'n'.
			return i - 2.0f * Vector2::proj(i, n);
		}

		inline Vector2::Vector2(float x_, float y_) : x(x_), y(y_) {}

		inline bool Vector2::operator==(const Vector2 &rhs) const
		{
			return Math::closeEnough(x, rhs.x) && Math::closeEnough(y, rhs.y);
		}

		inline bool Vector2::operator!=(const Vector2 &rhs) const
		{
			return !(*this == rhs);
		}

		inline Vector2 &Vector2::operator+=(const Vector2 &rhs)
		{
			x += rhs.x, y += rhs.y;
			return *this;
		}

		inline Vector2 &Vector2::operator-=(const Vector2 &rhs)
		{
			x -= rhs.x, y -= rhs.y;
			return *this;
		}

		inline Vector2 &Vector2::operator*=(float scalar)
		{
			x *= scalar, y *= scalar;
			return *this;
		}

		inline Vector2 &Vector2::operator/=(float scalar)
		{
			x /= scalar, y /= scalar;
			return *this;
		}

		inline Vector2 Vector2::operator+(const Vector2 &rhs) const
		{
			Vector2 tmp(*this);
			tmp += rhs;
			return tmp;
		}

		inline Vector2 Vector2::operator-(const Vector2 &rhs) const
		{
			Vector2 tmp(*this);
			tmp -= rhs;
			return tmp;
		}

		inline Vector2 Vector2::operator*(float scalar) const
		{
			return Vector2(x * scalar, y * scalar);
		}

		inline Vector2 Vector2::operator/(float scalar) const
		{
			return Vector2(x / scalar, y / scalar);
		}

		inline float Vector2::magnitude() const
		{
			return sqrtf((x * x) + (y * y));
		}

		inline float Vector2::magnitudeSq() const
		{
			return (x * x) + (y * y);
		}

		inline Vector2 Vector2::inverse() const
		{
			return Vector2(-x, -y);
		}

		inline void Vector2::normalize()
		{
			float invMag = 1.0f / magnitude();
			x *= invMag, y *= invMag;
		}

		inline void Vector2::set(float x_, float y_)
		{
			x = x_, y = y_;
		}
	}
}

#endif

