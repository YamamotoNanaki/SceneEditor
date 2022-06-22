#include "Scene.h"
#include "Input.h"
#include "Rand.h"
#include "Light.h"
#include "IFMath.h"
#include "DirectX12.h"
#include "PlayerObj.h"
#include "NormalObj.h"
#include <fstream>

using namespace std;
using namespace ImGui;

void IF::Scene::WriteData(char* _sceneName)
{
	if (strcmp(_sceneName, "\0") == 0)
	{
		Text("Error");
		return;
	}
	ofstream writing_file;
	string scene = _sceneName;
	string txt = ".txt";
	string name = "Data/Scene/";
	name = name + scene + txt;
	writing_file.open(name, std::ios::out);
	std::string writing_text = "test";
	writing_file << writing_text << std::endl;
	writing_file.close();
}

void IF::Scene::Initialize()
{
	//音源
	testSound = sound->LoadWave("Resources/Alarm01.wav");

	//光源設定
	light = LightManager::Instance();
	light->Initialize();
	light->DefaultLightSetting();
	for (int i = 0; i < 3; i++)
	{
		light->SetDirLightActive(i, true);
		light->SetPointLightActive(i, false);
		light->SetSpotLightActive(i, false);
	}
	light->SetCircleShadowActive(0, true);
	light->SetAmbientColor({ 1, 1, 1 });
	Object::StaticInitialize(device.Get(), commandList.Get(), light);

	//画像関連初期化
	graph->Initialize(tex->descRangeSRV, L"Resources/Shaders/ModelVS.hlsl", L"Resources/Shaders/ModelPS.hlsl", L"Resources/Shaders/ModelGS.hlsl");
	graph->Initialize2D(tex->descRangeSRV, L"Resources/Shaders/SpriteVS.hlsl", L"Resources/Shaders/SpritePS.hlsl");

	//モデルの読み込みとオブジェクトとの紐付け(空と地面)
	tex->Initialize();
	domeM = new Model;
	groundM = new Model;
	sphereM = new Model;
	domeM->LoadModel("skydome");
	groundM->LoadModel("ground");

	//カメラ関連初期化
	matPro = new Projection(45.0f, (float)winWidth, (float)winHeight);
	matView.eye = { 1,1,-5.0f };

	//そのほかの初期化
	Rand random;
	random.Initialize();

	sphereM->LoadModel("sphere", true);
	matView.Update();

	obj.SetViewport(viewport);
	obj.Add<NormalObj>(domeM, matView.GetAddressOf(), matPro->GetAddressOf(), &matView.eye, "dome");
	obj.Add<NormalObj>(groundM, matView.GetAddressOf(), matPro->GetAddressOf(), &matView.eye, "ground");
	obj.SetPosition({ 0,-2,0 }, "ground");
	obj.Add<PlayerObj>(sphereM, matView.GetAddressOf(), matPro->GetAddressOf(), &matView.eye, "player");

	//2D関連
	sprite.StaticInitialize(this->device.Get(), this->commandList.Get(), (float)winWidth, (float)winHeight);
	SGraph = tex->LoadTexture("Resources/texture.png");
	sprite.Initialize(SGraph, { 300,300 });

	//sound->SoundPlay(testSound);

#ifdef _DEBUG
	//IMGUI
	gui.Initialize(this->hwnd, this->device.Get(), tex->srvHeap.Get(), DirectX12::Instance()->swapchain.Get());

	//デバッグ用
	dText.Initialize(tex->LoadTexture("Resources/debugfont.png"));

#endif // _DEBUG
}

void IF::Scene::StaticInitialize(int winWidth, int winHeight, ID3D12Device* device, ID3D12GraphicsCommandList* commandList, vector<D3D12_VIEWPORT> viewport, HWND& hwnd)
{
	this->winWidth = winWidth;
	this->winHeight = winHeight;
	this->device = device;
	this->commandList = commandList;
	this->viewport = viewport;
	this->hwnd = hwnd;
	Graphic::SetDevice(device);
	Texture::setDevice(device);
	Model::SetDevice(device);
}

