#include "ModelManager.h"
#include "imgui.h"

IF::ModelManager::~ModelManager()
{
	modelList.clear();
	assert(modelList.empty() && "インスタンスを破棄できません");
}

void IF::ModelManager::GUI()
{
	for (auto com : modelList)
	{
		ImGui::Text("%s", com->GetTag().c_str());
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

void IF::ModelManager::OutputJson(nlohmann::json& j)
{
	int i = 0;
	for (auto com : modelList)
	{
		j["model"]["tag"][i] = com->GetTag();
		j["model"]["name"][i] = com->name;
		j["model"]["smooth"][i] = com->smooth;
		i++;
	}
}
