#include "Particle.h"
#include "View.h"
#include "Ease.h"
#include "DirectX12.h"
#include <cassert>

using namespace IF;
using namespace std;

ICamera* Particle::camera = nullptr;
PV Particle::vi{};

void Particle::Initialize()
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
	ID3D12Device* device = DirectX12::Instance()->GetDevice();
	result = device->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffTransform));
	assert(SUCCEEDED(result));
	//定数バッファのマッピング
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));

	cb.Initialize();
}

void Particle::VIInitialize()
{
	VertexPos vertices[] = {
		// x   y   z        u    v
		//前
		{{0, 0, 0}},	//左下
	};

	ID3D12Device* device = DirectX12::Instance()->GetDevice();
	vi.SetVerticleIndex(vertices, _countof(vertices));
	vi.Initialize();
}

void Particle::MatUpdate()
{
	Matrix matScale, matTrams, matRot;

	//スケール、回転、平行移動
	matScale = MatrixScaling(scale[0], scale[1], scale[2]);
	matTrams = MatrixTranslation(position.x, position.y, position.z);
	matRot = MatrixRotationZ(ConvertToRadians(rota));
	//ワールド行列の合成
	matWorld = MatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrams;

	//定数バッファへのデータ転送
	constMapTransform->mat = matWorld * *camera->GetMatView() * *camera->GetMatPro();
	constMapTransform->matBillboard = View::matBillBoard;
}

void IF::Particle::Update()
{
	timer.SafeUpdate();
	if (timer.IsEnd())
	{
		deleteFlag = true;
		return;
	}
	//色
	double (*ease)(double, double, double, double);
	ease = Ease::Lerp;
	if (colorF)
	{
		for (int i = 0; i < 4; i++)
		{
			if (colorEase[i] == Lerp)ease = Ease::Lerp;
			else if (colorEase[i] == EaseIn)ease = Ease::InQuad;
			else if (colorEase[i] == EaseOut)ease = Ease::OutQuad;
			else if (colorEase[i] == EaseInOut)ease = Ease::InOutQuad;
			color[i] = ease(startcolor[i], endcolor[i], deleteTime, timer.NowTime());
		}
	}
	cb.SetColor(color[0], color[1], color[2], color[3]);

	//回転
	if (rotaF)
	{
		if (rotaEase == Lerp)ease = Ease::Lerp;
		else if (rotaEase == EaseIn)ease = Ease::InQuad;
		else if (rotaEase == EaseOut)ease = Ease::OutQuad;
		else if (rotaEase == EaseInOut)ease = Ease::InOutQuad;
		rota = ease(rotastart, rotaend, deleteTime, timer.NowTime());
	}
	else
	{
		rota += rotaspeed;
	}

	//スケール
	if (scaleF)
	{
		for (int i = 0; i < 3; i++)
		{
			if (scaleEase[i] == Lerp)ease = Ease::Lerp;
			else if (scaleEase[i] == EaseIn)ease = Ease::InQuad;
			else if (scaleEase[i] == EaseOut)ease = Ease::OutQuad;
			else if (scaleEase[i] == EaseInOut)ease = Ease::InOutQuad;
			scale[i] = ease(startScale[i], endScale[i], deleteTime, timer.NowTime());
		}
	}

	if (setSpeedPosFlag)
	{
		//スピード
		if (speedF)
		{
			for (int i = 0; i < 3; i++)
			{
				if (speedEase[i] == Lerp)ease = Ease::Lerp;
				else if (speedEase[i] == EaseIn)ease = Ease::InQuad;
				else if (speedEase[i] == EaseOut)ease = Ease::OutQuad;
				else if (speedEase[i] == EaseInOut)ease = Ease::InOutQuad;
				speed[i] = ease(startSpeed[i], endSpeed[i], deleteTime, timer.NowTime());
			}
		}
		position.x += speed[0];
		position.y += speed[1];
		position.z += speed[2];
		if (isGravity)position.y += gravity;
	}
	else
	{
		//ポジション
		if (posF)
		{
			float pos[3];
			for (int i = 0; i < 3; i++)
			{
				if (posEase[i] == Lerp)ease = Ease::Lerp;
				else if (posEase[i] == EaseIn)ease = Ease::InQuad;
				else if (posEase[i] == EaseOut)ease = Ease::OutQuad;
				else if (posEase[i] == EaseInOut)ease = Ease::InOutQuad;
				pos[i] = ease(startposition[i], endposition[i], deleteTime, timer.NowTime());
			}
			position = pos;
		}
	}
}

void IF::Particle::DrawBefore(ID3D12RootSignature* root, D3D_PRIMITIVE_TOPOLOGY topology)
{
	ID3D12GraphicsCommandList* commandList = DirectX12::Instance()->GetCmdList();
	commandList->SetGraphicsRootSignature(root);
	commandList->IASetPrimitiveTopology(topology);
}

void Particle::Draw()
{
	ID3D12GraphicsCommandList* commandList = DirectX12::Instance()->GetCmdList();
	commandList->SetGraphicsRootConstantBufferView(0, cb.GetGPUAddress());
	//頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, &vi.GetVertexView());
	//定数バッファビューの設定
	commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
	//描画コマンド
	commandList->DrawInstanced((UINT)vi.GetSize(), 1, 0, 0);
}

Particle::~Particle()
{
	constBuffTransform->Unmap(0, nullptr);
}
