#pragma once

#include "Vec2.h"

namespace mml
{

	class Vec3 : public Vec2
	{
	public:
		float z = 0.0f;
		
		Vec3();
		Vec3(float X, float Y, float Z);
		Vec3(const Vec3& vec);

		virtual void normalize() override;
		float magnitude() const;
		float dot(const Vec3& vec) const;
		Vec3 cross(const Vec3& vec) const;

		friend Vec3 operator+(const Vec3& left, const Vec3& right);
		friend Vec3 operator-(const Vec3& left, const Vec3& right);
		friend Vec3 operator*(const Vec3& left, const Vec3& right);
		friend Vec3 operator/(const Vec3& left, const Vec3& right);

		friend bool operator==(const Vec3& left, const Vec3& right);
		friend bool operator!=(const Vec3& left, const Vec3& right);

		friend Vec3 operator*(const Vec3& left, const float& right);
		friend Vec3 operator/(const Vec3& left, const float& right);
	};
}