#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <cstdlib>
#include "Window.h"
#include "DirectX12.h"
#include "Input.h"
//#include "Scene.h"
#include "FPS.h"
#include "SceneManager.h"
//#ifdef _DEBUG
#include "Debug.h"
//#endif // _DEBUG

using namespace IF;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	{
		const int winWidth = 1280, winHeight = 720;  // 縦幅

		Window::Instance()->Initialize(winWidth, winHeight, L"SceneEditor");
#ifdef _DEBUG
		Debug();
#endif // _DEBUG
		DirectX12::Instance()->Initialize(Window::Instance()->hwnd, winWidth, winHeight);
		Input::Instance()->Initialize(Window::Instance()->w.hInstance, Window::Instance()->hwnd);
		LightManager::Instance()->SetDeviceCommand(DirectX12::Instance()->device.Get(), DirectX12::Instance()->commandList.Get());
		Sound::Instance()->Initialize();
		/*IScene* scene = new Scene;*/
		/*scene->StaticInitialize(winWidth, winHeight, DirectX12::Instance()->device.Get(), DirectX12::Instance()->commandList.Get(), DirectX12::Instance()->viewport, Window::Instance()->hwnd);
		scene->Initialize();*/
		SceneManager* sceneM = SceneManager::Instance();
		sceneM->Initialize(winWidth, winHeight, DirectX12::Instance()->device.Get(), DirectX12::Instance()->commandList.Get(),
			DirectX12::Instance()->viewport, Window::Instance()->hwnd);
		FPS fps;
		fps.Initialize(60);

		while (!Input::Instance()->KDown(KEY::ESC))
		{
			//メッセージ
			if (Window::Instance()->Message())break;

			sceneM->Update();

			DirectX12::Instance()->DrawBefore();
			sceneM->Draw();
			DirectX12::Instance()->DrawAfter();
			fps.FPSFixed();
		}
		sceneM->Release();
		/*scene->Delete();*/
		LightManager::DeleteInstance();
		Input::DeleteInstance();
		Sound::DeleteInstance();
		Graphic::DeleteInstance();
		Texture::DeleteInstance();
		/*delete scene;*/
		DirectX12::DeleteInstance();
		Window::DeleteInstance();
	}
	//_CrtDumpMemoryLeaks();
	return 0;
}