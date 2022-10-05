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
#include "NormalObj.h"
#include <fstream>
#include <iostream>

using namespace std;
using namespace ImGui;
using namespace nlohmann;

void IF::Scene::Initialize()
{
	tex->Initialize();
	tex->LoadTexture("white.png");
}
#ifdef _DEBUG

void IF::Scene::OutputJson(std::string failename)
{
	json j;
	tex->OutputJson(j);
	modelM->OutputJson(j);
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

	for (auto& i : j["texture"]["name"])
	{
		tex->LoadTexture(i);
	}
	for (auto i : j["model"])
	{
		if (i["type"] == 0)
		{
			modelM->Load(i["tag"], i["smooth"], i["name"]);
		}
		else if (i["type"] >= 1)modelM->Create(i["tag"], i["smooth"], i["tex"], i["type"]);
		modelM->SetTexture(i["tex"], i["tag"]);
	}
	for (auto i : j["object"]["object"])
	{
		objM->Add<Obj>(modelM->GetModel(i["model"]), i["tag"], i["BillBoard"], i["prefab"]);
		objM->SetAi(i["tag"], i["AI"]);
		objM->SetPosition({ i["pos"]["x"],i["pos"]["y"],i["pos"]["z"] }, i["tag"]);
		objM->SetRotation({ i["rot"]["x"],i["rot"]["y"],i["rot"]["z"] }, i["tag"]);
		objM->SetScale({ i["sca"]["x"],i["sca"]["y"],i["sca"]["z"] }, i["tag"]);
		objM->SetCollision(i["collision"], i["tag"]);
	}
}

void IF::Scene::StaticInitialize(int winWidth, int winHeight, ID3D12Device* device, ID3D12GraphicsCommandList* commandList, vector<D3D12_VIEWPORT> viewport, HWND& hwnd)
{
	objM->SetViewport(viewport);
	Texture::setDevice(device);
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
	ModelManager::DeleteInstance();
}
