#include "Float3.h"
#include <assert.h>

using namespace IF;

Float3::Float3(float x, float y, float z) :x(x), y(y), z(z) {}

void IF::Float3::Set(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Float3 Float3::operator+() const
{
	return *this;
}

Float3 Float3::operator-() const
{
	return Float3(-x, -y, -z);
}

Float3 Float3::operator*() const
{
	return *this;
}

Float3& IF::Float3::operator+=(float f)
{
	x = x + f;
	y = y + f;
	z = z + f;
	return *this;
}

Float3& IF::Float3::operator-=(float f)
{
	x = x - f;
	y = y - f;
	z = z - f;
	return *this;
}

Float3& Float3::operator+=(Float3 f)
{
	x = x + f.x;
	y = y + f.y;
	z = z + f.z;
	return *this;
}

Float3& Float3::operator-=(Float3 f)
{
	x = x - f.x;
	y = y - f.y;
	z = z - f.z;
	return *this;
}

Float3& Float3::operator*=(Float3 f)
{
	x = x * f.x;
	y = y * f.y;
	z = z * f.z;
	return *this;
}

Float3& Float3::operator/=(Float3 f)
{
	assert(f.x == 0 && "—ëœZ");
	assert(f.y == 0 && "—ëœZ");
	assert(f.z == 0 && "—ëœZ");
	x = x / f.x;
	y = y / f.y;
	z = z / f.z;
	return *this;
}

Float3& Float3::operator*=(float f)
{
	x = x * f;
	y = y * f;
	z = z * f;
	return *this;
}

Float3& Float3::operator/=(float f)
{
	assert(f == 0 && "—ëœZ");
	x = x - f;
	y = y - f;
	z = z - f;
	return *this;
}

Float3 IF::Float3::operator=(float* f)
{
	x = f[0];
	y = f[1];
	z = f[2];
	return *this;
}

Float3 IF::operator/(Float3 f, float s)
{
	return Float3(f.x / s, f.y / s, f.z / s);
}

Float3 IF::operator*(Float3 f, float s)
{
	return Float3(f.x * s, f.y * s, f.z * s);
}

Float3 IF::operator*(float s, Float3 f)
{
	return Float3(f.x * s, f.y * s, f.z * s);
}

Float3 IF::operator+(Float3 f1, Float3 f2)
{
	return Float3(f1.x + f2.x, f1.y + f2.y, f1.z + f2.z);
}

Float3 IF::operator-(Float3 f1, Float3 f2)
{
	return Float3(f1.x - f2.x, f1.y - f2.y, f1.z - f2.z);
}
