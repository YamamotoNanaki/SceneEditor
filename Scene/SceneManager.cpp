#include "SceneManager.h"
#include "imgui.h"
#include "nlohmann/json.hpp"
#include <fstream>

using namespace IF;
using namespace std;
using namespace nlohmann;

void IF::SceneManager::Initialize(int winWidth, int winHeight, ID3D12Device* device, ID3D12GraphicsCommandList* commandList,
	vector<D3D12_VIEWPORT> viewport, HWND& hwnd)
{
	scene->StaticInitialize(winWidth, winHeight, device, commandList, viewport, hwnd);
	scene->Initialize();
	scene->InputJson(Next());
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
	scene->Delete();
}

void IF::SceneManager::Load()
{
	std::ifstream reading_file;
	string name = "Data/Scene/SceneManager.json";
	reading_file.open(name, std::ios::in);
	std::string line;
	json j;
	reading_file >> j;
	reading_file.close();
}

string IF::SceneManager::Next(unsigned short nextNum)
{
	if (next.size() < nextNum)return now;
	return next[nextNum];
}

void IF::SceneManager::Output()
{
}

void IF::SceneManager::GUI(bool& flag)
{
	int i = 0;
	static vector<int> mode;
	vector<string>newnext;
	ImGui::Begin("SceneManager");
	ImGui::Text(now.c_str());
	for (int j = 0; j < next.size(); j++)
	{
		if (ImGui::TreeNode("nextscene"))
		{
			for (auto com : sceneList)
			{
				if (i % 3)ImGui::SameLine();
				ImGui::RadioButton(com.c_str(), &mode[j], i);
				if (mode[j] == i)
				{
					newnext.push_back(com.c_str());
				}
				i++;
			}
			ImGui::TreePop();
		}
	}
	if (ImGui::Button("Confirm"))
	{
		Output();
		flag = false;
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancel"))
	{
		flag = false;
	}
	ImGui::End();
}
