#include "Sprite.h"
#include <cassert>
#include "Texture.h"

using namespace IF;
using namespace Microsoft::WRL;

ComPtr < ID3D12GraphicsCommandList> Sprite::commandList = nullptr;
ComPtr < ID3D12Device> Sprite::device = nullptr;
Matrix Sprite::matPro;
std::vector<D3D12_VIEWPORT> Sprite::viewport;

void IF::Sprite::StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, std::vector<D3D12_VIEWPORT> viewport, float winWidth, float winHeight)
{
	SetDeviceCommand(device, commandList);
	SetViewport(viewport);
	Sprite::matPro = MatrixOrthoGraphicProjection(0, winWidth, 0, winHeight, 0, 1);
}

IF::Sprite::~Sprite()
{
	//constBuffTransform->Unmap(0, nullptr);
	delete vi;
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
	//�萔�o�b�t�@�̃q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//�萔�o�b�t�@�̃��\�[�X�ݒ�
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferMatrix) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffTransform));
	assert(SUCCEEDED(result));


	//�萔�o�b�t�@�̃}�b�s���O
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));

	cb.Initialize(device.Get());
}

void IF::Sprite::DrawBefore(ID3D12RootSignature* root, D3D_PRIMITIVE_TOPOLOGY topology)
{
	commandList->SetGraphicsRootSignature(root);
	commandList->IASetPrimitiveTopology(topology);
}

void IF::Sprite::Update()
{
	matWorld = MatrixIdentity();
	matWorld *= MatrixScaling(scale.x, scale.y, 1);
	matWorld *= MatrixRotationZ(rotation);
	matWorld *= MatrixTranslation(position.x, position.y, 0);

	//�萔�o�b�t�@�ւ̃f�[�^�]��
	constMapTransform->mat = matWorld * matPro;
}

void IF::Sprite::SetViewport(std::vector<D3D12_VIEWPORT> viewport)
{
	Sprite::viewport = viewport;
}

void IF::Sprite::Draw()
{
	commandList->SetGraphicsRootConstantBufferView(0, cb.GetGPUAddress());
	//���_�o�b�t�@�̐ݒ�
	commandList->IASetVertexBuffers(0, 1, &vi->GetVertexView());
	//�萔�o�b�t�@�r���[�̐ݒ�
	commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
	Texture::Instance()->setTexture(commandList.Get(), texNum);
	for (int i = 0; i < viewport.size(); i++)
	{
		commandList->RSSetViewports(1, &viewport[i]);
		//�`��R�}���h
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

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	TransferVertex();
}

void IF::Sprite::SetColor(int r, int g, int b, int a)
{
	cb.SetColor(r, g, b, a);
}

void IF::Sprite::SetBright(int r, int g, int b)
{
	cb.SetBright(r, g, b);
}

void IF::Sprite::SetAlpha(int alpha)
{
	cb.SetAlpha(alpha);
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

	ComPtr<ID3D12Resource> texBuff = Texture::Instance()->tex[texNum].texbuff.Get();

	if (texBuff)
	{
		D3D12_RESOURCE_DESC resDesc = texBuff->GetDesc();

		float tex_left = texBase.x / resDesc.Width;
		float tex_right = (texBase.x + texSize.x) / resDesc.Width;
		float tex_top = texBase.y / resDesc.Height;
		float tex_bottom = (texBase.y + texSize.y) / resDesc.Height;

		vertices[LB].uv = { tex_left,	tex_bottom }; // ����
		vertices[LT].uv = { tex_left,	tex_top }; // ����
		vertices[RB].uv = { tex_right,	tex_bottom }; // �E��
		vertices[RT].uv = { tex_right,	tex_top }; // �E��
	}

	vi->SetVerticle(vertices);
	vi->Transfer();
}
