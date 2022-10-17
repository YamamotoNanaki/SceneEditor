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
	float _4_1, float _4_2, float _4_3, float _4_4)
{
	m[0][0] = _1_1;  m[0][1] = _1_2;  m[0][2] = _1_3;  m[0][3] = _1_4;
	m[1][0] = _2_1;  m[1][1] = _2_2;  m[1][2] = _2_3;  m[1][3] = _2_4;
	m[2][0] = _3_1;  m[2][1] = _3_2;  m[2][2] = _3_3;  m[2][3] = _3_4;
	m[3][0] = _4_1;  m[3][1] = _4_2;  m[3][2] = _4_3;  m[3][3] = _4_4;
}

Matrix IF::Matrix::operator+() const
{
	return *this;
}

Matrix IF::Matrix::operator-() const
{
	return Matrix(-m[0][0], -m[0][1], -m[0][2], -m[0][3], -m[1][0], -m[1][1], -m[1][2], -m[1][3], -m[2][0], -m[2][1], -m[2][2], -m[2][3], -m[3][0], -m[3][1], -m[3][2], -m[3][3]);
}

Matrix IF::Matrix::operator*(float s)const
{
	Matrix a;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			a.m[i][j] = m[i][j] * s;
		}
	}

	return a;
}

Matrix IF::Matrix::operator/(float s)const
{
	Matrix a;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			a.m[i][j] = m[i][j] / s;
		}
	}

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
	m[0][0] = x;
	m[0][1] = y;
	m[0][2] = z;
	m[0][3] = w;
}

void IF::Matrix::SetY(float x, float y, float z, float w)
{
	m[1][0] = x;
	m[1][1] = y;
	m[1][2] = z;
	m[1][3] = w;
}

void IF::Matrix::SetZ(float x, float y, float z, float w)
{
	m[2][0] = x;
	m[2][1] = y;
	m[2][2] = z;
	m[2][3] = w;
}

void IF::Matrix::SetW(float x, float y, float z, float w)
{
	m[3][0] = x;
	m[3][1] = y;
	m[3][2] = z;
	m[3][3] = w;
}

void IF::Matrix::SetX(Vector3 v, float w)
{
	m[0][0] = v.x;
	m[0][1] = v.y;
	m[0][2] = v.z;
	m[0][3] = w;
}

void IF::Matrix::SetY(Vector3 v, float w)
{
	m[1][0] = v.x;
	m[1][1] = v.y;
	m[1][2] = v.z;
	m[1][3] = w;
}

void IF::Matrix::SetZ(Vector3 v, float w)
{
	m[2][0] = v.x;
	m[2][1] = v.y;
	m[2][2] = v.z;
	m[2][3] = w;
}

void IF::Matrix::SetW(Vector3 v, float w)
{
	m[3][0] = v.x;
	m[3][1] = v.y;
	m[3][2] = v.z;
	m[3][3] = w;
}

Matrix IF::MatrixIdentity()
{
	return Matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
}

Matrix IF::MatrixTranspose(const Matrix& m)
{
	return Matrix(
		m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0],
		m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1],
		m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2],
		m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]);
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
	a.m[0][0] = cs + x * x * (1 - cs);
	a.m[0][1] = x * y * (1 - cs) - z * si;
	a.m[0][2] = z * y * (1 - cs) + x * si;
	a.m[1][0] = x * y * (1 - cs) - z * si;
	a.m[1][1] = cs + y * y * (1 - cs);
	a.m[1][2] = z * y * (1 - cs) - x * si;
	a.m[2][0] = z * x * (1 - cs) - y * si;
	a.m[2][1] = z * y * (1 - cs) + x * si;
	a.m[2][2] = cs + z * z * (1 - cs);

	a.m[0][3] = 0;
	a.m[1][3] = 0;
	a.m[2][3] = 0;
	a.m[3][0] = 0;
	a.m[3][1] = 0;
	a.m[3][2] = 0;
	a.m[3][3] = 1;

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

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			a.m[i][j] = m1.m[i][j] + m2.m[i][j];
		}
	}

	return a;
}

Matrix IF::operator-(Matrix m1, Matrix m2)
{
	Matrix a;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			a.m[i][j] = m1.m[i][j] - m2.m[i][j];
		}
	}

	return a;
}

Matrix IF::operator*(Matrix m1, Matrix m2)
{
	Matrix a;
	a.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0];
	a.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1];
	a.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2];
	a.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3];

	a.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0];
	a.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1];
	a.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2];
	a.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3];

	a.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0];
	a.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1];
	a.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2];
	a.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3];

	a.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0];
	a.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1];
	a.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2];
	a.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3];
	return a;
}