void IF::Scene::Update()
{
#ifdef _DEBUG
	static bool flag = false;
	static ImVec2 pos{ 0,0 };
	static float dlColor[] = { 1,1,1 };
	static Float3 spherePos = { -1,0,0 };
	static bool addObj = false;
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	NewFrame();
	Begin("hierarchy", (bool*)false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	if (CollapsingHeader("ObjectList"))
	{
		if (ImGui::Button("Add") && !addObj)addObj = true;
		obj.GUI();
	}
	if (CollapsingHeader("ModelList"))
	{

	}
	if (CollapsingHeader("Light"))
	{

	}
	if (CollapsingHeader("Camera"))
	{

	}
	End();
	Begin("sceneView", (bool*)false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	SetWindowPos({ 200,0 });
	SetWindowSize(ImVec2(800, 60));
	Text("SceneView");
	const char start[] = "DebugStart";
	const char stop[] = "DebugStop";
	if (ImGui::Button(flag == false ? start : stop))flag = !flag;
	End();
	Begin("SceneOutput", (bool*)false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	SetWindowPos({ 1000,0 });
	SetWindowSize(ImVec2(280, 100));
	static char _sceneName[256];
	ImGui::InputText("OutputName", _sceneName, sizeof(_sceneName));
	if (ImGui::Button("DataOutput"))
	{
		WriteData(_sceneName);
	}
	End();
	if (addObj)
	{
		static char _tagName[256];
		Begin("NewObjectSetting", (bool*)false, ImGuiWindowFlags_NoResize);
		//InputText("Tag", _tagName, sizeof(_tagName));
		//InputText("Tag", _tagName, sizeof(_tagName));
		if (ImGui::Button("Add"))addObj = false;
		if (ImGui::Button("Cancel"))addObj = false;
		End();
	}
	ShowDemoWindow();
	spherePos = obj.GetComponent<PlayerObj>()->GetPos();
	light->SetCircleShadowCasterPos(0, spherePos);
	if (flag) {
		Input* input = Input::Instance();
		input->Update();

		//光源

		//if (input->KDown(KEY::W))lightDir.m128_f32[1] += 1.0f;
		//if (input->KDown(KEY::S))lightDir.m128_f32[1] -= 1.0f;
		//if (input->KDown(KEY::D))lightDir.m128_f32[0] += 1.0f;
		//if (input->KDown(KEY::A))lightDir.m128_f32[0] -= 1.0f;

		light->SetCircleShadowDir(0, { csDir.x,csDir.y,csDir.z });
		light->SetCircleShadowAtten(0, csAtten);
		light->SetCircleShadowFactorAngle(0, csAngle);

		for (int i = 0; i < 3; i++)light->SetDirLightColor(i, { dlColor[0],dlColor[1],dlColor[2] });

		//カメラ
		if (input->KDown(KEY::UP))
		{
			matView.eye.z += 0.5f;
			matView.target.z += 0.5f;
		}
		if (input->KDown(KEY::DOWN))
		{
			matView.eye.z -= 0.5f;
			matView.target.z -= 0.5f;
		}
		if (input->KDown(KEY::RIGHT))
		{
			matView.eye.x += 0.5f;
			matView.target.x += 0.5f;
		}
		if (input->KDown(KEY::LEFT))
		{
			matView.eye.x -= 0.5f;
			matView.target.x -= 0.5f;
		}

		if (input->KDown(KEY::W))spherePos.y += 0.5f;
		if (input->KDown(KEY::S))spherePos.y -= 0.5f;

		obj.SetPosition(spherePos, "player");
		light->SetCircleShadowCasterPos(0, spherePos);
	}

#else

	Input* input = Input::Instance();
	input->Update();

	//光源
	static float dlColor[] = { 1,1,1 };
	static Float3 spherePos = { -1,0,0 };

	//if (input->KDown(KEY::W))lightDir.m128_f32[1] += 1.0f;
	//if (input->KDown(KEY::S))lightDir.m128_f32[1] -= 1.0f;
	//if (input->KDown(KEY::D))lightDir.m128_f32[0] += 1.0f;
	//if (input->KDown(KEY::A))lightDir.m128_f32[0] -= 1.0f;

	light->SetCircleShadowDir(0, { csDir.x,csDir.y,csDir.z });
	light->SetCircleShadowAtten(0, csAtten);
	light->SetCircleShadowFactorAngle(0, csAngle);

	for (int i = 0; i < 3; i++)light->SetDirLightColor(i, { dlColor[0],dlColor[1],dlColor[2] });

	//カメラ
	if (input->KDown(KEY::UP))
	{
		matView.eye.z += 0.5f;
		matView.target.z += 0.5f;
	}
	if (input->KDown(KEY::DOWN))
	{
		matView.eye.z -= 0.5f;
		matView.target.z -= 0.5f;
	}
	if (input->KDown(KEY::RIGHT))
	{
		matView.eye.x += 0.5f;
		matView.target.x += 0.5f;
	}
	if (input->KDown(KEY::LEFT))
	{
		matView.eye.x -= 0.5f;
		matView.target.x -= 0.5f;
	}

	if (input->KDown(KEY::W))spherePos.y += 0.5f;
	if (input->KDown(KEY::S))spherePos.y -= 0.5f;

	obj.SetPosition(spherePos, "player");
	light->SetCircleShadowCasterPos(0, spherePos);

	sprite.position = { 540,500 };
	sprite.Update();

#endif // _DEBUG
	matView.Update();
	light->Update();

	obj.Update();
	//sprite.Update();
	}

void IF::Scene::Draw()
{
	graph->DrawBlendMode(commandList.Get());
	Object::DrawBefore(graph->rootsignature.Get());
	obj.SetViewport(viewport);
	obj.Draw();

	//pgraph.DrawBlendMode(commandList, Blend::ADD);
	//tex->setTexture(commandList, efect);
	//fire->particle[0].DrawBefore(commandList, pgraph.rootsignature.Get(), tex->srvHeap.Get(), cb.GetGPUAddress(), D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	//fire->Draw(commandList, viewport);
	graph->DrawBlendMode(commandList.Get(), Blend::NORMAL2D);
	Sprite::DrawBefore(graph->rootsignature.Get());
	sprite.Draw(viewport);

#ifdef _DEBUG
	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList.Get());

	//デバッグ用
	//dText.Draw(viewport);

#endif // _DEBUG
}

void IF::Scene::Delete()
{
	//light->UnMap();
	delete matPro;
	delete groundM;
	delete domeM;
	delete sphereM;
	sound->SoundUnLoad(testSound);
	sound->Reset();
}
