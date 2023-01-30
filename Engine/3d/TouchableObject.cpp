#include "TouchableObject.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"

void IF::TouchableObject::Initialize(Model* model, bool prefab)
{
	obj.Initialize(model);
	this->prefab = prefab;
	ClassInitialize();

	MeshCollider* collider = new MeshCollider;
	SetCollider(collider);
	collider->ConstructTriangle(model);
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);
}
