
#include "Vec4.h"
#include <cmath>

namespace mml
{

	Vec4::Vec4() : 
		w(0.0f)
	{}
	Vec4::Vec4(float X, float Y, float Z, float W) :
		Vec3(X, Y, Z), w(W)
	{}
	Vec4::Vec4(const Vec4& vec) :
		Vec3(vec.x, vec.y, vec.z), w(vec.w)
	{}

	void Vec4::normalize()
	{
		float mag = magnitude();
		if (mag > 0)
		{
			x /= mag;
			y /= mag;
			z /= mag;
			w /= mag;
		}
	}

	float Vec4::magnitude() const
	{
		return std::sqrtf(x * x + y * y + z * z + w * w);
	}

	float Vec4::dot(const Vec4& vec) const
	{
		return x * vec.x + y * vec.y + z * vec.z + w * vec.w;
	}

	void Vec4::operator*=(const Vec4& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		w *= other.w;
	}
	
	void Vec4::operator+=(const Vec4& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
	}


	Vec4 operator+(const Vec4& left, const Vec4& right)
	{
		return Vec4(left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w);
	}
	Vec4 operator-(const Vec4& left, const Vec4& right)
	{
		return Vec4(left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w);
	}
	Vec4 operator*(const Vec4& left, const Vec4& right)
	{
		return Vec4(left.x * right.x, left.y * right.y, left.z * right.z, left.w * right.w);
	}
	Vec4 operator/(const Vec4& left, const Vec4& right)
	{
		return Vec4(left.x / right.x, left.y / right.y, left.z / right.z, left.w / right.w);
	}

	Vec4 operator*(const Vec4& left, const float& right)
	{
		return Vec4(left.x * right, left.y * right, left.z * right, left.w * right);
	}
	Vec4 operator/(const Vec4& left, const float& right)
	{
		return Vec4(left.x / right, left.y / right, left.z / right, left.w / right);
	}
}