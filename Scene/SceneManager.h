#pragma once
#include "Scene.h"
#include <d3d12.h>
#include <vector>
//
//namespace IF
//{
//	class SceneManager
//	{
//	private:
//		IScene* scene;
//		Microsoft::WRL::ComPtr< ID3D12Device>device;
//		Microsoft::WRL::ComPtr< ID3D12GraphicsCommandList>commandList;
//		HWND hwnd;
//		std::vector<D3D12_VIEWPORT>viewport;
//		int winWidth, winHeight;
//	public:
//		void Initialize(int winWidth, int winHeight, ID3D12Device* device, ID3D12GraphicsCommandList* commandList, std::vector<D3D12_VIEWPORT> viewport,HWND& hwnd);
//		void Update();
//		void Draw();
//	private:
//		SceneManager() {};
//		~SceneManager();
//		SceneManager(const SceneManager&) {};
//		SceneManager& operator=(const SceneManager&) {};
//	};
//}
