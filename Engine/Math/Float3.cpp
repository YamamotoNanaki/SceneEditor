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
	return *this;
}

Float3 Float3::operator*() const
{
	return *this;
}

Float3& Float3::operator+=(Float3 f)
{
	return *this = (this->x + f.x, this->y + f.y, this->z + f.z);
}

Float3& Float3::operator-=(Float3 f)
{
	return *this = (this->x - f.x, this->y - f.y, this->z - f.z);
}

Float3& Float3::operator*=(Float3 f)
{
	return *this = (this->x * f.x, this->y * f.y, this->z * f.z);
}

Float3& Float3::operator/=(Float3 f)
{
	assert(f.x == 0 && "—ëœŽZ");
	assert(f.y == 0 && "—ëœŽZ");
	assert(f.z == 0 && "—ëœŽZ");
	return *this = (this->x / f.x, this->y / f.y, this->z / f.z);
}

Float3& Float3::operator*=(float f)
{
	return *this = (this->x * f, this->y * f, this->z * f);
}

Float3& Float3::operator/=(float f)
{
	assert(f == 0 && "—ëœŽZ");
	return *this = (this->x / f, this->y / f, this->z / f);
}
