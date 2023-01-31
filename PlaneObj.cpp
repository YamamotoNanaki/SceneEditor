#include "PlaneObj.h"
#include "CollisionManager.h"
#include "PlaneCollider.h"
#include "CollisionAttribute.h"

using namespace IF;
using namespace std;

//’†‚Éì‚Á‚½ƒNƒ‰ƒX‚Æ“¯‚¶–¼‘O‚ð‘‚­
const std::string PlaneObj::objName = "PlaneObj";

void IF::PlaneObj::ClassInitialize()
{
	SetCollider(new PlaneCollider());
	collider->SetAttribute(COLLISION_ATTR_ENEMYS);
}

void IF::PlaneObj::ClassUpdate()
{
	if (!rayHit)obj.SetBright(255, 255, 255);
	PlaneCollider* p = dynamic_cast<PlaneCollider*>(collider);
	p->SetDistance(obj.position.y);
	collider->Update();
	rayHit = false;
}

void IF::PlaneObj::OnCollision(const CollisionInfo& info)
{
	obj.SetBright(255, 0, 0);
}
