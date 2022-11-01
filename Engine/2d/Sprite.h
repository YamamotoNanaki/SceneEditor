#pragma once
#include "SpriteVI.h"
#include "IFMath.h"
#include "ConstBuff.h"
#include "Timer.h"
#include <vector>
#include <wrl.h>
#include <string>

namespace IF
{
	struct ConstBufferColor
	{
		Float4 color;	//�F(RGBA)
	};

	struct ConstBufferMatrix
	{
		Matrix mat;
	};

	class Sprite
	{
		template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	private:
		static ComPtr<ID3D12GraphicsCommandList> commandList;
		static ComPtr<ID3D12Device> device;
		static Matrix matPro;
		static std::vector<D3D12_VIEWPORT> viewport;

	private:
		SV* vi = nullptr;
		ConstBuff cb;

	public:
		//�萔�o�b�t�@
		ComPtr<ID3D12Resource> constBuffTransform;
		//�萔�o�b�t�@�}�b�v
		ConstBufferMatrix* constMapTransform = nullptr;
		//�A�t�B���ϊ����
		Float2 scale = { 1,1 };
		float rotation = 0;
		Float2 position = { 0,0 };
		Float2 size = { 100,100 };
		//���[���h�ϊ��s��
		Matrix matWorld;
		Float2 texBase = { 0,0 };
		Float2 texSize = { 100,100 };
		Float2 anchorpoint = { 0.5,0.5 };
		bool flipX = false;
		bool flipY = false;
		std::string tag;
		unsigned int texNum = 0;
		bool deleteFlag = false;
		float color[4] = { 1,1,1,1 };

	public:
		bool drawFlag = true;
		~Sprite();
		static void StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, std::vector<D3D12_VIEWPORT> viewport, float winWidth, float winHeight);
		static void SetDeviceCommand(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
		static void SetViewport(std::vector<D3D12_VIEWPORT> viewport);
		virtual void Initialize(unsigned int texNum, Float2 size = { 100,100 }, bool flipX = false, bool flipY = false);
		virtual void TransferVertex();
		static void DrawBefore(ID3D12RootSignature* root, D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		virtual void Update();
		virtual void DebugUpdate();
		virtual void Draw();
		bool DeleteSprite();
		virtual void SetPosition(Float2 position);
		virtual void SetSize(Float2 size);
		virtual void SetTextureRect(Float2 texBase, Float2 texSize);
		virtual void SetColor(int r, int g, int b, int a);
		virtual void SetBright(int r, int g, int b);
		virtual void SetAlpha(int a);

#ifdef _DEBUG
		virtual void GUI();
#endif
	};
}