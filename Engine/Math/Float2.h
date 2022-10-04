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
		void operator+=(Float2 f);
		void operator-=(Float2 f);
		void operator*=(Float2 f);
		void operator/=(Float2 f);
		void operator*=(float f);
		void operator/=(float f);
	};
};
