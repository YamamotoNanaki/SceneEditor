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

bool IF::Collision::CheckSphere(Primitive& s1, Primitive& s2)
{
	Vector3 v1 = s1.GetCenter();
	Vector3 v2 = s2.GetCenter();
	float r1 = s2.GetRadius();
	float r2 = s2.GetRadius();
	if ((v2.x - v1.x) * (v2.x - v1.x) + (v2.y - v1.y) * (v2.y - v1.y) + (v2.z - v1.z) * (v2.z - v1.z) <= (r2 + r1) * (r2 + r1))
		return true;
	return false;
}

bool IF::Collision::CheckAABB(const Primitive& obj1, const Primitive& obj2)
{
	return ((obj1.v2.x > obj2.v1.x) &&
		(obj1.v1.x < obj2.v2.x) &&
		(obj1.v2.y > obj2.v1.y) &&
		(obj1.v1.y < obj2.v2.y) &&
		(obj1.v2.z > obj2.v1.z) &&
		(obj1.v1.z < obj2.v2.z));
}

bool IF::Collision::CheckCircleXY(const Primitive& Circle1, const Primitive& Circle2)
{
	return ((Circle2.v1.x - Circle1.v1.x) * (Circle2.v1.x - Circle1.v1.x) +
		(Circle2.v1.y - Circle1.v1.y) * (Circle2.v1.y - Circle1.v1.y) <=
		(Circle1.f + Circle2.f) * (Circle1.f + Circle2.f));
}

bool IF::Collision::CheckCircleXYAABB(const Primitive& Circle, const Primitive& box)
{
	Primitive a;
	a.SetMinPos({ Circle.v1.x - Circle.f,Circle.v1.y - Circle.f,Circle.v1.z - Circle.f });
	a.SetMaxPos({ Circle.v1.x + Circle.f,Circle.v1.y + Circle.f,Circle.v1.z + Circle.f });
	if (CheckAABB(box, a))
	{
		if ((Circle.v1.x > box.v1.x) && (Circle.v1.x < box.v2.x) &&
			(Circle.v1.y > box.v2.y - Circle.f) && (Circle.v1.y < box.v1.y + Circle.f))return true;
		if ((Circle.v1.x > box.v1.x - Circle.f) && (Circle.v1.x < box.v2.x + Circle.f) &&
			(Circle.v1.y > box.v2.y) && (Circle.v1.y < box.v1.y))return true;
		if ((box.v1.x - Circle.v1.x) * (box.v1.x - Circle.v1.x) + (box.v2.y - Circle.v1.y) *
			(box.v2.y - Circle.v1.y) < Circle.f * Circle.f)return true;
		if ((box.v1.x - Circle.v1.x) * (box.v1.x - Circle.v1.x) + (box.v1.y - Circle.v1.y) *
			(box.v1.y - Circle.v1.y) < Circle.f * Circle.f)return true;
		if ((box.v2.x - Circle.v1.x) * (box.v2.x - Circle.v1.x) + (box.v2.y - Circle.v1.y) *
			(box.v2.y - Circle.v1.y) < Circle.f * Circle.f)return true;
		if ((box.v2.x - Circle.v1.x) * (box.v2.x - Circle.v1.x) + (box.v1.y - Circle.v1.y) *
			(box.v1.y - Circle.v1.y) < Circle.f * Circle.f)return true;
	}
	return false;
}
