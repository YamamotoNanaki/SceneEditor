#pragma once
#include "Float2.h"

namespace IF
{
	class Vector2
	{
	public:
		float x;
		float y;

	public:
		Vector2();
		Vector2(float x, float y);
		void Set(float x, float y);

		float Length() const;
		Vector2& Normalize();
		float Dot(const Vector2& v) const;
		float Cross(const Vector2& v) const;

		Vector2 operator+() const;
		Vector2 operator-() const;
		Vector2& operator+=(const Vector2& v);
		Vector2& operator-=(const Vector2& v);
		Vector2& operator*=(float s);
		Vector2& operator/=(float s);

	};
	const Vector2 operator+(const Vector2& v1, const Vector2& v2);
	const Vector2 operator-(const Vector2& v1, const Vector2& v2);
	const Vector2 operator*(float s, const Vector2& v);
	const Vector2 operator*(const Vector2& v, float s);
	const Vector2 operator/(const Vector2& v1, float s);
};
