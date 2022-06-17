#include "Scene.h"
#include "Input.h"
#include "Rand.h"
#include "Light.h"
#include "IFMath.h"
#include "DirectX12.h"

using namespace std;

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
	Object::StaticInitialize(this->device.Get(), this->commandList.Get(), light);

	//画像関連初期化
	graph->Initialize(tex->descRangeSRV, L"Resources/Shaders/ModelVS.hlsl", L"Resources/Shaders/ModelPS.hlsl", L"Resources/Shaders/ModelGS.hlsl");
	graph->Initialize2D(tex->descRangeSRV, L"Resources/Shaders/SpriteVS.hlsl", L"Resources/Shaders/SpritePS.hlsl");

	//モデルの読み込みとオブジェクトとの紐付け(空と地面)
	tex->Initialize();
	domeM.LoadModel("skydome");
	groundM.LoadModel("ground");
	domeObj.Initialize(&domeM);
	groundObj.Initialize(&groundM);
	groundObj.position = { 0,-2,0 };

	//カメラ関連初期化
	matPro = new Projection(45.0f, (float)winWidth, (float)winHeight);
	matView.eye = { 1,1,-5.0f };

	//そのほかの初期化
	Rand random;
	random.Initialize();

	sphereM.LoadModel("sphere", true);
	sphereO.Initialize(&sphereM);

	sphereO.position = { -1,0,0 };
	sphereO.scale = { 0.5,0.5,0.5 };
	matView.Update();


	//2D関連
	sprite.StaticInitialize(this->device.Get(), this->commandList.Get(), (float)winWidth, (float)winHeight);
	SGraph = tex->LoadTexture("Resources/texture.png");
	sprite.Initialize(SGraph, { 300,300 });

	//sound->SoundPlay(testSound);

	//IMGUI
	gui.Initialize(this->hwnd, this->device.Get(), tex->srvHeap.Get(), DirectX12::Instance()->swapchain.Get());

	//デバッグ用
#ifdef _DEBUG
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

	Float3 rot = sphereO.rotation;
	rot.y += 0.05f;
	sphereO.rotation = rot;
	sphereO.position = spherePos;
	light->SetCircleShadowCasterPos(0, spherePos);

	matView.Update();
	light->Update();

	domeObj.Update(matView.Get(), matPro->Get(), matView.eye);
	groundObj.Update(matView.Get(), matPro->Get(), matView.eye);
	sphereO.Update(matView.Get(), matPro->Get(), matView.eye);

	sprite.position = { 540,500 };
	sprite.Update();

	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("test");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(500, 200));
	ImGui::Text("test");
	ImGui::ColorEdit3("pointLightColor", dlColor, ImGuiColorEditFlags_Float);
	ImGui::End();

	//デバッグ用
#ifdef _DEBUG
	//dText.Print(100,100, 2, "matView.eye.x : %f",matView.eye.x);
#endif // _DEBUG
}

void IF::Scene::Draw()
{
	graph->DrawBlendMode(commandList.Get());
	Object::DrawBefore(graph->rootsignature.Get());
	domeObj.Draw(viewport);
	groundObj.Draw(viewport);
	sphereO.Draw(viewport);

	//pgraph.DrawBlendMode(commandList, Blend::ADD);
	//tex->setTexture(commandList, efect);
	//fire->particle[0].DrawBefore(commandList, pgraph.rootsignature.Get(), tex->srvHeap.Get(), cb.GetGPUAddress(), D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	//fire->Draw(commandList, viewport);
	graph->DrawBlendMode(commandList.Get(), Blend::NORMAL2D);
	Sprite::DrawBefore(graph->rootsignature.Get());
	sprite.Draw(viewport);

	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList.Get());

	//デバッグ用
#ifdef _DEBUG
	//dText.Draw(viewport);

#endif // _DEBUG
}

void IF::Scene::Delete()
{
	//light->UnMap();
	delete matPro;
	sound->SoundUnLoad(testSound);
	sound->Reset();
}
