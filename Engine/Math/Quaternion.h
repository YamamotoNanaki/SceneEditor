#pragma once
#include "Vector3.h"
#include "Matrix.h"

namespace IF
{
	struct Quaternion
	{
		float x, y, z, w;
	};
	Quaternion quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f);
	Quaternion quaternion(const Vector3& v, float angle);
	float Dot(const Quaternion& q1, const Quaternion& q2);
	float Length(const Quaternion q);
	Quaternion normalize(const Quaternion& q);
	Quaternion Conjugate(const Quaternion& q);

	Quaternion operator+ (const Quaternion& q);
	Quaternion operator- (const Quaternion& q);

	Quaternion operator+= (Quaternion& q1, const Quaternion& q2);
	Quaternion operator-= (Quaternion& q1, const Quaternion& q2);
	Quaternion operator*= (Quaternion& q1, const Quaternion& q2);
	Quaternion operator*= (Quaternion& q, float s);
	Quaternion operator/= (Quaternion& q, float s);

	Quaternion operator+(const Quaternion& q1, const Quaternion& q2);
	Quaternion operator-(const Quaternion& q1, const Quaternion& q2);
	Quaternion operator*(const Quaternion& q1, const Quaternion& q2);
	Quaternion operator*(const Quaternion& q, float s);
	Quaternion operator*(float s, const Quaternion& q);
	Quaternion operator/(const Quaternion& q, float s);

	Quaternion SLerp(const Quaternion& q1, const Quaternion& q2, float t);
	Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t);
	Matrix Rotate(const Quaternion& q);
	Quaternion quaternion(const Matrix& m);
	Vector3 GetAxis(const Quaternion& q);
	/// <summary>
	/// 角度求める
	/// </summary>
	/// <param name="q">クオタニオン</param>
	/// <returns>rad</returns>
	float GetRadian(const Quaternion& q);
}
