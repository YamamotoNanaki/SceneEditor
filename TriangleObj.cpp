#include "TriangleObj.h"
#include "CollisionManager.h"
#include "TriangleCollider.h"

using namespace IF;
using namespace std;

//’†‚Éì‚Á‚½ƒNƒ‰ƒX‚Æ“¯‚¶–¼‘O‚ð‘‚­
const std::string TriangleObj::objName = "TriangleObj";

void IF::TriangleObj::ClassInitialize()
{
	SetCollider(new TriangleCollider({ 2,2,2 }, { 2, -2, 0 }, { -2, 0, -2 }));
}

void IF::TriangleObj::ClassUpdate()
{
	obj.SetBright(255, 255, 255);
	collider->Update();
}

void IF::TriangleObj::OnCollision(const CollisionInfo& info)
{
	obj.SetBright(0, 255, 0);
}
