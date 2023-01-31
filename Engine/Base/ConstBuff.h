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
		void Initialize();
		~ConstBuff();
		D3D12_GPU_VIRTUAL_ADDRESS GetGPUAddress();

	public:
		void SetBright(int red, int green, int blue);
		void SetAlpha(int alpha);
		void SetColor(int red, int green, int blue, int alpha);
		void SetColor(float red, float green, float blue, float alpha);
		void SetColor(Float4 color);
		Float4 GetColor();
	};
}