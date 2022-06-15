#include "Matrix.h"
#include <math.h>
#include <cassert>

using namespace IF;

IF::Matrix::Matrix()
{
	*this = MatrixIdentity();
}

IF::Matrix::Matrix(float _1_1, float _1_2, float _1_3, float _1_4,
	float _2_1, float _2_2, float _2_3, float _2_4,
	float _3_1, float _3_2, float _3_3, float _3_4,
	float _4_1, float _4_2, float _4_3, float _4_4) :
	_1_1(_1_1), _1_2(_1_2), _1_3(_1_3), _1_4(_1_4),
	_2_1(_2_1), _2_2(_2_2), _2_3(_2_3), _2_4(_2_4),
	_3_1(_3_1), _3_2(_3_2), _3_3(_3_3), _3_4(_3_4),
	_4_1(_4_1), _4_2(_4_2), _4_3(_4_3), _4_4(_4_4) {}

Matrix IF::Matrix::operator+() const
{
	return *this;
}

Matrix IF::Matrix::operator-() const
{
	return Matrix(-_1_1, -_1_2, -_1_3, -_1_4, -_2_1, -_2_2, -_2_3, -_2_4, -_3_1, -_3_2, -_3_3, -_3_4, -_4_1, -_4_2, -_4_3, -_4_4);
}

Matrix IF::Matrix::operator*(float s)const
{
	Matrix a;
	a._1_1 = _1_1 * s;
	a._1_2 = _1_2 * s;
	a._1_3 = _1_3 * s;
	a._1_4 = _1_4 * s;
	a._2_1 = _2_1 * s;
	a._2_2 = _2_2 * s;
	a._2_3 = _2_3 * s;
	a._2_4 = _2_4 * s;
	a._3_1 = _3_1 * s;
	a._3_2 = _3_2 * s;
	a._3_3 = _3_3 * s;
	a._3_4 = _3_4 * s;
	a._4_1 = _4_1 * s;
	a._4_2 = _4_2 * s;
	a._4_3 = _4_3 * s;
	a._4_4 = _4_4 * s;

	return a;
}

Matrix IF::Matrix::operator/(float s)const
{
	Matrix a;
	a._1_1 = _1_1 / s;
	a._1_2 = _1_2 / s;
	a._1_3 = _1_3 / s;
	a._1_4 = _1_4 / s;
	a._2_1 = _2_1 / s;
	a._2_2 = _2_2 / s;
	a._2_3 = _2_3 / s;
	a._2_4 = _2_4 / s;
	a._3_1 = _3_1 / s;
	a._3_2 = _3_2 / s;
	a._3_3 = _3_3 / s;
	a._3_4 = _3_4 / s;
	a._4_1 = _4_1 / s;
	a._4_2 = _4_2 / s;
	a._4_3 = _4_3 / s;
	a._4_4 = _4_4 / s;

	return a;
}

Matrix& IF::Matrix::operator+=(Matrix m)
{
	*this = *this + m;
	return *this;
}

Matrix& IF::Matrix::operator-=(Matrix m)
{
	*this = *this - m;
	return *this;
}

Matrix& IF::Matrix::operator*=(Matrix m)
{
	*this = *this * m;
	return *this;
}

Matrix& IF::Matrix::operator*=(float s)
{
	*this = *this * s;
	return *this;
}

Matrix& IF::Matrix::operator/=(float s)
{
	*this = *this / s;
	return *this;
}

void IF::Matrix::SetX(float x, float y, float z, float w)
{
	_1_1 = x;
	_1_2 = y;
	_1_3 = z;
	_1_4 = w;
}

void IF::Matrix::SetY(float x, float y, float z, float w)
{
	_2_1 = x;
	_2_2 = y;
	_2_3 = z;
	_2_4 = w;
}

void IF::Matrix::SetZ(float x, float y, float z, float w)
{
	_3_1 = x;
	_3_2 = y;
	_3_3 = z;
	_3_4 = w;
}

void IF::Matrix::SetW(float x, float y, float z, float w)
{
	_4_1 = x;
	_4_2 = y;
	_4_3 = z;
	_4_4 = w;
}

void IF::Matrix::SetX(Vector3 v, float w)
{
	_1_1 = v.x;
	_1_2 = v.y;
	_1_3 = v.z;
	_1_4 = w;
}

