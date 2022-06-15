#pragma once
#include <d3d12.h>
#include <WRL.h>
#include "ConstStruct.h"

namespace IF
{
	class ConstBuff
	{
		template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	private:
		ComPtr<ID3D12Resource> constBuffMaterial = nullptr;
		ConstBufferDataColor* constMapMaterial = nullptr;
	public:
		void Initialize(ID3D12Device* device);
		D3D12_GPU_VIRTUAL_ADDRESS GetGPUAddress();

	private:
		short R = 0, G = 0, B = 0, A = 0;
		float r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;

	public:
		void SetBright(int red, int green, int blue);
		void SetAlpha(int alpha);
		void SetColor(int red, int green, int blue, int alpha);
	};
}