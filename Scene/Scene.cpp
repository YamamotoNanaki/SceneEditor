#include "Scene.h"
#include "Input.h"
#include "Rand.h"
#include "Light.h"
#include "IFMath.h"
#include "DirectX12.h"
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
	graph->Initialize(tex->descRangeSRV, L"Data/Shaders/ModelVS.hlsl", L"Data/Shaders/ModelPS.hlsl", L"Data/Shaders/ModelGS.hlsl");
	graph->Initialize2D(tex->descRangeSRV, L"Data/Shaders/SpriteVS.hlsl", L"Data/Shaders/SpritePS.hlsl");
	graph->InitializeParticle(tex->descRangeSRV);
	cameraM->Add<Camera>("mainCamera", 45, (float)winWidth, (float)winHeight);
	lightM->Initialize();
	lightM->DefaultLightSetting();
	for (int i = 0; i < 3; i++)
	{
		lightM->SetDirLightActive(i, true);
		lightM->SetPointLightActive(i, false);
		lightM->SetSpotLightActive(i, false);
	}
	lightM->SetAmbientColor({ 1, 1, 1 });

	objM->SetCamera(cameraM->GetCamera("mainCamera"));
	DebugText::Instance()->Initialize(tex->LoadTexture("debugfont.png", 1022));
#ifdef _DEBUG
	gui.Initialize(this->hwnd, this->device, tex->srvHeap.Get(), DirectX12::Instance()->swapchain.Get());
#endif
}
#ifdef _DEBUG

void IF::Scene::OutputJson(std::string failename)
{
	json j;
	objM->OutputJson(j);
	string s = j.dump(4);
	ofstream writing_file;
	string scene = failename;
	string txt = ".json";
	string name = "Data/Scene/";
	string faile;
	faile = name + scene + txt;
	writing_file.open(faile, std::ios::out);
	writing_file << s << std::endl;
	writing_file.close();

	json j2;
	particleM->OutputJson(j2);
	s = j2.dump(4);
	string type = "Particle";
	faile = name + scene + type + txt;
	writing_file.open(faile, std::ios::out);
	writing_file << s << std::endl;
	writing_file.close();

	json j3;
	tex->OutputJson(j3);
	s = j3.dump(4);
	type = "Texture";
	faile = name + type + txt;
	writing_file.open(faile, std::ios::out);
	writing_file << s << std::endl;
	writing_file.close();


	json j4;
	modelM->OutputJson(j4);
	s = j4.dump(4);
	type = "Model";
	faile = name + type + txt;
	writing_file.open(faile, std::ios::out);
	writing_file << s << std::endl;
	writing_file.close();


	json j5;
	cameraM->OutputJson(j5);
	s = j5.dump(4);
	type = "Camera";
	faile = name + type + txt;
	writing_file.open(faile, std::ios::out);
	writing_file << s << std::endl;
	writing_file.close();


	json j6;
	spriteM->OutputJson(j6);
	s = j6.dump(4);
	type = "Sprite";
	faile = name + scene + type + txt;
	writing_file.open(faile, std::ios::out);
	writing_file << s << std::endl;
	writing_file.close();
}

