#include "ConstBuff.h"
#include "Util.h"
#include <cassert>

void IF::ConstBuff::Initialize(ID3D12Device* device)
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
	constBuffMaterial->Unmap(0, nullptr);							//マッピング解除

	R = 255, G = 255, B = 255, A = 255;
	r = 1, g = 1, b = 1, a = 1;
}

D3D12_GPU_VIRTUAL_ADDRESS IF::ConstBuff::GetGPUAddress()
{
	return constBuffMaterial->GetGPUVirtualAddress();
}

void IF::ConstBuff::SetBright(int red, int green, int blue)
{
	if (red == R && green == G && blue == B)return;
	if (red != R)r = TypeConversionByte(red);
	if (green != G)g = TypeConversionByte(green);
	if (blue != B)b = TypeConversionByte(blue);
	R = red, G = green, B = blue;
	constMapMaterial->color = Float4(r, g, b, a);					//RGBAで半透明の赤
}

void IF::ConstBuff::SetAlpha(int alpha)
{
	if (alpha == A);
	a = TypeConversionByte(alpha);
	A = alpha;
	constMapMaterial->color = Float4(r, g, b, a);					//RGBAで半透明の赤
}

void IF::ConstBuff::SetColor(int red, int green, int blue, int alpha)
{
	if (red == R && green == G && blue == B && alpha == A) return;
	if (red != R)r = TypeConversionByte(red);
	if (green != G)g = TypeConversionByte(green);
	if (blue != B)b = TypeConversionByte(blue);
	if (alpha != A)a = TypeConversionByte(alpha);
	R = red, G = green, B = blue, A = alpha;
	constMapMaterial->color = Float4(r, g, b, a);					//RGBAで半透明の赤
}
