#pragma once

namespace IF
{
	struct Float2
	{
		float x;
		float y;

		Float2(float x = 0.0f, float y = 0.0f);
		void Set(float x, float y);
		Float2 operator+()const;
		Float2 operator-()const;
		Float2 operator*()const;
		Float2& operator+=(Float2 f);
		Float2& operator-=(Float2 f);
		Float2& operator*=(Float2 f);
		Float2& operator/=(Float2 f);
		Float2& operator*=(float f);
		Float2& operator/=(float f);
	};
};
