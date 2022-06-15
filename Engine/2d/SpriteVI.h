#pragma once
#include "IFMath.h"
#include <d3d12.h>
#include <wrl.h>
#include <vector>

namespace IF
{
	struct Vertex2D
	{
		Float3 pos;
		Float2 uv;
	};
	class SV
	{
		template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	private:
		// 頂点バッファ
		ComPtr<ID3D12Resource> vertBuff;
		// 頂点バッファビュー
		D3D12_VERTEX_BUFFER_VIEW vbView{};
		Vertex2D vertices[4];

	public:
		void Initialize(ID3D12Device* device);
		D3D12_VERTEX_BUFFER_VIEW& GetVertexView();
		void SetVerticle(Vertex2D* vertices);
		unsigned int GetSize();
		void Transfer();
		~SV();
	};
}