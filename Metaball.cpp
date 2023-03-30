#include "Metaball.h"
#include "Graphic.h"
#include "ObjectManager.h"
#include "Rand.h"
#include <cstdint>

using namespace IF;
using namespace std;

void IF::Metaball::GetCamera()
{
	camera = ObjectManager::Instance()->GetCamera();
}

void IF::Metaball::Initialize()
{
	//ルックアップテーブルをテクスチャにして送信
	DataTransferGPU();
	Graphic::Instance()->InitializeMetaball(Texture::Instance()->descRangeSRV);

	TransferConstBuffer();

	//
	UpdateMargingCubesSpace();
	UpdateNumSpheres();
	GetCamera();
}

void IF::Metaball::Update()
{
	Matrix matWorld, matScale, matTrams, matRot;

	//スケール、回転、平行移動
	matScale = MatrixScaling(scale.x, scale.y, scale.z);
	matTrams = MatrixTranslation(pos.x, pos.y, pos.z);
	//ワールド行列の合成
	matWorld = MatrixIdentity();
	matWorld *= matScale;
	matWorld *= matTrams;

	//定数バッファへのデータ転送
	constMapMatrix->mat = matWorld * *camera->GetMatView() * *camera->GetMatPro();
	constMapMatrix->matBillboard = View::matBillBoard;

	static float _ftime = 0;
	constMapNumSpheres->time = _ftime;
	_ftime += 0.2;
	if (_ftime >= D3D12_FLOAT32_MAX)_ftime = 0;
	constMapNumSpheres->smoothUnionValue = 6;
}

IF::Metaball::~Metaball()
{
	matrixTransform->Unmap(0, nullptr);
	margingCubesSpaceTransform->Unmap(0, nullptr);
	numSpheresTransform->Unmap(0, nullptr);
}

void IF::Metaball::Draw()
{
	ID3D12GraphicsCommandList* commandList = DirectX12::Instance()->GetCmdList();
	//頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, &vi.GetVertexView());
	//定数バッファビューの設定
	commandList->SetGraphicsRootConstantBufferView(0, matrixTransform->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(2, margingCubesSpaceTransform->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(3, numSpheresTransform->GetGPUVirtualAddress());
	//描画コマンド
	commandList->DrawInstanced((UINT)vi.GetSize(), 1, 0, 0);
}

void IF::Metaball::DrawBefore()
{

	ID3D12GraphicsCommandList* commandList = DirectX12::Instance()->GetCmdList();
	Graphic* g = Graphic::Instance();
	g->DrawBlendMode(Blend::METABALL);
	commandList->SetGraphicsRootSignature(g->rootsignature.Get());
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	//仮
	Texture::Instance()->SetTexture(texNum);
}

void IF::Metaball::DrawAfter()
{
}

void IF::Metaball::UpdateMargingCubesSpace()
{
	const Vector3 marchingSpace = { margingSpaceSize, margingSpaceSize, margingSpaceSize };
	const Vector3 numCells = Vector3(numMarchingSegments, numMarchingSegments, numMarchingSegments);
	const Vector3 cellSize = Vector3(marchingSpace.x / numCells.x, marchingSpace.y / numCells.y, marchingSpace.z / numCells.z);

	const float numVertices = numCells.x * numCells.y * numCells.z * 15;  // 1セルの頂点の数は15個

	vi.ResetVerticle();
	vector<VertexID> vertices;
	//for (size_t i = 0; i < numVertices; i++) {
		VertexID a = { 0,{} };
	//	vertices.push_back(a);
	//	vertices.push_back(a);
	//	vertices.push_back(a);
	//}
	vertices.push_back(a);
	a.pos = { 0,1,0 };
	vertices.push_back(a);
	a.pos = { 1,0,0 };
	vertices.push_back(a);
	a.pos = { 1,1,0 };
	vertices.push_back(a);
	vi.SetVerticle(vertices, vertices.size());
	vi.Initialize();

	//constbufferに転送
	constMapMargingCubesSpace->cellSize = cellSize;
	constMapMargingCubesSpace->numCells = numCells;
}

void IF::Metaball::UpdateNumSpheres()
{
	MetaballSpheres m;
	m.pos = { 0,0,0 };
	m.scale = { 1,1,1 };
	m.color = { 0.7,0.4,0.2,1 };
	m.activ = true;
	//constbufferに転送
	for (size_t i = 0; i < numSpheres; i++)
	{
		m.randomValues = { ((float)Rand::GetRand(0,1001)) / 1000,((float)Rand::GetRand(0,1001)) / 1000,((float)Rand::GetRand(0,1001)) / 1000,((float)Rand::GetRand(0,1001)) / 1000 };
		constMapNumSpheres->sphere[i] = m;
	}
	constMapNumSpheres->numSpheres = numSpheres;
}

void IF::Metaball::TransferConstBuffer()
{
	HRESULT result;
	//定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferBillboard) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//定数バッファの生成
	ID3D12Device* device = DirectX12::Instance()->GetDevice();
	result = device->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&matrixTransform));
	assert(SUCCEEDED(result));
	//定数バッファのマッピング
	result = matrixTransform->Map(0, nullptr, (void**)&constMapMatrix);
	assert(SUCCEEDED(result));

	resdesc.Width = (sizeof(ConstBufferMargingCubesSpace) + 0xff) & ~0xff;
	//定数バッファの生成
	device = DirectX12::Instance()->GetDevice();
	result = device->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&margingCubesSpaceTransform));
	assert(SUCCEEDED(result));
	//定数バッファのマッピング
	result = margingCubesSpaceTransform->Map(0, nullptr, (void**)&constMapMargingCubesSpace);
	assert(SUCCEEDED(result));

	resdesc.Width = (sizeof(ConstBufferNumSpheres) + 0xff) & ~0xff;
	//定数バッファの生成
	device = DirectX12::Instance()->GetDevice();
	result = device->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&numSpheresTransform));
	assert(SUCCEEDED(result));
	//定数バッファのマッピング
	result = numSpheresTransform->Map(0, nullptr, (void**)&constMapNumSpheres);
	assert(SUCCEEDED(result));

}

