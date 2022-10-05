#include "Scene.h"
#include "Input.h"
#include "Rand.h"
#include "Light.h"
#include "IFMath.h"
#include "DirectX12.h"
#include "PlayerObj.h"
#include "NormalObj.h"
#include "DebugCamera.h"
#include "Camera.h"
#include "nlohmann/json.hpp"
#include "SceneManager.h"
#include "Collision.h"
#include <fstream>
#include <iostream>

using namespace std;
using namespace ImGui;
using namespace nlohmann;

void IF::Scene::Initialize()
{

}
#ifdef _DEBUG

void IF::Scene::OutputJson(std::string failename)
{
	json j;
	tex->OutputJson(j);
	objM->OutputJson(j);

	string s = j.dump(4);

	ofstream writing_file;
	string scene = failename;
	string txt = ".json";
	string name = "Data/Scene/";
	name = name + scene + txt;
	writing_file.open(name, std::ios::out);
	writing_file << s << std::endl;
	writing_file.close();
}
#endif
void IF::Scene::InputJson(std::string failename)
{
	objM->Reset();
	tex->TexReset();
	std::ifstream reading_file;
	string scene = failename;
	string txt = ".json";
	string name = "Data/Scene/";
	bool flag = false;
	name = name + scene + txt;
	reading_file.open(name, std::ios::in);
	json j;
	reading_file >> j;
	reading_file.close();

}

void IF::Scene::StaticInitialize(int winWidth, int winHeight, ID3D12Device* device, ID3D12GraphicsCommandList* commandList, vector<D3D12_VIEWPORT> viewport, HWND& hwnd)
{

}

void IF::Scene::Update()
{

}

void IF::Scene::Draw()
{

}

void IF::Scene::Delete()
{
	ObjectManager::DeleteInstance();
}
