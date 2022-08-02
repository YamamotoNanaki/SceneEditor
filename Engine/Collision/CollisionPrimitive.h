#pragma once
#include "IFMath.h"

namespace IF
{
	enum primitiveType
	{
		SpherePri, PlanePri, RayPri,
		NotPri = 65535
	};
	struct Primitive
	{
		virtual void SetCenter(Vector3 c) = 0;
		virtual void SetDir(Vector3 d) = 0;
		virtual void SetRadius(float r) = 0;
	};
	struct Sphere :public Primitive
	{
		Vector3 center = { 0,0,0 };
		float radius = 1.0f;
		inline void SetCenter(Vector3 c)
		{
			center = c;
		}
		inline void SetDir(Vector3 d) {}
		inline void SetRadius(float r)
		{
			radius = r;
		}
	};
	struct Plane :public Primitive
	{
		Vector3 normal = { 0,1,0 };
		float distance = 0.0f;
		inline void SetCenter(Vector3 c)
		{
			normal = c;
		}
		inline void SetDir(Vector3 d) {}
		inline void SetRadius(float r)
		{
			distance = r;
		}
	};
	struct Ray :public Primitive
	{
		Vector3 start = { 0,0,0 };
		Vector3 dir = { 1,0,0 };
		inline void SetCenter(Vector3 c)
		{
			start = c;
		}
		inline void SetDir(Vector3 d)
		{
			dir = d;
		}
		inline void SetRadius(float r) {}
	};
}

