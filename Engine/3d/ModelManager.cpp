#include "ModelManager.h"
#include "imgui.h"
#include "Debug.h"
#include "Texture.h"

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
void IF::ModelManager::GUI()
{
	ImGui::Begin("ModelList");
	auto buff = modelList;
	static bool flag;
	static std::string tag;
	if (!flag)
	{
		for (auto com : buff)
		{
			if (ImGui::TreeNode(com->GetTag().c_str()))
			{
				if (ImGui::Button("ChangeTexture"))
				{
					flag = true;
					tag = com->GetTag();
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
	else
	{
		ImGui::Text(tag.c_str());
		static int texNum = GetTexture(tag);
		if (ImGui::TreeNode("SetTexture"))
		{
			Texture::Instance()->TexNum(&texNum);
			ImGui::TreePop();
		}
		if (ImGui::Button("change"))
		{
			SetTexture(texNum, tag);
			flag = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			flag = false;
		}
	}
	if (ImGui::CollapsingHeader("AddModel"))
	{
		static int smoot = 0;
		static int loadMode = 0;
		static int texNum = 0;
		static char tag[256];
		static char fName[256];
		ImGui::InputText("Tag", tag, sizeof(tag));
		if (loadMode == 0)ImGui::InputText("FaileName", fName, sizeof(fName));
		if (ImGui::TreeNode("LoadMode"))
		{
			ImGui::RadioButton("LoadModel", &loadMode, LOAD_MODEL);
			ImGui::RadioButton("CreateCube", &loadMode, CREATE_CUBE);
			ImGui::RadioButton("CreateTriangle", &loadMode, CREATE_TRIANGLE);
			ImGui::RadioButton("CreateCircle", &loadMode, CREATE_CIRCLE);
			ImGui::RadioButton("CreateSphere", &loadMode, CREATE_SPHERE);
			ImGui::RadioButton("CreateRay", &loadMode, CREATE_RAY);
			ImGui::RadioButton("CreateSquare", &loadMode, CREATE_POLYGON_SQUARE);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Texture"))
		{
			Texture::Instance()->TexNum(&texNum);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Smoothing"))
		{
			ImGui::RadioButton("true", &smoot, 1);
			ImGui::SameLine();
			ImGui::RadioButton("false", &smoot, 0);
			ImGui::TreePop();
		}
		static bool error = false;
		if (ImGui::Button("Add"))
		{
			if (loadMode == 0)
			{
				if (!Load(tag, smoot, fName))error = true;
				else
				{
					error = false;
				}
			}
			if (loadMode >= 1)
			{
				Create(tag, smoot, texNum, loadMode);
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			error = false;
		}
		if (error)
		{
			ImGui::Text("Error");
		}
	}
	ImGui::End();
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