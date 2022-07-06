#pragma once
#include "SpriteVI.h"
#include "IFMath.h"
#include "ConstBuff.h"
#include <vector>
#include <wrl.h>
#include <string>

namespace IF
{
	struct ConstBufferColor
	{
		Float4 color;	//色(RGBA)
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
		//定数バッファ
		ComPtr<ID3D12Resource> constBuffTransform;
		//定数バッファマップ
		ConstBufferMatrix* constMapTransform = nullptr;
		//アフィン変換情報
		Float2 scale = { 1,1 };
		float rotation = 0;
		Float2 position = { 0,0 };
		Float2 size = { 100,100 };
		//ワールド変換行列
		Matrix matWorld;
		Float2 texBase = { 0,0 };
		Float2 texSize = { 100,100 };
		Float2 anchorpoint = { 0.5,0.5 };
		bool flipX = false;
		bool flipY = false;
		std::string tag;
		unsigned int texNum = 0;

	public:
		~Sprite();
		static void StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, std::vector<D3D12_VIEWPORT> viewport, float winWidth, float winHeight);
		static void SetDeviceCommand(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
		static void SetViewport(std::vector<D3D12_VIEWPORT> viewport);
		void Initialize(unsigned int texNum, Float2 size = { 100,100 }, bool flipX = false, bool flipY = false);
		void TransferVertex();
		static void DrawBefore(ID3D12RootSignature* root, D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		void Update();
		void Draw();
		void SetPosition(Float2 position);
		void SetSize(Float2 size);
		void SetTextureRect(Float2 texBase, Float2 texSize);
		void SetColor(int r, int g, int b, int a);
		void SetBright(int r, int g, int b);
		void SetAlpha(int a);
	};
}