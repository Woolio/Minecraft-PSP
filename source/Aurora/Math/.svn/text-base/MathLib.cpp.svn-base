#include <Aurora/Math/MathLib.h>
#include <pspmath.h>

namespace Aurora
{
	namespace Math
	{
		//const float Math::PI = 3.1415926f;
		const float Math::HALF_PI = 3.1415926f / 2.0f;
		const float Math::QUARTER_PI = 3.1415926f / 4.0f;
		const float Math::TWO_PI = 3.1415926f * 2.0f;
		const float Math::EPSILON = 1e-6f;

		int Math::nextPower2(int x)
		{
			int i = x & (~x + 1);

			while (i < x)
				i <<= 1;	

			return i;
		}
	}
}
