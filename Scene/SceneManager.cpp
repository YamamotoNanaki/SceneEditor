#include "SceneManager.h"
#include "imgui.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>

using namespace IF;
using namespace std;
using namespace nlohmann;

void IF::SceneManager::Initialize(int winWidth, int winHeight, ID3D12Device* device, ID3D12GraphicsCommandList* commandList,
	vector<D3D12_VIEWPORT> viewport, HWND& hwnd)
{
	Load(&now);
	scene = new Scene;
	scene->StaticInitialize(winWidth, winHeight, device, commandList, viewport, hwnd);
	scene->Initialize();
	scene->InputJson(now);
}

void IF::SceneManager::Update()
{
	scene->Update();
}

void IF::SceneManager::Draw()
{
	scene->Draw();
}

void IF::SceneManager::Delete()
{
	scene->OutputJson(now);
	scene->Delete();
	delete scene;
}

void IF::SceneManager::Load(std::string* startscene)
{
	std::ifstream reading_file;
	string name = "Data/Scene/SceneManager.json";
	reading_file.open(name, std::ios::in);
	json j;
	reading_file >> j;
	reading_file.close();

	sceneList.clear();
	int i = 0;
	if (startscene != nullptr)*startscene = j["start"];
	for (auto com : j["scene"])
	{
		string a = com["name"];
		vector<string> b;
		for (auto com2 : com["next"])
		{
			b.push_back(com2);
		}
		sceneList.push_back({ b,a });
		if (j["start"] == a)
		{
			nownext = b;
		}
	}
}

void IF::SceneManager::Next(unsigned short nextNum)
{
	if (nownext.size() < nextNum)
	{
		scene->InputJson(now);
	}
	else
	{
		scene->InputJson(nownext[nextNum]);
		now = nownext[nextNum];
		for (auto com : sceneList)
		{
			if (now == com.name)
			{
				nownext = com.next;
			}
		}
	}
}

SceneManager* IF::SceneManager::Instance()
{
	static SceneManager* inst = new SceneManager;
	return inst;
}

void IF::SceneManager::Release()
{
	delete SceneManager::Instance();
}

//#ifdef _DEBUG
void IF::SceneManager::Output()
{
	json j;
	int i = 0;
	j["start"] = start;
	for (auto com : sceneList)
	{
		j["scene"][i]["name"] = com.name;
		int k = 0;
		for (auto com2 : com.next)
		{
			j["scene"][i]["next"][k] = com2;
			k++;
		}
		i++;
	}

	string s = j.dump(4);
	ofstream writing_file;
	string name = "Data/Scene/SceneManager.json";
	writing_file.open(name, std::ios::out);
	writing_file << s << std::endl;
	writing_file.close();
}

void IF::SceneManager::GUI(bool& flag)
{
	int i = 0;
	static vector<int> mode;
	static int mode2 = 0;
	static int mode3 = 0;
	static int mode4 = 0;
	static int oldmode3 = 0;
	static bool flag2 = false;
	static string oldStart;
	static vector<SceneDatas> next;
	static char name[256];
	if (flag2 == false)
	{
		flag2 = true;
		oldStart = start;
		next = sceneList;
		string _faileName = "scene";
		int i = 0;
		int j = 0;
		for (auto com : sceneList)
		{
			if (i == 0)
			{
				if (com.name == now)mode4 = j;
				if (com.name == _faileName)
				{
					i++;
				}
			}
			else
			{
				std::string a = _faileName + (char)(i + 48);
				if (com.name == a)
				{
					i++;
				}
			}
			j++;
		}
		if (i != 0)_faileName += (char)(i + 48);
		strcpy_s(name, _faileName.c_str());
	}
	ImGui::Begin("SceneManager", (bool*)false, ImGuiWindowFlags_NoResize);
	if (ImGui::TreeNode("NewScene"))
	{
		ImGui::InputText("SceneName", name, sizeof(name));
		if (ImGui::Button("add"))
		{
			vector<string>a;
			next.push_back({ a,name });
			sceneList.push_back({ a,name });
		}
		ImGui::TreePop();
	}
	i = 0;
	if (ImGui::TreeNode("ChangeScene"))
	{
		for (auto com : sceneList)
		{
			if (i % 3)ImGui::SameLine();
			ImGui::RadioButton(com.name.c_str(), &mode4, i);
			if (mode4 == i)
			{
				start = com.name;
			}
			i++;
		}
		if (ImGui::Button("Change"))
		{
			scene->OutputJson(now);
			scene->InputJson(sceneList[mode4].name);
			now = sceneList[mode4].name;
		}
		ImGui::TreePop();
	}
	i = 0;
	if (ImGui::TreeNode("StartScene"))
	{
		for (auto com : sceneList)
		{
			if (i % 3)ImGui::SameLine();
			ImGui::RadioButton(com.name.c_str(), &mode2, i);
			if (mode2 == i)
			{
				start = com.name;
			}
			i++;
		}
		ImGui::TreePop();
	}
	i = 0;
	if (ImGui::TreeNode("SceneSetting"))
	{
		oldmode3 = mode3;
		if (ImGui::TreeNode("Setting"))
		{
			for (auto com : sceneList)
			{
				if (i % 3)ImGui::SameLine();
				ImGui::RadioButton(com.name.c_str(), &mode3, i);
				i++;
			}
			ImGui::TreePop();
		}
		if (ImGui::Button("Add"))
		{
			next[mode3].next.push_back(sceneList[0].name);
		}
		ImGui::SameLine();
		if (ImGui::Button("Delete") && next[mode3].next.size() > 1)
		{
			next[mode3].next.pop_back();
		}
		if (next[mode3].next.size() != mode.size() || oldmode3 != mode3)
		{
			mode.clear();
			for (int i = 0; i < next[mode3].next.size(); i++)
			{
				int j = 0;
				for (j; j < sceneList.size(); j++)if (next[mode3].next[i] == sceneList[j].name)break;
				mode.push_back(j);
			}
		}
		for (int j = 0; j < next[mode3].next.size(); j++)
		{
			i = 0;
			string name = "NextScene";
			name += (char)(j + 48);
			if (ImGui::TreeNode(name.c_str()))
			{
				for (auto com : sceneList)
				{
					if (i % 3)ImGui::SameLine();
					ImGui::RadioButton(com.name.c_str(), &mode[j], i);
					if (mode[j] == i)
					{
						next[mode3].next[j] = com.name;
					}
					i++;
				}
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
	if (ImGui::Button("Confirm"))
	{
		sceneList = next;
		for (auto com : sceneList)
		{
			if (com.name == now)
			{
				nownext = com.next;
				break;
			}
		}
		flag = false;
		flag2 = false;
		Output();
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancel"))
	{
		start = oldStart;
		flag = false;
		flag2 = false;
	}
	ImGui::End();
}
//#endif

IF::SceneManager::~SceneManager()
{
	Delete();
}
