#include "ObjectManager.h"
#include <cassert>
#include "imgui.h"

using namespace IF;

IF::ObjectManager::~ObjectManager()
{
	for (auto com : objList)delete com;
	objList.clear();
	assert(objList.empty() && "インスタンスを破棄できません");
}

IF::ObjectManager* IF::ObjectManager::Instance()
{
	static ObjectManager* inst = new ObjectManager;
	return inst;
}

void IF::ObjectManager::DeleteInstance()
{
	delete ObjectManager::Instance();
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
	ChangePushback("Reticle");
}

Primitive* IF::ObjectManager::GetPrimitive(std::string tag)
{
	for (auto com : objList)
	{
		if (com->tag == tag)
		{
			return com->GetPrimitive();
		}
	}
	return nullptr;
}

void IF::ObjectManager::InputJson(nlohmann::json& j)
{

}

#ifdef _DEBUG
void IF::ObjectManager::GUI()
{
	auto buff = objList;
	for (auto com : buff)
	{
		if (ImGui::TreeNode(com->tag.c_str())) {
			com->GUI();
			if (ImGui::Button("Delete"))
			{
				objList.remove(com);
				delete com;
			}
			ImGui::TreePop();
		}
	}
}

std::string IF::ObjectManager::GUIRadio()
{
	int i = 0;
	static int mode = 0;
	std::string r;
	for (auto com : objList)
	{
		if (i % 3)ImGui::SameLine();
		ImGui::RadioButton(com->tag.c_str(), &mode, i);
		if (mode == i)
		{
			r = com->tag;
		}
		i++;
	}
	return r;
}

void IF::ObjectManager::OutputJson(nlohmann::json& j)
{
	int i = 0;
	//if (camera->tag == "debug")j["object"]["camera"] = "mainCamera";
	//else j["object"]["camera"] = camera->tag;
	for (auto com : objList)
	{
		j["object"]["object"][i]["AI"] = com->GetAi();
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
		j["object"]["object"][i]["collision"] = (int)com->GetCollision();
		j["object"]["object"][i]["prefab"] = com->GetPrefab();
		i++;
	}
}
#endif

void IF::ObjectManager::Delete(std::string tag)
{
	for (auto com : objList)
	{
		if (com->tag == tag)
		{
			objList.remove(com);
			delete com;
			return;
		}
	}
}
