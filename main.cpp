#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <cstdlib>
#include "Window.h"
#include "DirectX12.h"
#include "Input.h"
#include "SceneManager.h"
#include "Rand.h"

using namespace IF;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	{
		Rand::Initialize();
		const int winWidth = 1280, winHeight = 720;  // 縦幅

		Window::Instance()->Initialize(winWidth, winHeight, L"SceneEditor");
		DirectX12::Instance()->Initialize(Window::Instance()->hwnd, winWidth, winHeight);
		Input::Instance()->InputFlag(USE_INPUT_KEY | USE_INPUT_MOUSE);
		Input::Instance()->Initialize(Window::Instance()->w.hInstance, Window::Instance()->hwnd);
		LightManager::Instance()->SetDeviceCommand(DirectX12::Instance()->GetDevice(), DirectX12::Instance()->GetCmdList());
		Sound::Instance()->Initialize();
		SceneManager* sceneM = SceneManager::Instance();
		sceneM->Initialize();

		while (!Input::Instance()->KeyDown(KEY::ESC))
		{
			//メッセージ
			//if (Window::Instance()->Message())break;
			if (sceneM->Update())break;

			sceneM->Draw();
		}
		sceneM->Release();
		Input::DeleteInstance();
		Window::DeleteInstance();


#ifdef _DEBUG
		ID3D12DebugDevice* debugInterface;
		if (SUCCEEDED(DirectX12::Instance()->GetDevice()->QueryInterface(&debugInterface)))
		{
			debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
			debugInterface->Release();
		}
#endif
	}
	//_CrtDumpMemoryLeaks();
	return 0;
}