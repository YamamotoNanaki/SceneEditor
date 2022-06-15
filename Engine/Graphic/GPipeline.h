#pragma once
#include <d3d12.h>

namespace IF
{
	class GPipeline
	{
	public:
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc[12] = {};
		GPipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, ID3DBlob* gsBlob, D3D12_INPUT_ELEMENT_DESC* inputLayout, int layoutCount);
		void RootSignature(ID3D12RootSignature& rootsignature);

	};
}