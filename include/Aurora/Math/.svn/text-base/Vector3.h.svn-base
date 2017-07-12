#ifndef AURORA_VECTOR3_H
#define AURORA_VECTOR3_H

#include <Aurora/Math/MathLib.h>
#include <pspmath.h>

namespace Aurora
{
	namespace Math
	{
		class Vector3
		{
			friend Vector3 operator*(float lhs, const Vector3 &rhs);
			friend Vector3 operator-(const Vector3 &v);

		public:
			float x, y, z;

			static Vector3 cross(const Vector3 &p, const Vector3 &q);
			static float distance(const Vector3 &pt1, const Vector3 &pt2);
			static float distanceSq(const Vector3 &pt1, const Vector3 &pt2);  
			static float dot(const Vector3 &p, const Vector3 &q);
			static Vector3 lerp(const Vector3 &p, const Vector3 &q, float t);
			static void orthogonalize(Vector3 &v1, Vector3 &v2);
			static void orthogonalize(Vector3 &v1, Vector3 &v2, Vector3 &v3);
			static Vector3 proj(const Vector3 &p, const Vector3 &q);
			static Vector3 perp(const Vector3 &p, const Vector3 &q);
			static Vector3 reflect(const Vector3 &i, const Vector3 &n);
			static Vector3 normalized(Vector3 pt1);


			static Vector3 Normal(Vector3 vTriangle[]);
			static Vector3 cross3(Vector3 p, Vector3 q);
			static Vector3 clamp(Vector3 value1,Vector3 min,Vector3 max);
			static Vector3 fromVectors(Vector3 vPoint1, Vector3 vPoint2);

			Vector3() {x= y= z= 0.0f;}
			Vector3(float x_, float y_, float z_);

			bool operator==(const Vector3 &rhs) const;
			bool operator!=(const Vector3 &rhs) const;

			Vector3 &operator+=(const Vector3 &rhs);
			Vector3 &operator*=(const Vector3 &rhs);
			Vector3 &operator-=(const Vector3 &rhs);
			Vector3 &operator*=(float scalar);
			Vector3 &operator/=(float scalar);

			Vector3 operator+(const Vector3 &rhs) const;
			Vector3 operator-(const Vector3 &rhs) const;
			Vector3 operator*(const Vector3 &rhs) const;
			Vector3 operator*(float scalar) const;
			Vector3 operator/(float scalar) const;

			float magnitude() const;
			float magnitudeSq() const;
			Vector3 inverse() const;
			Vector3 rotatedXZ(float angle);
			Vector3 rotatedZY(float angle);
			Vector3 rotatedXY(float angle);
			void normalize();
			void set(float x_, float y_, float z_);

			void saturate(void)
			    {
			        if (x < 0)
			            x = 0;
			        else if (x > 1)
			            x = 1;

			        if (y < 0)
			            y = 0;
			        else if (y > 1)
			            y = 1;

			        if (z < 0)
			            z = 0;
			        else if (z > 1)
			            z = 1;
				}
		};

		inline Vector3 Vector3::Normal(Vector3 vTriangle[])
		{														// Get 2 vectors from the polygon (2 sides), Remember the order!
			Vector3 vVector1 = vTriangle[2]- vTriangle[0];
			Vector3 vVector2 = vTriangle[1]- vTriangle[0];

			Vector3 vNormal = Vector3::cross3(vVector1, vVector2);		// Take the cross product of our 2 vectors to get a perpendicular vector

			vNormal.normalize();								// Use our function we created to normalize the normal (Makes it a length of one)

			return vNormal;										// Return our normal at our desired length
		}

		inline Vector3 Vector3::cross3(Vector3 p, Vector3 q)
		{
		    return Vector3((p.y * q.z) - (p.z * q.y),
		        (p.z * q.x) - (p.x * q.z),
		        (p.x * q.y) - (p.y * q.x));
		}

		inline Vector3 Vector3::clamp(Vector3 value1,Vector3 min,Vector3 max)
		{
			Vector3 result;

			float x = value1.x;
			x = (x > max.x) ? max.x : x;
			x = (x < min.x) ? min.x : x;
			float y = value1.y;
			y = (y > max.y) ? max.y : y;
			y = (y < min.y) ? min.y : y;
			float z = value1.z;
			z = (z > max.z) ? max.z : z;
			z = (z < min.z) ? min.z : z;
			result.x = x;
			result.y = y;
			result.z = z;

			return result;
		}

