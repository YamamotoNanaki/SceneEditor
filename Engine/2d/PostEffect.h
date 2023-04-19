#pragma once
#include <wrl.h>
#include "Sprite.h"
#include "ConstStruct.h"

namespace IF
{
	class PostEffect : public Sprite
	{
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource>texBuff[2];
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descHeapSRV;
		Microsoft::WRL::ComPtr<ID3D12Resource>depthBuff;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descHeapRTV;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descHeapDSV;
		Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineState;
		Microsoft::WRL::ComPtr<ID3D12PipelineState>pipelineStateAdd;
		Microsoft::WRL::ComPtr<ID3D12RootSignature>rootSignature;
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuffPostEffect = nullptr;
		ConstBufferPostEffect* constMapPostEffect = nullptr;

	private:
		static const float clearColor[4];
		static unsigned int PostEfectTexNum;

	public:
		PostEffect();
		~PostEffect();
		void Draw(bool add = false);
		void Update();
		void Initialize();
		void DrawBefore();
		void DrawAfter();
		/// <summary>
		/// RGBシフトをします
		/// </summary>
		/// <param name="shift">0~0.005以内がちょうど良いと思います</param>
		void SetRGBShift(float shift);

	private:
		void CreateGraphicsPipelineState();
	};
}
