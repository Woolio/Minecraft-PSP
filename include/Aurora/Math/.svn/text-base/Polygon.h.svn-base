#ifndef POLYGON_H_
#define POLYGON_H_

#include <math.h>
#include <Aurora/Math/Vector3.h>
#include <pspmath.h>

namespace Aurora
{
	namespace Math
	{
		class Polygon
		{

		public:
			Polygon();

			static float PlaneDistance(Vector3 Normal, Vector3 Point);
			static bool IntersectedPlane(Vector3 vPoly[], Vector3 vLine[], Vector3 &vNormal, float &originDistance);
			static float AngleBetweenVectors(Vector3 Vector1, Vector3 Vector2);
			static Vector3 IntersectionPoint(Vector3 vNormal, Vector3 vLine[], double distance);
			static bool InsidePolygon(Vector3 vIntersection, Vector3 Poly[], long verticeCount);
			static bool IntersectedPolygon(Vector3 vPoly[], Vector3 vLine[], int verticeCount,Vector3 &vIntersection,Vector3 &vNormal);
		};
	}
}

#endif /* POLYGON_H_ */

