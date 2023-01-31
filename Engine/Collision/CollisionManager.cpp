#include "CollisionManager.h"
#include "BaseCollider.h"
#include "Collision.h"
#include "MeshCollider.h"

using namespace IF;
using namespace std;

CollisionManager* IF::CollisionManager::Instance()
{
	static CollisionManager instance;
	return &instance;
}

void IF::CollisionManager::CheckAllCollisions()
{
	forward_list<BaseCollider*>::iterator itA;
	forward_list<BaseCollider*>::iterator itB;

	itA = colliders.begin();
	for (; itA != colliders.end(); ++itA)
	{
		itB = itA;
		++itB;
		for (; itB != colliders.end(); ++itB)
		{
			BaseCollider* colA = *itA;
			BaseCollider* colB = *itB;

			//ともに球
			if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE && colB->GetShapeType() == COLLISIONSHAPE_SPHERE)
			{
				Sphere* SphereA = dynamic_cast<Sphere*>(colA);
				Sphere* SphereB = dynamic_cast<Sphere*>(colB);
				Vector3 inter;
				if (Collision::CheckSphere(*SphereA, *SphereB, &inter))
				{
					colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
				}
			}
			else if (colA->GetShapeType() == COLLISIONSHAPE_MESH && colB->GetShapeType() == COLLISIONSHAPE_SPHERE)
			{
				MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);
				Sphere* sphere = dynamic_cast<Sphere*>(colB);
				Vector3 inter;
				if (meshCollider->CheckCollisionSphere(*sphere, &inter))
				{
					colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
				}
			}
			else if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE && colB->GetShapeType() == COLLISIONSHAPE_MESH)
			{
				MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colB);
				Sphere* sphere = dynamic_cast<Sphere*>(colA);
				Vector3 inter;
				if (meshCollider->CheckCollisionSphere(*sphere, &inter))
				{
					colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
				}
			}
			else if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE && colB->GetShapeType() == COLLISIONSHAPE_PLANE)
			{
				Sphere* sphere = dynamic_cast<Sphere*>(colA);
				Plane* plane = dynamic_cast<Plane*>(colB);
				Vector3 inter;
				if (Collision::CheckSpherePlane(*sphere, *plane, &inter))
				{
					colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
				}
			}
			else if (colA->GetShapeType() == COLLISIONSHAPE_PLANE && colB->GetShapeType() == COLLISIONSHAPE_SPHERE)
			{
				Sphere* sphere = dynamic_cast<Sphere*>(colB);
				Plane* plane = dynamic_cast<Plane*>(colA);
				Vector3 inter;
				if (Collision::CheckSpherePlane(*sphere, *plane, &inter))
				{
					colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
				}
			}
			else if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE && colB->GetShapeType() == COLLISIONSHAPE_TRIANGLE)
			{
				Sphere* sphere = dynamic_cast<Sphere*>(colA);
				Triangle* triangle = dynamic_cast<Triangle*>(colB);
				Vector3 inter;
				if (Collision::CheckSphereTriangle(*sphere, *triangle, &inter))
				{
					colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
				}
			}
			else if (colA->GetShapeType() == COLLISIONSHAPE_TRIANGLE && colB->GetShapeType() == COLLISIONSHAPE_SPHERE)
			{
				Sphere* sphere = dynamic_cast<Sphere*>(colB);
				Triangle* triangle = dynamic_cast<Triangle*>(colA);
				Vector3 inter;
				if (Collision::CheckSphereTriangle(*sphere, *triangle, &inter))
				{
					colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter));
					colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter));
				}
			}
			else if (colA->GetShapeType() == COLLISIONSHAPE_RAY && colB->GetShapeType() == COLLISIONSHAPE_PLANE)
			{
				Plane* plane = dynamic_cast<Plane*>(colB);
				Ray* ray = dynamic_cast<Ray*>(colA);
				Vector3 inter;
				float distance;
				if (Collision::CheckRayPlane(*ray, *plane, &distance, &inter))
				{
					colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter, &distance));
					colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter, &distance));
				}
			}
			else if (colA->GetShapeType() == COLLISIONSHAPE_PLANE && colB->GetShapeType() == COLLISIONSHAPE_RAY)
			{
				Plane* plane = dynamic_cast<Plane*>(colA);
				Ray* ray = dynamic_cast<Ray*>(colB);
				Vector3 inter;
				float distance;
				if (Collision::CheckRayPlane(*ray, *plane, &distance, &inter))
				{
					colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter, &distance));
					colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter, &distance));
				}
			}
			else if (colA->GetShapeType() == COLLISIONSHAPE_RAY && colB->GetShapeType() == COLLISIONSHAPE_TRIANGLE)
			{
				Triangle* triangle = dynamic_cast<Triangle*>(colB);
				Ray* ray = dynamic_cast<Ray*>(colA);
				Vector3 inter;
				float distance;
				if (Collision::CheckRayTriangle(*ray, *triangle, &distance, &inter))
				{
					colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter, &distance));
					colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter, &distance));
				}
			}
			else if (colA->GetShapeType() == COLLISIONSHAPE_TRIANGLE && colB->GetShapeType() == COLLISIONSHAPE_RAY)
			{
				Triangle* triangle = dynamic_cast<Triangle*>(colA);
				Ray* ray = dynamic_cast<Ray*>(colB);
				Vector3 inter;
				float distance;
				if (Collision::CheckRayTriangle(*ray, *triangle, &distance, &inter))
				{
					colA->OnCollision(CollisionInfo(colB->GetObject3d(), colB, inter, &distance));
					colB->OnCollision(CollisionInfo(colA->GetObject3d(), colA, inter, &distance));
				}
			}
		}
	}
}

