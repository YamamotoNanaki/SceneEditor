#pragma once
#include "Scene.h"
#include <list>
#include <string>

namespace IF
{
	class SceneManager
	{
	private:
		IScene* scene = nullptr;
		std::string now{};
		std::vector<std::string>next;
		std::list<std::string>sceneList;

		SceneManager();
		SceneManager(const SceneManager&);
		SceneManager& operator=(const SceneManager&) {}
		inline ~SceneManager()
		{
			delete scene;
		}

	public:
		void Initialize(int winWidth, int winHeight, ID3D12Device* device, ID3D12GraphicsCommandList* commandList,
			std::vector<D3D12_VIEWPORT> viewport, HWND& hwnd);
		void Update();
		void Draw();
		void Delete();
		void Load();
		std::string Next(unsigned short nextNum = 0);

#ifdef _DEBUG
		void Output();
		void GUI(bool& flag);
#endif
	};
}
