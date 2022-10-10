#include "Player.h"
#include "imgui.h"
#include "ObjectManager.h"
#include "CameraManager.h"
#include "Collision.h"
#include "Sound.h"

using namespace IF;
using namespace std;

vector<D3D12_VIEWPORT>Player::viewport;
const std::string Player::objName = "Player";


void IF::Player::GameUpdate()
{

}

void IF::Player::SetCollision()
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
}

void IF::Player::MatrixUpdate()
{
	obj.Update(*matView, *matProjection, *cameraPos, mode);
}

void IF::Player::IsEnemyHitNotWeak()
{
}

void IF::Player::IsEnemyHitWakPoint()
{
}

void IF::Player::CollisionObject()
{
}

void IF::Player::DeleteObj()
{
	if (deleteFlag)
	{
		ObjectManager::Instance()->Delete(tag);
	}
}

void IF::Player::MatInitialize(Matrix* matView, Matrix* matProjection, Float3* cameraPos, BillBoard::BillBoardMode mode)
{
	SetView(matView);
	SetProjection(matProjection);
	SetCamera(cameraPos);
	SetBillBoard(mode);
}

void IF::Player::Initialize(Model* model, bool prefab)
{
	obj.Initialize(model);
	this->prefab = prefab;
}

void IF::Player::Update()
{

	GameUpdate();
	SetCollision();
	MatrixUpdate();

}

void IF::Player::Draw()
{
	if (prefab)return;
	if (texNum == 0)obj.Draw(viewport);
	else obj.Draw(viewport, texNum);
}


#ifdef _DEBUG
void IF::Player::GUI()
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