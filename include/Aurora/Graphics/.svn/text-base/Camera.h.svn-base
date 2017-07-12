#ifndef AURORA_CAMERA_H
#define AURORA_CAMERA_H

#include <Aurora/Math/Vector3.h>
#include <Aurora/Math/Frustum.h>
#include <pspmath.h>

using namespace Aurora::Math;

namespace Aurora
{
	namespace Graphics
	{
		class Camera
		{
		public:
			Camera();
			virtual ~Camera();

			void PositionCamera(float positionX, float positionY, float positionZ,
						 		    float viewX,     float viewY,     float viewZ,
									float upVectorX, float upVectorY, float upVectorZ);

			void Move(float speed);
			void MoveTo(Vector3 newPos);
			void MovePhysic(float speed);
			Vector3 MoveFoCollision(float speed);
			void MoveAfterCollision(Vector3 newPos);

			void Strafe(float speed);
			void StrafePhysic(float speed);

			void MovePhysicNoY(float speed);
			//Vector3 StrafeFoCollision(float speed);
			//void StrafeAfterCollision(Vector3 newPos);

			void RotateView(float angle, float x, float y, float z);
			void PitchView(float speed);


			Frustum mFrustum;
			bool needUpdate;


		//private:

			// The camera's position
			Vector3 m_vPosition;
			
			// The camera's offset
			Vector3 m_vOffset;

			// The camera's view
			Vector3 m_vView;

			// The camera's up vector
			Vector3 m_vUpVector;

			// The camera's strafe vector
			Vector3 m_vStrafe;

			//TODO wtf is this for??
			Vector3 m_vVelocity;

			Vector3 vVector;

			float upDownAngle;
		};
	}
}

#endif
