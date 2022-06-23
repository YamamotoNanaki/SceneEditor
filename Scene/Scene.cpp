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
	modelM.Load("dome", false, "skydome");
	modelM.Load("ground", false, "ground");
	modelM.Load("sphere", true, "sphere");

	//カメラ関連初期化
	matPro = new Projection(45.0f, (float)winWidth, (float)winHeight);
	matView.eye = { 1,1,-5.0f };

	//そのほかの初期化
	Rand random;
	random.Initialize();

	matView.Update();

	objM.SetViewport(viewport);
	objM.Add<NormalObj>(modelM.GetModel("dome"), matView.GetAddressOf(), matPro->GetAddressOf(), &matView.eye, "dome");
	objM.Add<NormalObj>(modelM.GetModel("ground"), matView.GetAddressOf(), matPro->GetAddressOf(), &matView.eye, "ground");
	objM.SetPosition({ 0,-2,0 }, "ground");
	objM.Add<PlayerObj>(modelM.GetModel("sphere"), matView.GetAddressOf(), matPro->GetAddressOf(), &matView.eye, "player");

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
	static string _tagName = "Object";
	static char _ctagName[256];
	static int _objtagNum = 0;
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	NewFrame();
	Begin("hierarchy", (bool*)false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	if (CollapsingHeader("ObjectList"))
	{
		if (ImGui::Button("Add") && !addObj)
		{
			_tagName = "Object";
			addObj = true;
			_objtagNum = objM.GetTagNum(_tagName);
			if (_objtagNum != 0)_tagName += (char)(_objtagNum + 48);
			strcpy_s(_ctagName, _tagName.c_str());
		}
		objM.GUI();
	}
	if (CollapsingHeader("ModelList"))
	{
		modelM.GUI();
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
	SetWindowSize(ImVec2(800, 40));
	Text("SceneView");
	const char start[] = "DebugStart";
	const char stop[] = "DebugStop";
	ImGui::SameLine();
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
		static string _mtag;
		static int typeM = 0;
		static int typeB = 0;
		Begin("NewObjectSetting", (bool*)false, ImGuiWindowFlags_NoResize);
		if (ImGui::TreeNode("setModel"))
		{
			_mtag = modelM.GUIRadio();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("modelType"))
		{
			ImGui::RadioButton("NormalObj", &typeM, 0);
			ImGui::SameLine();
			ImGui::RadioButton("PlayerObj", &typeM, 1);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("BillBoard"))
		{
			ImGui::RadioButton("not", &typeB, 0);
			ImGui::SameLine();
			ImGui::RadioButton("BOARD", &typeB, 1);
			ImGui::SameLine();
			ImGui::RadioButton("YBOARD", &typeB, 2);
			ImGui::TreePop();
		}
		InputText("Tag", _ctagName, sizeof(_ctagName));
		if (ImGui::Button("Add"))
		{
			if (typeM == 0)
			{
				objM.Add<NormalObj>(modelM.GetModel(_mtag), matView.GetAddressOf(), matPro->GetAddressOf(), &matView.eye, _ctagName, typeB);
			}
			if (typeM == 1)
			{
				objM.Add<PlayerObj>(modelM.GetModel(_mtag), matView.GetAddressOf(), matPro->GetAddressOf(), &matView.eye, _ctagName, typeB);
			}
			addObj = false;
		}
		if (ImGui::Button("Cancel"))addObj = false;
		End();
	}
	ShowDemoWindow();
	spherePos = objM.GetComponent<PlayerObj>()->GetPos();
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

		objM.SetPosition(spherePos, "player");
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

	objM.Update();
	//sprite.Update();
	}

void IF::Scene::Draw()
{
	graph->DrawBlendMode(commandList.Get());
	Object::DrawBefore(graph->rootsignature.Get());
	objM.SetViewport(viewport);
	objM.Draw();

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
	sound->SoundUnLoad(testSound);
	sound->Reset();
}
