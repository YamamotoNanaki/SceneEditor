#pragma once
#include <vector>
#include <wrl.h>
#include <d3d12.h>
#include "IFMath.h"

namespace IF
{
	struct VertexPos
	{
		Float3 pos;			//XYZç¿ïW
	};
	class PV
	{
	private:
		static Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff;

	public:
		static std::vector<VertexPos> vertices;
		static D3D12_VERTEX_BUFFER_VIEW vbView;

	public:
		static void SetVerticleIndex(VertexPos* vertices, int vertexCount);
		void Initialize();
		D3D12_VERTEX_BUFFER_VIEW& GetVertexView();
		size_t GetSize();
	};
}