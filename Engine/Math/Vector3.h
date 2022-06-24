#pragma once
#include "Float3.h"

namespace IF
{
	class Vector3
	{
	public:
		float x;
		float y;
		float z;

	public:
		Vector3();
		Vector3(float x, float y, float z);
		void Set(float x, float y, float z);

		float Length() const;
		Vector3& Normalize();
		float Dot(const Vector3& v) const;
		Vector3& Cross(const Vector3& v)const;

		Vector3 operator+() const;
		Vector3 operator-() const;
		Vector3& operator+=(const Vector3& v);
		Vector3& operator-=(const Vector3& v);
		Vector3& operator*=(float s);
		Vector3& operator/=(float s);
		//Vector3& operator=(const Float3 f);
	};

	Vector3 VectorSubtract(Vector3 v1, Vector3 v2);
	Vector3 operator/(Vector3 v, float s);
};
