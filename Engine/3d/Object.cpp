#include "Object.h"
#include "View.h"
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>
#include "Light.h"
#include "Graphic.h"
#include "DirectX12.h"

using namespace IF;
using namespace std;
using namespace IF::BillBoard;
using namespace Microsoft::WRL;

LightManager* Object::lightPtr = nullptr;

void IF::Object::DrawBefore(ID3D12RootSignature* root, D3D_PRIMITIVE_TOPOLOGY topology)
{
	ID3D12GraphicsCommandList* commandList = DirectX12::Instance()->GetCmdList();
	commandList->SetGraphicsRootSignature(root);
	commandList->IASetPrimitiveTopology(topology);
}

void IF::Object::SetModel(Model* model)
{
	this->model = model;
}

void IF::Object::Initialize(Model* model)
{
	HRESULT result;
	//定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//定数バッファの生成
	result = DirectX12::Instance()->GetDevice()->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffTransform));
	assert(SUCCEEDED(result));


	//定数バッファのマッピング
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));

	//アウトライン
	result = DirectX12::Instance()->GetDevice()->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffOutLine));
	assert(SUCCEEDED(result));


	//定数バッファのマッピング
	result = constBuffOutLine->Map(0, nullptr, (void**)&OutLineTransform);
	assert(SUCCEEDED(result));

	OutLineTransform->lightFlag = false;

	this->model = model;

	cb.Initialize();
	outLineCb.Initialize();
}


void IF::Object::Initialize(FBXModel* fmodel)
{
	HRESULT result;
	//定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//定数バッファの生成
	result = DirectX12::Instance()->GetDevice()->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffTransform));
	assert(SUCCEEDED(result));

	auto h = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto d = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataSkin) + 0xff) & ~0xff);
	//定数バッファの生成
	result = DirectX12::Instance()->GetDevice()->CreateCommittedResource(
		&h, D3D12_HEAP_FLAG_NONE, &d, D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&constBuffSkin));
	assert(SUCCEEDED(result));

	//定数バッファのマッピング
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));
	result = constBuffSkin->Map(0, nullptr, (void**)&constMapSkin);

	//アウトライン
	result = DirectX12::Instance()->GetDevice()->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffOutLine));
	assert(SUCCEEDED(result));


	//定数バッファのマッピング
	result = constBuffOutLine->Map(0, nullptr, (void**)&OutLineTransform);
	assert(SUCCEEDED(result));

	OutLineTransform->lightFlag = false;

	this->fmodel = fmodel;

	cb.Initialize();
	outLineCb.Initialize();
}

void Object::Update(Matrix matView, Matrix matProjection, Float3 cameraPos, int mode)
{
	UpdateWorldMatrix(mode);

	//定数バッファへのデータ転送
	constMapTransform->viewPro = matView * matProjection;
	constMapTransform->world = matWorld;
	constMapTransform->cameraPos = cameraPos;
	constMapTransform->polygonSize = 1;
	constMapTransform->explosion = 0;
	constMapTransform->lightFlag = lightFlag;
	constMapTransform->toonFlag = toonFlag;

	if (fmodel == nullptr || fmodel->bones.size() == 0)return;
	animTimer += 0.01f;
	if (animTimer > fmodel->animations[0].duration)animTimer = 0;
	fmodel->BoneTransform(animTimer);
	for (int i = 0; i < fmodel->bones.size(); i++)
	{
		constMapSkin->bones[i] = fmodel->bones[i].finalMatrix;
	}
}

void IF::Object::UpdateWorldMatrix(int mode)
{
	Matrix matScale, matRot, matTrams;

	//スケール、回転、平行移動
	matScale = MatrixScaling(scale.x, scale.y, scale.z);
	matRot = MatrixIdentity();
	matRot *= MatrixRotationZ(rotation.z);
	matRot *= MatrixRotationX(rotation.x);
	matRot *= MatrixRotationY(rotation.y);
	matTrams = MatrixTranslation(position.x, position.y, position.z);
	//ワールド行列の合成
	matWorld = MatrixIdentity();
	billbord = mode;
	if (mode == BILLBOARD)matWorld *= View::matBillBoard;
	else if (mode == YBOARD)matWorld *= View::matBillBoardY;
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrams;
	//親オブジェクトがあれば
	if (parent != nullptr)
	{
		matWorld *= parent->matWorld;
	}
}

