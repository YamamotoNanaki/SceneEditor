#pragma once
#include "CollisionPrimitive.h"

namespace IF
{
	class Collision
	{
	public:
		static bool CheckSpherePlane(const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);
		static bool CheckRaySphere(const Ray& ray, const Sphere& sphere, float* distance = nullptr, Vector3* inter = nullptr);
		static bool CheckRayPlane(const Ray& ray, const Plane& plane, float* distance = nullptr, Vector3* inter = nullptr);
		static bool CheckRayTriangle(const Ray& ray, const Triangle& triangle, float* distance = nullptr, Vector3* inter = nullptr);
		//static bool CheckSphere(const Sphere& s1, const Sphere& s2, Vector3* inter = nullptr);
		static bool CheckSphere(const Sphere& sphereA, const Sphere& sphereB, Vector3* inter = nullptr, Vector3* reject = nullptr);
		//static bool CheckSphereTriangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter = nullptr);
		static bool CheckSphereTriangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter = nullptr, Vector3* reject = nullptr);
		//static bool CheckAABB(const Primitive& box1, const Primitive& box2);
		//static bool CheckCircleXY(const Primitive& Circle1, const Primitive& Circle2);
		//static bool CheckCircleXYAABB(const Primitive& Circle, const Primitive& box);
	private:
		static void ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle, Vector3* closest);
	};
}
