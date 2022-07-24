#pragma once
#include "CollisionPrimitive.h"

namespace IF
{
	class Collision
	{
	public:
		static bool CheckSpherePlane(const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);
	};
}
