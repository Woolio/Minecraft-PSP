#ifndef AURORA_MATRIX3_H
#define AURORA_MATRIX3_H

#include <Aurora/Math/MathLib.h>
#include <pspmath.h>

namespace Aurora
{
	namespace Math
	{

		class Matrix3
		{
			friend Vector3 operator*(const Vector3 &lhs, const Matrix3 &rhs);
			friend Matrix3 operator*(float scalar, const Matrix3 &rhs);

		public:
			static const Matrix3 IDENTITY;
			static Matrix3 createFromAxes(const Vector3 &x, const Vector3 &y, const Vector3 &z);
			static Matrix3 createFromAxesTransposed(const Vector3 &x, const Vector3 &y, const Vector3 &z);
			static Matrix3 createFromHeadPitchRoll(float headDegrees, float pitchDegrees, float rollDegrees);
			static Matrix3 createMirror(const Vector3 &planeNormal);
			static Matrix3 createOrient(const Vector3 &from, const Vector3 &to);
			static Matrix3 createRotate(const Vector3 &axis, float degrees);
			static Matrix3 createScale(float sx, float sy, float sz);

			Matrix3() {}
			Matrix3(float m11, float m12, float m13,
				float m21, float m22, float m23,
				float m31, float m32, float m33);
			~Matrix3() {}

			float *operator[](int row);
			const float *operator[](int row) const;

			bool operator==(const Matrix3 &rhs) const;
			bool operator!=(const Matrix3 &rhs) const;

			Matrix3 &operator+=(const Matrix3 &rhs);
			Matrix3 &operator-=(const Matrix3 &rhs);
			Matrix3 &operator*=(const Matrix3 &rhs);
			Matrix3 &operator*=(float scalar);
			Matrix3 &operator/=(float scalar);

			Matrix3 operator+(const Matrix3 &rhs) const;
			Matrix3 operator-(const Matrix3 &rhs) const;
			Matrix3 operator*(const Matrix3 &rhs) const;
			Matrix3 operator*(float scalar) const;
			Matrix3 operator/(float scalar) const;

			float determinant() const;
			void fromAxes(const Vector3 &x, const Vector3 &y, const Vector3 &z);
			void fromAxesTransposed(const Vector3 &x, const Vector3 &y, const Vector3 &z);
			void fromHeadPitchRoll(float headDegrees, float pitchDegrees, float rollDegrees);
			void identity();
			Matrix3 inverse() const;
			void orient(const Vector3 &from, const Vector3 &to);
			void rotate(const Vector3 &axis, float degrees);
			void scale(float sx, float sy, float sz);
			void toAxes(Vector3 &x, Vector3 &y, Vector3 &z) const;
			void toAxesTransposed(Vector3 &x, Vector3 &y, Vector3 &z) const;
			void toHeadPitchRoll(float &headDegrees, float &pitchDegrees, float &rollDegrees) const;
			Matrix3 transpose() const;

		private:
			float mtx[3][3];
		};

		inline Vector3 operator*(const Vector3 &lhs, const Matrix3 &rhs)
		{
			return Vector3(
				(lhs.x * rhs.mtx[0][0]) + (lhs.y * rhs.mtx[1][0]) + (lhs.z * rhs.mtx[2][0]),
				(lhs.x * rhs.mtx[0][1]) + (lhs.y * rhs.mtx[1][1]) + (lhs.z * rhs.mtx[2][1]),
				(lhs.x * rhs.mtx[0][2]) + (lhs.y * rhs.mtx[1][2]) + (lhs.z * rhs.mtx[2][2]));
		}

		inline Matrix3 operator*(float scalar, const Matrix3 &rhs)
		{
			return rhs * scalar;
		}

		inline Matrix3 Matrix3::createFromAxes(const Vector3 &x, const Vector3 &y, const Vector3 &z)
		{
			Matrix3 tmp;
			tmp.createFromAxes(x, y, z);
			return tmp;
		}

		inline Matrix3 Matrix3::createFromAxesTransposed(const Vector3 &x, const Vector3 &y, const Vector3 &z)
		{
			Matrix3 tmp;
			tmp.fromAxesTransposed(x, y, z);
			return tmp;
		}

		inline Matrix3 Matrix3::createFromHeadPitchRoll(float headDegrees, float pitchDegrees, float rollDegrees)
		{
			Matrix3 tmp;
			tmp.fromHeadPitchRoll(headDegrees, pitchDegrees, rollDegrees);
			return tmp;
		}

		inline Matrix3 Matrix3::createOrient(const Vector3 &from, const Vector3 &to)
		{
			Matrix3 tmp;
			tmp.orient(from, to);
			return tmp;
		}

		inline Matrix3 Matrix3::createRotate(const Vector3 &axis, float degrees)
		{
			Matrix3 tmp;
			tmp.rotate(axis, degrees);
			return tmp;
		}

