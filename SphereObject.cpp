#include "SphereObject.h"
#include "SphereCollider.h"
#include "CollisionManager.h"

using namespace IF;
using namespace std;

//中に作ったクラスと同じ名前を書く
const std::string SphereObject::objName = "SphereObject";

void IF::SphereObject::ClassInitialize()
{
	SetCollider(new SphereCollider({ 0,0,0 }, obj.scale.x));
}

void IF::SphereObject::ClassUpdate()
{
	obj.SetBright(255, 255, 255);
	UpdateWorldMatrix();
	collider->Update();
}

void IF::SphereObject::OnCollision(const CollisionInfo& info)
{
	obj.SetBright(255, 0, 0);
}
