#include "TriangleObj.h"
#include "CollisionManager.h"
#include "TriangleCollider.h"
#include "CollisionAttribute.h"

using namespace IF;
using namespace std;

//中に作ったクラスと同じ名前を書く
const std::string TriangleObj::objName = "TriangleObj";

void IF::TriangleObj::ClassInitialize()
{
	SetCollider(new TriangleCollider({ 2,2,2 }, { 2, -2, 0 }, { -2, 0, -2 }));
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);
}

void IF::TriangleObj::ClassUpdate()
{
	if (!rayHit)obj.SetBright(255, 255, 255);
	collider->Update();
	rayHit = false;
}

void IF::TriangleObj::OnCollision(const CollisionInfo& info)
{
	obj.SetBright(0, 255, 0);
}
