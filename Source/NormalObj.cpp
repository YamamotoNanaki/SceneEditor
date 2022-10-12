#include "NormalObj.h"
#include "imgui.h"
#include "ObjectManager.h"
#include "CameraManager.h"
#include "Collision.h"
#include "Sound.h"

using namespace IF;
using namespace std;

const std::string Normal::objName = "Normal";


void IF::Normal::Initialize(Model* model, bool prefab)
{
	obj.Initialize(model);
	this->prefab = prefab;
}

void IF::Normal::Update()
{
	if (colision != nullptr)
	{
		if (ptype == NotPri);
		else if (ptype == RayPri)
		{
			colision->SetCenter(SetVector3(obj.position));
			colision->SetDir(SetVector3({ obj.position.x + obj.scale.x, obj.position.y + obj.scale.y, obj.position.z + obj.scale.z + 3 }));
		}
		else
		{
			colision->SetCenter(SetVector3({ obj.position.x,obj.position.y + 1,obj.position.z }));
			colision->SetRadius((obj.scale.x + obj.scale.y + obj.scale.z) / 3.0f);
		}
	}
	MatUpdate();
}

void IF::Normal::Draw()
{
	if (prefab)return;
	if (texNum == 0)obj.Draw(viewport);
	else obj.Draw(viewport, texNum);
}

