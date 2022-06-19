#pragma once
#include "ConstBuff.h"
#include "Model.h"
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
		//�ϐ��錾
		//-----------------------

		float angleY = 0.0f;
		float angleX = 0.0f;
		int sukai = 0;
		int efect = 0;
		int sky = 0;
		int groundTex = 0;
		Model* sphereM = nullptr;
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

		//�n�ʂƋ�
		Model* domeM = nullptr;
		Model* groundM = nullptr;
		//�J����
		View matView;
		Projection* matPro = nullptr;
		//����
		LightManager* light = nullptr;
		//����
		Sound* sound = Sound::Instance();
		ObjectManager obj;

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

		//�f�o�b�O�p
#ifdef _DEBUG
		DebugText dText;

#endif // _DEBUG


	public:
		void Initialize()override;
		void StaticInitialize(int winWidth, int winHeight, ID3D12Device* device, ID3D12GraphicsCommandList* commandList, vector<D3D12_VIEWPORT> viewport, HWND& hwnd)override;
		void Update()override;
		void Draw()override;
		void Delete()override;
	};
}