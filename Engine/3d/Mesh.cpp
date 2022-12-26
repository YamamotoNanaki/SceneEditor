#include "Mesh.h"
#include "DirectX12.h"
#include "Texture.h"
#include <cassert>

using namespace std;

void IF::Mesh::Draw(ID3D12Resource* address)
{
	static ID3D12GraphicsCommandList* commandList = DirectX12::Instance()->GetCmdList();
	if (material.tex == true)Texture::Instance()->SetTexture(material.texNum);
	//頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, &GetVertexView());
	//インデックスバッファの設定
	commandList->IASetIndexBuffer(&GetIndexView());
	//定数バッファビューの設定
	commandList->SetGraphicsRootConstantBufferView(2, address->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(3, constBuffTransform1->GetGPUVirtualAddress());
	//描画コマンド
	commandList->DrawIndexedInstanced((UINT)GetSize(), 1, 0, 0, 0);
}

void IF::Mesh::Draw(ID3D12Resource* address, unsigned short texNum)
{
	static ID3D12GraphicsCommandList* commandList = DirectX12::Instance()->GetCmdList();
	Texture::Instance()->SetTexture(texNum);
	//頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, &GetVertexView());
	//インデックスバッファの設定
	commandList->IASetIndexBuffer(&GetIndexView());
	//定数バッファビューの設定
	commandList->SetGraphicsRootConstantBufferView(2, address->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(3, constBuffTransform1->GetGPUVirtualAddress());
	//描画コマンド
	commandList->DrawIndexedInstanced((UINT)GetSize(), 1, 0, 0, 0);
}

void IF::Mesh::SetVerticleIndex(std::vector<VertexBone> vertices, size_t vertexCount, std::vector<UINT> indices, size_t indexCount)
{
	for (int i = 0; i < vertexCount; i++)
	{
		this->vertices.push_back(vertices[i]);
	}
	for (int i = 0; i < indexCount; i++)
	{
		this->indices.push_back(indices[i]);
	}
}

void IF::Mesh::SetVerticleIndex(VertexBone* vertices, size_t vertexCount, UINT* indices, size_t indexCount)
{
	for (int i = 0; i < vertexCount; i++)
	{
		this->vertices.push_back(vertices[i]);
	}
	for (int i = 0; i < indexCount; i++)
	{
		this->indices.push_back(indices[i]);
	}
}

void IF::Mesh::Initialize()
{
	HRESULT result;

	//定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapProp2{};
	heapProp2.Type = D3D12_HEAP_TYPE_UPLOAD;
	//定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferMaterial) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = DirectX12::Instance()->GetDevice()->CreateCommittedResource(
		&heapProp2, D3D12_HEAP_FLAG_NONE, &resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffTransform1));
	assert(SUCCEEDED(result));

	result = constBuffTransform1->Map(0, nullptr, (void**)&constMapMaterial);
	assert(SUCCEEDED(result));

	constMapMaterial->ambient = material.ambient;
	constMapMaterial->diffuse = material.diffuse;
	constMapMaterial->specular = material.specular;
	constMapMaterial->alpha = material.alpha;

	constBuffTransform1->Unmap(0, nullptr);

	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());


#pragma endregion 頂点データー

	//---------------------------

#pragma region 頂点バッファの確保
// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{};   // ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

	D3D12_RESOURCE_DESC resDesc{};  // リソース設定
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// 頂点バッファの生成
	ID3D12Device* device = DirectX12::Instance()->GetDevice();
	result = device->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE, &resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

#pragma endregion 頂点バッファの確保

	//---------------------------

#pragma region 頂点バッファへのデータ転送
// GPU上のバッファに対応した仮想メモリを取得
	VertexBone* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	// 全頂点に対して
	for (int i = 0; i < vertices.size(); i++)
	{
		vertMap[i] = vertices[i];   // 座標をコピー
	}

	// マップを解除
	vertBuff->Unmap(0, nullptr);

#pragma endregion 頂点バッファへのデータ転送

	//-----------------------------

#pragma region 頂点バッファビューの作成
// 頂点バッファビューの作成

	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

#pragma endregion 頂点バッファへビューの作成

	//-------------------------------

	if (!indices.size() == 0)
	{

#pragma region インデックスバッファの生成

		// インデックスデータ全体のサイズ
		UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices.size());
		// インデックスバッファの設定
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Width = sizeIB;//インデックスバッファの生成
		resDesc.Height = 1;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		result = device->CreateCommittedResource(
			&heapProp,				//ヒープ設定
			D3D12_HEAP_FLAG_NONE,
			&resDesc,				//リソース設定
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&indexBuff));

#pragma endregion インデックスバッファの生成

		//----------------------------

#pragma region インデックスバッファへのデータ転送
//GPU上のバッファに対応した仮想メモリを取得
		uint16_t* indexMap = nullptr;
		result = indexBuff->Map(0, nullptr, (void**)&indexMap);

		//全インデックスに対して
		for (int i = 0; i < indices.size(); i++)
		{
			indexMap[i] = indices[i];	//インデックスをコピー
		}

		//つながりを解除
		indexBuff->Unmap(0, nullptr);

		ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
		ibView.Format = DXGI_FORMAT_R16_UINT;
		ibView.SizeInBytes = sizeIB;

#pragma endregion インデックスバッファへのデータ転送

	}

}

D3D12_VERTEX_BUFFER_VIEW& IF::Mesh::GetVertexView()
{
	return vbView;
}

D3D12_INDEX_BUFFER_VIEW& IF::Mesh::GetIndexView()
{
	return ibView;
}

size_t IF::Mesh::GetSize()
{
	return indices.size();
}