void IF::Scene::DebugUpdate()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	NewFrame();
	static bool flag = false;
	ImGui::Begin("Debug");
	ImGui::Text("Saved when debugging starts");
	if (!flag)
	{
		if (ImGui::Button("DebugStart") || (Input::Instance()->KeyDown(DIK_LCONTROL) && Input::Instance()->KeyTriggere(DIK_RETURN)))
		{
			flag = true;
			OutputJson(SceneManager::Instance()->GetNowScene());
		}
	}
	else
	{
		if (ImGui::Button("DebugStop") || (Input::Instance()->KeyDown(DIK_LCONTROL) && Input::Instance()->KeyTriggere(DIK_RETURN)))
		{
			flag = false;
			InputJson(SceneManager::Instance()->GetNowScene());
		}
	}
	ImGui::End();

	static bool obj = true;
	static bool model = false;
	static bool t = false;
	static bool camera = false;
	static bool spr = false;
	static bool particle = false;
	ImGui::Begin("GUIList");
	{
		ImGui::Checkbox("ObjectList", &obj);
		ImGui::Checkbox("ModelList", &model);
		ImGui::Checkbox("TextureList", &t);
		ImGui::Checkbox("CameraList", &camera);
		ImGui::Checkbox("SpriteList", &spr);
		ImGui::Checkbox("ParticleList", &particle);
	}
	ImGui::End();
	if (obj)objM->GUI();
	if (model)modelM->GUI();
	if (t)tex->GUI();
	if (camera)cameraM->GUI();
	if (spr)spriteM->GUI();
	if (particle)particleM->GUI();

	cameraM->AutoUpdate();
	lightM->Update();
	if (flag)
	{
		objM->Update();
		objM->DeleteObject();
		spriteM->Update();
		particleM->Update();
		spriteM->DeleteSprite();
	}
	else
	{
		objM->DebugUpdate();
		spriteM->DebugUpdate();
		particleM->DebugUpdate();
	}
}
#endif
void IF::Scene::InputJson(std::string failename)
{
	objM->Reset();
	tex->TexReset();
	cameraM->Reset();
	modelM->Reset();
	spriteM->Reset();
	particleM->Reset();
	std::ifstream reading_file;
	string scene = failename;
	string txt = ".json";
	string name = "Data/Scene/";
	string type = "Texture";
	string error = "Basic";
	string faile;
	faile = name + type + txt;
	reading_file.open(faile, std::ios::in);
	json j;
	reading_file >> j;
	reading_file.close();
	for (auto& i : j["texture"]["name"])
	{
		tex->LoadTexture(i);
	}

	type = "Model";
	faile = name + type + txt;
	reading_file.open(faile, std::ios::in);
	json j2;
	reading_file >> j2;
	reading_file.close();
	for (auto i : j2["model"])
	{
		if (i["type"] == 0)
		{
			modelM->Load(i["tag"], i["smooth"], i["name"]);
		}
		else if (i["type"] >= 1)modelM->Create(i["tag"], i["smooth"], i["tex"], i["type"]);
		modelM->SetTexture(i["tex"], i["tag"]);
	}

	type = "Camera";
	faile = name + type + txt;
	reading_file.open(faile, std::ios::in);
	json j3;
	reading_file >> j3;
	reading_file.close();
	for (auto i : j3["camera"])
	{
		if ("Camera" == i["CameraName"])cameraM->Add<Camera>(i["tag"], 45.0f, (float)winWidth, (float)winHeight);
		else if ("Debug" == i["CameraName"])cameraM->Add<DebugCamera>(i["tag"], 45.0f, (float)winWidth, (float)winHeight);
		else continue;
		cameraM->SetEye({ i["eye"]["x"],i["eye"]["y"],i["eye"]["z"] }, i["tag"]);
		cameraM->SetTarget({ i["target"]["x"],i["target"]["y"],i["target"]["z"] }, i["tag"]);
		//cameraM->SetRota(i["rota"], i["tag"]);
	}

	type = "";
	faile = name + scene + type + txt;
	reading_file.open(faile, std::ios::in);
	if (reading_file.fail())
	{
		faile = name + error + txt;
		reading_file.open(faile, std::ios::in);
	}
	json j4;
	reading_file >> j4;
	reading_file.close();
	if (j4["object"]["camera"] == "debug")objM->SetCamera(cameraM->GetCamera("mainCamera"));
	else objM->SetCamera(cameraM->GetCamera(j4["object"]["camera"]));
	for (auto i : j4["object"]["object"])
	{
		if ("Normal" == i["ObjectName"])objM->Add<Normal>(modelM->GetModel(i["model"]), i["tag"], i["BillBoard"], 0);
		if ("Player" != i["ObjectName"])objM->SetPosition({ i["pos"]["x"],i["pos"]["y"],i["pos"]["z"] }, i["tag"]);
		objM->SetRotation({ i["rot"]["x"],i["rot"]["y"],i["rot"]["z"] }, i["tag"]);
		objM->SetScale({ i["sca"]["x"],i["sca"]["y"],i["sca"]["z"] }, i["tag"]);
		Float4 f = { i["color"]["x"],i["color"]["y"],i["color"]["z"],i["color"]["w"] };
		objM->SetColor(f, i["tag"]);
		objM->SetCollision(i["collision"], i["tag"]);
	}
	objM->CollisionInitialize();

	type = "Sprite";
	faile = name + scene + type + txt;
	reading_file.open(faile, std::ios::in);
	if (reading_file.fail())
	{
		faile = name + error + type + txt;
		reading_file.open(faile, std::ios::in);
	}
	json j5;
	reading_file >> j5;
	reading_file.close();
	for (auto i : j5["sprite"])
	{
		spriteM->Add(i["tex"], i["tag"], i["type"]);
		spriteM->SetPosition({ i["pos"]["x"],i["pos"]["y"] }, i["tag"]);
		spriteM->SetRotation(i["rot"], i["tag"]);
		spriteM->SetScale({ i["sca"]["x"],i["sca"]["y"] }, i["tag"]);
	}

	type = "Particle";
	faile = name + scene + type + txt;
	reading_file.open(faile, std::ios::in);
	if (reading_file.fail())
	{
		faile = name + error + type + txt;
		reading_file.open(faile, std::ios::in);
	}
	json j7;
	reading_file >> j7;
	reading_file.close();
	particleM->InputJson(j7);
}

