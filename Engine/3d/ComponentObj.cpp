#include "ComponentObj.h"
#include "imgui.h"
#include "ObjectManager.h"

using namespace IF;
using namespace std;

vector<D3D12_VIEWPORT>CObject::viewport;

CObject::~CObject()
{ 
	CObjDelete();
}

void IF::CObject::DeleteObj()
{
	if (deleteFlag)
	{
		ObjectManager::Instance()->Delete(tag);
	}
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
		ImGui::RadioButton("Not", &type, NotPri);
		ImGui::TreePop();
		if (type != old)
		{
			ptype = type;
			SetCollision(type);
		}
	}
}
#endif