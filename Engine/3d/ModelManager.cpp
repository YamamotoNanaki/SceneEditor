#include "ModelManager.h"
#include "imgui.h"
#include "Debug.h"

using namespace IF;

IF::ModelManager::~ModelManager()
{
	for (auto buff : modelList)
	{
		delete buff;
	}
	modelList.clear();
	assert(modelList.empty() && "インスタンスを破棄できません");
}

void IF::ModelManager::StaticInitialize(ID3D12Device* device)
{
	Model::SetDevice(device);
}

ModelManager* IF::ModelManager::Instance()
{
	static ModelManager* inst = DEBUG_NEW ModelManager;
	return inst;
}

void IF::ModelManager::DeleteInstance()
{
	delete ModelManager::Instance();
}

#ifdef _DEBUG
void IF::ModelManager::GUI(bool* flag, std::string* tag)
{
	auto buff = modelList;
	for (auto com : buff)
	{
		if (ImGui::TreeNode(com->GetTag().c_str()))
		{
			if (ImGui::Button("ChangeTexture"))
			{
				*flag = true;
				*tag = com->GetTag();
			}
			if (ImGui::Button("Delete"))
			{
				modelList.remove(com);
				delete com;
			}
			ImGui::TreePop();
		}
	}
}

std::string IF::ModelManager::GUIRadio()
{
	int i = 0;
	static int mode = 0;
	std::string r;
	for (auto com : modelList)
	{
		if (i % 3)ImGui::SameLine();
		ImGui::RadioButton(com->GetTag().c_str(), &mode, i);
		if (mode == i)
		{
			r = com->GetTag().c_str();
		}
		i++;
	}
	return r;
}
#endif

void IF::ModelManager::OutputJson(nlohmann::json& j)
{
	int i = 0;
	for (auto com : modelList)
	{
		j["model"][i]["tag"] = com->GetTag();
		j["model"][i]["name"] = com->name;
		j["model"][i]["smooth"] = com->smooth;
		j["model"][i]["tex"] = com->GetTexNum();
		j["model"][i]["type"] = com->type;
		i++;
	}
}