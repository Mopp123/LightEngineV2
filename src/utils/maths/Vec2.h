#pragma once


namespace mml
{
	class Vec3;


	class Vec2
	{
	public:
		float x = 0.0f;
		float y = 0.0f;

		Vec2();
		Vec2(float X, float Y);
		Vec2(const Vec2& vec);
		Vec2(const Vec3& vec);

		virtual void normalize();
		float magnitude() const;
		float dot(const Vec2& vec) const;

		friend Vec2 operator+(const Vec2& left, const Vec2& right);
		friend Vec2 operator-(const Vec2& left, const Vec2& right);
		friend Vec2 operator*(const Vec2& left, const Vec2& right);
		friend Vec2 operator/(const Vec2& left, const Vec2& right);

		friend Vec2 operator*(const Vec2& left, const float& right);
		friend Vec2 operator/(const Vec2& left, const float& right);
	};
}