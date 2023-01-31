#include "RayCastObj.h"
#include "imgui.h"
#include "ObjectManager.h"
#include "CameraManager.h"
#include "Collision.h"
#include "Sound.h"
#include "Input.h"
#include "SphereCollider.h"
#include "RaycastHit.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

using namespace IF;
using namespace std;

//中に作ったクラスと同じ名前を書く
const std::string RayCastObj::objName = "RayCastObj";

void IF::RayCastObj::ClassInitialize()
{
	float radius = obj.scale.x;
	SetCollider(new SphereCollider({ 0,1,0 }, radius));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
}

void IF::RayCastObj::ClassUpdate()
{
	UpdateWorldMatrix();
	collider->Update();

	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);

	// 球の上端から球の下端までのレイキャスト
	Ray ray;
	ray.start = sphereCollider->center;
	ray.dir = { 0,-1,0 };
	RaycastHit raycastHit;

	if (CollisionManager::Instance()->Raycast(ray, COLLISION_ATTR_ENEMYS,&raycastHit))
	{
		if (isOnce)
		{
			raycastHit.object->SetColor({ 1,0,0,1 });
			raycastHit.object->rayHit = true;
		}
		else
		{
			for (auto& itr : raycastHit.allobject)
			{
				itr->SetColor({ 1,0,0,1 });
				itr->rayHit = true;
			}
		}
		obj.SetBright(255, 0, 0);
	}
	else
	{
		obj.SetBright(255, 255, 255);
	}
}
