#include "IFMath.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <cassert>

using namespace IF;

Float4 IF::SetFloat4ToFloat3(Float3 f, float w)
{
	return Float4(f.x, f.y, f.z, w);
}

Float4 IF::SetFloat4ToFoat2(Float2 f, float z, float w)
{
	return Float4(f.x, f.y, z, w);
}

Float3 IF::SetFloat3ToFloat2(Float2 f, float z)
{
	return Float3(f.x, f.y, z);
}

Float4 IF::SetFloat4ToVector3(Vector3 v, float w)
{
	return Float4(v.x, v.y, v.z, w);
}

Float3 IF::SetFloat3(Vector3 v)
{
	return Float3(v.x, v.y, v.z);
}

Float2 IF::SetFloat3ToVector2(Vector2 v)
{
	return Float2(v.x, v.y);
}

Vector3 IF::SetVector3(Float3 f)
{
	return Vector3(f.x, f.y, f.z);
}

Vector3 IF::SetVector3ToFloat2(Float2 f, float z)
{
	return Vector3(f.x, f.y, z);
}

Vector2 IF::SetVector2(Float2 f)
{
	return Vector2(f.x, f.y);
}

const Vector3 IF::Vector3Zero()
{
	return Vector3(0, 0, 0);
}

bool IF::Vector3Equal(const Vector3& v1, const Vector3& v2)
{
	return (((v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z)) != 0);
}

Vector3& IF::Vector3Normalize(Vector3& v)
{
	float length = v.Length();
	if (length != 0)
	{
		Vector3 a = v / length;
		return a;
	}
	return v;
}

float IF::Vector3Dot(const Vector3& v1, const Vector3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3 IF::Vector3Cross(const Vector3& v1, const Vector3& v2)
{
	Vector3 vec(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
	return vec;
}

bool IF::NearEqual(float S1, float S2, float Epsilon)
{
	float Delta = S1 - S2;
	return (fabsf(Delta) <= Epsilon);
}

void IF::ScalarSinCos(float* pSin, float* pCos, float Value)
{
	assert(pSin);
	assert(pCos);

	// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
	float quotient = M_PI_2 * Value;
	if (Value >= 0.0f)
	{
		quotient = static_cast<float>(static_cast<int>(quotient + 0.5f));
	}
	else
	{
		quotient = static_cast<float>(static_cast<int>(quotient - 0.5f));
	}
	float y = Value - M_PI * 2 * quotient;

	// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
	float sign;
	if (y > M_PI_2)
	{
		y = M_PI - y;
		sign = -1.0f;
	}
	else if (y < -M_PI_2)
	{
		y = -M_PI - y;
		sign = -1.0f;
	}
	else
	{
		sign = +1.0f;
	}

	float y2 = y * y;

	// 11-degree minimax approximation
	*pSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

	// 10-degree minimax approximation
	float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
	*pCos = sign * p;
}

float IF::ConvertToRadians(float fDegrees)
{
	return fDegrees * (M_PI / 180.0f);
}

float IF::ConvertToDegrees(float fRadians)
{
	return fRadians * (180.0f / M_PI);
}

Vector3 IF::Vector3Transform(const Vector3& v, const Matrix& m)
{
	Vector3 mx = { m._1_1,m._1_2,m._1_3 };
	Vector3 my = { m._2_1,m._2_2,m._2_3 };
	Vector3 mz = { m._3_1,m._3_2,m._3_3 };
	Vector3 mw = { m._4_1,m._4_2,m._4_3 };

	Vector3 Result = {
		v.z * mz.x + mw.x,
		v.z * mz.y + mw.y,
		v.z * mz.z + mw.z };
	Result = {
		v.y * my.x + Result.x,
		v.y * my.y + Result.y,
		v.y * my.z + Result.z };
	Result = {
		v.x * mx.x + Result.x,
		v.x * mx.y + Result.y,
		v.x * mx.z + Result.z };

	return Result;
}

Vector3 IF::VectorNegate(Vector3 m)
{
	return -m;
}

Matrix IF::MatrixPerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ, float FarZ)
{
	assert(NearZ > 0.f && FarZ > 0.f);
	assert(!NearEqual(FovAngleY, 0.0f, 0.00001f * 2.0f));
	assert(!NearEqual(AspectRatio, 0.0f, 0.00001f));
	assert(!NearEqual(FarZ, NearZ, 0.00001f));

	float    SinFov;
	float    CosFov;
	ScalarSinCos(&SinFov, &CosFov, 0.5f * FovAngleY);

	float Height = CosFov / SinFov;
	float Width = Height / AspectRatio;
	float fRange = FarZ / (FarZ - NearZ);

	return Matrix(Width, 0, 0, 0, 0, Height, 0, 0, 0, 0, fRange, 1, 0, 0, -fRange * NearZ, 0);
}