void IF::Matrix::SetY(Vector3 v, float w)
{
	_2_1 = v.x;
	_2_2 = v.y;
	_2_3 = v.z;
	_2_4 = w;
}

void IF::Matrix::SetZ(Vector3 v, float w)
{
	_3_1 = v.x;
	_3_2 = v.y;
	_3_3 = v.z;
	_3_4 = w;
}

void IF::Matrix::SetW(Vector3 v, float w)
{
	_4_1 = v.x;
	_4_2 = v.y;
	_4_3 = v.z;
	_4_4 = w;
}

Matrix IF::MatrixIdentity()
{
	return Matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
}

Matrix IF::MatrixTranspose(const Matrix& m)
{
	return Matrix(
		m._1_1, m._2_1, m._3_1, m._4_1,
		m._1_2, m._2_2, m._3_2, m._4_2,
		m._1_3, m._2_3, m._3_3, m._4_3,
		m._1_4, m._2_4, m._3_4, m._4_4);
}

Matrix IF::MatrixScaling(float sx, float sy, float sz)
{
	return Matrix(sx, 0, 0, 0, 0, sy, 0, 0, 0, 0, sz, 0, 0, 0, 0, 1);
}

Matrix IF::MatrixRotationX(float angle)
{
	float fSinAngle = sinf(angle);
	float fCosAngle = cosf(angle);
	return Matrix(1, 0, 0, 0, 0, fCosAngle, fSinAngle, 0, 0, -fSinAngle, fCosAngle, 0, 0, 0, 0, 1);
}

Matrix IF::MatrixRotationY(float angle)
{
	float fSinAngle = sinf(angle);
	float fCosAngle = cosf(angle);
	return Matrix(fCosAngle, 0, -fSinAngle, 0, 0, 1, 0, 0, fSinAngle, 0, fCosAngle, 0, 0, 0, 0, 1);
}

Matrix IF::MatrixRotationZ(float angle)
{
	float fSinAngle = sinf(angle);
	float fCosAngle = cosf(angle);
	return Matrix(fCosAngle, fSinAngle, 0, 0, -fSinAngle, fCosAngle, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
}

Matrix IF::MatrixRotation(float x, float y, float z, float angle)
{
	float cs = cosf(angle);
	float si = sinf(angle);
	Matrix a;
	a._1_1 = cs + x * x * (1 - cs);
	a._1_2 = x * y * (1 - cs) - z * si;
	a._1_3 = z * y * (1 - cs) + x * si;
	a._2_1 = x * y * (1 - cs) - z * si;
	a._2_2 = cs + y * y * (1 - cs);
	a._2_3 = z * y * (1 - cs) - x * si;
	a._3_1 = z * x * (1 - cs) - y * si;
	a._3_2 = z * y * (1 - cs) + x * si;
	a._3_3 = cs + z * z * (1 - cs);

	a._1_4 = 0;
	a._2_4 = 0;
	a._3_4 = 0;
	a._4_1 = 0;
	a._4_2 = 0;
	a._4_3 = 0;
	a._4_4 = 1;

	return a;
}

Matrix IF::MatrixTranslation(float tx, float ty, float tz)
{
	return Matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, tx, ty, tz, 1);
}

Matrix IF::MatrixOrthoGraphicProjection(float left, float right, float top, float bottom, float near, float far)
{
	float width = 2.0f / (right - left);
	float height = -2.0f / (bottom - top);
	float zRange = 1.0f / (far - near);
	return Matrix(width, 0, 0, 0, 0, height, 0, 0, 0, 0, zRange, 0, -1, 1, 0, 1);
}

Matrix IF::operator+(Matrix m1, Matrix m2)
{
	Matrix a;
	a._1_1 = m1._1_1 + m2._1_1;
	a._1_2 = m1._1_2 + m2._1_2;
	a._1_3 = m1._1_3 + m2._1_3;
	a._1_4 = m1._1_4 + m2._1_4;
	a._2_1 = m1._2_1 + m2._2_1;
	a._2_2 = m1._2_2 + m2._2_2;
	a._2_3 = m1._2_3 + m2._2_3;
	a._2_4 = m1._2_4 + m2._2_4;
	a._3_1 = m1._3_1 + m2._3_1;
	a._3_2 = m1._3_2 + m2._3_2;
	a._3_3 = m1._3_3 + m2._3_3;
	a._3_4 = m1._3_4 + m2._3_4;
	a._4_1 = m1._4_1 + m2._4_1;
	a._4_2 = m1._4_2 + m2._4_2;
	a._4_3 = m1._4_3 + m2._4_3;
	a._4_4 = m1._4_4 + m2._4_4;

	return a;
}

