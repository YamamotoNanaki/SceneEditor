#pragma once
#include "ConstBuff.h"
#include "Texture.h"
#include "Graphic.h"
#include "Fire.h"
#include "Light.h"
#include "Sprite.h"
#include "Sound.h"
#include "GUI.h"
#include "ObjectManager.h"
#include "ModelManager.h"
#include "CameraManager.h"
#include "SpriteManager.h"
#include "DebugText.h"



namespace IF
{
	class Scene
	{
		template <class T>using vector = std::vector<T>;
		template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	private:
		//変数宣言
		//-----------------------
		ObjectManager* objM = ObjectManager::Instance();
		Texture* tex = Texture::Instance();


	public:
		void InputJson(std::string failename);
		void Initialize();
		void StaticInitialize(int winWidth, int winHeight, ID3D12Device* device, ID3D12GraphicsCommandList* commandList, vector<D3D12_VIEWPORT> viewport, HWND& hwnd);
		void Update();
		void Draw();
		void Delete();
		
		//デバッグ用
#ifdef _DEBUG
		void OutputJson(std::string failename);

#endif // _DEBUG
	};
}