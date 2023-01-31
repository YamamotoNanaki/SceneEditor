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
	if (inter)
	{
		inter->x = ray.start.x + t * ray.dir.x;
		inter->y = ray.start.y + t * ray.dir.y;
		inter->z = ray.start.z + t * ray.dir.z;
	}
	return true;
}

bool IF::Collision::CheckRayPlane(const Ray& ray, const Plane& plane, float* distance, Vector3* inter)
{
	const float epsilon = 1.0e-5f;
	float d1 = Vector3Dot(plane.normal, ray.dir);

	if (d1 > -epsilon)return false;

	float d2 = Vector3Dot(plane.normal, ray.start);
	float dist = d2 - plane.distance;
	float t = dist / -d1;
	if (t < 0)return false;

	if (distance)*distance = t;
	if (inter)*inter = ray.start + t * ray.dir;
	return true;
}

bool IF::Collision::CheckRayTriangle(const Ray& ray, const Triangle& triangle, float* distance, Vector3* inter)
{
	Plane plane;
	Vector3 interPlane;
	plane.normal = triangle.normal;
	plane.distance = Vector3Dot(triangle.normal, triangle.p0);

	if (!CheckRayPlane(ray, plane, distance, &interPlane))return false;

	const float epsilon = 1.0e-5f;

	Vector3 m;
	Vector3 pt_p0 = triangle.p0 - interPlane;
	Vector3 p0_p1 = triangle.p1 - triangle.p0;
	m = Vector3Cross(pt_p0, p0_p1);

	if (Vector3Dot(m, triangle.normal) < -epsilon)return false;

	Vector3 pt_p1 = triangle.p1 - interPlane;
	Vector3 p1_p2 = triangle.p2 - triangle.p1;
	m = Vector3Cross(pt_p1, p1_p2);

	if (Vector3Dot(m, triangle.normal) < -epsilon)return false;

	Vector3 pt_p2 = triangle.p2 - interPlane;
	Vector3 p2_p0 = triangle.p0 - triangle.p2;
	m = Vector3Cross(pt_p2, p2_p0);

	if (Vector3Dot(m, triangle.normal) < -epsilon)return false;

	if (inter)*inter = interPlane;

	return true;
}

bool IF::Collision::CheckSphere(const Sphere& sphereA, const Sphere& sphereB, Vector3* inter, Vector3* reject)
{
	// ���S�_�̋�����2�� <= ���a�̘a��2��@�Ȃ����
	float dist = Vector3Dot(sphereA.center - sphereB.center, sphereA.center - sphereB.center);

	float radius2 = sphereA.radius + sphereB.radius;
	radius2 *= radius2;

	if (dist <= radius2) {
		if (inter) {
			// A�̔��a��0�̎����W��B�̒��S�@B�̔��a��0�̎����W��A�̒��S�@�ƂȂ�悤�⊮
			float t = sphereB.radius / (sphereA.radius + sphereB.radius);
			*inter = VectorLerp(sphereA.center, sphereB.center, t);
		}
		// �����o���x�N�g�����v�Z
		if (reject) {
			float rejectLen = sphereA.radius + sphereB.radius - sqrtf(dist);
			*reject = Vector3Normalize(sphereA.center - sphereB.center);
			*reject *= rejectLen;
		}
		return true;
	}

	return false;
}

bool IF::Collision::CheckSphereTriangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter, Vector3* reject)
{
	Vector3 p;
	// ���̒��S�ɑ΂���ŋߐړ_�ł���O�p�`��ɂ���_p��������
	ClosestPtPoint2Triangle(sphere.center, triangle, &p);
	// �_p�Ƌ��̒��S�̍����x�N�g��
	Vector3 v = p - sphere.center;
	// �����̓������߂�
	//�i�����x�N�g�����m�̓��ς͎O�����̒藝�̃��[�g�����̎��ƈ�v����j
	float distanceSquare = Vector3Dot(v, v);
	// ���ƎO�p�`�̋��������a�ȉ��Ȃ瓖�����Ă��Ȃ�
	if (distanceSquare > sphere.radius * sphere.radius)	return false;
	// �[����_���v�Z
	if (inter) {
		// �O�p�`��̍ŋߐړ_p���^����_�Ƃ���
		*inter = p;
	}
	// �����o���x�N�g�����v�Z
	if (reject) {
		float ds = Vector3Dot(sphere.center, triangle.normal);
		float dt = Vector3Dot(triangle.p0, triangle.normal);
		float rejectLen = dt - ds + sphere.radius;
		*reject = triangle.normal * rejectLen;
	}
	return true;
}

//bool IF::Collision::CheckSphere(const Sphere& s1, const Sphere& s2, Vector3* inter)
//{
//	Vector3 vec = s2.center - s1.center;
//
//	float sqLength = vec.Length();
//	sqLength *= sqLength;
//
//	float r = s1.radius + s2.radius;
//
//	if (sqLength > r * r)return false;
//
//	if (inter)*inter = vec / 2;
//
//	return true;
//}

