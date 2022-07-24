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
#include <fstream>
#include <iostream>

using namespace std;
using namespace ImGui;
using namespace nlohmann;

void IF::Scene::Initialize()
{
	tex->Initialize();
	tex->LoadTexture("white.png");
	//音源
	testSound = sound->LoadWave("Data/Resources/Alarm01.wav");

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
	light->SetAmbientColor({ 1, 1, 1 });
	Object::StaticInitialize(device.Get(), commandList.Get(), light);

	//画像関連初期化
	graph->Initialize(tex->descRangeSRV, L"Data/Shaders/ModelVS.hlsl", L"Data/Shaders/ModelPS.hlsl", L"Data/Shaders/ModelGS.hlsl");
	graph->Initialize2D(tex->descRangeSRV, L"Data/Shaders/SpriteVS.hlsl", L"Data/Shaders/SpritePS.hlsl");

	//モデルの読み込みとオブジェクトとの紐付け(空と地面)
	modelM.Load("dome", false, "skydome");
	modelM.Load("ground", false, "ground");
	modelM.Load("sphere", true, "sphere");

	tex->GUIInit();
#ifdef _DEBUG
	//カメラ関連初期化
	cameraM.Add<DebugCamera>("debug", 45, (float)winWidth, (float)winHeight);
	cameraM.Add<Camera>("mainCamera", 45, (float)winWidth, (float)winHeight);
	//オブジェクト初期化
	objM.SetCamera(cameraM.GetCamera("debug"));
	objM.SetViewport(viewport);
	objM.Add<UsuallyObj>(modelM.GetModel("dome"), "dome");
	objM.Add<UsuallyObj>(modelM.GetModel("ground"), "ground");
	objM.SetPosition({ 0,-2,0 }, "ground");
	objM.Add<PlayerObj>(modelM.GetModel("sphere"), "player");
#else
	//カメラ関連初期化
	cameraM.Add<Camera>("mainCamera", 45, (float)winWidth, (float)winHeight);
	//オブジェクト初期化
	objM.SetCamera(cameraM.GetCamera("mainCamera"));
	objM.SetViewport(viewport);
	objM.Add<UsuallyObj>(modelM.GetModel("dome"), "dome");
	objM.Add<UsuallyObj>(modelM.GetModel("ground"), "ground");
	objM.SetPosition({ 0,-2,0 }, "ground");
	objM.Add<PlayerObj>(modelM.GetModel("sphere"), "player");
#endif

	//2D関連
	Sprite::StaticInitialize(this->device.Get(), this->commandList.Get(), viewport, (float)winWidth, (float)winHeight);
	SGraph = tex->LoadTexture("kakuninn.png");
	spriteM.Add(SGraph, "pix");

	//sound->SoundPlay(testSound);
	//デバッグ用
	dText.Initialize(tex->LoadTexture("debugfont.png"));

#ifdef _DEBUG
	//IMGUI
	gui.Initialize(this->hwnd, this->device.Get(), tex->srvHeap.Get(), DirectX12::Instance()->swapchain.Get());


#endif // _DEBUG
}

void IF::Scene::OutputJson(std::string failename)
{
	json j;
	for (int i = 1; i < tex->tex.size(); i++)
	{
		if (tex->tex[i].free == false)continue;
		j["texture"]["name"][i - 1] = tex->tex[i].texName;
	}
	modelM.OutputJson(j);
	cameraM.OutputJson(j);
	objM.OutputJson(j);
	spriteM.OutputJson(j);

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

bool IF::Scene::InputJson(std::string failename)
{
	objM.Reset();
	spriteM.Reset();
	modelM.Reset();
	cameraM.Reset();
	std::ifstream reading_file;
	string scene = failename;
	string txt = ".json";
	string name = "Data/Scene/";
	bool flag = false;
	name = name + scene + txt;
	reading_file.open(name, std::ios::in);
	if (!reading_file.is_open())
	{
		reading_file.open("Data/Scene/Base.json", std::ios::in);
		flag = true;
	}
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
			modelM.Load(i["tag"], i["smooth"], i["name"]);
		}
		else if (i["type"] >= 1)modelM.Create(i["tag"], i["smooth"], i["tex"], i["type"]);
		modelM.SetTexture(i["tex"], i["tag"]);
	}
	for (auto i : j["camera"])
	{
		if (i["type"])cameraM.Add<Camera>(i["tag"], 45.0f, (float)winWidth, (float)winHeight);
		else cameraM.Add<DebugCamera>(i["tag"], 45.0f, (float)winWidth, (float)winHeight);
	}
