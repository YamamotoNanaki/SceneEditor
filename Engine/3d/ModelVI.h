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
	struct VertexID
	{
		float vertexID;
		Float3 pos;			//XYZ座標
	};
	class MVI
	{
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff = nullptr;
		std::unordered_map<unsigned int, std::vector<unsigned int>>smoothData;

	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		D3D12_VERTEX_BUFFER_VIEW vbView{};
		D3D12_INDEX_BUFFER_VIEW ibView{};

	public:
		void SetVerticleIndex(std::vector<Vertex> vertices, size_t vertexCount, std::vector<unsigned int> indices, size_t indexCount);
		void SetVerticleIndex(Vertex* vertices, size_t vertexCount, unsigned int* indices, size_t indexCount);
		void Initialize(bool smoothing, bool flag = false);
		D3D12_VERTEX_BUFFER_VIEW& GetVertexView();
		D3D12_INDEX_BUFFER_VIEW& GetIndexView();
		size_t GetSize();
		void AddSmoothData(unsigned int indexPos, unsigned int indexVer);
		void ResetVerticle();
	};
	class MV
	{
	private:
		Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;
		std::unordered_map<unsigned int, std::vector<unsigned int>>smoothData;

	public:
		std::vector<VertexID> vertices;
		D3D12_VERTEX_BUFFER_VIEW vbView{};

	public:
		void SetVerticle(std::vector<VertexID> vertices, size_t vertexCount);
		void SetVerticle(VertexID* vertices, size_t vertexCount);
		void Initialize();
		D3D12_VERTEX_BUFFER_VIEW& GetVertexView();
		size_t GetSize();
		void ResetVerticle();
	};
}