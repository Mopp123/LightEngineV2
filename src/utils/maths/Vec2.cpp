
#include "Vec2.h"
#include "Vec3.h"
#include <cmath>

namespace mml
{
	Vec2::Vec2() :
		x(0.0f), y(0.0f)
	{}
	Vec2::Vec2(float X, float Y) : 
		x(X), y(Y)
	{}
	Vec2::Vec2(const Vec2& vec) : 
		x(vec.x), y(vec.y)
	{}
	Vec2::Vec2(const Vec3& vec) :
		x(vec.x), y(vec.y)
	{}

	void Vec2::normalize()
	{
		float mag = magnitude();
		x /= mag;
		y /= mag;
	}
	float Vec2::magnitude() const
	{
		return std::sqrtf(x * x + y * y);
	}
	float Vec2::dot(const Vec2& vec) const
	{
		return x * vec.x + y * vec.y;
	}

	Vec2 operator+(const Vec2& left, const Vec2& right) 
	{
		return Vec2(left.x + right.x, left.y + right.y);
	}

	Vec2 operator-(const Vec2& left, const Vec2& right) 
	{
		return Vec2(left.x - right.x, left.y - right.y);
	}

	Vec2 operator*(const Vec2& left, const Vec2& right) 
	{
		return Vec2(left.x * right.x, left.y * right.y);
	}

	Vec2 operator/(const Vec2& left, const Vec2& right) 
	{
		return Vec2(left.x / right.x, left.y / right.y);
	}


	Vec2 operator*(const Vec2& left, const float& right) 
	{
		return Vec2(left.x * right, left.y * right);
	}
	Vec2 operator/(const Vec2& left, const float& right) 
	{
		return Vec2(left.x / right, left.y / right);
	}
}