#include "PlaneObj.h"
#include "CollisionManager.h"
#include "PlaneCollider.h"

using namespace IF;
using namespace std;

//’†‚Éì‚Á‚½ƒNƒ‰ƒX‚Æ“¯‚¶–¼‘O‚ğ‘‚­
const std::string PlaneObj::objName = "PlaneObj";

void IF::PlaneObj::ClassInitialize()
{
	SetCollider(new PlaneCollider());
}

void IF::PlaneObj::ClassUpdate()
{
	obj.SetBright(255, 255, 255);
	PlaneCollider* p = dynamic_cast<PlaneCollider*>(collider);
	p->SetDistance(obj.position.y);
	collider->Update();
}

void IF::PlaneObj::OnCollision(const CollisionInfo& info)
{
	obj.SetBright(255, 0, 0);
}
