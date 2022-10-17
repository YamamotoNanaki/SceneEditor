#include "Vector2.h"
#include <cmath>

using namespace IF;

Vector2::Vector2() :x(0), y(0) {};
Vector2::Vector2(float x, float y) :x(x), y(y) {};

void Vector2::Set(float x, float y)
{
	this->x = x;
	this->y = y;
};

float Vector2::Length() const
{
	return sqrtf(x * x + y * y);
}

Vector2& Vector2::Normalize()
{
	float length = Length();
	if (length != 0)
	{
		return *this /= length;
	}
	return *this;
}

float Vector2::Dot(const Vector2& v) const
{
	return x * v.x + y * v.y;
}

float Vector2::Cross(const Vector2& v) const
{
	return x * v.y - y * v.x;
}

Vector2 Vector2::operator+() const
{
	return *this;
}

Vector2 Vector2::operator-() const
{
	return Vector2(-x, -y);
}

Vector2& Vector2::operator+=(const Vector2& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

Vector2& Vector2::operator*=(float s)
{
	x *= s;
	y *= s;
	return *this;
}

Vector2& Vector2::operator/=(float s)
{
	x /= s;
	y /= s;
	return *this;
}

Vector2& Vector2::operator=(const Float2 f)
{
	x = f.x;
	y = f.y;
	return *this;
}

const Vector2 operator+(const Vector2& v1, const Vector2& v2)
{
	Vector2 temp(v1);
	return temp += v2;
}

const Vector2 operator-(const Vector2& v1, const Vector2& v2)
{
	Vector2 temp(v1);
	return temp -= v2;
}

const Vector2 operator*(const Vector2& v, float s)
{
	Vector2 temp(v);
	return temp *= s;
}

const Vector2 operator*(float s, const Vector2& v)
{
	Vector2 temp(v);
	return temp *= s;
}

const Vector2 operator/(const Vector2& v, float s)
{
	Vector2 temp(v);
	return temp /= s;
}
