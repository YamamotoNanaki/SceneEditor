//#include "ParticleVI.h"
//
//using namespace DirectX;
//using namespace IF;
//using namespace std;
//
//void IF::PV::SetVerticleIndex(VertexPos* vertices, int vertexCount)
//{
//	for (int i = 0; i < vertexCount; i++)
//	{
//		this->vertices.emplace_back(vertices[i]);
//	}
//}
//
//void PV::Initialize(ID3D12Device* device)
//{
//	HRESULT result;
//
//	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
//	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());
//
//
//	//---------------------------
//
//	// 頂点バッファの設定
//	D3D12_HEAP_PROPERTIES heapProp{};   // ヒープ設定
//	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
//
//	D3D12_RESOURCE_DESC resDesc{};  // リソース設定
//	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
//	resDesc.Height = 1;
//	resDesc.DepthOrArraySize = 1;
//	resDesc.MipLevels = 1;
//	resDesc.SampleDesc.Count = 1;
//	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//	// 頂点バッファの生成
//
//	result = device->CreateCommittedResource(
//		&heapProp, // ヒープ設定
//		D3D12_HEAP_FLAG_NONE, &resDesc, // リソース設定
//		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff));
//	assert(SUCCEEDED(result));
//
//	//------------------------
//
//// GPU上のバッファに対応した仮想メモリを取得
//	VertexPos* vertMap = nullptr;
//	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
//	assert(SUCCEEDED(result));
//
//	// 全頂点に対して
//	for (int i = 0; i < vertices.size(); i++)
//	{
//		vertMap[i] = vertices[i];   // 座標をコピー
//	}
//
//	// マップを解除
//	vertBuff->Unmap(0, nullptr);
//
//	//-----------------------------
//
//// 頂点バッファビューの作成
//
//	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
//	vbView.SizeInBytes = sizeVB;
//	vbView.StrideInBytes = sizeof(vertices[0]);
//}
//
//D3D12_VERTEX_BUFFER_VIEW& IF::PV::GetVertexView()
//{
//	return vbView;
//}
//
//size_t IF::PV::GetSize()
//{
//	return vertices.size();
//}
