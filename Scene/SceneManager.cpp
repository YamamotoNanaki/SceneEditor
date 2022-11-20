#include "SceneManager.h"
#include "imgui.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include "Debug.h"
#include "Window.h"
#include "Ease.h"
#include "Texture.h"
#include "Graphic.h"
#include "Input.h"
#include "ParticleManager.h"
#include <fstream>

using namespace IF;
using namespace std;
using namespace nlohmann;

void IF::SceneManager::Initialize()
{
	Load(&now);
	scene = make_unique<Scene>();
	Scene::StaticInitialize();
	scene->Initialize();
	scene->InputJson(now);
}

bool IF::SceneManager::Update()
{
#ifdef _DEBUG
	static int endgui = 0;
#endif
	//メッセージ
	if (Input::Instance()->KeyDown(KEY::ESC))
	{
		endFlag = true;

#ifdef _DEBUG
		endgui = 0;
#endif
	}
	if (Window::Instance()->Message())
	{
#ifdef _DEBUG
		scene->OutputJson(now);
#endif
		return true;
	}

#ifdef _DEBUG
#else
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	string str = "now scene : ";
	str += now;
	std::function<void()>f = [&]()
	{
		ImGui::Text("camera move  : arrow key");
		ImGui::Text("camera reset : C     key");
		ImGui::Text("next scene   : Enter key");
		if (now == "mainScene2")
		{
			ImGui::Text("emitter      : Space key");
		}
		if (now == "mainScene3")
		{
			ImGui::Text("explosion    : 1     key");
			ImGui::Text("scale        : 2     key");
			ImGui::Text("all          : 0     key");
			ImGui::Text("object reset : R     key");
		}
	};
	GUI::NewGui(f, str);
	if (now == "mainScene4")
	{
		ParticleManager::Instance()->GUI();
	}
	if (Input::Instance()->KeyTriggere(KEY::ENTER))
	{
		static int i = 2;
		chengeFlag = true;
		next = "mainScene";
		if (i > 1 && i < 5)
		{
			next += (i + 48);
			i++;
		}
		else
		{
			i = 2;
		}
	}
#endif

#ifdef _DEBUG
	if (isInitialized)//ゲーム画面への処理
	{
		if (allGreen)
		{
			scene->Update();
		}
		if (chengeFlag)
		{
			//ロード画面への遷移アニメーション終了後
			if (true)
			{
				isInitialized = false;
				allGreen = false;
				sceneInitialize = std::async(std::launch::async, [this] {return SceneInitialize(); });
			}
			else//ロード画面への遷移アニメーション処理
			{

			}
		}
		else//ゲーム画面への遷移アニメーション処理
		{

		}
	}
	else//ロード画面の処理
	{


	}

#else
	if (isInitialized)//ゲーム画面への処理
	{
		if (sceneChengeTimer.NowTime() <= 0)
		{
			scene->Update();
		}
		if (chengeFlag)
		{
			//ロード画面への遷移アニメーション終了後
			if (sceneChengeTimer.IsEnd())
			{
				//camera.Update();
				isInitialized = false;
				//loadObj.GameInit();
				sceneInitialize = std::async(std::launch::async, [this] {return SceneInitialize(); });
			}
			else//ロード画面への遷移アニメーション処理
			{
				sceneChengeTimer.SafeUpdate();
				//黒全画面スプライトのフェードイン
			}
		}
		else//ゲーム画面への遷移アニメーション処理
		{
			//camera.Update();
			sceneChengeTimer.SafeDownUpdate();
			//黒全画面スプライトのフェードアウト(絶対アルファを0にすること)
		}
	}
	else//ロード画面の処理
	{
		//camera.Update();
		//loadObj.Update();

	}
#endif
	//scene->Update();
	//if (chengeFlag)
	//{
	//	scene->InputJson(next);
	//	now = next;
	//	next = "";
	//	chengeFlag = false;
	//	isInitialized = true;
	//}

#ifdef _DEBUG
	ImGui::Begin("SceneSave");
	if (ImGui::Button("SAVE"))
	{
		scene->OutputJson(now);
	}
	ImGui::End();
	GUI();
	if (endFlag == true)
	{
		ImGui::Begin("End");
		if (endgui == 0)
		{
			ImGui::Text("SceneSave");
			if (ImGui::Button("YES"))
			{
				Output();
				scene->OutputJson(now);
				endgui = 1;
			}
			ImGui::SameLine();
			if (ImGui::Button("No"))
			{
				endgui = 1;
			}
		}
		if (endgui == 1)
		{
			ImGui::Text("ClosedWindow");
			if (ImGui::Button("YES"))
			{
				return true;
			}
			ImGui::SameLine();
			if (ImGui::Button("No"))
			{
				endgui = 0;
				endFlag = false;
			}
		}
		ImGui::End();
	}
	return false;
#else
	return endFlag;
#endif
}

