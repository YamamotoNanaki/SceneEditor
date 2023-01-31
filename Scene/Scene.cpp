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
#include "Window.h"
#include "CollisionObj.h"
#include "Ease.h"
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
	graph->InitializeFBX(tex->descRangeSRV, L"Data/Shaders/ModelAnimVS.hlsl", L"Data/Shaders/ModelAnimPS.hlsl", L"Data/Shaders/ModelAnimGS.hlsl");
	float winWidth = Window::Instance()->winWidth;
	float winHeight = Window::Instance()->winHeight;
	cameraM->Add<Camera>("mainCamera", 45, winWidth, winHeight);
	lightM->Initialize();
	lightM->DefaultLightSetting();
	for (int i = 0; i < 3; i++)
	{
		lightM->SetDirLightActive(i, false);
		lightM->SetPointLightActive(i, false);
		lightM->SetSpotLightActive(i, false);
	}
	lightM->SetDirLightActive(0, true);
	lightM->SetDirLightDir(0, { -1,-1,0.7 });

	objM->SetCamera(cameraM->GetCamera("mainCamera"));
	DebugText::Instance()->Initialize(tex->LoadTexture("debugfont.png", 1022));
	//#ifdef _DEBUG
	gui.Initialize();
	//#endif
	postEffect = DEBUG_NEW PostEffect;
	postEffect->Initialize();

}
//#ifdef _DEBUG

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
			Reset();
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
//#endif
void IF::Scene::InputJson(std::string failename)
{
	std::ifstream reading_file;
	string scene = failename;
	string txt = ".json";
	string name = "Data/Scene/";
	string type = "Texture";
	string error = "Basic";
	string faile;
	static bool once = false;
	if (!once)
	{
		faile = name + type + txt;
		reading_file.open(faile, std::ios::in);
		json j;
		reading_file >> j;
		reading_file.close();
		for (auto& i : j["texture"]["name"])
		{
			tex->LoadTexture(i);
		}
		once = true;
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
	float winWidth = Window::Instance()->winWidth;
	float winHeight = Window::Instance()->winHeight;
	for (auto i : j3["camera"])
	{
		if ("Camera" == i["CameraName"])cameraM->Add<Camera>(i["tag"], 45.0f, winWidth, winHeight);
		else if ("Debug" == i["CameraName"])cameraM->Add<DebugCamera>(i["tag"], 45.0f, winWidth, winHeight);
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
	objM->IntputJson(j4);

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
		Sprite* s = spriteM->Add(i["tex"], i["tag"], i["type"], { i["anchorpoint"]["x"],i["anchorpoint"]["y"] });
		spriteM->SetPosition({ i["pos"]["x"],i["pos"]["y"] }, i["tag"]);
		spriteM->SetRotation(i["rot"], i["tag"]);
		spriteM->SetScale({ i["sca"]["x"],i["sca"]["y"] }, i["tag"]);
		s->color[0] = i["color"]["r"];
		s->color[1] = i["color"]["g"];
		s->color[2] = i["color"]["b"];
		s->color[3] = i["color"]["a"];
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

	nowScene = failename;
	if (nowScene == "scene")
	{
		Normal* obj0 = objM->GetAddress<Normal>("Normal");
		Normal* obj1 = objM->GetAddress<Normal>("Normal1");
		obj0->SetToon(true);
		obj1->SetToon(true);
	}
	if (nowScene == "scene2")
	{
		FBXModel* fmodel = modelLoader.FBXLoad("simple", ".gltf", true);
		Normal* obj0 = objM->GetAddress<Normal>("Normal");
		if (obj0 && fmodel)
		{
			obj0->FBXInitialize(fmodel, false);
		}
	}
	if (nowScene == "scene3")
	{
		Normal* obj0 = objM->GetAddress<Normal>("Normal");
		obj0->NormalInitialize();
	}
	if (nowScene == "scene5")
	{
		for (int i = 0; i < soundNums.size(); i++)
		{
			string s = "testsound";
			s += to_string(i);
			soundNums[i] = Sound::Instance()->LoadWave(s);
		}
	}
	sceneChange = false;
}

void IF::Scene::StaticInitialize()
{
	Sprite::StaticInitialize();
	Object::StaticInitialize();
	Emitter::StaticInitialize();
}

void IF::Scene::Update()
{
	Input::Instance()->Input::Update();
	if (Input::Instance()->KeyTriggere(KEY::ENTER) && !sceneChange)
	{
		sceneChange = true;
		sceneNumber++;
		if (sceneNumber == 1)
		{
			SceneManager::Instance()->SceneChange("MainScene");
		}
		else if (sceneNumber == 2)
		{
			SceneManager::Instance()->SceneChange("scene");
		}
		else if (sceneNumber == 3)
		{
			SceneManager::Instance()->SceneChange("scene1");
		}
		else if (sceneNumber == 4)
		{
			SceneManager::Instance()->SceneChange("scene2");
		}
		else if (sceneNumber == 5)
		{
			SceneManager::Instance()->SceneChange("scene3");
		}
		else if (sceneNumber == 6)
		{
			SceneManager::Instance()->SceneChange("scene5");
		}
		else if (sceneNumber == 7)
		{
			SceneManager::Instance()->SceneChange("scene4");
		}
		else
		{
			SceneManager::Instance()->SceneChange("startScene");
			sceneNumber = 0;
		}
	}
	if (sceneChange)
	{
		postEffect->SetRGBShift(0);
		postEffect->SetNega(false);
		postEffect->SetGrayscale(false);
		postEffect->SetSepia(0);
	}

#ifdef _DEBUG
	DebugUpdate();
#else
	if (nowScene == "scene4")
	{
		DebugUpdate();
		return;
	}
	cameraM->AutoUpdate();
	objM->Update();
	spriteM->Update();
	particleM->Update();
	spriteM->DeleteSprite();
	objM->DeleteObject();

	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	string s = "scene";
	s += std::to_string(sceneNumber);


	ImGui::Begin(s.c_str());
	ImGui::Text("camera : arrow keys");
	ImGui::Text("camera reset : C key");
	ImGui::Text("Next Scene : Enter key");
	if (nowScene == "scene3")
	{
		ImGui::Text("ball movement : Controller L stick");
		ImGui::Text("ball jump : Controller ABXY");
	}
	ImGui::End();
	static float ambient[3] = { 0.4, 0.4, 0.4 };
	static bool dirLActive[3] = { true,false,false };
	static float dirLDir[3][3] = { {-1,-1,-1},{1,1,0.7},{0,0,1} };
	static float dirLColor[3][3] = { {1,1,1},{1,1,1},{1,1,1} };
	static bool pointLActive[3] = { false,false,false };
	static float pointLPos[3][3] = { {-1,-1,-1},{1,1,0.7},{0,-1,1} };
	static float pointLAtten[3][3] = { {1,1,1},{1,1,1},{1,1,1} };
	static float pointLColor[3][3] = { {1,0,0},{0,1,0},{0,0,1} };
	static bool spotLActive[3] = { false,false,false };
	static float spotLPos[3][3] = { {0,5,0},{0,5,0},{0,5,0} };
	static float spotDir[3][3] = { {0,-1,0},{0,-1,0},{0,-1,0} };
	static float spotLAtten[3][3] = { {0,0,0}, {0,0,0}, {0,0,0} };
	static float spotLFactorAngle[3][2] = { {20,30},{20,30},{20,30} };
	static float spotLColor[3][3] = { {1,1,0},{0,1,1},{1,0,1} };
	static bool shadowActive[3] = { true,true,false };
	static float shadowPos[3][3] = { {0,5,0},{0,5,0},{0,5,0} };
	static float shadowDir[3][3] = { { 0,-1,0},{ 0,-1,0},{ 0,-1,0} };
	static float shadowAtten[3][3] = { {0.5,0.6,0}, {0.5,0.6,0}, {0.5,0.6,0} };
	static float shadowFactorAngle[3][2] = { {0,0.5},{0,0.5},{0,0.5} };
	ImGui::Begin("Light Settings");
	ImGui::ColorEdit3("ambient color", ambient);
	if (ImGui::CollapsingHeader("Directional Light"))
	{
		for (int i = 0; i < 3; i++)
		{
			string s2 = "dirLight";
			s2 += to_string(i);
			if (ImGui::TreeNode(s2.c_str()))
			{
				ImGui::Checkbox("active", &dirLActive[i]);
				ImGui::DragFloat3("Dir", dirLDir[i], 0.01);
				ImGui::ColorEdit3("color", dirLColor[i]);
				ImGui::TreePop();
			}
		}
	}
	if (ImGui::CollapsingHeader("Point Light"))
	{
		for (int i = 0; i < 3; i++)
		{
			string s2 = "pointLight";
			s2 += to_string(i);
			if (ImGui::TreeNode(s2.c_str()))
			{
				ImGui::Checkbox("active", &pointLActive[i]);
				ImGui::DragFloat3("Pos", pointLPos[i], 0.01);
				ImGui::DragFloat3("atten", pointLAtten[i], 0.01);
				ImGui::ColorEdit3("color", pointLColor[i]);
				ImGui::TreePop();
			}
		}
	}
	if (ImGui::CollapsingHeader("Spot Light"))
	{
		for (int i = 0; i < 3; i++)
		{
			string s2 = "spotLight";
			s2 += to_string(i);
			if (ImGui::TreeNode(s2.c_str()))
			{
				ImGui::Checkbox("active", &spotLActive[i]);
				ImGui::DragFloat3("Pos", spotLPos[i], 0.01);
				ImGui::DragFloat3("Dir", spotDir[i], 0.01);
				ImGui::DragFloat3("atten", spotLAtten[i], 0.01);
				ImGui::DragFloat2("FactorAngle", spotLFactorAngle[i], 0.01);
				ImGui::ColorEdit3("color", spotLColor[i]);
				ImGui::TreePop();
			}
		}
	}
	if (ImGui::CollapsingHeader("Circle Shadow"))
	{
		for (int i = 0; i < 2; i++)
		{
			string s2 = "CircleShadow";
			s2 += to_string(i);
			if (ImGui::TreeNode(s2.c_str()))
			{
				ImGui::Checkbox("active", &shadowActive[i]);
				ImGui::TreePop();
			}
		}
	}
	ImGui::End();
	if (nowScene == "MainScene" || nowScene == "scene")
	{
		Normal* obj0 = objM->GetAddress<Normal>("Normal");
		Normal* obj1 = objM->GetAddress<Normal>("Normal1");
		ImGui::Begin("Object Settings");
		if (obj0)
		{
			Float3 pos = obj0->GetPos();
			Float3 rot = obj0->GetRota();
			float p[3] = { pos.x,pos.y,pos.z };
			float r[3] = { rot.x,rot.y,rot.z };
			if (ImGui::TreeNode("obj0"))
			{
				ImGui::DragFloat3("Pos", p, 0.01);
				ImGui::DragFloat3("Rot", r, 0.01);
				ImGui::TreePop();
			}
			shadowPos[0][0] = p[0];
			shadowPos[0][1] = p[1] - obj0->GetScale().y;
			shadowPos[0][2] = p[2];
			obj0->SetPos({ p[0], p[1], p[2] });
			obj0->SetRota({ r[0], r[1], r[2] });
		}
		if (obj1)
		{
			Float3 pos = obj1->GetPos();
			Float3 rot = obj1->GetRota();
			float p[3] = { pos.x,pos.y,pos.z };
			float r[3] = { rot.x,rot.y,rot.z };
			if (ImGui::TreeNode("obj1"))
			{
				ImGui::DragFloat3("Pos", p, 0.01);
				ImGui::DragFloat3("Rot", r, 0.01);
				ImGui::TreePop();
			}
			shadowPos[1][0] = p[0];
			shadowPos[1][1] = p[1] - obj1->GetScale().y;
			shadowPos[1][2] = p[2];
			obj1->SetPos({ p[0], p[1], p[2] });
			obj1->SetRota({ r[0], r[1], r[2] });
		}
		ImGui::End();
	}
	else if (nowScene == "scene1")
	{
		static float shift = 0;
		static bool s = false;
		static Timer t;
		ImGui::Begin("PostEffect");
		if (ImGui::TreeNode("RGBShift"))
		{
			ImGui::DragFloat("RGBShift", &shift, 0.001);
			if (ImGui::Button("RGBShift ex effect start"))
			{
				s = true;
				t.Set(60);
			}
			ImGui::TreePop();
		}
		static bool g = false;
		static float sepia = false;
		if (ImGui::TreeNode("grayscale"))
		{
			ImGui::Checkbox("grayscale", &g);
			ImGui::DragFloat("Sepia", &sepia, 0.001);
			ImGui::TreePop();
		}
		static bool n = false;
		if (ImGui::TreeNode("negative-positive conversion"))
		{
			ImGui::Checkbox("negative", &n);
			ImGui::TreePop();
		}
		ImGui::End();
		if (s)
		{
			t.SafeUpdate();
			int timer = t.NowTime();
			if (timer < 30)
			{
				shift = Ease::Lerp(0, 0.005f, 30, timer);
			}
			else
			{
				shift = Ease::Lerp(0.005f, 0, 30, timer - 30);
			}
			if (t.IsEnd())
			{
				s = false;
				shift = 0;
			}
		}
		postEffect->SetRGBShift(shift);
		postEffect->SetNega(n);
		postEffect->SetGrayscale(g);
		postEffect->SetSepia(sepia);
	}
	if (nowScene == "scene3")
	{
		Normal* obj0 = objM->GetAddress<Normal>("Normal");
		CollisionObj* obj1 = objM->GetAddress<CollisionObj>("CollisionObj");

		obj0->NormalUpdate();

		Float3 pos = obj0->GetPos();
		shadowPos[0][0] = pos.x;
		shadowPos[0][1] = pos.y - obj0->GetScale().y;
		shadowPos[0][2] = pos.z;

		Float3 pos2 = obj1->GetPos();
		shadowPos[1][0] = pos2.x;
		shadowPos[1][1] = pos2.y - obj1->GetScale().y;
		shadowPos[1][2] = pos2.z;
	}
	for (int i = 0; i < 3; i++)
	{
		lightM->SetDirLightActive(i, dirLActive[i]);
		lightM->SetDirLightDir(i, { dirLDir[i][0],dirLDir[i][1],dirLDir[i][2] });
		lightM->SetDirLightColor(i, { dirLColor[i][0],dirLColor[i][1],dirLColor[i][2] });
		lightM->SetPointLightActive(i, pointLActive[i]);
		lightM->SetPointLightAtten(i, { pointLAtten[i][0],pointLAtten[i][1],pointLAtten[i][2] });
		lightM->SetPointLightColor(i, { pointLColor[i][0],pointLColor[i][1],pointLColor[i][2] });
		lightM->SetPointLightPos(i, { pointLPos[i][0],pointLPos[i][1],pointLPos[i][2] });
		lightM->SetSpotLightActive(i, spotLActive[i]);
		lightM->SetSpotLightAtten(i, { spotLAtten[i][0],spotLAtten[i][1],spotLAtten[i][2] });
		lightM->SetSpotLightColor(i, { spotLColor[i][0],spotLColor[i][1],spotLColor[i][2] });
		lightM->SetSpotLightPos(i, { spotLPos[i][0],spotLPos[i][1],spotLPos[i][2] });
		lightM->SetSpotLightDir(i, { spotDir[i][0],spotDir[i][1],spotDir[i][2] });
		lightM->SetSpotLightFactorAngle(i, { spotLFactorAngle[i][0],spotLFactorAngle[i][1] });
		lightM->SetCircleShadowActive(i, shadowActive[i]);
		lightM->SetCircleShadowAtten(i, { shadowAtten[i][0],shadowAtten[i][1],shadowAtten[i][2] });
		lightM->SetCircleShadowCasterPos(i, { shadowPos[i][0],shadowPos[i][1],shadowPos[i][2] });
		lightM->SetCircleShadowDir(i, { shadowDir[i][0],shadowDir[i][1],shadowDir[i][2] });
		lightM->SetCircleShadowFactorAngle(i, { shadowFactorAngle[i][0],shadowFactorAngle[i][1] });
	}
	lightM->SetAmbientColor({ ambient[0],ambient[1],ambient[2] });
	lightM->Update();

	if (nowScene == "scene5")
	{
		Sound* inst = Sound::Instance();
		static int v[4] = { 120,50,120,120 };
		static bool r[4] = { false,false,false,false };
		ImGui::Begin("Sound Settings");
		ImGui::Text("\"isRoop\" should be set on playback");
		for (int i = 0; i < soundNums.size(); i++)
		{
			string s = "testsound";
			s += to_string(i);
			if (ImGui::TreeNode(s.c_str()))
			{
				ImGui::Checkbox("isRoop", &r[i]);
				ImGui::DragInt("volume", &v[i]);
				inst->SetVolume(soundNums[i], v[i]);
				if (ImGui::Button("BGM Start"))
				{
					inst->SoundPlay(soundNums[i], r[i]);
				}
				if (ImGui::Button("BGM Stop"))
				{
					inst->StopSound(soundNums[i]);
				}
				ImGui::TreePop();
			}
		}
		ImGui::End();
	}

#endif
}

void IF::Scene::Draw()
{
	particleM->DrawPostEffect(graph->rootsignature.Get());
	// ポストエフェクト
	Object::DrawBefore(graph->rootsignature.Get());
	postEffect->DrawBefore();

	graph->DrawBlendMode(Blend::OUTLINE);
	objM->OutLineDraw();

	graph->DrawBlendMode();
	objM->Draw();

	postEffect->DrawAfter();

	DirectX12::Instance()->DrawBefore();
	DirectX12::Instance()->DrawSetViewport();

	graph->DrawBlendMode(Blend::NORMAL2D);
	Sprite::DrawBefore(graph->rootsignature.Get());
	spriteM->BackGroundDraw();

	graph->DrawBlendMode(Blend::NOBLEND);
	postEffect->Draw();
	particleM->Draw();

	Sprite::DrawBefore(graph->rootsignature.Get());
	graph->DrawBlendMode(Blend::NORMAL2D);
	spriteM->ForeGroundDraw();

	//#ifdef _DEBUG
	ImGui::Render();
	ID3D12GraphicsCommandList* commandList = DirectX12::Instance()->GetCmdList();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);

	//デバッグ用

//#endif // _DEBUG

	DirectX12::Instance()->DrawAfter();
}

void IF::Scene::Delete()
{
	ObjectManager::DeleteInstance();
	ModelManager::DeleteInstance();
	CameraManager::DeleteInstance();
	SpriteManager::DeleteInstance();
	DebugText::DeleteInstance();
	delete postEffect;
}

void IF::Scene::Reset()
{
	Sound* inst = Sound::Instance();
	objM->Reset();
	//tex->TexReset();
	cameraM->Reset();
	modelM->Reset();
	spriteM->Reset();
	particleM->Reset();
	if (nowScene == "scene5")
	{
		for (int i = 0; i < soundNums.size(); i++)
		{
			inst->StopSound(soundNums[i]);
			inst->SoundUnLoad(soundNums[i]);
		}
	}
}
