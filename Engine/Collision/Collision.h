#pragma once
#include "CollisionPrimitive.h"

namespace IF
{
	class Collision
	{
	public:
		static bool CheckSpherePlane(Primitive& sphere, Primitive& plane, Vector3* inter = nullptr);
		static bool CheckRaySphere(Primitive& ray, Primitive& sphere, float* distance = nullptr, Vector3* inter = nullptr);
		static bool CheckSphereSphere(Primitive& s1, Primitive& s2);
	};
}