void IF::Object::MatWorldUpdate()
{
	Matrix matScale, matRot, matTrams;

	//スケール、回転、平行移動
	matScale = MatrixScaling(scale.x, scale.y, scale.z);
	matRot = MatrixIdentity();
	matRot *= MatrixRotationZ(rotation.z);
	matRot *= MatrixRotationX(rotation.x);
	matRot *= MatrixRotationY(rotation.y);
	matTrams = MatrixTranslation(position.x, position.y, position.z);
	//ワールド行列の合成
	matWorld = MatrixIdentity();
	if (billbord == BILLBOARD)matWorld *= View::matBillBoard;
	else if (billbord == YBOARD)matWorld *= View::matBillBoardY;
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrams;
	//親オブジェクトがあれば
	if (parent != nullptr)
	{
		matWorld *= parent->matWorld;
	}
	constMapTransform->world = matWorld;
}

void Object::Draw()
{
	MatWorldUpdate();
	if (model == nullptr)
	{
		assert(0 && "モデルがセットされていません");
		return;
	}

	ID3D12GraphicsCommandList* commandList = DirectX12::Instance()->GetCmdList();
	lightPtr->Draw(4);
	commandList->SetGraphicsRootConstantBufferView(0, cb.GetGPUAddress());
	model->Draw(constBuffTransform.Get());
}

void IF::Object::OutLineDraw()
{
	if (!outLineFlag)return;
	if (model == nullptr)
	{
		assert(0 && "モデルがセットされていません");
		return;
	}

	*OutLineTransform = *constMapTransform;
	OutLineTransform->lightFlag = false;
	scale += outLineWidth;

	Matrix matScale, matRot, matTrams;

	//スケール、回転、平行移動
	matScale = MatrixScaling(scale.x, scale.y, scale.z);
	matRot = MatrixIdentity();
	matRot *= MatrixRotationZ(rotation.z);
	matRot *= MatrixRotationX(rotation.x);
	matRot *= MatrixRotationY(rotation.y);
	matTrams = MatrixTranslation(position.x, position.y, position.z);
	//ワールド行列の合成
	matWorld = MatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrams;
	//親オブジェクトがあれば
	if (parent != nullptr)
	{
		matWorld *= parent->matWorld;
	}

	OutLineTransform->world = matWorld;

	outLineCb.SetColor(outLineColor);


	ID3D12GraphicsCommandList* commandList = DirectX12::Instance()->GetCmdList();
	lightPtr->Draw(4);
	commandList->SetGraphicsRootConstantBufferView(0, outLineCb.GetGPUAddress());
	model->Draw(constBuffOutLine.Get(), 1);

	scale -= outLineWidth;
}

void IF::Object::Draw(unsigned short texNum)
{
	MatWorldUpdate();
	if (model == nullptr)
	{
		assert(0 && "モデルがセットされていません");
		return;
	}

	lightPtr->Draw(4);

	model->Draw(constBuffTransform.Get(), texNum);
}

void IF::Object::FBXDraw()
{
	if (fmodel == nullptr)
	{
		assert(0 && "モデルがセットされていません");
		return;
	}
	Graphic::Instance()->DrawBlendMode(Blend::ANIMNORMAL);
	ID3D12GraphicsCommandList* commandList = DirectX12::Instance()->GetCmdList();
	lightPtr->Draw(4);
	commandList->SetGraphicsRootConstantBufferView(0, cb.GetGPUAddress());
	commandList->SetGraphicsRootConstantBufferView(5, constBuffSkin->GetGPUVirtualAddress());
	fmodel->Draw(constBuffTransform.Get());
}

Object::~Object()
{
	constBuffTransform->Unmap(0, nullptr);
	if (fmodel == nullptr)return;
	constBuffSkin->Unmap(0, nullptr);
}

void IF::Object::StaticInitialize()
{
	Object::lightPtr = LightManager::Instance();
}

void IF::Object::SetColor(int r, int g, int b, int a)
{
	cb.SetColor(r, g, b, a);
}
void IF::Object::SetColorF(float r, float g, float b, float a)
{
	cb.SetColor(r, g, b, a);
}

void IF::Object::SetBright(int r, int g, int b)
{
	cb.SetBright(r, g, b);
}

void IF::Object::SetAlpha(int alpha)
{
	cb.SetAlpha(alpha);
}
