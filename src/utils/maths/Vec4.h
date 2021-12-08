#pragma once

#include "Vec3.h"

namespace mml
{

	class Vec4 : public Vec3
	{
	public:
		float w = 0.0f;

		Vec4();
		Vec4(float X, float Y, float Z, float W);
		Vec4(const Vec4& vec);

		virtual void normalize() override;
		float magnitude() const;
		float dot(const Vec4& vec) const;
		
		void operator*=(const Vec4& other);
		void operator+=(const Vec4& other);

		friend Vec4 operator+(const Vec4& left, const Vec4& right);
		friend Vec4 operator-(const Vec4& left, const Vec4& right);
		friend Vec4 operator*(const Vec4& left, const Vec4& right);
		friend Vec4 operator/(const Vec4& left, const Vec4& right);

		friend Vec4 operator*(const Vec4& left, const float& right);
		friend Vec4 operator/(const Vec4& left, const float& right);
	};
}
