#include "ComponentObj.h"
#include "ImGui.h"
#include "ObjectManager.h"

using namespace IF;
using namespace std;

void IF::CObject::Initialize(Model* model, bool prefab)
{
	obj.Initialize(model);
	this->prefab = prefab;
	ClassInitialize();
}

void IF::CObject::Update()
{
	ClassUpdate();
	MatUpdate();
}

void IF::CObject::Draw()
{
	if (prefab)return;
	if (texNum == 0)obj.Draw();
	else obj.Draw(texNum);
}

CObject::~CObject()
{
	CObjDelete();
}

void IF::CObject::DebugUpdate()
{
	MatUpdate();
}

static float offset = 0.2f;
void IF::CObject::ClassUpdate() {}
void IF::CObject::ClassInitialize() {}
void IF::CObject::CollisionUpdate()
{
	if (collision != nullptr)
	{
		//collision->SetRadius()
		if (ptype == NotPri);
		else if (ptype == RayPri)
		{
			collision->SetCenter(SetVector3(obj.position));
			collision->SetDir(SetVector3({ obj.position.x + obj.scale.x, obj.position.y + obj.scale.y, obj.position.z + obj.scale.z + 3 }));
		}
		else if (ptype == BoxPri)
		{
			collision->SetMinPos({ obj.position.x - (obj.scale.x) - offset,obj.position.y -
				(obj.scale.y) - offset,obj.position.z - (obj.scale.z) - offset });
			collision->SetMaxPos({ obj.position.x + (obj.scale.x) + offset,obj.position.y +
				(obj.scale.y) + offset,obj.position.z + (obj.scale.z) + offset });
		}
		else
		{
			collision->SetCenter(SetVector3({ obj.position.x,obj.position.y + 1,obj.position.z }));
			collision->SetRadius((obj.scale.x + obj.scale.y + obj.scale.z) / 3.0f);
		}
	}
}

bool IF::CObject::WeightSaving(float max)
{
	float a = cameraPos->x - obj.position.x - obj.scale.x;
	float b = cameraPos->y - obj.position.y - obj.scale.y;
	float c = cameraPos->z - obj.position.z - obj.scale.z;
	if ((a<-max || a>max) && (b<-max || b>max) && (c<-max || c>max))return false;
	return true;
}

bool IF::CObject::WeightSavingXYZ(float maxX,float maxY,float maxZ)
{
	float a = cameraPos->x - obj.position.x - obj.scale.x;
	float b = cameraPos->y - obj.position.y - obj.scale.y;
	float c = cameraPos->z - obj.position.z - obj.scale.z;
	if ((a<-maxX || a>maxX) && (b<-maxY || b>maxY) && (c<-maxZ || c>maxZ))return false;
	return true;
}

bool IF::CObject::DeleteObj()
{
	return deleteFlag;
}

void IF::CObject::MatInitialize(Matrix* matView, Matrix* matProjection, Float3* cameraPos, int mode)
{
	SetView(matView);
	SetProjection(matProjection);
	SetCamera(cameraPos);
	SetBillBoard(mode);
}

#ifdef _DEBUG
void IF::CObject::GUI()
{
	if (ImGui::TreeNode("Position"))
	{
		float p[3] = { obj.position.x,obj.position.y,obj.position.z };
		ImGui::DragFloat3("", p, 0.05f);
		obj.position = { p[0],p[1],p[2] };
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Rotation"))
	{
		float r[3] = { ConvertToDegrees(obj.rotation.x),ConvertToDegrees(obj.rotation.y),ConvertToDegrees(obj.rotation.z) };
		ImGui::DragFloat3("", r, 0.5f);
		for (int i = 0; i < 3; i++)
		{
			if (r[i] >= 360)r[i] -= 360;
			if (r[i] < 0)r[i] += 360;
		}
		obj.rotation = { ConvertToRadians(r[0]),ConvertToRadians(r[1]), ConvertToRadians(r[2]) };
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Scale"))
	{
		float s[3] = { obj.scale.x,obj.scale.y,obj.scale.z };
		ImGui::DragFloat3("", s, 0.05f);
		obj.scale = { s[0], s[1], s[2] };
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Color"))
	{
		static float s[4] = { obj.GetColor().x,obj.GetColor().y,obj.GetColor().z,obj.GetColor().w };
		ImGui::ColorEdit4("", s);
		obj.SetColorF(s[0], s[1], s[2], s[3]);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Collision"))
	{
		static int type = NotPri;
		static int old = 0;
		type = old = ptype;
		ImGui::RadioButton("Ray", &type, RayPri);
		ImGui::SameLine();
		ImGui::RadioButton("Sphere", &type, SpherePri);
		ImGui::RadioButton("Plane", &type, PlanePri);
		ImGui::SameLine();
		ImGui::RadioButton("Box", &type, BoxPri);
		ImGui::RadioButton("CircleXY", &type, CircleXYPri);
		ImGui::SameLine();
		ImGui::RadioButton("Not", &type, NotPri);
		if (type != old)
		{
			ptype = type;
			SetCollision(type);
		}
		if (type != NotPri)
		{
			ImGui::Text("CollisionCheck");
			ImGui::Text("v1:%0.3f,%0.3f,%0.3f", collision->v1.x, collision->v1.y, collision->v1.z);
			ImGui::Text("v2:%0.3f,%0.3f,%0.3f", collision->v2.x, collision->v2.y, collision->v2.z);
			ImGui::Text("f:%0.3f", collision->f);
		}
		ImGui::TreePop();
	}
}
#endif