#pragma once

#include "Vec4.h"
#include "Mat4.h"

namespace mml
{

	class Quat : public Vec4
	{
	public:

		Quat();
		// * w is the real part ..obviously..
		Quat(float x, float y, float z, float w);
		Quat(const Vec3& axis, const float& angle);

		Quat conjugate() const;

		Mat4 toRotationMatrix() const;

		Vec3 forward() const;

		friend Quat operator*(const Quat& left, const Quat& right);
		friend Quat operator*(const Quat& left, const Vec3& right);
		friend Quat operator*(const Quat& left, const float& right);
		friend Quat operator+(const Quat& left, const Quat& right);
		friend Quat operator-(const Quat& left, const Quat& right);

		friend bool operator==(const Quat& left, const Quat& right);
		friend bool operator!=(const Quat& left, const Quat& right);

	};
}