		inline Vector3 Vector3::fromVectors(Vector3 vPoint1, Vector3 vPoint2)
		{
			Vector3 vVector;								// Initialize our variable to zero

			vVector.x = vPoint1.x - vPoint2.x;					// Get the X value of our new vector
			vVector.y = vPoint1.y - vPoint2.y;					// Get the Y value of our new vector
			vVector.z = vPoint1.z - vPoint2.z;					// Get the Z value of our new vector

			return vVector;										// Return our new vector
		}

		inline Vector3 operator*(float lhs, const Vector3 &rhs)
		{
			return Vector3(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z);
		}

		inline Vector3 operator-(const Vector3 &v)
		{
			return Vector3(-v.x, -v.y, -v.z);
		}

		inline Vector3 Vector3::cross(const Vector3 &p, const Vector3 &q)
		{
			return Vector3((p.y * q.z) - (p.z * q.y),
				(p.z * q.x) - (p.x * q.z),
				(p.x * q.y) - (p.y * q.x));
		}

		inline float Vector3::distance(const Vector3 &pt1, const Vector3 &pt2)
		{
			// Calculates the distance between 2 points.
			float f = distanceSq(pt1, pt2);
			float result;
			__asm__ volatile (
				".set			push\n"					// save assembler option
				".set			noreorder\n"			// suppress reordering
				"mtv     %1, S000\n"
				"vsqrt.s S000, S000\n"
				"mfv     %0, S000\n"
				".set			pop\n"					// restore assembler option
			: "=r"(result) : "r"(f));
			return result;
		}

		inline float Vector3::distanceSq(const Vector3 &pt1, const Vector3 &pt2)
		{
			// Calculates the squared distance between 2 points.
			return ((pt1.x - pt2.x) * (pt1.x - pt2.x))+ ((pt1.y - pt2.y) * (pt1.y - pt2.y))	+ ((pt1.z - pt2.z) * (pt1.z - pt2.z));
		}

		inline float Vector3::dot(const Vector3 &p, const Vector3 &q)
		{
			//return (p.x * q.x) + (p.y * q.y) + (p.z * q.z);
			float v;

			__asm__ (
				//".set			push\n"					// save assembler option
				//".set			noreorder\n"			// suppress reordering
				"lv.s			s000, 0 + %1\n"			// s000 = pv1->x
				"lv.s			s001, 4 + %1\n"			// s001 = pv1->y
				"lv.s			s002, 8 + %1\n"			// s002 = pv1->z
				"lv.s			s010, 0 + %2\n"			// s010 = pv2->x
				"lv.s			s011, 4 + %2\n"			// s011 = pv2->y
				"lv.s			s012, 8 + %2\n"			// s012 = pv2->z
				"vdot.t			s000, c000, c010\n"		// s000 = c000 dot c010
				"sv.s			s000, %0\n"				// v    = s000
				//".set			pop\n"					// restore assembler option
				: "=m"(v)
				: "m"(p), "m"(q)
			);

			return (v);
		}

		inline Vector3 Vector3::lerp(const Vector3 &p, const Vector3 &q, float t)
		{
			// Linearly interpolates from 'p' to 'q' as t varies from 0 to 1.
			return p + t * (q - p);
		}

		inline void Vector3::orthogonalize(Vector3 &v1, Vector3 &v2)
		{
			// Performs Gram-Schmidt Orthogonalization on the 2 basis vectors to
			// turn them into orthonormal basis vectors.
			v2 = v2 - proj(v2, v1);
			v2.normalize();
		}

		inline void Vector3::orthogonalize(Vector3 &v1, Vector3 &v2, Vector3 &v3)
		{
			// Performs Gram-Schmidt Orthogonalization on the 3 basis vectors to
			// turn them into orthonormal basis vectors.

			v2 = v2 - proj(v2, v1);
			v2.normalize();

			v3 = v3 - proj(v3, v1) - proj(v3, v2);
			v3.normalize();
		}

		inline Vector3 Vector3::proj(const Vector3 &p, const Vector3 &q)
		{
			// Calculates the projection of 'p' onto 'q'.
			float length =  q.magnitude();
			return (Vector3::dot(p, q) / (length * length)) * q;
		}

		inline Vector3 Vector3::perp(const Vector3 &p, const Vector3 &q)
		{
			// Calculates the component of 'p' perpendicular to 'q'.
			float length = q.magnitude();
			return p - ((Vector3::dot(p, q) / (length * length)) * q);
		}

		inline Vector3 Vector3::reflect(const Vector3 &i, const Vector3 &n)
		{
			// Calculates reflection vector from entering ray direction 'i'
			// and surface normal 'n'.
			return i - 2.0f * Vector3::proj(i, n);
		}

		inline Vector3 Vector3::normalized(Vector3 pt1)
		{
			Vector3 temp = pt1;
			temp.normalize();
			return temp;
		}

