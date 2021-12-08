
#include "Vec3.h"
#include <cmath>

namespace mml
{
	Vec3::Vec3() :
		z(0.0f)
	{}
	Vec3::Vec3(float X, float Y, float Z) : 
		Vec2(X, Y), z(Z)
	{}
	Vec3::Vec3(const Vec3& vec) : 
		Vec2(vec.x, vec.y), z(vec.z)
	{}

	void Vec3::normalize()
	{
		float mag = magnitude();
		if (mag == 0.0f)
			return;
		x /= mag;
		y /= mag;
		z /= mag;
	}

	float Vec3::magnitude() const
	{
		float c = x * x + y * y + z * z;
		if (c == 0.0f)
			return c;

		return std::sqrtf(c);
	}

	float Vec3::dot(const Vec3& vec) const
	{
		return x * vec.x + y * vec.y + z * vec.z;
	}

	Vec3 Vec3::cross(const Vec3& vec) const
	{
		return Vec3(
			y * vec.z - z * vec.y, 
			z * vec.x - x * vec.z, 
			x * vec.y - y * vec.x
		);
	}

	Vec3 operator+(const Vec3& left, const Vec3& right)
	{
		return Vec3(left.x + right.x, left.y + right.y, left.z + right.z);
	}
	Vec3 operator-(const Vec3& left, const Vec3& right)
	{
		return Vec3(left.x - right.x, left.y - right.y, left.z - right.z);
	}
	Vec3 operator*(const Vec3& left, const Vec3& right)
	{
		return Vec3(left.x * right.x, left.y * right.y, left.z * right.z);
	}
	Vec3 operator/(const Vec3& left, const Vec3& right)
	{
		return Vec3(left.x / right.x, left.y / right.y, left.z / right.z);
	}

	bool operator==(const Vec3& left, const Vec3& right)
	{
		return (left.x == right.x && left.y == right.y && left.z == right.z);
	}
	bool operator!=(const Vec3& left, const Vec3& right)
	{
		return !(left == right);
	}

	Vec3 operator*(const Vec3& left, const float& right)
	{
		return Vec3(left.x * right, left.y * right, left.z * right);
	}
	Vec3 operator/(const Vec3& left, const float& right)
	{
		return Vec3(left.x / right, left.y / right, left.z / right);
	}
}