#ifdef _DEBUG
	objM.SetCamera(cameraM.GetCamera("debug"));
#else
	objM.SetCamera(cameraM.GetCamera(j["object"]["camera"]));
#endif
	for (auto i : j["object"]["object"])
	{
		if (i["type"] == 0)objM.Add<UsuallyObj>(modelM.GetModel(i["model"]), i["tag"], i["BillBoard"]);
		else if (i["type"] == 1)objM.Add<PlayerObj>(modelM.GetModel(i["model"]), i["tag"], i["BillBoard"]);
		else;
		objM.SetPosition({ i["pos"]["x"],i["pos"]["y"],i["pos"]["z"] }, i["tag"]);
		objM.SetRotation({ i["rot"]["x"],i["rot"]["y"],i["rot"]["z"] }, i["tag"]);
		objM.SetScale({ i["sca"]["x"],i["sca"]["y"],i["sca"]["z"] }, i["tag"]);
	}
	for (auto i : j["sprite"])
	{
		spriteM.Add(i["tex"], i["tag"]);
		spriteM.SetPosition({ i["pos"]["x"],i["pos"]["y"] }, i["tag"]);
		spriteM.SetRotation(i["rot"], i["tag"]);
		spriteM.SetScale({ i["sca"]["x"],i["sca"]["y"] }, i["tag"]);
	}

	if (flag)OutputJson(failename);

	return true;
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
	static int hoge = 0;
