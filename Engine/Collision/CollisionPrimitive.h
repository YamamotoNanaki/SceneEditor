#pragma once
#include "IFMath.h"

namespace IF
{
	struct Sphere
	{
		Vector3 center = { 0,0,0 };
		float radius = 1.0f;
	};
	struct Plane
	{
		Vector3 normal = { 0,1,0 };
		float distance = 0.0f;
	};
}

