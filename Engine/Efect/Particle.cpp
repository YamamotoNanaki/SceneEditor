//#include "Particle.h"
//#include "View.h"
//#include "MathConvert.h"
//
//using namespace DirectX;
//using namespace IF;
//
//ID3D12Device* Particle::device = nullptr;
//ID3D12GraphicsCommandList* Particle::commandList = nullptr;
//PV* Particle::vi = nullptr;
//
//void Particle::Initialize()
//{
//	HRESULT result;
//	//定数バッファのヒープ設定
//	D3D12_HEAP_PROPERTIES heapProp{};
//	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
//	//定数バッファのリソース設定
//	D3D12_RESOURCE_DESC resdesc{};
//	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	resdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
//	resdesc.Height = 1;
//	resdesc.DepthOrArraySize = 1;
//	resdesc.MipLevels = 1;
//	resdesc.SampleDesc.Count = 1;
//	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//	//定数バッファの生成
//	result = device->CreateCommittedResource(
//		&heapProp, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
//		IID_PPV_ARGS(&constBuffTransform));
//	assert(SUCCEEDED(result));
//	//定数バッファのマッピング
//	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
//	assert(SUCCEEDED(result));
//
//}
//
//void Particle::VIInitialize()
//{
//	VertexPos vertices[] = {
//		// x   y   z        u    v
//		//前
//		{{0, 0, 0}},	//左下
//	};
//
//	vi->SetVerticleIndex(vertices, _countof(vertices));
//	vi->Initialize(device);
//}
//
//void IF::Particle::SetDeviceCommand(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
//{
//	Particle::device = device;
//	Particle::commandList = commandList;
//}
//
//void Particle::Update(XMMATRIX matView, XMMATRIX matProjection, XMMATRIX matBillBoard)
//{
//	XMMATRIX matScale, matTrams;
//
//	//スケール、回転、平行移動
//	matScale = XMMatrixScaling(scale, scale, scale);
//	matTrams = XMMatrixTranslation(position.x, position.y, position.z);
//	//ワールド行列の合成
//	matWorld = XMMatrixIdentity();
//	matWorld *= matScale;
//	matWorld *= matTrams;
//
//	//定数バッファへのデータ転送
//	constMapTransform->mat = MatrixConvert(matWorld * matView * matProjection);
//	constMapTransform->matBillboard = MatrixConvert(matBillBoard);
//}
//
//void IF::Particle::DrawBefore(ID3D12RootSignature* root, ID3D12DescriptorHeap* srvHeap, D3D12_GPU_VIRTUAL_ADDRESS GPUAddress, D3D_PRIMITIVE_TOPOLOGY topology)
//{
//	commandList->SetGraphicsRootSignature(root);
//	commandList->IASetPrimitiveTopology(topology);
//	commandList->SetGraphicsRootConstantBufferView(0, GPUAddress);
//}
//
//void Particle::Draw(vector<D3D12_VIEWPORT> viewport)
//{
//	for (int i = 0; i < viewport.size(); i++)
//	{
//		commandList->RSSetViewports(1, &viewport[i]);
//		//頂点バッファの設定
//		commandList->IASetVertexBuffers(0, 1, &vi->GetVertexView());
//		//定数バッファビューの設定
//		commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
//		//描画コマンド
//		commandList->DrawInstanced((UINT)vi->GetSize(), 1, 0, 0);
//	}
//}
//
//Particle::~Particle()
//{
//	constBuffTransform->Unmap(0, nullptr);
//	delete vi;
//}
