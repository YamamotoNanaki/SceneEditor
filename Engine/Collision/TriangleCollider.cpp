#include "TriangleCollider.h"

void IF::TriangleCollider::Update()
{
	// ワールド行列から座標を抽出
	const Matrix& matWorld = object->GetMatWorld();

	objPos.x = matWorld.m[3][0];
	objPos.y = matWorld.m[3][1];
	objPos.z = matWorld.m[3][2];
	Triangle::p0 = objPos + p0 + offset;
	Triangle::p1 = objPos + p1 + offset;
	Triangle::p2 = objPos + p2 + offset;
	Triangle::ComputeNormal();
}
