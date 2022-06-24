#include "Vector3.h"
#include <cmath>

using namespace IF;

Vector3::Vector3() :x(0), y(0), z(0) {}
Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

void Vector3::Set(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

float Vector3::Length() const
{
	return sqrtf(x * x + y * y + z * z);
}

Vector3& Vector3::Normalize()
{
	float length = Length();
	if (length != 0)
	{
		return *this /= length;
	}
	return *this;
}

float Vector3::Dot(const Vector3& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

Vector3& Vector3::Cross(const Vector3& v) const
{
	Vector3 vec(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	return vec;
}

Vector3 Vector3::operator+() const
{
	return *this;
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vector3& Vector3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector3& Vector3::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

//Vector3& Vector3::operator=(const Float3 f)
//{
//	x = f.x;
//	y = f.y;
//	z = f.z;
//	return *this;
//}

const Vector3 operator+(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp += v2;
}

const Vector3 operator-(const Vector3& v1, const Vector3& v2)
{
	Vector3 temp(v1);
	return temp -= v2;
}

const Vector3 operator*(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp *= s;
}

const Vector3 operator*(float s, const Vector3& v)
{
	Vector3 temp(v);
	return temp *= s;
}

const Vector3 operator/(const Vector3& v, float s)
{
	Vector3 temp(v);
	return temp /= s;
}

Vector3 IF::VectorSubtract(Vector3 v1, Vector3 v2)
{
	Vector3 a(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
	return a;
}

Vector3 IF::operator/(Vector3 v, float s)
{
	return Vector3(v.x / s, v.y / s, v.z / s);
}
