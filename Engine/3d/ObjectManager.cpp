#include "ObjectManager.h"
#include <cassert>
#include "imgui.h"

IF::ObjectManager::~ObjectManager()
{
	objList.clear();
	assert(objList.empty() && "インスタンスを破棄できません");
}

void IF::ObjectManager::Draw()
{
	for (auto com : objList)
	{
		com->Draw();
	}
}

void IF::ObjectManager::Update()
{
	auto buff = objList;
	for (auto com : buff)
	{
		com->Update();
	}
}

#ifdef _DEBUG
void IF::ObjectManager::GUI()
{
	for (auto com : objList)
	{

		if (ImGui::TreeNode(com->tag.c_str())) {
			if (ImGui::TreeNode("Position"))
			{
				float p[3] = { com->GetPos().x,com->GetPos().y,com->GetPos().z };
				ImGui::InputFloat3("", p);
				com->SetPos({ p[0],p[1],p[2] });
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Rotation"))
			{
				float r[3] = { ConvertToDegrees(com->GetRota().x),ConvertToDegrees(com->GetRota().y),ConvertToDegrees(com->GetRota().z) };
				ImGui::InputFloat3("", r);
				for (int i = 0; i < 3; i++)
				{
					if (r[i] >= 360)r[i] -= 360;
					if (r[i] < 0)r[i] += 360;
				}
				com->SetRota({ ConvertToRadians(r[0]),ConvertToRadians(r[1]), ConvertToRadians(r[2]) });
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Scale"))
			{
				float s[3] = { com->GetScale().x,com->GetScale().y,com->GetScale().z };
				ImGui::InputFloat3("", s);
				com->SetScale({ s[0], s[1], s[2] });
				ImGui::TreePop();
			}

			ImGui::TreePop();
		}
	}
}
#endif