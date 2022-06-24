#pragma once
#include "ConstBuff.h"
#include "View.h"
#include "Projection.h"
#include "Texture.h"
#include "Graphic.h"
#include "Fire.h"
#include "Light.h"
#include "Sprite.h"
#include "IScene.h"
#include "Sound.h"
#include "GUI.h"
#include "ObjectManager.h"
#include "ModelManager.h"

#ifdef _DEBUG
#include "DebugText.h"

#endif // _DEBUG


namespace IF
{
	class Scene :public IScene
	{
		template <class T>using vector = std::vector<T>;
		template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	private:
		//変数宣言
		//-----------------------

		float angleY = 0.0f;
		float angleX = 0.0f;
		int sukai = 0;
		int efect = 0;
		int sky = 0;
		int groundTex = 0;
		Float3 slDir{ 0.0f,-1.0f,0.0f };
		Float3 slPos{ 0.0f,5.0f,0.0f };
		Float3 slColor{ 1.0f,1.0f,1.0f };
		Float3 slAtten{ 0.0f,0.0f,0.0f };
		Float2 slAngle{ 20.0f,30.0f };
		Float3 csDir{ 0.0f,-1.0f,0.0f };
		Float3 csAtten{ 0.5f,0.6f,0.0f };
		Float2 csAngle{ 0.0f,0.7f };
		Sprite sprite{};
		unsigned int SGraph = 0;
		unsigned int testSound = 0;

		//-----------------------

		//光源
		LightManager* light = nullptr;
		//音源
		Sound* sound = Sound::Instance();

		//マネジャー
		ObjectManager objM;
		ModelManager modelM;

	public:
		Texture* tex = Texture::Instance();
		Graphic* graph = Graphic::Instance();

	private:
		int winWidth{};
		int winHeight{};
		ComPtr<ID3D12Device> device;
		ComPtr<ID3D12GraphicsCommandList> commandList;
		vector<D3D12_VIEWPORT> viewport;
		HWND hwnd = nullptr;

	private:
		GUI gui;

		//デバッグ用
#ifdef _DEBUG
		DebugText dText;

#endif // _DEBUG
	private:
		void WriteData(char* _sceneName);

	public:
		void Initialize()override;
		void StaticInitialize(int winWidth, int winHeight, ID3D12Device* device, ID3D12GraphicsCommandList* commandList, vector<D3D12_VIEWPORT> viewport, HWND& hwnd)override;
		void Update()override;
		void Draw()override;
		void Delete()override;
	};
}