#pragma once
#include "CollisionPrimitive.h"

namespace IF
{
	class Collision
	{
	public:
		static bool CheckSpherePlane(const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);
		static bool CheckRaySphere(const Ray& ray, const Sphere& sphere, float* distance = nullptr, Vector3* inter = nullptr);
	};
}
