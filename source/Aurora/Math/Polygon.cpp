#include <Aurora/Math/Polygon.h>

namespace Aurora
{
	namespace Math
	{

		Polygon::Polygon()
		{
			// TODO Auto-generated constructor stub

		}

		float Polygon::PlaneDistance(Vector3 Normal, Vector3 Point)
		{
			float distance = 0;									// This variable holds the distance from the plane tot he origin
										// Basically, the negated dot product of the normal of the plane and the point. (More about the dot product in another tutorial)
			distance = - ((Normal.x * Point.x) + (Normal.y * Point.y) + (Normal.z * Point.z));

			return distance;									// Return the distance
		}

		bool Polygon::IntersectedPlane(Vector3 vPoly[], Vector3 vLine[], Vector3 &vNormal, float &originDistance)
		{
			float distance1=0, distance2=0;						// The distances from the 2 points of the line from the plane

			vNormal = Vector3::Normal(vPoly);							// We need to get the normal of our plane to go any further

			// Let's find the distance our plane is from the origin.  We can find this value
			// from the normal to the plane (polygon) and any point that lies on that plane (Any vertice)
			originDistance = Polygon::PlaneDistance(vNormal, vPoly[0]);

			// Get the distance from point1 from the plane using: Ax + By + Cz + D = (The distance from the plane)

			distance1 = ((vNormal.x * vLine[0].x)  +					// Ax +
						 (vNormal.y * vLine[0].y)  +					// Bx +
						 (vNormal.z * vLine[0].z)) + originDistance;	// Cz + D

			// Get the distance from point2 from the plane using Ax + By + Cz + D = (The distance from the plane)

			distance2 = ((vNormal.x * vLine[1].x)  +					// Ax +
						 (vNormal.y * vLine[1].y)  +					// Bx +
						 (vNormal.z * vLine[1].z)) + originDistance;	// Cz + D

			// Now that we have 2 distances from the plane, if we times them together we either
			// get a positive or negative number.  If it's a negative number, that means we collided!
			// This is because the 2 points must be on either side of the plane (IE. -1 * 1 = -1).

			if(distance1 * distance2 >= 0)			// Check to see if both point's distances are both negative or both positive
			   return false;						// Return false if each point has the same sign.  -1 and 1 would mean each point is on either side of the plane.  -1 -2 or 3 4 wouldn't...

			return true;							// The line intersected the plane, Return TRUE
		}




		float Polygon::AngleBetweenVectors(Vector3 Vector1, Vector3 Vector2)
		{
			// Get the dot product of the vectors
				float dotProduct = Vector3::dot(Vector1, Vector2);

				// Get the product of both of the vectors magnitudes
				float vectorsMagnitude = Vector1.magnitude() * Vector2.magnitude();

				float angle = dotProduct / vectorsMagnitude;

				if(vectorsMagnitude != 0.0f && angle != 0.0f && angle >= -1.0f && angle< 1.0f)
					angle = vfpu_acosf( angle );

				if(angle > 1000.0f || angle < -1000.0f)
					return 0;

				// Return the angle in radians
				return( angle );
		}

		Vector3 Polygon::IntersectionPoint(Vector3 vNormal, Vector3 vLine[], double distance)
		{
			Vector3 vPoint , vLineDir;		// Variables to hold the point and the line's direction
			double Numerator = 0.0, Denominator = 0.0, dist = 0.0;

			vLineDir = Vector3::fromVectors(vLine[1], vLine[0]);		// Get the Vector of the line
			vLineDir.normalize();				// Normalize the lines vector


			Numerator = - (vNormal.x * vLine[0].x +		// Use the plane equation with the normal and the line
						   vNormal.y * vLine[0].y +
						   vNormal.z * vLine[0].z + distance);

			Denominator = Vector3::dot(vNormal, vLineDir);		// Get the dot product of the line's vector and the normal of the plane

			if( Denominator == 0.0)						// Check so we don't divide by zero
				return vLine[0];						// Return an arbitrary point on the line

			dist = Numerator / Denominator;				// Divide to get the multiplying (percentage) factor

			vPoint.x = (float)(vLine[0].x + (vLineDir.x * dist));
			vPoint.y = (float)(vLine[0].y + (vLineDir.y * dist));
			vPoint.z = (float)(vLine[0].z + (vLineDir.z * dist));

			return vPoint;								// Return the intersection point
		}

		bool Polygon::InsidePolygon(Vector3 vIntersection, Vector3 Poly[], long verticeCount)
		{
			//const double MATCH_FACTOR = 0.9999;		// Used to cover up the error in floating point
			float Angle = 0.0f;						// Initialize the angle
			Vector3 vA, vB;						// Create temp vectors

			for (int i = 0; i < verticeCount; i++)		// Go in a circle to each vertex and get the angle between
			{
				vA = Vector3::fromVectors(Poly[i], vIntersection);	// Subtract the intersection point from the current vertex
														// Subtract the point from the next vertex
				vB = Vector3::fromVectors(Poly[(i + 1) % verticeCount], vIntersection);

				Angle += Polygon::AngleBetweenVectors(vA, vB);	// Find the angle between the 2 vectors and add them all up as we go along
			}

			if(Angle >= 3.141592f )	// If the angle is greater than 2 PI, (360 degrees)
				return true;							// The point is inside of the polygon

			return false;								// If you get here, it obviously wasn't inside the polygon, so Return FALSE
		}

		bool Polygon::IntersectedPolygon(Vector3 vPoly[], Vector3 vLine[], int verticeCount,Vector3 &vIntersection,Vector3 &vNormal)
		{
			//Vector3 vNormal;
			float originDistance = 0;

			if(!Polygon::IntersectedPlane(vPoly, vLine,   vNormal,   originDistance))
				return false;

			vIntersection = Polygon::IntersectionPoint(vNormal, vLine, originDistance);

			if(Polygon::InsidePolygon(vIntersection, vPoly, verticeCount))
				return true;							// We collided!	  Return success


			// If we get here, we must have NOT collided

			return false;								// There was no collision, so return false
		}
	}
}
