#ifndef AURORA_MATHLIB_H
#define AURORA_MATHLIB_H

#include <cmath>
#include <cstdlib>
#include <pspmath.h>

namespace Aurora
{
	namespace Math
	{
		class Math
		{
		public:
			//static const float PI;
			static const float HALF_PI;
			static const float QUARTER_PI;
			static const float TWO_PI;
			static const float EPSILON;

			template <typename T>
			static T bilerp(const T &a, const T &b, const T &c, const T &d, float u, float v)
			{
				// Performs a bilinear interpolation.
				//  P(u,v) = e + v(f - e)
				//  
				//  where
				//  e = a + u(b - a)
				//  f = c + u(d - c)
				//  u in range [0,1]
				//  v in range [0,1]

				return a * ((1.0f - u) * (1.0f - v))
					+ b * (u * (1.0f - v))
					+ c * (v * (1.0f - u))
					+ d * (u * v);
			}

			static void cartesianToSpherical(float x, float y, float z,
				float &rho, float &phi, float &theta)
			{
				// All angles are in radians.
				// rho = distance from origin O to point P (i.e., length of OP)
				// phi = angle between OP and the XZ plane
				// theta = angle between X-axis and OP projected onto XZ plane

				rho = sqrtf((x * x) + (y * y) + (z * z));
				phi = asinf(y / rho);
				theta = atan2f(z, x);
			}

			static bool closeEnough(float f1, float f2)
			{
				// Determines whether the two floating-point values f1 and f2 are
				// close enough together that they can be considered equal.

				return fabsf((f1 - f2) / ((f2 == 0.0f) ? 1.0f : f2)) < EPSILON;
			}

			static float degreesToRadians(float degrees)
			{
				return (degrees * 3.1415926f) / 180.0f;
			}

			static bool isPower2(int x)
			{
				return ((x > 0) && ((x & (x - 1)) == 0));
			}

			template <typename T>
			static T lerp(const T &a, const T &b, float t)
			{
				// Performs a linear interpolation.
				//  P(t) = (1 - t)a + tb
				//       = a + t(b - a)
				//
				//  where
				//  t in range [0,1]

				return a + (b - a) * t;
			}

			static int nextMultipleOf(int multiple, int value)
			{
				// Returns the closest multiple of value that isn't less than value.

				return multiple * ((value + (multiple - 1)) / multiple);
			}

			static int nextPower2(int x);

			static float radiansToDegrees(float radians)
			{
				return (radians * 180.0f) / 3.1415926f;
			}

			static float random(float min, float max)
			{
				// Returns a random number in range [min,max].

				return min + (max - min)
					* (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
			}

			static float smoothstep(float a, float b, float x)
			{
				// Returns a gradual transition of 'x' from 0 to 1 beginning at
				// threshold 'a' and ending at threshold 'b'.
				//
				// References:
				//  [1] http://www.rendermanacademy.com/docs/smoothstep.htm
				//  [2] http://www.brlcad.org/doxygen/d8/d33/noise_8c-source.html
				//  [3] Natalya Tatarchuk, "Efficient High-Level Shader Development",
				//      Game Developers Conference Europe, August 2003.

				if (x < a)
				{
					return 0.0f;
				}
				else if (x >= b)
				{
					return 1.0f;
				}
				else
				{
					x = (x - a) / (b - a);
					return x * x * (3.0f - 2.0f * x);
				}
			}

			static void sphericalToCartesian(float rho, float phi, float theta,
				float &x, float &y, float &z)
			{
				// All angles are in radians.
				// rho = distance from origin O to point P (i.e., length of OP)
				// phi = angle between OP and the XZ plane
				// theta = angle between X-axis and OP projected onto XZ plane

				x = rho * cosf(phi) * cosf(theta);
				y = rho * sinf(phi);
				z = rho * cosf(phi) * sinf(theta);
			}
		};
	}
}


#endif
