#pragma once
#include "IFMath.h"

namespace IF
{
	enum primitiveType
	{
		SpherePri, PlanePri, RayPri, BoxPri,
		NotPri = 65535
	};
	struct Primitive
	{
		Vector3 v1 = { 0,0,0 };
		float f = 1.0f;
		Vector3 v2 = { 1,0,0 };
	private:
		inline void SetVector1(Vector3 v)
		{
			v1 = v;
		}
		inline void SetVector2(Vector3 v)
		{
			v2 = v;
		}
		inline void SetFloat(float f)
		{
			this->f = f;
		}
		inline Vector3 GetVector1()
		{
			return v1;
		}
		inline Vector3 GetVector2()
		{
			return v2;
		}
		inline float GetFloat()
		{
			return f;
		}
	public:
		inline void SetCenter(Vector3 c)
		{
			SetVector1(c);
		}
		inline void SetRadius(float r)
		{
			SetFloat(r);
		}
		inline const Vector3 GetCenter()
		{
			return GetVector1();
		}
		inline const float GetRadius()
		{
			return GetFloat();
		}
		//inline void SetCenter(Vector3 c)
		//{
		//	SetVector1(c);
		//}
		//inline void SetRadius(float r)
		//{
		//	SetFloat(r);
		//}
		//inline const Vector3 GetCenter()
		//{
		//	return GetVector1();
		//}
		//inline const float GetRadius()
		//{
		//	return GetFloat();
		//}
		//inline void SetCenter(Vector3 c)
		//{
		//	SetVector1(c);
		//}
		inline void SetDir(Vector3 d)
		{
			SetVector2(d);
		}
		//inline const Vector3 GetCenter()
		//{
		//	return GetVector1();
		//}
		inline const Vector3 GetDir()
		{
			return GetVector2();
		}
		inline void SetMinPos(Vector3 min)
		{
			SetVector1(min);
		}
		inline void SetMaxPos(Vector3 max)
		{
			SetVector2(max);
		}
		inline const Vector3 GetMinPos()
		{
			return GetVector1();
		}
		inline const Vector3 GetMaxPos()
		{
			return GetVector2();
		}
	};
	struct Sphere :public Primitive
	{

	};
	struct Plane :public Primitive
	{

	};
	struct Ray :public Primitive
	{

	};
	struct Box :public Primitive
	{

	};
}

