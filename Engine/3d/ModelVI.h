#pragma once
#include <vector>
#include <wrl.h>
#include <d3d12.h>
#include <unordered_map>
#include "IFMath.h"

namespace IF
{
	struct Vertex
	{
		Float3 pos;			//XYZ座標
		Float3 normal;		//法線ベクトル
		Float2 uv;			//UV座標
	};
	class MVI
	{
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff = nullptr;
		std::vector<Vertex> vertices;
		std::unordered_map<unsigned short, std::vector<unsigned short>>smoothData;

	public:
		std::vector<unsigned short> indices;
		D3D12_VERTEX_BUFFER_VIEW vbView{};
		D3D12_INDEX_BUFFER_VIEW ibView{};

	public:
		void SetVerticleIndex(std::vector<Vertex> vertices, size_t vertexCount, std::vector<unsigned short> indices, size_t indexCount);
		void SetVerticleIndex(Vertex* vertices, size_t vertexCount, unsigned short* indices, size_t indexCount);
		void Initialize(ID3D12Device* device, bool smoothing, bool flag = false);
		D3D12_VERTEX_BUFFER_VIEW& GetVertexView();
		D3D12_INDEX_BUFFER_VIEW& GetIndexView();
		size_t GetSize();
		void AddSmoothData(unsigned short indexPos, unsigned short indexVer);
	};
}