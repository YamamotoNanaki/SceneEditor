#include "ObjectManager.h"
#include "ModelManager.h"
#include <cassert>
#include "imgui.h"
#include "Debug.h"

using namespace IF;
using namespace std;

IF::ObjectManager::~ObjectManager()
{
	for (auto com : objList)delete com;
	objList.clear();
	assert(objList.empty() && "インスタンスを破棄できません");
}

IF::ObjectManager* IF::ObjectManager::Instance()
{
	static ObjectManager* inst = DEBUG_NEW ObjectManager;
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

#ifdef _DEBUG
void IF::ObjectManager::GUI()
{
	auto buff = objList;
	ImGui::Begin("Object");
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
	if (ImGui::CollapsingHeader("NewObject"))
	{
		static char tagC[256];
		static string tag;
		static string model = ModelManager::Instance()->GUIRadio();
		static int board = 0;
		static int objn = 0;
		static int error = 0;
		if (error)ImGui::Text("error : %s", error == 1 ? "tag == \0" : "model == nullptr");
		ImGui::InputText("Tag", tagC, sizeof(tagC));
		tag = tagC;
		if (ImGui::TreeNode("setModel"))
		{
			model = ModelManager::Instance()->GUIRadio();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("BillBoard"))
		{
			ImGui::RadioButton("NotBillBoard", &board, 0);
			ImGui::SameLine();
			ImGui::RadioButton("BillBoard", &board, 1);
			ImGui::SameLine();
			ImGui::RadioButton("YBillBoard", &board, 2);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("ObjectName"))
		{
			ImGui::RadioButton("Normel", &objn, 0);
			ImGui::RadioButton("Player", &objn, 1);
			ImGui::RadioButton("Enemy", &objn, 2);
			ImGui::TreePop();
		}
		if (ImGui::Button("Add"))
		{
			string s = tag;
			if (s == "\0")error = 1;
			else
			{
				if (objn == 0)
				{
					Normal* b = Add<Normal>(ModelManager::Instance()->GetModel(model), tag, board);
					if (b == nullptr)
					{
						error = 2;
					}
				}
				//if (objn == 1) {
				//	Player* b = Add<Player>(ModelManager::Instance()->GetModel(model), tag, board);
				//	if (b == nullptr)
				//	{
				//		error = 2;
				//	}
				//}
				//if (objn == 2) {
				//	Enemy* b = Add<Enemy>(ModelManager::Instance()->GetModel(model), tag, board);
				//	if (b == nullptr)
				//	{
				//		error = 2;
				//	}
				//}
			}
		}
	}
	ImGui::End();
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
	j["object"]["camera"] = camera->tag;
	for (auto com : objList)
	{
		j["object"]["object"][i]["ObjectName"] = com->GetObjName();
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
		j["object"]["object"][i]["color"]["x"] = com->GetColor().x;
		j["object"]["object"][i]["color"]["y"] = com->GetColor().y;
		j["object"]["object"][i]["color"]["z"] = com->GetColor().z;
		j["object"]["object"][i]["color"]["w"] = com->GetColor().w;
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
