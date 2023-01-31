#include "ComponentObj.h"
#include "GUI.h"
#include "ObjectManager.h"
#include "ModelManager.h"
#include "CollisionManager.h"
#include "BaseCollider.h"

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
	if (prefab)return;

	ClassUpdate();
	MatUpdate();
	if (collider)collider->Update();
}

void IF::CObject::Draw()
{
	if (prefab)return;
	if (texNum == 0)obj.Draw();
	else obj.Draw(texNum);
}

void IF::CObject::OutLineDraw()
{
	if (prefab)return;
	obj.OutLineDraw();
}

CObject::~CObject()
{
	if (collider)
	{
		CollisionManager::Instance()->RemoveCollider(collider);
		delete collider;
	}
}

void IF::CObject::DebugUpdate()
{
	MatUpdate();
}

void IF::CObject::SetCollider(BaseCollider* collider)
{
	collider->SetObject(this);
	this->collider = collider;
	CollisionManager::Instance()->AddCollider(collider);
	UpdateWorldMatrix();
	collider->Update();
}

static float offset = 0.2f;
void IF::CObject::ClassUpdate() {}
Matrix IF::CObject::GetMatWorld()
{
	return obj.matWorld;
}
void IF::CObject::ClassInitialize() {}

bool IF::CObject::WeightSaving(float max)
{
	float a = cameraPos->x - obj.position.x - obj.scale.x;
	float b = cameraPos->y - obj.position.y - obj.scale.y;
	float c = cameraPos->z - obj.position.z - obj.scale.z;
	if ((a<-max || a>max) && (b<-max || b>max) && (c<-max || c>max))return false;
	return true;
}

bool IF::CObject::WeightSavingXYZ(float maxX, float maxY, float maxZ)
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

void IF::CObject::ClassInputJson(nlohmann::json& j) {}
void IF::CObject::InputJson(nlohmann::json& j)
{
	obj.rotation = { j["rot"]["x"],j["rot"]["y"],j["rot"]["z"] };
	obj.position = { j["pos"]["x"],j["pos"]["y"],j["pos"]["z"] };
	obj.scale = { j["sca"]["x"],j["sca"]["y"],j["sca"]["z"] };
	obj.outLineColor = { j["outLineColor"]["x"],j["outLineColor"]["y"],j["outLineColor"]["z"],j["outLineColor"]["w"]};
	obj.outLineFlag = j["outLineFlag"];
	obj.outLineWidth = j["outLineSize"];
	obj.SetColorF(j["color"]["x"], j["color"]["y"], j["color"]["z"], j["color"]["w"]);
	ClassInputJson(j);
}
#ifdef _DEBUG
void IF::CObject::OutputJson(nlohmann::json& j)
{
	j["tag"] = tag;
	j["model"] = GetModelTag();
	j["pos"]["x"] = obj.position.x;
	j["pos"]["y"] = obj.position.y;
	j["pos"]["z"] = obj.position.z;
	j["rot"]["x"] = obj.rotation.x;
	j["rot"]["y"] = obj.rotation.y;
	j["rot"]["z"] = obj.rotation.z;
	j["sca"]["x"] = obj.scale.x;
	j["sca"]["y"] = obj.scale.y;
	j["sca"]["z"] = obj.scale.z;
	j["outLineFlag"] = obj.outLineFlag;
	j["outLineSize"] = obj.outLineWidth;
	j["outLineColor"]["x"] = obj.outLineColor.x;
	j["outLineColor"]["y"] = obj.outLineColor.y;
	j["outLineColor"]["z"] = obj.outLineColor.z;
	j["outLineColor"]["w"] = obj.outLineColor.w;
	j["color"]["x"] = GetColor().x;
	j["color"]["y"] = GetColor().y;
	j["color"]["z"] = GetColor().z;
	j["color"]["w"] = GetColor().w;
	j["BillBoard"] = (int)mode;
	j["prefab"] = prefab;
	ClassOutputJson(j);
}
void IF::CObject::GUI()
{
	static string m;
	GUI::CheckBox("prefab", &prefab);
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
	if (ImGui::TreeNode("OutLine"))
	{
		if (ImGui::Checkbox("OutLineFlag", &obj.outLineFlag));
		if (ImGui::TreeNode("OutLineSize"))
		{
			static float* s = &obj.outLineWidth;
			ImGui::DragFloat("outLineSize", s);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("OutLineColor"))
		{
			static float s[4] = { obj.outLineColor.x,obj.outLineColor.y,obj.outLineColor.z,obj.outLineColor.w };
			ImGui::ColorEdit4("", s);
			obj.outLineColor = { s[0], s[1], s[2], s[3] };
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}
	if (ImGui::CollapsingHeader("Change Model"))
	{
		m = ModelManager::Instance()->GUIRadio();
		if (GUI::ButtonString("Change"))
		{
			obj.SetModel(ModelManager::Instance()->GetModel(m));
		}
	}
	//if (ImGui::TreeNode("Collision"))
	//{
	//	static int type = NotPri;
	//	static int old = 0;
	//	type = old = ptype;
	//	ImGui::RadioButton("Ray", &type, RayPri);
	//	ImGui::SameLine();
	//	ImGui::RadioButton("Sphere", &type, SpherePri);
	//	ImGui::RadioButton("Plane", &type, PlanePri);
	//	ImGui::SameLine();
	//	ImGui::RadioButton("Box", &type, BoxPri);
	//	ImGui::RadioButton("CircleXY", &type, CircleXYPri);
	//	ImGui::SameLine();
	//	ImGui::RadioButton("Not", &type, NotPri);
	//	if (type != old)
	//	{
	//		ptype = type;
	//		SetCollision(type);
	//	}
	//	if (type != NotPri)
	//	{
	//		ImGui::Text("CollisionCheck");
	//		ImGui::Text("v1:%0.3f,%0.3f,%0.3f", collision->v1.x, collision->v1.y, collision->v1.z);
	//		ImGui::Text("v2:%0.3f,%0.3f,%0.3f", collision->v2.x, collision->v2.y, collision->v2.z);
	//		ImGui::Text("f:%0.3f", collision->f);
	//	}
	//	ImGui::TreePop();
	//}
	ClassUI();
}
void IF::CObject::ClassUI() {}
void IF::CObject::ClassOutputJson(nlohmann::json& j) {}
#endif