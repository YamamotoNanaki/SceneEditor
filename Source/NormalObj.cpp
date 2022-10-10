#include "NormalObj.h"
#include "imgui.h"
#include "ObjectManager.h"
#include "CameraManager.h"
#include "Collision.h"
#include "Sound.h"

using namespace IF;
using namespace std;

vector<D3D12_VIEWPORT>Normal::viewport;
const std::string Normal::objName = "Normal";


void IF::Normal::DeleteObj()
{
	if (deleteFlag)
	{
		ObjectManager::Instance()->Delete(tag);
	}
}

void IF::Normal::MatInitialize(Matrix* matView, Matrix* matProjection, Float3* cameraPos, BillBoard::BillBoardMode mode)
{
	SetView(matView);
	SetProjection(matProjection);
	SetCamera(cameraPos);
	SetBillBoard(mode);
}

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
	obj.Update(*matView, *matProjection, *cameraPos, mode);
}

void IF::Normal::Draw()
{
	if (prefab)return;
	if (texNum == 0)obj.Draw(viewport);
	else obj.Draw(viewport, texNum);
}


#ifdef _DEBUG
void IF::Normal::GUI()
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
		ImGui::RadioButton("Not", &type, NotPri);
		ImGui::TreePop();
		if (type != old)ptype = type;
	}
}
#endif