void IF::Scene::StaticInitialize(int winWidth, int winHeight, ID3D12Device* device, ID3D12GraphicsCommandList* commandList, vector<D3D12_VIEWPORT> viewport, HWND& hwnd)
{
	this->winWidth = winWidth;
	this->winHeight = winHeight;
	this->device = device;
	this->commandList = commandList;
	this->viewport = viewport;
	this->hwnd = hwnd;
	objM->SetViewport(viewport);
	Texture::setDevice(device);
	ModelManager::StaticInitialize(device);
	Graphic::SetDevice(device);
	Sprite::StaticInitialize(device, commandList, viewport, winWidth, winHeight);
	Object::StaticInitialize(device, commandList, lightM);
	Emitter::StaticInitialize(device, commandList, viewport);
}

void IF::Scene::Update()
{
	Input::Instance()->Input::Update();

#ifdef _DEBUG
	DebugUpdate();
#else
	cameraM->AutoUpdate();
	lightM->Update();
	objM->Update();
	spriteM->Update();
	particleM->Update();
	spriteM->DeleteSprite();
	objM->DeleteObject();
#endif
}

void IF::Scene::Draw()
{
	graph->DrawBlendMode(commandList, Blend::NORMAL2D);
	Sprite::DrawBefore(graph->rootsignature.Get());
	spriteM->SetViewport(viewport);
	spriteM->BackGroundDraw();

	graph->DrawBlendMode(commandList);
	Object::DrawBefore(graph->rootsignature.Get());
	objM->SetViewport(viewport);
	objM->Draw();

	particleM->Draw(graph->rootsignature.Get());


	graph->DrawBlendMode(commandList, Blend::NORMAL2D);
	Sprite::DrawBefore(graph->rootsignature.Get());
	spriteM->SetViewport(viewport);
	spriteM->ForeGroundDraw();

#ifdef _DEBUG
	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);

	//デバッグ用

#endif // _DEBUG
}

void IF::Scene::Delete()
{
	ObjectManager::DeleteInstance();
	ModelManager::DeleteInstance();
	CameraManager::DeleteInstance();
	SpriteManager::DeleteInstance();
	DebugText::DeleteInstance();
}
