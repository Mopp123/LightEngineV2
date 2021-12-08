
#include "Quat.h"
#include <cmath>

namespace mml
{

	Quat::Quat() :
		Vec4(0, 0, 0, 1)
	{}

	Quat::Quat(float x, float y, float z, float w) :
		Vec4(x, y, z, w)
	{}

	Quat::Quat(const Vec3& axis, const float& angle)
	{
		const float sinHalfAngle = sin(angle / 2.0f);
		x = axis.x * sinHalfAngle;
		y = axis.y * sinHalfAngle;
		z = axis.z * sinHalfAngle;
		w = cos(angle / 2.0f);
	}


	Quat Quat::conjugate() const
	{
		return Quat(-x, -y, -z, w);
	}


	// Used wikipedia "Quat-derived rotation matrix"
	// This can only be used for "unit quaternion"
	Mat4 Quat::toRotationMatrix() const
	{
		Quat q = *this;

		// We force the usage of unit quaternion here...
		q.normalize();

		float s = 1.0f;

		float sqx = q.x * q.x;
		float sqy = q.y * q.y;
		float sqz = q.z * q.z;

		Mat4 rotationMatrix;

		rotationMatrix[0] = 1.0f - 2 * s * (sqy + sqz);
		rotationMatrix[4] = 2 * s * (q.x * q.y - q.z * q.w);
		rotationMatrix[8] = 2 * s * (q.x * q.z + q.y * q.w);

		rotationMatrix[1] = 2 * s * (q.x * q.y + q.z * q.w);
		rotationMatrix[5] = 1.0f - 2 * s * (sqx + sqz);
		rotationMatrix[9] = 2 * s * (q.y * q.z - q.x * q.w);

		rotationMatrix[2] = 2 * s * (q.x * q.z - q.y * q.w);
		rotationMatrix[6] = 2 * s * (q.y * q.z + q.x * q.w);
		rotationMatrix[10] = 1.0f - 2 * s * (sqx + sqy);

		rotationMatrix[15] = 1.0f;

		return rotationMatrix;
	}

	// *NOTE Somethings fucky here, where i found these (https://www.gamedev.net/forums/topic/56471-extracting-direction-vectors-from-quaternion/)
	// In these my systems, up seems to be left and forward up, ..or something...
	mml::Vec3 Quat::forward() const
	{
		mml::Vec3 forw;

		forw.x = 2 * (x * z - w * y);
		forw.y = 2 * (y*z + w * x);
		forw.z = -(1 - 2 * (x * x + y * y));
		forw.normalize();

		return forw;
	}

	Quat operator*(const Quat& left, const Quat& right)
	{
		Quat q;
		q.x = left.x * right.w + left.w * right.x + left.y * right.z - left.z * right.y;
		q.y = left.y * right.w + left.w * right.y + left.z * right.x - left.x * right.z;
		q.z = left.z * right.w + left.w * right.z + left.x * right.y - left.y * right.x;
		q.w = left.w * right.w - left.x * right.x - left.y * right.y - left.z * right.z;
		return q;
	}

	Quat operator*(const Quat& left, const Vec3& right)
	{
		Quat q;
		q.x = left.w * right.x + left.y * right.z - left.z * right.y;
		q.y = left.w * right.y + left.z * right.x - left.x * right.z;
		q.z = left.w * right.z + left.x * right.y - left.y * right.x;
		q.w = -left.x * right.x - left.y * right.y - left.z * right.z;
		return q;
	}

	Quat operator*(const Quat& left, const float& right)
	{
		return Quat(left.x * right, left.y * right, left.z * right, left.w * right);
	}

	Quat operator+(const Quat& left, const Quat& right)
	{
		return Quat(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w);
	}

	Quat operator-(const Quat& left, const Quat& right)
	{
		return Quat(left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w);
	}


	bool operator==(const Quat& left, const Quat& right)
	{
		return (left.x == right.x && left.y == right.y && left.z == right.z && left.w == right.w);
	}
	bool operator!=(const Quat& left, const Quat& right)
	{
		return !(left == right);
	}
}