Matrix IF::operator-(Matrix m1, Matrix m2)
{
	Matrix a;
	a._1_1 = m1._1_1 - m2._1_1;
	a._1_2 = m1._1_2 - m2._1_2;
	a._1_3 = m1._1_3 - m2._1_3;
	a._1_4 = m1._1_4 - m2._1_4;
	a._2_1 = m1._2_1 - m2._2_1;
	a._2_2 = m1._2_2 - m2._2_2;
	a._2_3 = m1._2_3 - m2._2_3;
	a._2_4 = m1._2_4 - m2._2_4;
	a._3_1 = m1._3_1 - m2._3_1;
	a._3_2 = m1._3_2 - m2._3_2;
	a._3_3 = m1._3_3 - m2._3_3;
	a._3_4 = m1._3_4 - m2._3_4;
	a._4_1 = m1._4_1 - m2._4_1;
	a._4_2 = m1._4_2 - m2._4_2;
	a._4_3 = m1._4_3 - m2._4_3;
	a._4_4 = m1._4_4 - m2._4_4;

	return a;
}

Matrix IF::operator*(Matrix m1, Matrix m2)
{
	Matrix a;
	a._1_1 = m1._1_1 * m2._1_1 + m1._1_2 * m2._2_1 + m1._1_3 * m2._3_1 + m1._1_4 * m2._4_1;
	a._1_2 = m1._1_1 * m2._1_2 + m1._1_2 * m2._2_2 + m1._1_3 * m2._3_2 + m1._1_4 * m2._4_2;
	a._1_3 = m1._1_1 * m2._1_3 + m1._1_2 * m2._2_3 + m1._1_3 * m2._3_3 + m1._1_4 * m2._4_3;
	a._1_4 = m1._1_1 * m2._1_4 + m1._1_2 * m2._2_4 + m1._1_3 * m2._3_4 + m1._1_4 * m2._4_4;

	a._2_1 = m1._2_1 * m2._1_1 + m1._2_2 * m2._2_1 + m1._2_3 * m2._3_1 + m1._2_4 * m2._4_1;
	a._2_2 = m1._2_1 * m2._1_2 + m1._2_2 * m2._2_2 + m1._2_3 * m2._3_2 + m1._2_4 * m2._4_2;
	a._2_3 = m1._2_1 * m2._1_3 + m1._2_2 * m2._2_3 + m1._2_3 * m2._3_3 + m1._2_4 * m2._4_3;
	a._2_4 = m1._2_1 * m2._1_4 + m1._2_2 * m2._2_4 + m1._2_3 * m2._3_4 + m1._2_4 * m2._4_4;

	a._3_1 = m1._3_1 * m2._1_1 + m1._3_2 * m2._2_1 + m1._3_3 * m2._3_1 + m1._3_4 * m2._4_1;
	a._3_2 = m1._3_1 * m2._1_2 + m1._3_2 * m2._2_2 + m1._3_3 * m2._3_2 + m1._3_4 * m2._4_2;
	a._3_3 = m1._3_1 * m2._1_3 + m1._3_2 * m2._2_3 + m1._3_3 * m2._3_3 + m1._3_4 * m2._4_3;
	a._3_4 = m1._3_1 * m2._1_4 + m1._3_2 * m2._2_4 + m1._3_3 * m2._3_4 + m1._3_4 * m2._4_4;

	a._4_1 = m1._4_1 * m2._1_1 + m1._4_2 * m2._2_1 + m1._4_3 * m2._3_1 + m1._4_4 * m2._4_1;
	a._4_2 = m1._4_1 * m2._1_2 + m1._4_2 * m2._2_2 + m1._4_3 * m2._3_2 + m1._4_4 * m2._4_2;
	a._4_3 = m1._4_1 * m2._1_3 + m1._4_2 * m2._2_3 + m1._4_3 * m2._3_3 + m1._4_4 * m2._4_3;
	a._4_4 = m1._4_1 * m2._1_4 + m1._4_2 * m2._2_4 + m1._4_3 * m2._3_4 + m1._4_4 * m2._4_4;
	return a;
}