		inline Vector3 Vector3::rotatedXZ(float angle)
		{
			Vector3 temp;

			temp.x = (x * cosf(angle)) - (z * sinf(angle));
			temp.y = y;
			temp.z = (z * cosf(angle)) + (x * sinf(angle));

			return temp;
		}

		inline Vector3 Vector3::rotatedZY(float angle)
		{
			Vector3 temp;

			temp.x = x;
			temp.y = (y * cosf(angle)) + (z * sinf(angle));
			temp.z = (z * cosf(angle)) - (y * sinf(angle));

			return temp;
		}

		inline Vector3 Vector3::rotatedXY(float angle)
		{
			Vector3 temp;

			temp.x = (x * cosf(angle)) - (y * sinf(angle));
			temp.y = (y * cosf(angle)) + (x * sinf(angle));;
			temp.z = z;

			return temp;
		}


		inline Vector3::Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

		inline Vector3 &Vector3::operator+=(const Vector3 &rhs)
		{
			x += rhs.x, y += rhs.y, z += rhs.z;
			return *this;
		}

		inline Vector3 &Vector3::operator*=(const Vector3 &rhs)
		{
			x *= rhs.x, y *= rhs.y, z *= rhs.z;
			return *this;
		}

		inline bool Vector3::operator==(const Vector3 &rhs) const
		{
			return Math::closeEnough(x, rhs.x) && Math::closeEnough(y, rhs.y) && Math::closeEnough(z, rhs.z);
		}

		inline bool Vector3::operator!=(const Vector3 &rhs) const
		{
			return !(*this == rhs);
		}

		inline Vector3 &Vector3::operator-=(const Vector3 &rhs)
		{
			x -= rhs.x, y -= rhs.y, z -= rhs.z;
			return *this;
		}

		inline Vector3 &Vector3::operator*=(float scalar)
		{
			x *= scalar, y *= scalar, z *= scalar;
			return *this;
		}

		inline Vector3 &Vector3::operator/=(float scalar)
		{
			x /= scalar, y /= scalar, z /= scalar;
			return *this;
		}

		inline Vector3 Vector3::operator+(const Vector3 &rhs) const
		{
			Vector3 tmp(*this);
			tmp += rhs;
			return tmp;
		}

		inline Vector3 Vector3::operator*(const Vector3 &rhs) const
		{
			Vector3 tmp(*this);
			tmp *= rhs;
			return tmp;
		}

		inline Vector3 Vector3::operator-(const Vector3 &rhs) const
		{
			Vector3 tmp(*this);
			tmp -= rhs;
			return tmp;
		}

		inline Vector3 Vector3::operator*(float scalar) const
		{
			return Vector3(x * scalar, y * scalar, z * scalar);    
		}

		inline Vector3 Vector3::operator/(float scalar) const
		{
			return Vector3(x / scalar, y / scalar, z / scalar);
		}

		inline float Vector3::magnitude() const
		{
			//return sqrtf((x * x) + (y * y) + (z * z));
			float f = (x * x) + (y * y) + (z * z);
			/*__asm__ (
				".set			push\n"					// save assembler option
				".set			noreorder\n"			// suppress reordering
				"lv.s			s000, 0 + %1\n"			// s000 = this->x
				"lv.s			s001, 4 + %1\n"			// s001 = this->y
				"lv.s			s002, 8 + %1\n"			// s002 = this->z
				"vdot.t			s000, c000, c000\n"		// s000 = s000*s000 + s001*s001 + s002*s002
				"vsqrt.s		s000, s000\n"			// s000 = sqrt(s000)
				"sv.s			s000, %0\n"				// f    = s000
				".set			pop\n"					// restore assembler option
				: "=m"(f)
				: "m"(*this)
			);*/
			float result;
			__asm__ volatile (
				".set			push\n"					// save assembler option
				".set			noreorder\n"			// suppress reordering
				"mtv     %1, S000\n"
				"vsqrt.s S000, S000\n"
				"mfv     %0, S000\n"
				".set			pop\n"					// restore assembler option
			: "=r"(result) : "r"(f));
			return result;
		}

		inline float Vector3::magnitudeSq() const
		{
			return (x * x) + (y * y) + (z * z);
		}

		inline Vector3 Vector3::inverse() const
		{
			return Vector3(-x, -y, -z);
		}

		inline void Vector3::normalize()
		{
			float m = magnitude();
			if(m != 0.0f)
			{
				float invMag = 1.0f / m;
				x *= invMag, y *= invMag, z *= invMag;
			}
		}

		inline void Vector3::set(float x_, float y_, float z_)
		{
			x = x_, y = y_, z = z_;
		}
	}
}

#endif
