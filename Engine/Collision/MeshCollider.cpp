#include "MeshCollider.h"
#include "Collision.h"
#include "IFMath.h"

using namespace IF;
using namespace std;

void IF::MeshCollider::ConstructTriangle(Model* model)
{
	triangles.clear();
	std::vector<MVI*> meshes;
	meshes.push_back(model->GetMeshes());

	int start = 0;

	for (auto itr : meshes)
	{
		vector<unsigned int>& indices = itr->indices;
		vector<Vertex>& vertices = itr->vertices;
		size_t triangleNum = indices.size() / 3;
		triangles.resize(triangles.size() + triangleNum);
		for (int i = 0; i < triangleNum; i++)
		{
			Triangle& tri = triangles[start + i];
			int idx0 = indices[i * 3 + 0];
			int idx1 = indices[i * 3 + 1];
			int idx2 = indices[i * 3 + 2];

			tri.p0 = { vertices[idx0].pos.x,vertices[idx0].pos.y,vertices[idx0].pos.z };
			tri.p1 = { vertices[idx1].pos.x,vertices[idx1].pos.y,vertices[idx1].pos.z };
			tri.p2 = { vertices[idx2].pos.x,vertices[idx2].pos.y,vertices[idx2].pos.z };

			tri.ComputeNormal();
		}
		start += triangleNum;
	}
}

void IF::MeshCollider::Update()
{
	invMatWorld = MatrixInverse(GetObject3d()->GetMatWorld());
}

//bool IF::MeshCollider::CheckCollisionSphere(const Sphere& sphere, Vector3* inter)
//{
//	Sphere localSphere;
//	localSphere.center = Vector3Transform(sphere.center, invMatWorld);
//	localSphere.radius *= Vector3Length({ invMatWorld.m[0][0],invMatWorld.m[0][1],invMatWorld.m[0][2] });
//
//	vector<Triangle>::const_iterator it = triangles.cbegin();
//
//	for (; it != triangles.cend(); ++it)
//	{
//		const Triangle& triangle = *it;
//		if (Collision::CheckSphereTriangle(localSphere, triangle, inter))
//		{
//			if (inter)
//			{
//				const Matrix matWorld = GetObject3d()->GetMatWorld();
//				*inter = Vector3Transform(*inter, matWorld);
//			}
//			return true;
//		}
//	}
//	return false;
//}

bool IF::MeshCollider::CheckCollisionSphere(const Sphere& sphere, Vector3* inter, Vector3* reject)
{
	// オブジェクトのローカル座標系での球を得る（半径はXスケールを参照)
	Sphere localSphere;
	localSphere.center = Vector3Transform(sphere.center, invMatWorld);
	localSphere.radius *= Vector3Length({ invMatWorld.m[0][0],invMatWorld.m[0][1],invMatWorld.m[0][2] });

	std::vector<Triangle>::const_iterator it = triangles.cbegin();

	for (; it != triangles.cend(); ++it) {
		const Triangle& triangle = *it;

		if (Collision::CheckSphereTriangle(localSphere, triangle, inter, reject)) {
			if (inter) {
				const Matrix& matWorld = GetObject3d()->GetMatWorld();

				*inter = Vector3Transform(*inter, matWorld);
			}
			if (reject) {
				const Matrix& matWorld = GetObject3d()->GetMatWorld();

				*reject = Vector3TransformNormal(*reject, matWorld);
			}
			return true;
		}
	}

	return false;
}

bool IF::MeshCollider::CheckCollisionRay(const Ray& ray, float* distance, Vector3* inter)
{
	Ray localRay;
	localRay.start = Vector3Transform(ray.start, invMatWorld);
	localRay.dir = Vector3TransformNormal(ray.dir, invMatWorld);

	vector<Triangle>::const_iterator it = triangles.cbegin();

	int i = 0;
	for (; it != triangles.cend(); ++it)
	{
		if (i == 5 || i == 9) {
			static int a = 0;
			a++;
		}
		i++;

		Vector3 tempInter;
		const Triangle& triangle = *it;
		if (Collision::CheckRayTriangle(localRay, triangle, nullptr, &tempInter))
		{
			const Matrix matWorld = GetObject3d()->GetMatWorld();
			tempInter = Vector3Transform(tempInter, matWorld);

			if (distance)
			{
				Vector3 sub = tempInter - ray.start;
				*distance = Vector3Dot(sub, ray.dir);
			}

			if (inter)
			{
				*inter = tempInter;
			}

			return true;
		}
	}
	return false;
}
