#pragma once
//#include <DirectXMath.h>
//#include <vector>
//#include <wrl.h>
//#include <d3d12.h>
//
//namespace IF
//{
//	struct VertexPos
//	{
//		DirectX::XMFLOAT3 pos;			//XYZç¿ïW
//	};
//	class PV
//	{
//	private:
//		Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;
//
//	public:
//		std::vector<VertexPos> vertices;
//		D3D12_VERTEX_BUFFER_VIEW vbView{};
//
//	public:
//		void SetVerticleIndex(VertexPos* vertices, int vertexCount);
//		void Initialize(ID3D12Device* device);
//		D3D12_VERTEX_BUFFER_VIEW& GetVertexView();
//		size_t GetSize();
//	};
//}