void IF::Metaball::DataTransferGPU()
{
	//ルックアップテーブルをテクスチャにして送信
	const size_t dataSize = 4096;
	const uint8_t data[dataSize] = {
		255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 8, 3, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 1, 9, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 1, 8, 3, 9, 8, 1, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 1, 2, 10, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 8, 3, 1, 2, 10, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 9, 2, 10, 0, 2, 9, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 2, 8, 3, 2, 10, 8, 10, 9, 8, 255, 255, 255, 255, 255, 255, 255, 3, 11, 2, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 11, 2, 8, 11, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 1, 9, 0, 2, 3, 11, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 1, 11, 2, 1, 9, 11, 9, 8, 11, 255, 255, 255, 255, 255, 255, 255, 3, 10, 1, 11, 10, 3, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 10, 1, 0, 8, 10, 8, 11, 10, 255, 255, 255, 255, 255, 255, 255, 3, 9, 0, 3, 11, 9, 11, 10, 9, 255, 255, 255, 255, 255, 255, 255, 9, 8, 10, 10, 8, 11, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 4, 7, 8, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 4, 3, 0, 7, 3, 4, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 1, 9, 8, 4, 7, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 4, 1, 9, 4, 7, 1, 7, 3, 1, 255, 255, 255, 255, 255, 255, 255, 1, 2, 10, 8, 4, 7, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 3, 4, 7, 3, 0, 4, 1, 2, 10, 255, 255, 255, 255, 255, 255, 255, 9, 2, 10, 9, 0, 2, 8, 4, 7, 255, 255, 255, 255, 255, 255, 255, 2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, 255, 255, 255, 255, 8, 4, 7, 3, 11, 2, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 11, 4, 7, 11, 2, 4, 2, 0, 4, 255, 255, 255, 255, 255, 255, 255, 9, 0, 1, 8, 4, 7, 2, 3, 11, 255, 255, 255, 255, 255, 255, 255, 4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, 255, 255, 255, 255, 3, 10, 1, 3, 11, 10, 7, 8, 4, 255, 255, 255, 255, 255, 255, 255, 1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, 255, 255, 255, 255, 4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, 255, 255, 255, 255, 4, 7, 11, 4, 11, 9, 9, 11, 10, 255, 255, 255, 255, 255, 255, 255, 9, 5, 4, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 9, 5, 4, 0, 8, 3, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 5, 4, 1, 5, 0, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 8, 5, 4, 8, 3, 5, 3, 1, 5, 255, 255, 255, 255, 255, 255, 255, 1, 2, 10, 9, 5, 4, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 3, 0, 8, 1, 2, 10, 4, 9, 5, 255, 255, 255, 255, 255, 255, 255, 5, 2, 10, 5, 4, 2, 4, 0, 2, 255, 255, 255, 255, 255, 255, 255, 2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, 255, 255, 255, 255, 9, 5, 4, 2, 3, 11, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 11, 2, 0, 8, 11, 4, 9, 5, 255, 255, 255, 255, 255, 255, 255, 0, 5, 4, 0, 1, 5, 2, 3, 11, 255, 255, 255, 255, 255, 255, 255, 2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, 255, 255, 255, 255, 10, 3, 11, 10, 1, 3, 9, 5, 4, 255, 255, 255, 255, 255, 255, 255, 4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, 255, 255, 255, 255, 5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, 255, 255, 255, 255, 5, 4, 8, 5, 8, 10, 10, 8, 11, 255, 255, 255, 255, 255, 255, 255, 9, 7, 8, 5, 7, 9, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 9, 3, 0, 9, 5, 3, 5, 7, 3, 255, 255, 255, 255, 255, 255, 255, 0, 7, 8, 0, 1, 7, 1, 5, 7, 255, 255, 255, 255, 255, 255, 255, 1, 5, 3, 3, 5, 7, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 9, 7, 8, 9, 5, 7, 10, 1, 2, 255, 255, 255, 255, 255, 255, 255, 10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, 255, 255, 255, 255, 8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, 255, 255, 255, 255, 2, 10, 5, 2, 5, 3, 3, 5, 7, 255, 255, 255, 255, 255, 255, 255, 7, 9, 5, 7, 8, 9, 3, 11, 2, 255, 255, 255, 255, 255, 255, 255, 9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, 255, 255, 255, 255, 2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, 255, 255, 255, 255, 11, 2, 1, 11, 1, 7, 7, 1, 5, 255, 255, 255, 255, 255, 255, 255, 9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, 255, 255, 255, 255, 5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, 255, 11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, 255, 11, 10, 5, 7, 11, 5, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 10, 6, 5, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 8, 3, 5, 10, 6, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 9, 0, 1, 5, 10, 6, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 1, 8, 3, 1, 9, 8, 5, 10, 6, 255, 255, 255, 255, 255, 255, 255, 1, 6, 5, 2, 6, 1, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 1, 6, 5, 1, 2, 6, 3, 0, 8, 255, 255, 255, 255, 255, 255, 255, 9, 6, 5, 9, 0, 6, 0, 2, 6, 255, 255, 255, 255, 255, 255, 255, 5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, 255, 255, 255, 255, 2, 3, 11, 10, 6, 5, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 11, 0, 8, 11, 2, 0, 10, 6, 5, 255, 255, 255, 255, 255, 255, 255, 0, 1, 9, 2, 3, 11, 5, 10, 6, 255, 255, 255, 255, 255, 255, 255, 5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, 255, 255, 255, 255, 6, 3, 11, 6, 5, 3, 5, 1, 3, 255, 255, 255, 255, 255, 255, 255, 0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, 255, 255, 255, 255, 3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, 255, 255, 255, 255, 6, 5, 9, 6, 9, 11, 11, 9, 8, 255, 255, 255, 255, 255, 255, 255, 5, 10, 6, 4, 7, 8, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 4, 3, 0, 4, 7, 3, 6, 5, 10, 255, 255, 255, 255, 255, 255, 255, 1, 9, 0, 5, 10, 6, 8, 4, 7, 255, 255, 255, 255, 255, 255, 255, 10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, 255, 255, 255, 255, 6, 1, 2, 6, 5, 1, 4, 7, 8, 255, 255, 255, 255, 255, 255, 255, 1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, 255, 255, 255, 255, 8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, 255, 255, 255, 255, 7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, 255, 3, 11, 2, 7, 8, 4, 10, 6, 5, 255, 255, 255, 255, 255, 255, 255, 5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, 255, 255, 255, 255, 0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, 255, 255, 255, 255, 9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, 255, 8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, 255, 255, 255, 255, 5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, 255, 0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, 255, 6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, 255, 255, 255, 255, 10, 4, 9, 6, 4, 10, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 4, 10, 6, 4, 9, 10, 0, 8, 3, 255, 255, 255, 255, 255, 255, 255, 10, 0, 1, 10, 6, 0, 6, 4, 0, 255, 255, 255, 255, 255, 255, 255, 8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, 255, 255, 255, 255, 1, 4, 9, 1, 2, 4, 2, 6, 4, 255, 255, 255, 255, 255, 255, 255, 3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, 255, 255, 255, 255, 0, 2, 4, 4, 2, 6, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 8, 3, 2, 8, 2, 4, 4, 2, 6, 255, 255, 255, 255, 255, 255, 255, 10, 4, 9, 10, 6, 4, 11, 2, 3, 255, 255, 255, 255, 255, 255, 255, 0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, 255, 255, 255, 255, 3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, 255, 255, 255, 255, 6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, 255, 9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, 255, 255, 255, 255, 8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, 255, 3, 11, 6, 3, 6, 0, 0, 6, 4, 255, 255, 255, 255, 255, 255, 255, 6, 4, 8, 11, 6, 8, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 7, 10, 6, 7, 8, 10, 8, 9, 10, 255, 255, 255, 255, 255, 255, 255, 0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, 255, 255, 255, 255, 10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, 255, 255, 255, 255, 10, 6, 7, 10, 7, 1, 1, 7, 3, 255, 255, 255, 255, 255, 255, 255, 1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, 255, 255, 255, 255, 2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, 255, 7, 8, 0, 7, 0, 6, 6, 0, 2, 255, 255, 255, 255, 255, 255, 255, 7, 3, 2, 6, 7, 2, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, 255, 255, 255, 255, 2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, 255, 1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, 255, 11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, 255, 255, 255, 255, 8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, 255, 0, 9, 1, 11, 6, 7, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, 255, 255, 255, 255, 7, 11, 6, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 7, 6, 11, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 3, 0, 8, 11, 7, 6, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 1, 9, 11, 7, 6, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 8, 1, 9, 8, 3, 1, 11, 7, 6, 255, 255, 255, 255, 255, 255, 255, 10, 1, 2, 6, 11, 7, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 1, 2, 10, 3, 0, 8, 6, 11, 7, 255, 255, 255, 255, 255, 255, 255, 2, 9, 0, 2, 10, 9, 6, 11, 7, 255, 255, 255, 255, 255, 255, 255, 6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, 255, 255, 255, 255, 7, 2, 3, 6, 2, 7, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 7, 0, 8, 7, 6, 0, 6, 2, 0, 255, 255, 255, 255, 255, 255, 255, 2, 7, 6, 2, 3, 7, 0, 1, 9, 255, 255, 255, 255, 255, 255, 255, 1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, 255, 255, 255, 255, 10, 7, 6, 10, 1, 7, 1, 3, 7, 255, 255, 255, 255, 255, 255, 255, 10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, 255, 255, 255, 255, 0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, 255, 255, 255, 255, 7, 6, 10, 7, 10, 8, 8, 10, 9, 255, 255, 255, 255, 255, 255, 255, 6, 8, 4, 11, 8, 6, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 3, 6, 11, 3, 0, 6, 0, 4, 6, 255, 255, 255, 255, 255, 255, 255, 8, 6, 11, 8, 4, 6, 9, 0, 1, 255, 255, 255, 255, 255, 255, 255, 9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, 255, 255, 255, 255, 6, 8, 4, 6, 11, 8, 2, 10, 1, 255, 255, 255, 255, 255, 255, 255, 1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, 255, 255, 255, 255, 4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, 255, 255, 255, 255, 10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, 255, 8, 2, 3, 8, 4, 2, 4, 6, 2, 255, 255, 255, 255, 255, 255, 255, 0, 4, 2, 4, 6, 2, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, 255, 255, 255, 255, 1, 9, 4, 1, 4, 2, 2, 4, 6, 255, 255, 255, 255, 255, 255, 255, 8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, 255, 255, 255, 255, 10, 1, 0, 10, 0, 6, 6, 0, 4, 255, 255, 255, 255, 255, 255, 255, 4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, 255, 10, 9, 4, 6, 10, 4, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 4, 9, 5, 7, 6, 11, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 8, 3, 4, 9, 5, 11, 7, 6, 255, 255, 255, 255, 255, 255, 255, 5, 0, 1, 5, 4, 0, 7, 6, 11, 255, 255, 255, 255, 255, 255, 255, 11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, 255, 255, 255, 255, 9, 5, 4, 10, 1, 2, 7, 6, 11, 255, 255, 255, 255, 255, 255, 255, 6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, 255, 255, 255, 255, 7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, 255, 255, 255, 255, 3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, 255, 7, 2, 3, 7, 6, 2, 5, 4, 9, 255, 255, 255, 255, 255, 255, 255, 9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, 255, 255, 255, 255, 3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, 255, 255, 255, 255, 6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, 255, 9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, 255, 255, 255, 255, 1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, 255, 4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, 255, 7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, 255, 255, 255, 255, 6, 9, 5, 6, 11, 9, 11, 8, 9, 255, 255, 255, 255, 255, 255, 255, 3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, 255, 255, 255, 255, 0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, 255, 255, 255, 255, 6, 11, 3, 6, 3, 5, 5, 3, 1, 255, 255, 255, 255, 255, 255, 255, 1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, 255, 255, 255, 255, 0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, 255, 11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, 255, 6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, 255, 255, 255, 255, 5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, 255, 255, 255, 255, 9, 5, 6, 9, 6, 0, 0, 6, 2, 255, 255, 255, 255, 255, 255, 255, 1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, 255, 1, 5, 6, 2, 1, 6, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, 255, 10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, 255, 255, 255, 255, 0, 3, 8, 5, 6, 10, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 10, 5, 6, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 11, 5, 10, 7, 5, 11, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 11, 5, 10, 11, 7, 5, 8, 3, 0, 255, 255, 255, 255, 255, 255, 255, 5, 11, 7, 5, 10, 11, 1, 9, 0, 255, 255, 255, 255, 255, 255, 255, 10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, 255, 255, 255, 255, 11, 1, 2, 11, 7, 1, 7, 5, 1, 255, 255, 255, 255, 255, 255, 255, 0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, 255, 255, 255, 255, 9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, 255, 255, 255, 255, 7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, 255, 2, 5, 10, 2, 3, 5, 3, 7, 5, 255, 255, 255, 255, 255, 255, 255, 8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, 255, 255, 255, 255, 9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, 255, 255, 255, 255, 9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, 255, 1, 3, 5, 3, 7, 5, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 8, 7, 0, 7, 1, 1, 7, 5, 255, 255, 255, 255, 255, 255, 255, 9, 0, 3, 9, 3, 5, 5, 3, 7, 255, 255, 255, 255, 255, 255, 255, 9, 8, 7, 5, 9, 7, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 5, 8, 4, 5, 10, 8, 10, 11, 8, 255, 255, 255, 255, 255, 255, 255, 5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, 255, 255, 255, 255, 0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, 255, 255, 255, 255, 10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, 255, 2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, 255, 255, 255, 255, 0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, 255, 0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, 255, 9, 4, 5, 2, 11, 3, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, 255, 255, 255, 255, 5, 10, 2, 5, 2, 4, 4, 2, 0, 255, 255, 255, 255, 255, 255, 255, 3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, 255, 5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, 255, 255, 255, 255, 8, 4, 5, 8, 5, 3, 3, 5, 1, 255, 255, 255, 255, 255, 255, 255, 0, 4, 5, 1, 0, 5, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, 255, 255, 255, 255, 9, 4, 5, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 4, 11, 7, 4, 9, 11, 9, 10, 11, 255, 255, 255, 255, 255, 255, 255, 0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, 255, 255, 255, 255, 1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, 255, 255, 255, 255, 3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, 255, 4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, 255, 255, 255, 255, 9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, 255, 11, 7, 4, 11, 4, 2, 2, 4, 0, 255, 255, 255, 255, 255, 255, 255, 11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, 255, 255, 255, 255, 2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, 255, 255, 255, 255, 9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, 255, 3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, 255, 1, 10, 2, 8, 7, 4, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 4, 9, 1, 4, 1, 7, 7, 1, 3, 255, 255, 255, 255, 255, 255, 255, 4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, 255, 255, 255, 255, 4, 0, 3, 7, 4, 3, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 4, 8, 7, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 9, 10, 8, 10, 11, 8, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 3, 0, 9, 3, 9, 11, 11, 9, 10, 255, 255, 255, 255, 255, 255, 255, 0, 1, 10, 0, 10, 8, 8, 10, 11, 255, 255, 255, 255, 255, 255, 255, 3, 1, 10, 11, 3, 10, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 1, 2, 11, 1, 11, 9, 9, 11, 8, 255, 255, 255, 255, 255, 255, 255, 3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, 255, 255, 255, 255, 0, 2, 11, 8, 0, 11, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 3, 2, 11, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 2, 3, 8, 2, 8, 10, 10, 8, 9, 255, 255, 255, 255, 255, 255, 255, 9, 10, 2, 0, 9, 2, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, 255, 255, 255, 255, 1, 10, 2, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 1, 3, 8, 9, 1, 8, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 9, 1, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 0, 3, 8, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255
	};

	tex = &Texture::Instance()->tex[texNum];
	Texture::Tex newtex;

	D3D12_HEAP_PROPERTIES texHeapProp{};
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	D3D12_RESOURCE_DESC texresDesc{};
	texresDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texresDesc.Format = DXGI_FORMAT_R8_UINT;
	texresDesc.Width = dataSize;			//幅
	texresDesc.Height = 1;	//高さ
	texresDesc.DepthOrArraySize = 1;
	texresDesc.MipLevels = 1;
	texresDesc.SampleDesc.Count = 1;

	ID3D12Device* device = DirectX12::Instance()->GetDevice();
	HRESULT result = device->CreateCommittedResource(		//GPUリソースの生成
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,		//テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&newtex.texbuff));

	if (newtex.texbuff == nullptr)assert(SUCCEEDED(true && "テクスチャの転送失敗"));

	//テクスチャバッファにデータ転送
	result = newtex.texbuff->WriteToSubresource(
		0,
		nullptr,				//全領域へコピー
		data,			//元データアドレス
		(UINT)dataSize,	//1ラインサイズ
		(UINT)dataSize	//1枚サイズ
	);
	assert(SUCCEEDED(result));
	UINT descriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	newtex.CPUHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(Texture::Instance()->srvHeap->GetCPUDescriptorHandleForHeapStart(), texNum, descriptorSize);
	newtex.GPUHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(Texture::Instance()->srvHeap->GetGPUDescriptorHandleForHeapStart(), texNum, descriptorSize);
	newtex.free = true;

	*tex = newtex;

	D3D12_RESOURCE_DESC resDesc = tex->texbuff->GetDesc();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};			//設定構造体
	//srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;	//RGBA
	srvDesc.Format = resDesc.Format;					//画像読み込み
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;		//2dテクスチャ
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	//ヒープの２番目にシェーダーリソースビュー作成
	device->CreateShaderResourceView(
		tex->texbuff.Get(),		//ビューと関連付けるバッファ
		&srvDesc,		//テクスチャ設定情報
		tex->CPUHandle);

	//テクスチャの送信完了
}

