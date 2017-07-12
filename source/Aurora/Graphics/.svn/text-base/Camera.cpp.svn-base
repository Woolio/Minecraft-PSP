#include <Aurora/Graphics/Camera.h>

namespace Aurora
{
	namespace Graphics
	{
		Camera::Camera()
		{
			//needUpdate = true;
			upDownAngle = 0.0f;
			m_vOffset = Vector3(0.0f, 0.0f, 0.0f);
		}

		Camera::~Camera()
		{

		}

		void Camera::PositionCamera(float positionX, float positionY, float positionZ,
								 		    float viewX,     float viewY,     float viewZ,
											float upVectorX, float upVectorY, float upVectorZ)
		{
			m_vPosition	= Vector3(positionX, positionY, positionZ);
			m_vView		= Vector3(viewX, viewY, viewZ);
			m_vUpVector	= Vector3(upVectorX, upVectorY, upVectorZ);
			vVector = m_vView - (m_vPosition + m_vOffset);
			vVector.normalize();
			needUpdate = true;
		}

		void Camera::Move(float speed)
		{
			vVector = m_vView - (m_vPosition + m_vOffset);
			vVector.normalize();

			m_vVelocity = vVector * speed;
			m_vPosition = m_vPosition + m_vVelocity;
			m_vView = m_vPosition + m_vOffset + vVector;

			needUpdate = true;
		}

		void Camera::MoveTo(Vector3 newPos)
		{
			vVector = m_vView - (m_vPosition + m_vOffset);
			vVector.normalize();

			m_vPosition = newPos;
			m_vView = m_vPosition + m_vOffset + vVector;

			needUpdate = true;
		}

		void Camera::MovePhysic(float speed)
		{
			vVector = m_vView - (m_vPosition + m_vOffset);
			vVector.normalize();

			m_vVelocity += vVector * speed;
			needUpdate = true;
		}

		void Camera::MovePhysicNoY(float speed)
		{
			vVector = m_vView - (m_vPosition + m_vOffset);
			vVector.y = 0.0f;
			vVector.normalize();

			m_vVelocity += vVector * speed;
			needUpdate = true;
		}

		Vector3 Camera::MoveFoCollision(float speed)
		{
			vVector = m_vView - (m_vPosition + m_vOffset);
			vVector.normalize();

			m_vVelocity = vVector * speed;

			Vector3 newPos = m_vPosition + m_vVelocity;

			return newPos;
		}

		void Camera::MoveAfterCollision(Vector3 newPos)
		{
			m_vPosition = newPos + m_vVelocity;
			m_vView = m_vPosition + m_vOffset + vVector;
			needUpdate = true;
		}

		void Camera::Strafe(float speed)
		{
			m_vStrafe = Vector3::cross(m_vView - m_vPosition, m_vUpVector);

			// Normalize the strafe vector
			m_vStrafe.normalize();

			// Add the strafe vector to our position
			m_vVelocity = m_vStrafe * speed;
			m_vPosition.x += m_vVelocity.x;
			m_vPosition.z += m_vVelocity.z;

			// Add the strafe vector to our view
			m_vView.x += m_vVelocity.x;
			m_vView.z += m_vVelocity.z;

			needUpdate = true;
		}

		void Camera::StrafePhysic(float speed)
		{
			m_vStrafe = Vector3::cross(m_vView - m_vPosition, m_vUpVector);

			m_vStrafe.y = 0.0f;

			// Normalize the strafe vector
			m_vStrafe.normalize();
			m_vVelocity += m_vStrafe * speed;
			needUpdate = true;
		}

		/*Vector3 Camera::StrafeFoCollision(float speed)
		{
			m_vStrafe = Vector3::cross(m_vView - m_vPosition, m_vUpVector);

			// Normalize the strafe vector
			m_vStrafe.normalize();

			// Add the strafe vector to our position
			m_vVelocity = m_vStrafe * speed;

			Vector3 newPos;

			m_vPosition.x += m_vVelocity.x;
			m_vPosition.z += m_vVelocity.z;
		}

		void Camera::StrafeAfterCollision(Vector3 newPos)
		{

		}*/

		void Camera::RotateView(float angle, float x, float y, float z)
		{
			if(y == 0.0f)//this is fix for rotating upside down camera
			{
				upDownAngle += angle;

				if(upDownAngle < -1.561f)
				{
					upDownAngle = -1.561f;
					return;
				}

				if(upDownAngle > 1.561f)
				{
					upDownAngle = 1.561f;
					return;
				}
			}

			Vector3 vNewView;

			// Get the view vector (The direction we are facing)
			Vector3 vView = m_vView - (m_vPosition + m_vOffset);

			// Calculate the sine and cosine of the angle once
			float cosTheta = vfpu_cosf(angle);
			float sinTheta = vfpu_sinf(angle);

			// Find the new x position for the new rotated point
			vNewView.x  = (cosTheta + (1 - cosTheta) * x * x)		* vView.x;
			vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* vView.y;
			vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* vView.z;

			// Find the new y position for the new rotated point
			vNewView.y  = ((1 - cosTheta) * x * y + z * sinTheta)	* vView.x;
			vNewView.y += (cosTheta + (1 - cosTheta) * y * y)		* vView.y;
			vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* vView.z;

			// Find the new z position for the new rotated point
			vNewView.z  = ((1 - cosTheta) * x * z - y * sinTheta)	* vView.x;
			vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* vView.y;
			vNewView.z += (cosTheta + (1 - cosTheta) * z * z)		* vView.z;

			// Now we just add the newly rotated vector to our position to set
			// our new rotated view of our camera.
			m_vView = m_vPosition + m_vOffset + vNewView;

			vVector = vNewView;
			needUpdate = true;
		}

		void Camera::PitchView(float speed)
		{
			Vector3 vAxis = Vector3::cross(m_vView - m_vPosition, m_vUpVector);
			vAxis.normalize();

			RotateView(speed, vAxis.x, vAxis.y, vAxis.z);
		}
	}
}

