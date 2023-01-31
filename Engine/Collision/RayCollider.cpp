#include "RayCollider.h"

void IF::RayCollider::Update()
{// ワールド行列から座標を抽出
	const Matrix& matWorld = object->GetMatWorld();

	start.x = matWorld.m[3][0];
	start.y = matWorld.m[3][1];
	start.z = matWorld.m[3][2];
	Ray::start = start + offset;
	Ray::dir = dir;
}