void IF::SceneManager::Draw()
{
#ifdef _DEBUG
	if (isInitialized)
	{
		scene->Draw();
		allGreen = true;
	}
#else
	if (isInitialized)
	{
		if (!chengeFlag)
		{
			scene->Draw();
		}
	}
	//黒全画面スプライトDraw;
#endif
}

void IF::SceneManager::Delete()
{
	scene->Delete();
}

void IF::SceneManager::Load(std::string* startscene)
{
	std::ifstream reading_file;
	string name = "Data/Scene/SceneManager.json";
	reading_file.open(name, std::ios::in);
	json j;
	reading_file >> j;
	reading_file.close();

	for (auto i : j["scene"])
	{
		sceneList.push_back(i["name"]);
	}

	int i = 0;
#ifdef _DEBUG
	if (startscene != nullptr)*startscene = j["debug"];
	start = j["start"];
#else
	if (startscene != nullptr)*startscene = j["start"];
	debugstart = j["debug"];
#endif
}

void IF::SceneManager::SceneInitialize()
{
	if (now == "mainScene4")
	{
		string scene = now;
		string txt = ".json";
		string name = "Data/Scene/";
		string faile;
		ofstream writing_file;
		json j2;
		ParticleManager::Instance()->OutputJson(j2);
		string s = j2.dump(4);
		string type = "Particle";
		faile = name + scene + type + txt;
		writing_file.open(faile, std::ios::out);
		writing_file << s << std::endl;
		writing_file.close();
	}
	scene->InputJson(next);
	now = next;
	next = "";
	chengeFlag = false;
	isInitialized = true;
}

void IF::SceneManager::SceneChange(std::string sceneName)
{
	chengeFlag = true;
	next = sceneName;
	sceneChengeTimer.Set(20);
}

SceneManager* IF::SceneManager::Instance()
{
	static SceneManager* inst = DEBUG_NEW SceneManager;
	return inst;
}

void IF::SceneManager::Release()
{
	delete SceneManager::Instance();
}

#ifdef _DEBUG
void IF::SceneManager::Output()
{
	json j;
	int i = 0;
	j["start"] = start;
	for (auto com : sceneList)
	{
		j["scene"][i]["name"] = com;
		i++;
	}

#ifdef _DEBUG
	j["debug"] = now;
#else
	j["debug"] = debugstart;
#endif

	string s = j.dump(4);
	ofstream writing_file;
	string name = "Data/Scene/SceneManager.json";
	writing_file.open(name, std::ios::out);
	writing_file << s << std::endl;
	writing_file.close();
}

void IF::SceneManager::GUI()
{
	int i = 0;
	static vector<int> mode;
	static int mode2 = 0;
	static int mode3 = 0;
	static int mode4 = 0;
	static int oldmode3 = 0;
	static bool flag2 = false;
	static string oldStart;
	static vector<string> next;
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
				if (com == now)mode4 = j;
				if (com == _faileName)
				{
					i++;
				}
			}
			else
			{
				std::string a = _faileName + (char)(i + 48);
				if (com == a)
				{
					i++;
				}
			}
			j++;
		}
		if (i != 0)_faileName += (char)(i + 48);
		strcpy_s(name, _faileName.c_str());
	}
	ImGui::Begin("SceneManager");
	ImGui::Text("NowScene : %s", now.c_str());
	if (ImGui::TreeNode("NewScene"))
	{
		ImGui::InputText("SceneName", name, sizeof(name));
		if (ImGui::Button("add"))
		{
			sceneList.push_back(name);
		}
		ImGui::TreePop();
	}
	i = 0;
	if (ImGui::TreeNode("ChangeScene"))
	{
		for (auto com : sceneList)
		{
			if (i % 3)ImGui::SameLine();
			ImGui::RadioButton(com.c_str(), &mode4, i);
			i++;
		}
		if (ImGui::Button("Change"))
		{
			//scene->OutputJson(now);
			scene->InputJson(sceneList[mode4]);
			now = sceneList[mode4];
		}
		ImGui::TreePop();
	}
	i = 0;
	if (ImGui::TreeNode("StartScene"))
	{
		for (auto com : sceneList)
		{
			if (i % 3)ImGui::SameLine();
			ImGui::RadioButton(com.c_str(), &mode2, i);
			if (mode2 == i)
			{
				start = com;
			}
			i++;
		}
		ImGui::Text("now start scene : %s", oldStart.c_str());
		ImGui::Text("new start scene : %s", start.c_str());
		ImGui::TreePop();
	}
	i = 0;
	if (ImGui::Button("Confirm"))
	{
		flag2 = false;
		Output();
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancel"))
	{
		start = oldStart;
		flag2 = false;
	}
	ImGui::End();
}
#endif

IF::SceneManager::~SceneManager()
{
	Delete();
}
