#pragma once
#include "Scene.h"
#include <list>
#include <string>
#include <memory>
#include <future>
#include "Sprite.h"
namespace IF
{
	class SceneManager
	{
	private:
		std::unique_ptr<Scene> scene;
		std::vector<std::string>sceneList{};
		std::string now;
		std::string next;
		std::string start;
		std::string debugstart;
		std::future<void> sceneInitialize;//非同期のロード
		ID3D12GraphicsCommandList* commandList = nullptr;
		SceneManager() {}
		SceneManager(const SceneManager&) {}
		SceneManager& operator=(const SceneManager&) {}
		~SceneManager();
		Timer sceneChengeTimer;
		bool endFlag = false;
		bool chengeFlag = false;
		bool isInitialized = true;//初期化処理が終わったかどうか

		//Camera camera;
	public:
		inline std::string GetNowScene()
		{
			return now;
		}
		void Delete();
		void Initialize();
		bool Update();
		void Draw();
		void Load(std::string* startscene = nullptr);
		void SceneChange(std::string sceneName);
		void SceneInitialize();
		static SceneManager* Instance();
		static void Release();

#ifdef _DEBUG
		void Output();
		void GUI();
		bool allGreen = true;
#endif
	};
}
