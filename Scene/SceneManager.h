#pragma once
#include "Scene.h"
#include <list>
#include <string>
#include <memory>

namespace IF
{
	class SceneManager
	{
	private:
		std::unique_ptr<Scene> scene;
		std::string now;
		std::vector<std::string>nownext;
		struct SceneDatas
		{
			std::vector<std::string>next;
			std::string name;
		};
		std::vector<SceneDatas>sceneList{};

		SceneManager() {}
		SceneManager(const SceneManager&) {}
		SceneManager& operator=(const SceneManager&) {}
		~SceneManager();

	public:
		void Delete();
		void Initialize(int winWidth, int winHeight, ID3D12Device* device, ID3D12GraphicsCommandList* commandList,
			std::vector<D3D12_VIEWPORT> viewport, HWND& hwnd);
		void Update();
		void Draw();
		void Load(std::string* startscene = nullptr);
		void Next(unsigned short nextNum = 0);
		static SceneManager* Instance();
		static void Release();

#ifdef _DEBUG
		void Output();
		void GUI(bool& flag);
		std::string start;
#endif
	};
}