bool IF::CollisionManager::Raycast(const Ray& ray, RaycastHit* hitInfo, float maxDistance)
{
	return Raycast(ray, 0xffff, hitInfo, maxDistance);
}

bool IF::CollisionManager::Raycast(const Ray& ray, unsigned short attribute, RaycastHit* hitInfo, float maxDistance)
{
	bool result = false;
	std::forward_list<BaseCollider*>::iterator it;
	std::forward_list<BaseCollider*>::iterator it_hit;
	float distance = maxDistance;
	Vector3 inter;

	// 全てのコライダーと総当りチェック
	it = colliders.begin();
	for (; it != colliders.end(); ++it) {
		BaseCollider* colA = *it;

		// 属性が合わなければスキップ
		if (!(colA->attribute & attribute)) {
			continue;
		}

		if (colA->GetShapeType() == COLLISIONSHAPE_SPHERE) {
			Sphere* sphere = dynamic_cast<Sphere*>(colA);

			float tempDistance;
			Vector3 tempInter;

			if (!Collision::CheckRaySphere(ray, *sphere, &tempDistance, &tempInter)) continue;
			if (tempDistance >= distance) continue;

			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = it;
		}
		else if (colA->GetShapeType() == COLLISIONSHAPE_MESH) {
			MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(colA);

			float tempDistance;
			Vector3 tempInter;
			if (!meshCollider->CheckCollisionRay(ray, &tempDistance, &tempInter)) continue;
			if (tempDistance >= distance) continue;

			result = true;
			distance = tempDistance;
			inter = tempInter;
			it_hit = it;
		}
	}

	if (result && hitInfo) {
		hitInfo->distance = distance;
		hitInfo->inter = inter;
		hitInfo->collider = *it_hit;
		hitInfo->object = hitInfo->collider->GetObject3d();
	}

	return result;
}

void IF::CollisionManager::QuerySphere(const Sphere& sphere, QueryCallback* callback, unsigned short attribute)
{
	assert(callback);

	std::forward_list<BaseCollider*>::iterator it;

	// 全てのコライダーと総当りチェック
	it = colliders.begin();
	for (; it != colliders.end(); ++it) {
		BaseCollider* col = *it;

		// 属性が合わなければスキップ
		if (!(col->attribute & attribute)) {
			continue;
		}

		// 球
		if (col->GetShapeType() == COLLISIONSHAPE_SPHERE) {
			Sphere* sphereB = dynamic_cast<Sphere*>(col);

			Vector3 tempInter;
			Vector3 tempReject;
			if (!Collision::CheckSphere(sphere, *sphereB, &tempInter, &tempReject)) continue;

			// 交差情報をセット
			QueryHit info;
			info.collider = col;
			info.object = col->GetObject3d();
			info.inter = tempInter;
			info.reject = tempReject;

			// クエリーコールバック呼び出し
			if (!callback->OnQueryHit(info)) {
				// 戻り値がfalseの場合、継続せず終了
				return;
			}
		}
		// メッシュ
		else if (col->GetShapeType() == COLLISIONSHAPE_MESH) {
			MeshCollider* meshCollider = dynamic_cast<MeshCollider*>(col);

			Vector3 tempInter;
			Vector3 tempReject;
			if (!meshCollider->CheckCollisionSphere(sphere, &tempInter, &tempReject)) continue;

			// 交差情報をセット
			QueryHit info;
			info.collider = col;
			info.object = col->GetObject3d();
			info.inter = tempInter;
			info.reject = tempReject;

			// クエリーコールバック呼び出し
			if (!callback->OnQueryHit(info)) {
				// 戻り値がfalseの場合、継続せず終了
				return;
			}
		}
	}
}
