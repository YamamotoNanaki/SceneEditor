#pragma once
#include "ConstStruct.h"
#include <wrl.h>
#include <string>
#include <vector>
#include <d3d12.h>

namespace IF
{
	const UINT MAX_BONE_INDICES = 4;
	struct VertexBone
	{
		Float3 pos{};			//XYZ座標
		Float3 normal{};		//法線ベクトル
		Float2 uv{};			//UV座標
		UINT boneIndex[MAX_BONE_INDICES] = {};
		float boneWeight[MAX_BONE_INDICES] = {};
	};
	class Mesh
	{
		struct Material
		{
			Float3 ambient;
			Float3 diffuse;
			Float3 specular;
			float alpha;
			std::string textureFilename;
			Material()
			{
				ambient = { 0.3f,0.3f,0.3f };
				diffuse = { 0.0f,0.0f,0.0f };
				specular = { 0.0f,0.0f,0.0f };
				alpha = 1.0f;
			}
			bool tex = false;
			unsigned short texNum = 1;
		};
	public:
		Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff = nullptr;
		Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff = nullptr;
		std::vector<VertexBone> vertices;
		Material material{};
		//定数バッファ
		Microsoft::WRL::ComPtr<ID3D12Resource> constBuffTransform1;
		ConstBufferMaterial* constMapMaterial = nullptr;

	public:
		std::vector<UINT> indices;
		D3D12_VERTEX_BUFFER_VIEW vbView{};
		D3D12_INDEX_BUFFER_VIEW ibView{};

	public:
		void Draw(ID3D12Resource* address);
		void Draw(ID3D12Resource* address, unsigned short texNum);
		void SetVerticleIndex(std::vector<VertexBone> vertices, size_t vertexCount, std::vector<UINT> indices, size_t indexCount);
		void SetVerticleIndex(VertexBone* vertices, size_t vertexCount, UINT* indices, size_t indexCount);
		void Initialize();
		D3D12_VERTEX_BUFFER_VIEW& GetVertexView();
		D3D12_INDEX_BUFFER_VIEW& GetIndexView();
		size_t GetSize();
	};
};
