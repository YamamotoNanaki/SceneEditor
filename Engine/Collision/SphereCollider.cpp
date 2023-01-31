#include "SphereCollider.h"
//#include "Object3d.h"

using namespace IF;

void SphereCollider::Update()
{
	// ワールド行列から座標を抽出
	const Matrix& matWorld = object->GetMatWorld();

	// 球のメンバ変数を更新
	Sphere::center.x = matWorld.m[3][0];
	Sphere::center.y = matWorld.m[3][1];
	Sphere::center.z = matWorld.m[3][2];
	Sphere::center += offset;
	Sphere::radius = radius;
}