		inline Matrix3 Matrix3::createScale(float sx, float sy, float sz)
		{
			Matrix3 tmp;
			tmp.scale(sx, sy, sz);
			return tmp;
		}

		inline Matrix3::Matrix3(float m11, float m12, float m13,
			float m21, float m22, float m23,
			float m31, float m32, float m33)
		{
			mtx[0][0] = m11, mtx[0][1] = m12, mtx[0][2] = m13;
			mtx[1][0] = m21, mtx[1][1] = m22, mtx[1][2] = m23;
			mtx[2][0] = m31, mtx[2][1] = m32, mtx[2][2] = m33;
		}

		inline float *Matrix3::operator[](int row)
		{
			return mtx[row];
		}

		inline const float *Matrix3::operator[](int row) const
		{
			return mtx[row];
		}

		inline bool Matrix3::operator==(const Matrix3 &rhs) const
		{
			return Math::closeEnough(mtx[0][0], rhs.mtx[0][0])
				&& Math::closeEnough(mtx[0][1], rhs.mtx[0][1])
				&& Math::closeEnough(mtx[0][2], rhs.mtx[0][2])
				&& Math::closeEnough(mtx[1][0], rhs.mtx[1][0])
				&& Math::closeEnough(mtx[1][1], rhs.mtx[1][1])
				&& Math::closeEnough(mtx[1][2], rhs.mtx[1][2])
				&& Math::closeEnough(mtx[2][0], rhs.mtx[2][0])
				&& Math::closeEnough(mtx[2][1], rhs.mtx[2][1])
				&& Math::closeEnough(mtx[2][2], rhs.mtx[2][2]);
		}

		inline bool Matrix3::operator!=(const Matrix3 &rhs) const
		{
			return !(*this == rhs);
		}

		inline Matrix3 &Matrix3::operator+=(const Matrix3 &rhs)
		{
			mtx[0][0] += rhs.mtx[0][0], mtx[0][1] += rhs.mtx[0][1], mtx[0][2] += rhs.mtx[0][2];
			mtx[1][0] += rhs.mtx[1][0], mtx[1][1] += rhs.mtx[1][1], mtx[1][2] += rhs.mtx[1][2];
			mtx[2][0] += rhs.mtx[2][0], mtx[2][1] += rhs.mtx[2][1], mtx[2][2] += rhs.mtx[2][2];
			return *this;
		}

		inline Matrix3 &Matrix3::operator-=(const Matrix3 &rhs)
		{
			mtx[0][0] -= rhs.mtx[0][0], mtx[0][1] -= rhs.mtx[0][1], mtx[0][2] -= rhs.mtx[0][2];
			mtx[1][0] -= rhs.mtx[1][0], mtx[1][1] -= rhs.mtx[1][1], mtx[1][2] -= rhs.mtx[1][2];
			mtx[2][0] -= rhs.mtx[2][0], mtx[2][1] -= rhs.mtx[2][1], mtx[2][2] -= rhs.mtx[2][2];
			return *this;
		}

		inline Matrix3 &Matrix3::operator*=(const Matrix3 &rhs)
		{
			Matrix3 tmp;

			// Row 1.
			tmp.mtx[0][0] = (mtx[0][0] * rhs.mtx[0][0]) + (mtx[0][1] * rhs.mtx[1][0]) + (mtx[0][2] * rhs.mtx[2][0]);
			tmp.mtx[0][1] = (mtx[0][0] * rhs.mtx[0][1]) + (mtx[0][1] * rhs.mtx[1][1]) + (mtx[0][2] * rhs.mtx[2][1]);
			tmp.mtx[0][2] = (mtx[0][0] * rhs.mtx[0][2]) + (mtx[0][1] * rhs.mtx[1][2]) + (mtx[0][2] * rhs.mtx[2][2]);

			// Row 2.
			tmp.mtx[1][0] = (mtx[1][0] * rhs.mtx[0][0]) + (mtx[1][1] * rhs.mtx[1][0]) + (mtx[1][2] * rhs.mtx[2][0]);
			tmp.mtx[1][1] = (mtx[1][0] * rhs.mtx[0][1]) + (mtx[1][1] * rhs.mtx[1][1]) + (mtx[1][2] * rhs.mtx[2][1]);
			tmp.mtx[1][2] = (mtx[1][0] * rhs.mtx[0][2]) + (mtx[1][1] * rhs.mtx[1][2]) + (mtx[1][2] * rhs.mtx[2][2]);

			// Row 3.
			tmp.mtx[2][0] = (mtx[2][0] * rhs.mtx[0][0]) + (mtx[2][1] * rhs.mtx[1][0]) + (mtx[2][2] * rhs.mtx[2][0]);
			tmp.mtx[2][1] = (mtx[2][0] * rhs.mtx[0][1]) + (mtx[2][1] * rhs.mtx[1][1]) + (mtx[2][2] * rhs.mtx[2][1]);
			tmp.mtx[2][2] = (mtx[2][0] * rhs.mtx[0][2]) + (mtx[2][1] * rhs.mtx[1][2]) + (mtx[2][2] * rhs.mtx[2][2]);

			*this = tmp;
			return *this;
		}

