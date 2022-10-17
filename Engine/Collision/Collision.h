#pragma once
#include "CollisionPrimitive.h"

namespace IF
{
	class Collision
	{
	public:
		static bool CheckSpherePlane(Primitive& sphere, Primitive& plane, Vector3* inter = nullptr);
		static bool CheckRaySphere(Primitive& ray, Primitive& sphere, float* distance = nullptr, Vector3* inter = nullptr);
		static bool CheckSphere(Primitive& s1, Primitive& s2);
		static bool CheckAABB(const Primitive& box1, const Primitive& box2);
		static bool CheckCircleXY(const Primitive& Circle1, const Primitive& Circle2);
	};
}
