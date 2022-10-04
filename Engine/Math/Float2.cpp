#include "Float2.h"
#include <assert.h>

using namespace IF;

Float2::Float2(float x, float y) :x(x), y(y) {}

void IF::Float2::Set(float x, float y)
{
	this->x = x;
	this->y = y;
}

Float2 Float2::operator+() const
{
	return *this;
}

Float2 Float2::operator-() const
{
	return *this;
}

Float2 Float2::operator*() const
{
	return *this;
}

void Float2::operator+=(Float2 f)
{
	*this = (this->x + f.x, this->y + f.y);
}

void Float2::operator-=(Float2 f)
{
	*this = (this->x - f.x, this->y - f.y);
}

void Float2::operator*=(Float2 f)
{
	*this = (this->x * f.x, this->y * f.y);
}

void Float2::operator/=(Float2 f)
{
	assert(f.x != 0 && "—ëœŽZ");
	assert(f.y != 0 && "—ëœŽZ");
	*this = (this->x / f.x, this->y / f.y);
}

void Float2::operator*=(float f)
{
	*this = (this->x * f, this->y * f);
}

void Float2::operator/=(float f)
{
	assert(f != 0 && "—ëœŽZ");
	this->x = this->x / f;
	this->y = this->y / f;
}
