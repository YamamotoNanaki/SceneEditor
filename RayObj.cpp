#include "RayObj.h"
#include "CollisionManager.h"
#include "RayCollider.h"

using namespace IF;
using namespace std;

//���ɍ�����N���X�Ɠ������O������
const std::string RayObj::objName = "RayObj";

void IF::RayObj::ClassInitialize()
{
	SetCollider(new RayCollider);
}

void IF::RayObj::ClassUpdate()
{
	obj.SetBright(255, 255, 255);
	collider->Update();
}

void IF::RayObj::OnCollision(const CollisionInfo& info)
{
	obj.SetBright(255, 0, 0);
}