#ifdef _DEBUG
	static bool flag = false;
	static ImVec2 pos{ 0,0 };
	static float dlColor[] = { 1,1,1 };
	static Float3 spherePos = { -1,0,0 };
	static bool addObj = false;
	static bool addSpr = false;
	static bool addModel = false;
	static bool addTex = false;
	static string _tagName = "Object";
	static char _ctagName[256];
	static int _objtagNum = 0;
	static int _spriteNum = 0;
	static int _ModeltagNum = 0;
	static bool texM = false;
	static bool texS = false;
	static string texCName;
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	NewFrame();
	Begin("hierarchy", (bool*)false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	if (CollapsingHeader("ObjectList"))
	{
		if (ImGui::Button("Add") && !addObj && !addModel && !addTex && !addSpr)
		{
			_tagName = "Object";
			addObj = true;
			_objtagNum = objM.GetTagNum(_tagName);
			if (_objtagNum != 0)_tagName += (char)(_objtagNum + 48);
			strcpy_s(_ctagName, _tagName.c_str());
		}
		objM.GUI();
	}
	if (CollapsingHeader("SpriteList"))
	{
		if (ImGui::Button("Add") && !addObj && !addModel && !addTex && !addSpr)
		{
			_tagName = "sprite";
			addSpr = true;
			_spriteNum = spriteM.GetTagNum(_tagName);
			if (_spriteNum != 0)_tagName += (char)(_spriteNum + 48);
			strcpy_s(_ctagName, _tagName.c_str());
		}
		spriteM.GUI(&texS, &texCName);
	}
	if (CollapsingHeader("ModelList"))
	{
		if (ImGui::Button("Add") && !addModel && !addObj && !addTex && !addSpr)
		{
			_tagName = "Model";
			addModel = true;
			_ModeltagNum = modelM.GetTagNum(_tagName);
			if (_ModeltagNum != 0)_tagName += (char)(_ModeltagNum + 48);
			strcpy_s(_ctagName, _tagName.c_str());
		}
		modelM.GUI(&texM, &texCName);
	}
	if (CollapsingHeader("Camera"))
	{
		cameraM.GUI();
	}
	if (CollapsingHeader("Light"))
	{

	}
	End();
	static bool sceneMFlag = false;
	Begin("Assets", (bool*)false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	if (tex->flag)
	{
		if (ImGui::Button("Return"))tex->flag = false;
		ImGui::SameLine();
		if (ImGui::Button("Add") && !addModel && !addObj && !addTex && !addSpr)addTex = true;
	}
	tex->GUI();
	End();
	Begin("sceneView", (bool*)false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	SetWindowPos({ 200,0 });
	SetWindowSize(ImVec2(800, 40));
	Text("SceneView");
	const char start[] = "DebugStart";
	const char stop[] = "DebugStop";
	ImGui::SameLine();
	if (ImGui::Button(flag == false ? start : stop))
	{
		if (flag)objM.SetCamera(cameraM.GetCamera("debug"));
		else objM.SetCamera(cameraM.GetCamera("mainCamera"));
		flag = !flag;
	}
	ImGui::SameLine();
	Text("DebugCamera On/Off : mouseMiddle");
	End();
	Begin("SceneOutput", (bool*)false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	SetWindowPos({ 1000,0 });
	SetWindowSize(ImVec2(280, 100));
	if (ImGui::Button("SceneManager"))sceneMFlag = true;
	/*static char _sceneName[256];
	ImGui::InputText("OutputName", _sceneName, sizeof(_sceneName));
	static bool error = false;
	static bool error2 = true;
	static bool success = false;
	static bool success2 = false;
	if (ImGui::Button("DataOutput"))
	{
		if (!strcmp(_sceneName, "\0"))
		{
			error = true;
			success = false;
			success2 = false;
			error2 = true;
		}
		else
		{
			OutputJson(_sceneName);
			error = false;
			success = true;
			success2 = false;
			error2 = true;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("DataInput"))
	{
		if (!strcmp(_sceneName, "\0"))
		{
			error = true;
			success2 = false;
			success = false;
			error2 = true;
		}
		else
		{
			error2 = InputJson(_sceneName);
			error = false;
			success = false;
			success2 = true;
		}
	}
	if (error)Text("Error : Please enter file name");
	if (!error2)Text("Error : File not found");
	if (success)Text("File output succeeded");
	if (success2)Text("File successfully entered");*/
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
			ImGui::RadioButton("Billboard", &typeB, 1);
			ImGui::SameLine();
			ImGui::RadioButton("YBillboard", &typeB, 2);
			ImGui::TreePop();
		}
		InputText("Tag", _ctagName, sizeof(_ctagName));
		if (ImGui::Button("Add"))
		{
			if (typeM == 0)
			{
				objM.Add<UsuallyObj>(modelM.GetModel(_mtag), _ctagName, typeB);
			}
			if (typeM == 1)
			{
				objM.Add<PlayerObj>(modelM.GetModel(_mtag), _ctagName, typeB);
			}
			addObj = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))addObj = false;
		End();
	}
	if (addModel)
	{
		static int smoot = 1;
		static int loadMode = 0;
		static char _faileName[256] = "file";
		static int texNum = 1;
		Begin("NewModelSetting", (bool*)false, ImGuiWindowFlags_NoResize);
		if (ImGui::TreeNode("Smoothing"))
		{
			ImGui::RadioButton("true", &smoot, 1);
			ImGui::SameLine();
			ImGui::RadioButton("false", &smoot, 0);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("LoadMode"))
		{
			ImGui::RadioButton("LoadModel", &loadMode, LOAD_MODEL);
			ImGui::SameLine();
			ImGui::RadioButton("CreateCube", &loadMode, CREATE_CUBE);
			ImGui::SameLine();
			ImGui::RadioButton("CreateTriangle", &loadMode, CREATE_TRIANGLE);
			ImGui::RadioButton("CreateCircle", &loadMode, CREATE_CIRCLE);
			ImGui::SameLine();
			ImGui::RadioButton("CreateSphere", &loadMode, CREATE_SPHERE);
			if (loadMode == 0)InputText("FaileName", _faileName, sizeof(_faileName));
			ImGui::TreePop();
		}
		if (loadMode >= 1)
		{
			if (ImGui::TreeNode("Texture"))
			{
				tex->TexNum(&texNum);
				ImGui::TreePop();
			}
		}
		InputText("Tag", _ctagName, sizeof(_ctagName));
		static bool error = false;
		if (ImGui::Button("Add"))
		{
			if (loadMode == 0)
			{
				if (!modelM.Load(_ctagName, smoot, _faileName))error = true;
				else
				{
					error = false;
					addModel = false;
				}
			}
			if (loadMode >= 1)
			{
				modelM.Create(_ctagName, smoot, texNum, loadMode);
				addModel = false;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			addModel = false;
			error = false;
		}
		if (error)
		{
			Text("Error : File not found");
		}
		End();
	}
	if (addTex)
	{
		static char _faileName[256] = "file";
		Begin("NewTextureSetting", (bool*)false, ImGuiWindowFlags_NoResize);
		InputText("FaileName", _faileName, sizeof(_faileName));
		static bool error = false;
		if (ImGui::Button("Add"))
		{
			if (!tex->LoadTexture(_faileName))error = true;
			else
			{
				error = false;
				addTex = false;
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			addTex = false;
			error = false;
		}
		if (error)
		{
			Text("Error : File not found");
		}
		End();
	}
	if (addSpr)
	{
		Begin("NewSpriteSetting", (bool*)false, ImGuiWindowFlags_NoResize);
		InputText("Tag", _ctagName, sizeof(_ctagName));
		static int texNum = 1;
		if (ImGui::TreeNode("LoadTexture"))
		{
			tex->TexNum(&texNum);
			ImGui::TreePop();
		}
		if (ImGui::Button("Add"))
		{
			spriteM.Add((unsigned short)texNum, _ctagName);
			addSpr = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			addSpr = false;
		}
		End();
	}
	if (texM)
	{
		Begin("textureManager", (bool*)false, ImGuiWindowFlags_NoResize);
		static int texNum = modelM.GetTexture(texCName);
		if (ImGui::TreeNode("LoadTexture"))
		{
			tex->TexNum(&texNum);
			ImGui::TreePop();
		}
		if (ImGui::Button("change"))
		{
			modelM.SetTexture(texNum, texCName);
			texM = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			texM = false;
		}
		End();
	}
	if (texS)
	{
		Begin("textureManager", (bool*)false, ImGuiWindowFlags_NoResize);
		static int texNum = spriteM.GetTexture(texCName);
		if (ImGui::TreeNode("LoadTexture"))
		{
			tex->TexNum(&texNum);
			ImGui::TreePop();
		}
		if (ImGui::Button("change"))
		{
			spriteM.SetTexture(texNum, texCName);
			texS = false;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			texS = false;
		}
		End();
	}
	if (sceneMFlag)
	{
		SceneManager::Instance()->GUI(sceneMFlag);
	}
	/*spherePos = objM.GetComponent<PlayerObj>()->GetPos();*/
	/*light->SetCircleShadowCasterPos(0, spherePos);*/
	Input* input = Input::Instance();
	input->Update();
	if (flag) {

		cameraM.Update("mainCamera");
	}
	else
	{
		cameraM.Update("debug");
	}

	if (input->KTriggere(KEY::ENTER))
	{
		SceneManager::Instance()->Next(0);
		hoge++;
		if (hoge > 4)hoge -= 5;
	}
#else

	Input* input = Input::Instance();
	input->Update();

	//光源
	static float dlColor[] = { 1,1,1 };
	static Float3 spherePos = { -1,0,0 };


	if (input->KTriggere(KEY::ENTER))
	{
		SceneManager::Instance()->Next(0);
		hoge++;
		if (hoge > 4)hoge -= 5;
	}

	cameraM.Update();

#endif // _DEBUG

	dText.Print(100, 50, 1.5, "now scene  : %d",hoge);
	dText.Print(100, 100, 1.5, "next scene : Enter key");
	if (hoge < 4)dText.Print(100, 140, 1.5, "camera     : Arrow key");
	else
	{
		dText.Print(100, 140, 1.5, "DebugCamera");
		dText.Print(100, 170, 1.5, "MouseL     : rotate");
		dText.Print(100, 200, 1.5, "MouseR     : move");
		dText.Print(100, 230, 1.5, "MouseMWheel: distance");
	}
	
	light->Update();

	objM.Update();
	spriteM.Update();
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
	spriteM.Draw();

	dText.Draw(viewport);
#ifdef _DEBUG
	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList.Get());

	//デバッグ用

#endif // _DEBUG
}

void IF::Scene::Delete()
{
	//sound->SoundUnLoad(testSound);
	//sound->Reset();
}
