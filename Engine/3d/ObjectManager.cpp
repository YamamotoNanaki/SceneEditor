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

//#ifdef _DEBUG
void IF::ObjectManager::GUI()
{
	auto buff = objList;
	for (auto com : buff)
	{
		if (ImGui::TreeNode(com->tag.c_str())) {
			if (ImGui::TreeNode("Position"))
			{
				float p[3] = { com->GetPos().x,com->GetPos().y,com->GetPos().z };
				ImGui::DragFloat3("", p, 0.05f);
				com->SetPos({ p[0],p[1],p[2] });
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Rotation"))
			{
				float r[3] = { ConvertToDegrees(com->GetRota().x),ConvertToDegrees(com->GetRota().y),ConvertToDegrees(com->GetRota().z) };
				ImGui::DragFloat3("", r, 0.5f);
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
				ImGui::DragFloat3("", s, 0.05f);
				com->SetScale({ s[0], s[1], s[2] });
				ImGui::TreePop();
			}
			if (ImGui::Button("Delete"))
			{
				objList.remove(com);
				delete com;
			}
			ImGui::TreePop();
		}
	}
}
//#endif
void IF::ObjectManager::OutputJson(nlohmann::json& j)
{
	int i = 0;
	if (camera->tag == "debug")j["object"]["camera"] = "mainCamera";
	else j["object"]["camera"] = camera->tag;
	for (auto com : objList)
	{
		UsuallyObj* buff = dynamic_cast<UsuallyObj*>(com);
		PlayerObj* buff1 = dynamic_cast<PlayerObj*>(com);
		if (buff != nullptr)j["object"]["object"][i]["type"] = 0;
		else if (buff1 != nullptr) j["object"]["object"][i]["type"] = 1;
		else j["object"]["object"][i]["type"] = 2;
		j["object"]["object"][i]["tag"] = com->tag;
		j["object"]["object"][i]["model"] = com->GetModelTag();
		j["object"]["object"][i]["pos"]["x"] = com->GetPos().x;
		j["object"]["object"][i]["pos"]["y"] = com->GetPos().y;
		j["object"]["object"][i]["pos"]["z"] = com->GetPos().z;
		j["object"]["object"][i]["rot"]["x"] = com->GetRota().x;
		j["object"]["object"][i]["rot"]["y"] = com->GetRota().y;
		j["object"]["object"][i]["rot"]["z"] = com->GetRota().z;
		j["object"]["object"][i]["sca"]["x"] = com->GetScale().x;
		j["object"]["object"][i]["sca"]["y"] = com->GetScale().y;
		j["object"]["object"][i]["sca"]["z"] = com->GetScale().z;
		j["object"]["object"][i]["BillBoard"] = (int)com->GetBillBoard();
		i++;
	}
}