void IF::Collision::ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle, Vector3* closest)
{
	// point��p0�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	Vector3 p0_p1 = triangle.p1 - triangle.p0;
	Vector3 p0_p2 = triangle.p2 - triangle.p0;
	Vector3 p0_pt = point - triangle.p0;

	float d1 = Vector3Dot(p0_p1, p0_pt);
	float d2 = Vector3Dot(p0_p2, p0_pt);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		// p0���ŋߖT
		*closest = triangle.p0;
		return;
	}

	// point��p1�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	Vector3 p1_pt = point - triangle.p1;

	float d3 = Vector3Dot(p0_p1, p1_pt);
	float d4 = Vector3Dot(p0_p2, p1_pt);

	if (d3 >= 0.0f && d4 <= d3)
	{
		// p1���ŋߖT
		*closest = triangle.p1;
		return;
	}

	// point��p0_p1�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p1��ɑ΂���ˉe��Ԃ�
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}

	// point��p2�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	Vector3 p2_pt = point - triangle.p2;

	float d5 = Vector3Dot(p0_p1, p2_pt);
	float d6 = Vector3Dot(p0_p2, p2_pt);
	if (d6 >= 0.0f && d5 <= d6)
	{
		*closest = triangle.p2;
		return;
	}

	// point��p0_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p2��ɑ΂���ˉe��Ԃ�
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}

	// point��p1_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p1_p2��ɑ΂���ˉe��Ԃ�
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
}

//bool IF::Collision::CheckSphereTriangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter)
//{
//	Vector3 p;
//	ClosetPtPoint2Triangle(sphere.center, triangle, &p);
//	Vector3 v = p - sphere.center;
//
//	float d = Vector3Dot(v, v);
//	if (d > sphere.radius * sphere.radius)return false;
//	if (inter)*inter = p;
//	return true;
//}

//bool IF::Collision::CheckAABB(const Primitive& obj1, const Primitive& obj2)
//{
//	return ((obj1.v2.x > obj2.v1.x) &&
//		(obj1.v1.x < obj2.v2.x) &&
//		(obj1.v2.y > obj2.v1.y) &&
//		(obj1.v1.y < obj2.v2.y) &&
//		(obj1.v2.z > obj2.v1.z) &&
//		(obj1.v1.z < obj2.v2.z));
//}
//
//bool IF::Collision::CheckCircleXY(const Primitive& Circle1, const Primitive& Circle2)
//{
//	return ((Circle2.v1.x - Circle1.v1.x) * (Circle2.v1.x - Circle1.v1.x) +
//		(Circle2.v1.y - Circle1.v1.y) * (Circle2.v1.y - Circle1.v1.y) <=
//		(Circle1.f + Circle2.f) * (Circle1.f + Circle2.f));
//}
//
//bool IF::Collision::CheckCircleXYAABB(const Primitive& Circle, const Primitive& box)
//{
//	Primitive a;
//	a.SetMinPos({ Circle.v1.x - Circle.f,Circle.v1.y - Circle.f,Circle.v1.z - Circle.f });
//	a.SetMaxPos({ Circle.v1.x + Circle.f,Circle.v1.y + Circle.f,Circle.v1.z + Circle.f });
//	if (CheckAABB(box, a))
//	{
//		if ((Circle.v1.x > box.v1.x) && (Circle.v1.x < box.v2.x) &&
//			(Circle.v1.y > box.v2.y - Circle.f) && (Circle.v1.y < box.v1.y + Circle.f))return true;
//		if ((Circle.v1.x > box.v1.x - Circle.f) && (Circle.v1.x < box.v2.x + Circle.f) &&
//			(Circle.v1.y > box.v2.y) && (Circle.v1.y < box.v1.y))return true;
//		if ((box.v1.x - Circle.v1.x) * (box.v1.x - Circle.v1.x) + (box.v2.y - Circle.v1.y) *
//			(box.v2.y - Circle.v1.y) < Circle.f * Circle.f)return true;
//		if ((box.v1.x - Circle.v1.x) * (box.v1.x - Circle.v1.x) + (box.v1.y - Circle.v1.y) *
//			(box.v1.y - Circle.v1.y) < Circle.f * Circle.f)return true;
//		if ((box.v2.x - Circle.v1.x) * (box.v2.x - Circle.v1.x) + (box.v2.y - Circle.v1.y) *
//			(box.v2.y - Circle.v1.y) < Circle.f * Circle.f)return true;
//		if ((box.v2.x - Circle.v1.x) * (box.v2.x - Circle.v1.x) + (box.v1.y - Circle.v1.y) *
//			(box.v1.y - Circle.v1.y) < Circle.f * Circle.f)return true;
//	}
//	return false;
//}