		inline Matrix3 &Matrix3::operator*=(float scalar)
		{
			mtx[0][0] *= scalar, mtx[0][1] *= scalar, mtx[0][2] *= scalar;
			mtx[1][0] *= scalar, mtx[1][1] *= scalar, mtx[1][2] *= scalar;
			mtx[2][0] *= scalar, mtx[2][1] *= scalar, mtx[2][2] *= scalar;
			return *this;
		}

		inline Matrix3 &Matrix3::operator/=(float scalar)
		{
			mtx[0][0] /= scalar, mtx[0][1] /= scalar, mtx[0][2] /= scalar;
			mtx[1][0] /= scalar, mtx[1][1] /= scalar, mtx[1][2] /= scalar;
			mtx[2][0] /= scalar, mtx[2][1] /= scalar, mtx[2][2] /= scalar;
			return *this;
		}

		inline Matrix3 Matrix3::operator+(const Matrix3 &rhs) const
		{
			Matrix3 tmp(*this);
			tmp += rhs;
			return tmp;
		}

		inline Matrix3 Matrix3::operator-(const Matrix3 &rhs) const
		{
			Matrix3 tmp(*this);
			tmp -= rhs;
			return tmp;
		}

		inline Matrix3 Matrix3::operator*(const Matrix3 &rhs) const
		{
			Matrix3 tmp(*this);
			tmp *= rhs;
			return tmp;
		}

		inline Matrix3 Matrix3::operator*(float scalar) const
		{
			Matrix3 tmp(*this);
			tmp *= scalar;
			return tmp;
		}

		inline Matrix3 Matrix3::operator/(float scalar) const
		{
			Matrix3 tmp(*this);
			tmp /= scalar;
			return tmp;
		}

		inline float Matrix3::determinant() const
		{
			return (mtx[0][0] * (mtx[1][1] * mtx[2][2] - mtx[1][2] * mtx[2][1]))
				- (mtx[0][1] * (mtx[1][0] * mtx[2][2] - mtx[1][2] * mtx[2][0]))
				+ (mtx[0][2] * (mtx[1][0] * mtx[2][1] - mtx[1][1] * mtx[2][0]));
		}

		inline void Matrix3::fromAxes(const Vector3 &x, const Vector3 &y, const Vector3 &z)
		{
			mtx[0][0] = x.x,  mtx[0][1] = x.y,  mtx[0][2] = x.z;
			mtx[1][0] = y.x,  mtx[1][1] = y.y,  mtx[1][2] = y.z;
			mtx[2][0] = z.x,  mtx[2][1] = z.y,  mtx[2][2] = z.z;
		}

		inline void Matrix3::fromAxesTransposed(const Vector3 &x, const Vector3 &y, const Vector3 &z)
		{
			mtx[0][0] = x.x,  mtx[0][1] = y.x,  mtx[0][2] = z.x;
			mtx[1][0] = x.y,  mtx[1][1] = y.y,  mtx[1][2] = z.y;
			mtx[2][0] = x.z,  mtx[2][1] = y.z,  mtx[2][2] = z.z;
		}

		inline void Matrix3::identity()
		{
			mtx[0][0] = 1.0f, mtx[0][1] = 0.0f, mtx[0][2] = 0.0f;
			mtx[1][0] = 0.0f, mtx[1][1] = 1.0f, mtx[1][2] = 0.0f;
			mtx[2][0] = 0.0f, mtx[2][1] = 0.0f, mtx[2][2] = 1.0f;
		}

		inline void Matrix3::toAxes(Vector3 &x, Vector3 &y, Vector3 &z) const
		{
			x.set(mtx[0][0], mtx[0][1], mtx[0][2]);
			y.set(mtx[1][0], mtx[1][1], mtx[1][2]);
			z.set(mtx[2][0], mtx[2][1], mtx[2][2]);
		}

		inline void Matrix3::toAxesTransposed(Vector3 &x, Vector3 &y, Vector3 &z) const
		{
			x.set(mtx[0][0], mtx[1][0], mtx[2][0]);
			y.set(mtx[0][1], mtx[1][1], mtx[2][1]);
			z.set(mtx[0][2], mtx[1][2], mtx[2][2]);
		}

		inline Matrix3 Matrix3::transpose() const
		{
			Matrix3 tmp;

			tmp[0][0] = mtx[0][0], tmp[0][1] = mtx[1][0], tmp[0][2] = mtx[2][0];
			tmp[1][0] = mtx[0][1], tmp[1][1] = mtx[1][1], tmp[1][2] = mtx[2][1];
			tmp[2][0] = mtx[0][2], tmp[2][1] = mtx[1][2], tmp[2][2] = mtx[2][2];

			return tmp;
		}

}
}

#endif