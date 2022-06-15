#include "Sprite.h"
#include <cassert>
#include "Texture.h"

using namespace IF;
using namespace Microsoft::WRL;

ComPtr < ID3D12GraphicsCommandList> Sprite::commandList = nullptr;
ComPtr < ID3D12Device> Sprite::device = nullptr;
Matrix Sprite::matPro;

void IF::Sprite::StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, float winWidth, float winHeight)
{
	SetDeviceCommand(device, commandList);

	Sprite::matPro = MatrixOrthoGraphicProjection(0, winWidth, 0, winHeight, 0, 1);
}

void IF::Sprite::SetDeviceCommand(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{
	assert(device);
	assert(commandList);

	Sprite::device = device;
	Sprite::commandList = commandList;
}

void IF::Sprite::Initialize(unsigned int texNum, Float2 size, bool flipX, bool flipY)
{
	vi = new SV;

	this->texNum = texNum;

	enum { LB, LT, RB, RT };
	Vertex2D vertices[4];

	float left = (0.0f - anchorpoint.x) * size.x;
	float right = (1.0f - anchorpoint.x) * size.x;
	float top = (0.0f - anchorpoint.y) * size.y;
	float bottom = (1.0f - anchorpoint.y) * size.y;
	if (flipX)
	{
		left = -left;
		right = -right;
	}

	if (flipY)
	{
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left,	bottom,	0.0f };
	vertices[LT].pos = { left,	top,	0.0f };
	vertices[RB].pos = { right,	bottom,	0.0f };
	vertices[RT].pos = { right,	top,	0.0f };
	float tex_left = 0;
	float tex_right = 1;
	float tex_top = 0;
	float tex_bottom = 1;
	vertices[LB].uv = { tex_left,	tex_bottom };
	vertices[LT].uv = { tex_left,	tex_top };
	vertices[RB].uv = { tex_right,	tex_bottom };
	vertices[RT].uv = { tex_right,	tex_top };

	vi->SetVerticle(vertices);
	vi->Initialize(device.Get());

	HRESULT result;
	//定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferMatrix) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//定数バッファの生成
	result = device->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffTransform));
	assert(SUCCEEDED(result));


	//定数バッファのマッピング
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));
}

void IF::Sprite::DrawBefore(ID3D12RootSignature* root, D3D12_GPU_VIRTUAL_ADDRESS GPUAddress, D3D_PRIMITIVE_TOPOLOGY topology)
{
	commandList->SetGraphicsRootSignature(root);
	commandList->IASetPrimitiveTopology(topology);
	commandList->SetGraphicsRootConstantBufferView(0, GPUAddress);
}

void IF::Sprite::Update()
{
	matWorld = MatrixIdentity();
	matWorld *= MatrixRotationZ(ConvertToRadians(rotation));
	matWorld *= MatrixTranslation(position.x, position.y, 0);

	//定数バッファへのデータ転送
	constMapTransform->mat = matWorld * matPro;
}

void IF::Sprite::Draw(std::vector<D3D12_VIEWPORT> viewport)
{
	for (int i = 0; i < viewport.size(); i++)
	{
		Texture::Instance()->setTexture(commandList.Get(), texNum);
		commandList->RSSetViewports(1, &viewport[i]);
		//頂点バッファの設定
		commandList->IASetVertexBuffers(0, 1, &vi->GetVertexView());
		//定数バッファビューの設定
		commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
		//描画コマンド
		commandList->DrawInstanced(vi->GetSize(), 1, 0, 0);
	}
}

void Sprite::SetPosition(Float2 position)
{
	this->position = position;
}

void Sprite::SetSize(Float2 size)
{
	this->size = size;
}

void Sprite::SetTextureRect(Float2 texBase, Float2 texSize)
{
	this->texBase = texBase;
	this->texSize = texSize;

	// 頂点バッファへのデータ転送
	TransferVertex();
}

void Sprite::TransferVertex()
{
	enum { LB, LT, RB, RT };
	Vertex2D vertices[4];

	float left = (0.0f - anchorpoint.x) * size.x;
	float right = (1.0f - anchorpoint.x) * size.x;
	float top = (0.0f - anchorpoint.y) * size.y;
	float bottom = (1.0f - anchorpoint.y) * size.y;
	if (flipX)
	{
		left = -left;
		right = -right;
	}

	if (flipY)
	{
		top = -top;
		bottom = -bottom;
	}

	vertices[LB].pos = { left,	bottom,	0.0f };
	vertices[LT].pos = { left,	top,	0.0f };
	vertices[RB].pos = { right,	bottom,	0.0f };
	vertices[RT].pos = { right,	top,	0.0f };
	float tex_left = 0;
	float tex_right = 1;
	float tex_top = 1;
	float tex_bottom = 0;
	vertices[LB].uv = { tex_left,	tex_bottom };
	vertices[LT].uv = { tex_left,	tex_top };
	vertices[RB].uv = { tex_right,	tex_bottom };
	vertices[RT].uv = { tex_right,	tex_top };

	ID3D12Resource* texBuff = Texture::Instance()->tex[texNum].texbuff.Get();

	if (texBuff)
	{
		D3D12_RESOURCE_DESC resDesc = texBuff->GetDesc();

		float tex_left = texBase.x / resDesc.Width;
		float tex_right = (texBase.x + texSize.x) / resDesc.Width;
		float tex_top = texBase.y / resDesc.Height;
		float tex_bottom = (texBase.y + texSize.y) / resDesc.Height;

		vertices[LB].uv = { tex_left,	tex_bottom }; // 左下
		vertices[LT].uv = { tex_left,	tex_top }; // 左上
		vertices[RB].uv = { tex_right,	tex_bottom }; // 右下
		vertices[RT].uv = { tex_right,	tex_top }; // 右上
	}

	vi->SetVerticle(vertices);
	vi->Transfer();
}
