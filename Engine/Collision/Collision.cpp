#include "Collision.h"

using namespace IF;

bool IF::Collision::CheckSpherePlane(const Sphere& sphere, const Plane& plane, Vector3* inter)
{
	float distV = Vector3Dot(sphere.center, plane.normal);
	float dist = distV - plane.distance;
	if (fabsf(dist) > sphere.radius)return false;

	if (inter)*inter = -dist * plane.normal + sphere.center;

	return true;
}

bool IF::Collision::CheckRaySphere(const Ray& ray, const Sphere& sphere, float* distance, Vector3* inter)
{
	Vector3 m = ray.start - sphere.center;
	float b = Vector3Dot(m, ray.dir);
	float c = Vector3Dot(m, m) - sphere.radius * sphere.radius;

	if (c > 0.0f && b > 0.0f)return false;
	float discr = b * b - c;
	if (discr < 0.0f)return false;

	float t = -b - sqrtf(discr);
	if (t < 0.0f)t = 0.0f;

	if (distance)*distance = t;
	if (inter)*inter = ray.start + t * ray.dir;
	return true;
}
