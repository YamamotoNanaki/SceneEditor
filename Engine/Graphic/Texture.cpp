#include "Texture.h"
#include <d3dx12.h>
#include <d3d12.h>
#include "Input.h"
#include "ImGui.h"

using namespace DirectX;
using namespace IF;
using namespace Microsoft::WRL;
using namespace std;

ComPtr<ID3D12Device> Texture::device = nullptr;

IF::Texture::Texture()
{
	descRangeSRV.NumDescriptors = 1;															//テクスチャ一つ
	descRangeSRV.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;									//種別はテクスチャ
	descRangeSRV.BaseShaderRegister = 0;														//0番スロットから
	descRangeSRV.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	tex[0].free = true;
}

Texture* IF::Texture::Instance()
{
	static Texture* inst = new Texture;
	return inst;
}

void IF::Texture::DeleteInstance()
{
	delete Texture::Instance();
}

void IF::Texture::setDevice(ID3D12Device* device)
{
	Texture::device = device;
}

void IF::Texture::Initialize()
{
	const size_t kMaxSRVCount = 2056;

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	HRESULT result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));

	textureSize = 0;
	for (int i = 0; i < textureMax; i++) {
		tex[i].texbuff.Reset();
		tex[i].CPUHandle.ptr = 0;
		tex[i].texName.clear();
		tex[i].free = false;
	}
}

unsigned short Texture::LoadTexture(const std::string filename)
{
	assert(textureSize < textureMax && "ヒープサイズが足りません");

	//WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	for (int i = 1; i < textureMax; i++)
	{
		if (tex[i].free == false)continue;
		if (tex[i].texName == filename)return i;
	}

	unsigned short num = 0;
	for (int i = 1; i < textureMax; i++)
	{
		if (tex[i].free == false)
		{
			num = i;
			break;
		}
	}

	Tex newtex;
	newtex.texName = filename;
	string file = "Data/Resources/" + filename;
	wchar_t szFile[256];
	MultiByteToWideChar(CP_ACP, 0, file.c_str(), -1, szFile, _countof(szFile));


	HRESULT result = LoadFromWICFile(
		szFile,			//Data\Resources\texture.pngを指定
		WIC_FLAGS_NONE, &metadata, scratchImg);

	ScratchImage mipChain{};
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(), TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result))
	{
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	metadata.format = MakeSRGB(metadata.format);

	D3D12_HEAP_PROPERTIES texHeapProp{};
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	D3D12_RESOURCE_DESC texresDesc{};
	texresDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texresDesc.Format = metadata.format;
	texresDesc.Width = metadata.width;			//幅
	texresDesc.Height = (UINT)metadata.height;	//高さ
	texresDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	texresDesc.MipLevels = (UINT16)metadata.mipLevels;
	texresDesc.SampleDesc.Count = 1;

	result = device->CreateCommittedResource(		//GPUリソースの生成
		&texHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,		//テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&newtex.texbuff));

	if (newtex.texbuff == nullptr)return 0;

	for (size_t i = 0; i < metadata.mipLevels; i++)
	{
		const Image* img = scratchImg.GetImage(i, 0, 0);
		//テクスチャバッファにデータ転送
		result = newtex.texbuff->WriteToSubresource(
			(UINT)i,
			nullptr,				//全領域へコピー
			img->pixels,			//元データアドレス
			(UINT)img->rowPitch,	//1ラインサイズ
			(UINT)img->slicePitch	//1枚サイズ
		);
		assert(SUCCEEDED(result));
	}
	UINT descriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	newtex.CPUHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(srvHeap->GetCPUDescriptorHandleForHeapStart(), num, descriptorSize);
	newtex.GPUHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(srvHeap->GetGPUDescriptorHandleForHeapStart(), num, descriptorSize);
	newtex.free = true;

	tex[num] = newtex;

	D3D12_RESOURCE_DESC resDesc = tex[num].texbuff->GetDesc();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};			//設定構造体
	//srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;	//RGBA
	srvDesc.Format = resDesc.Format;					//画像読み込み
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;		//2dテクスチャ
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	//ヒープの２番目にシェーダーリソースビュー作成
	device->CreateShaderResourceView(
		tex[num].texbuff.Get(),		//ビューと関連付けるバッファ
		&srvDesc,		//テクスチャ設定情報
		tex[num].CPUHandle);

	textureSize++;

	return num;
}

//#ifdef _DEBUG
void IF::Texture::GUI()
{
	if (!flag)flag = ImGui::ImageButton((ImTextureID)tex[folder].GPUHandle.ptr, { 96,96 });
	else
	{
		int j = 0;
		for (int i = 0; i < 256; i++)
		{
			if (i == folder)continue;
			if (j % 7 != 0)ImGui::SameLine();
			if (tex[i].free == true)
			{
				ImGui::Text("%03d", i);
				ImGui::SameLine();
				ImGui::Image((ImTextureID)tex[i].GPUHandle.ptr, { 96,96 });
				j++;
			}
		}
	}
}

void IF::Texture::TexNum(int* texNum)
{
	int j = 0;
	for (int i = 0; i < 256; i++)
	{
		if (i == folder)continue;
		if (!tex[i].free)continue;
		string _tagName;
		if (i < 10)_tagName = "00";
		else if (i < 100)_tagName = "0";
		else _tagName = "";
		_tagName += (char)(i + 48);
		if (j % 3 != 0)ImGui::SameLine();
		ImGui::RadioButton(_tagName.c_str(), texNum, i);
		j++;
	}
}
//#endif

void IF::Texture::GUIInit()
{
	folder = LoadTexture("folder.png");
}

void IF::Texture::setTexture(ID3D12GraphicsCommandList* commandList, unsigned short texHandle)
{
	ID3D12DescriptorHeap* heaps[] = { srvHeap.Get() };
	commandList->SetDescriptorHeaps(_countof(heaps), heaps);

	commandList->SetGraphicsRootDescriptorTable(1, tex[texHandle].GPUHandle);
}
