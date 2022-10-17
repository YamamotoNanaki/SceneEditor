#include "Quaternion.h"
#include <cmath>
#include "Quaternion.h"
#include "Quaternion.h"

using namespace IF;

Quaternion IF::quaternion(float x, float y, float z, float w)
{
	Quaternion r = { x,y,z,w };
	return r;
}

Quaternion IF::quaternion(const Vector3& v, float angle)
{
	float s = sin(angle / 2.0f);
	return quaternion(s * v.x, s * v.y, s * v.z, cos(angle / 2.0f));
}

float IF::Dot(const Quaternion& q1, const Quaternion& q2)
{
	float cos = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
	return cos;
}

float IF::Length(const Quaternion q)
{
	return (float)sqrt(Dot(q, q));
}

Quaternion IF::normalize(const Quaternion& q)
{
	Quaternion r = q;
	float l = Length(r);
	if (l != 0)r /= l;
	return r;
}

Quaternion IF::Conjugate(const Quaternion& q)
{
	Quaternion a = { -q.x, -q.y, -q.z, q.w };
	return a;
}

Quaternion IF::operator+(const Quaternion& q)
{
	return q;
}

Quaternion IF::operator-(const Quaternion& q)
{
	Quaternion r = { -q.x,-q.y,-q.z,-q.w };
	return r;
}

Quaternion IF::operator+=(Quaternion& q1, const Quaternion& q2)
{
	q1.x += q2.x;
	q1.y += q2.y;
	q1.z += q2.z;
	q1.w += q2.w;
	return q1;
}

Quaternion IF::operator-=(Quaternion& q1, const Quaternion& q2)
{
	q1.x -= q2.x;
	q1.y -= q2.y;
	q1.z -= q2.z;
	q1.w -= q2.w;
	return q1;
}

Quaternion IF::operator*=(Quaternion& q1, const Quaternion& q2)
{
	Quaternion q = {
		 q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x,
		-q1.x * q2.z + q1.y * q2.w + q1.z * q2.x + q1.w * q2.y,
		 q1.x * q2.y - q1.y * q2.x + q1.z * q2.w + q1.w * q2.z,
		-q1.x * q2.x - q1.y * q2.y - q1.z * q2.z + q1.w * q2.w
	};
	q1 = q;
	return q1;
}

Quaternion IF::operator*=(Quaternion& q, float s)
{
	q.x *= s;
	q.y *= s;
	q.z *= s;
	q.w *= s;
	return q;
}

Quaternion IF::operator/=(Quaternion& q, float s)
{
	return q *= 1.0f / s;
}

Quaternion IF::operator+(const Quaternion& q1, const Quaternion& q2)
{
	Quaternion r = q1;
	return r += q2;
}

Quaternion IF::operator-(const Quaternion& q1, const Quaternion& q2)
{
	Quaternion r = q1;
	return r -= q2;
}

Quaternion IF::operator*(const Quaternion& q1, const Quaternion& q2)
{
	Quaternion r = q1;
	return r *= q2;
}

Quaternion IF::operator*(const Quaternion& q, float s)
{
	Quaternion r = q;
	return r *= s;
}

Quaternion IF::operator*(float s, const Quaternion& q)
{
	Quaternion r = q;
	return r *= s;
}

Quaternion IF::operator/(const Quaternion& q1, float s)
{
	Quaternion r = q1;
	return r /= s;
}

Quaternion IF::SLerp(const Quaternion& q1, const Quaternion& q2, float t)
{
	float c = Dot(q1, q2);
	Quaternion t2 = q2;
	if (c < 0.0f)
	{
		c = -c;
		t2 = -q2;
	}
	float k0 = 1.0f - t;
	float k1 = t;
	if ((1.0f - c) > 0.001f)
	{
		float th = (float)acos(c);
		k0 = (float)(sin(th * k0) / sin(th));
		k1 = (float)(sin(th * k1) / sin(th));
	}
	return q1 * k0 + t2 * k1;
}

Quaternion IF::Lerp(const Quaternion& q1, const Quaternion& q2, float t)
{
	float c = Dot(q1, q2);
	Quaternion t2 = q2;
	if (c < 0.0f)
	{
		c = -c;
		t2 = -q2;
	}
	float k0 = 1.0f - t;
	float k1 = t;

	return q1 * k0 + t2 * k1;
}

Matrix IF::Rotate(const Quaternion& q)
{
	float xx = q.x * q.x;
	float yy = q.y * q.y;
	float zz = q.z * q.z;
	float xy = q.x * q.y;
	float xz = q.x * q.z;
	float yz = q.y * q.z;
	float wx = q.w * q.x;
	float wy = q.w * q.y;
	float wz = q.w * q.z;
	return Matrix(
		1.0f - yy - zz, xy + wz, xz - wy, 0.0f,
		xy - wz, 1.0f - xx - zz, yz + wx, 0.0f,
		xz + wy, yz - wx, 1.0f - xx - yy, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Quaternion IF::quaternion(const Matrix& m)
{
	Quaternion r;
	float t = m.m[0][0] + m.m[1][1] + m.m[2][2] + m.m[3][3];

	if (t >= 1.0f)
	{
		float f = 2.0f * sqrt(t);
		r.x = (m.m[1][2] - m.m[2][1]) / f;
		r.y = (m.m[2][0] - m.m[0][2]) / f;
		r.x = (m.m[0][1] - m.m[1][0]) / f;
		r.x = f / 4.0f;
		return r;
	}
	int i = 0;
	if (m.m[0][0] <= m.m[1][1])i = 1;
	if (m.m[2][2] <= m.m[i][i])i = 2;
	int j = (i + 1) % 3;
	int k = (j + 1) % 3;
	t = m.m[i][i] - m.m[j][j] - m.m[k][k] + 1.0f;
	float D = 2.0f * sqrt(t);
	float q[4];
	q[i] = D / 4.0f;
	q[j] = (m.m[j][i] + m.m[i][j]) / D;
	q[k] = (m.m[k][i] + m.m[i][k]) / D;
	q[3] = (m.m[j][k] - m.m[k][j]) / D;
	r.x = q[0];
	r.y = q[1];
	r.z = q[2];
	r.w = q[3];
	return r;
}

Vector3 IF::GetAxis(const Quaternion& q)
{
	Vector3 r;
	float x = q.x;
	float y = q.y;
	float z = q.z;
	float l = Length(q);

	r.x = x / l;
	r.y = y / l;
	r.z = z / l;

	return r;
}

float IF::GetRadian(const Quaternion& q)
{
	return 2 * acos(q.w);
}

