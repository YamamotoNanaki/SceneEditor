#include "ConstBuff.h"
#include "Util.h"
#include "DirectX12.h"
#include <cassert>

using namespace IF;

void IF::ConstBuff::Initialize()
{
	HRESULT result;
	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//GPUリソースの生成
	ID3D12Device* device = DirectX12::Instance()->GetDevice();
	result = device->CreateCommittedResource(
		&cbHeapProp,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,		//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial)
	);

	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);	//マッピング
	assert(SUCCEEDED(result));
	constMapMaterial->color = Float4(1, 1, 1, 1);					//RGBAで半透明の赤
}

IF::ConstBuff::~ConstBuff()
{
	//constBuffMaterial->Unmap(0, nullptr);							//マッピング解除
}

D3D12_GPU_VIRTUAL_ADDRESS IF::ConstBuff::GetGPUAddress()
{
	return constBuffMaterial->GetGPUVirtualAddress();
}

void IF::ConstBuff::SetBright(int red, int green, int blue)
{
	constMapMaterial->color.x = red / 255.f;
	constMapMaterial->color.y = green / 255.f;
	constMapMaterial->color.z = blue / 255.f;
}

void IF::ConstBuff::SetAlpha(int alpha)
{
	constMapMaterial->color.w = alpha / 255.f;
}

void IF::ConstBuff::SetColor(int red, int green, int blue, int alpha)
{
	constMapMaterial->color = Float4(red / 255.f, green / 255.f, blue / 255.f, alpha / 255.f);
}
void IF::ConstBuff::SetColor(float red, float green, float blue, float alpha)
{
	constMapMaterial->color = Float4(red, green, blue, alpha);
}

void IF::ConstBuff::SetColor(Float4 color)
{
	constMapMaterial->color = color;
}

Float4 IF::ConstBuff::GetColor()
{
	return constMapMaterial->color;
}
