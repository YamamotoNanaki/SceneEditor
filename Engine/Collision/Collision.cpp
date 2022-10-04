#include "Collision.h"

using namespace IF;

bool IF::Collision::CheckSpherePlane(Primitive& sphere, Primitive& plane, Vector3* inter)
{
	float distV = Vector3Dot(sphere.GetCenter(), plane.GetCenter());
	float dist = distV - plane.GetRadius();
	if (fabsf(dist) > sphere.GetRadius())return false;

	if (inter)*inter = -dist * plane.GetCenter() + sphere.GetCenter();

	return true;
}

bool IF::Collision::CheckRaySphere(Primitive& ray, Primitive& sphere, float* distance, Vector3* inter)
{
	Vector3 rc = ray.GetCenter();
	Vector3 rd = ray.GetDir();
	float sr = sphere.GetRadius();
	Vector3 m = rc - sphere.GetCenter();
	float b = Vector3Dot(m, rd);
	float c = Vector3Dot(m, m) - sr * sr;

	if (c > 0.0f && b > 0.0f)return false;
	float discr = b * b - c;
	if (discr < 0.0f)return false;

	float t = -b - sqrtf(discr);
	if (t < 0.0f)t = 0.0f;

	if (distance)*distance = t;
	if (inter)
	{
		inter->x = rc.x + t * rd.x;
		inter->y = rc.y + t * rd.y;
		inter->z = rc.z + t * rd.z;
	}
	if (t > sr)return false;
	return true;
}

bool IF::Collision::CheckSphereSphere(Primitive& s1, Primitive& s2)
{
	Vector3 v1 = s1.GetCenter();
	Vector3 v2 = s2.GetCenter();
	float r1 = s2.GetRadius();
	float r2 = s2.GetRadius();
	if ((v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y) + (v2.z - v1.z) * (v2.z - v1.z) <= (r2 + r1) * (r2 + r1))
		return true;
	return false;
}
