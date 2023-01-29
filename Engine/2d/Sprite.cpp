#include "Sprite.h"
#include "SpriteManager.h"
#include <cassert>
#include "Texture.h"
#include "Debug.h"
#include "imgui.h"
#include "DirectX12.h"
#include "Window.h"

using namespace IF;
using namespace Microsoft::WRL;

Matrix Sprite::matPro;

void IF::Sprite::StaticInitialize()
{
	Sprite::matPro = MatrixOrthoGraphicProjection(0, Window::Instance()->winWidth, 0, Window::Instance()->winHeight, 0, 1);
}

void IF::Sprite::BufferInitialize()
{
	Vertex2D vertices[4];

	enum { LB, LT, RB, RT };

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
	vi->Initialize();

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

IF::Sprite::~Sprite()
{
	delete vi;
}

void IF::Sprite::Initialize(unsigned int texNum, Float2 size, Float2 anchorpoint, bool flipX, bool flipY)
{
	vi = DEBUG_NEW SV;

	this->texNum = texNum;

	enum { LB, LT, RB, RT };
	Vertex2D vertices[4];

	this->anchorpoint = anchorpoint;

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
	vi->Initialize();

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
	ID3D12Device* device = DirectX12::Instance()->GetDevice();
	result = device->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffTransform));
	assert(SUCCEEDED(result));


	//定数バッファのマッピング
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));

	cb.Initialize();
	ClassInitialize();
}

void IF::Sprite::DrawBefore(ID3D12RootSignature* root, D3D_PRIMITIVE_TOPOLOGY topology)
{
	ID3D12GraphicsCommandList* commandList = DirectX12::Instance()->GetCmdList();
	commandList->SetGraphicsRootSignature(root);
	commandList->IASetPrimitiveTopology(topology);
}

void IF::Sprite::Update()
{
	ClassUpdate();
	matWorld = MatrixIdentity();
	matWorld *= MatrixScaling(scale.x, scale.y, 1);
	matWorld *= MatrixRotationZ(rotation);
	matWorld *= MatrixTranslation(position.x, position.y, 0);

	cb.SetColor(color[0], color[1], color[2], color[3]);

	//定数バッファへのデータ転送
	constMapTransform->mat = matWorld * matPro;
}

void IF::Sprite::ClassUpdate()
{
}

void IF::Sprite::ClassInitialize()
{
}

void IF::Sprite::Draw()
{
	ID3D12GraphicsCommandList* commandList = DirectX12::Instance()->GetCmdList();
	if (!drawFlag)return;
	commandList->SetGraphicsRootConstantBufferView(0, cb.GetGPUAddress());
	//頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, &vi->GetVertexView());
	//定数バッファビューの設定
	commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
	Texture::Instance()->SetTexture(texNum);
	//描画コマンド
	commandList->DrawInstanced(vi->GetSize(), 1, 0, 0);
}

bool IF::Sprite::DeleteSprite()
{
	return deleteFlag;
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

void IF::Sprite::SetColor(int r, int g, int b, int a)
{
	color[0] = (float)r / 255.f;
	color[1] = (float)g / 255.f;
	color[2] = (float)b / 255.f;
	color[3] = (float)a / 255.f;
}

void IF::Sprite::SetBright(int r, int g, int b)
{
	color[0] = (float)r / 255.f;
	color[1] = (float)g / 255.f;
	color[2] = (float)b / 255.f;
}

void IF::Sprite::SetAlpha(int alpha)
{
	color[3] = (float)alpha / 255.f;
}

#ifdef _DEBUG
void IF::Sprite::GUI()
{
	if (ImGui::TreeNode("Position"))
	{
		float p[2] = { position.x,position.y };
		ImGui::DragFloat2("", p);
		position = { p[0],p[1] };
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Rotation"))
	{
		float r = ConvertToDegrees(rotation);
		ImGui::DragFloat("rota", &r, 0.5f);
		if (r >= 360)r -= 360;
		if (r < 0)r += 360;
		rotation = ConvertToRadians(r);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Scale"))
	{
		float s[2] = { scale.x,scale.y };
		ImGui::DragFloat2("", s, 0.05f);
		scale = { s[0],s[1] };
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("color"))
	{
		ImGui::ColorEdit4("color", color);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Change Tex"))
	{
		static int num = texNum;
		Texture::Instance()->TexNum(&num);
		texNum = (unsigned int)num;
		ImGui::TreePop();
	}
}

void IF::Sprite::DebugUpdate()
{
	matWorld = MatrixIdentity();
	matWorld *= MatrixScaling(scale.x, scale.y, 1);
	matWorld *= MatrixRotationZ(rotation);
	matWorld *= MatrixTranslation(position.x, position.y, 0);

	cb.SetColor(color[0], color[1], color[2], color[3]);

	//定数バッファへのデータ転送
	constMapTransform->mat = matWorld * matPro;
}
#endif

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

		vertices[LB].uv = { tex_left,	tex_bottom }; // 左下
		vertices[LT].uv = { tex_left,	tex_top }; // 左上
		vertices[RB].uv = { tex_right,	tex_bottom }; // 右下
		vertices[RT].uv = { tex_right,	tex_top }; // 右上
	}

	vi->SetVerticle(vertices);
	vi->Transfer();
}
