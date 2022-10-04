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
		virtual const Vector3 GetCenter() = 0;
		virtual const Vector3 GetDir() = 0;
		virtual const float GetRadius() = 0;
	};
	struct Sphere :public Primitive
	{
		Vector3 center = { 0,0,0 };
		float radius = 1.0f;
		inline void SetCenter(Vector3 c)override
		{
			center = c;
		}
		inline void SetDir(Vector3 d) {}
		inline void SetRadius(float r)
		{
			radius = r;
		}
		inline const Vector3 GetCenter() override
		{
			return center;
		}
		inline const Vector3 GetDir() override { return { 0,0,0 }; }
		inline const float GetRadius()
		{
			return radius;
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
		inline const Vector3 GetCenter()
		{
			return normal;
		}
		inline const Vector3 GetDir() { return { 0,0,0 }; }
		inline const float GetRadius() 
		{
			return distance;
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

		inline const Vector3 GetCenter()
		{
			return start;
		}
		inline const Vector3 GetDir()
		{
			return dir;
		}
		inline const float GetRadius() { return 0.0f; }
	};
}

