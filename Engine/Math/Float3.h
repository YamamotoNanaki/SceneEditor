#pragma once

namespace IF
{
	struct Float3
	{
		float x;
		float y;
		float z;

		Float3(float x = 0.0f, float y = 0.0f, float z = 0.0f);
		void Set(float x, float y, float z);
		Float3 operator+()const;
		Float3 operator-()const;
		Float3 operator*()const;
		Float3& operator+=(Float3 f);
		Float3& operator-=(Float3 f);
		Float3& operator*=(Float3 f);
		Float3& operator/=(Float3 f);
		Float3& operator*=(float f);
		Float3& operator/=(float f);
	};
};