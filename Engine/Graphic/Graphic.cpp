#include "Graphic.h"
#include <cassert>
#include <d3dcompiler.h>
#include "Debug.h"
#include "DirectX12.h"

using namespace std;
using namespace IF;
using namespace Microsoft::WRL;

Graphic* IF::Graphic::Instance()
{
	static Graphic inst;
	return &inst;
}

void IF::Graphic::CompillerArray(LPCWSTR fillname, int num)
{
	HRESULT result;
	Blobs.emplace_back(nullptr);
	LPCSTR s[3] = { "vs_5_0","ps_5_0","gs_5_0" };

	// シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(fillname,  // シェーダファイル名
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", s[num], // エントリーポイント名、シェーダーモデル	指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0, &Blobs[Blobs.size() - 1], &Blobs[ShaderCode::error]);

	//-------------------------

	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		string error;
		error.resize(Blobs[ShaderCode::error]->GetBufferSize());

		copy_n((char*)Blobs[ShaderCode::error]->GetBufferPointer(),
			Blobs[ShaderCode::error]->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}

void Graphic::Compiller(LPCWSTR vs, LPCWSTR ps, LPCWSTR gs, char compile)
{
	HRESULT result = S_OK;
	vector<LPCWSTR> s;
	if (compile & ShaderCompile::vs)
		s.emplace_back(vs);
	if (compile & ShaderCompile::ps)
		s.emplace_back(ps);
	if (compile & ShaderCompile::gs)
		s.emplace_back(gs);
	if (Blobs.size() == 0)Blobs.emplace_back(nullptr);
	for (int i = 0; i < s.size(); i++)
	{
		CompillerArray(s[i], i);
	}
}

//HRESULT Graphic::Initialize(ID3D12Device* device, D3D12_DESCRIPTOR_RANGE& descRangeSRV)
//{
//	HRESULT result;
//
//	RootParam root(descRangeSRV, 1);
//
//	result = Compiller(L"ModelVS.hlsl", L"ModelPS.hlsl", L"ModelGS.hlsl");
//
//	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
//		{// xyz座標
//			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
//		},
//		{// 法線ベクトル
//			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
//		},
//		{// uv座標
//			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
//		},
//	};
//	GPipeline pipeline(Blobs[ShaderCode::vs].Get(), Blobs[ShaderCode::ps].Get(), Blobs[ShaderCode::gs].Get(), inputLayout, _countof(inputLayout));
//
//	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
//	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//横繰り返し
//	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//縦繰り返し
//	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//奥行繰り返し
//	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//ボーダーの時は黒
//	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//リニア補完
//	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//ミップマップ最大値
//	samplerDesc.MinLOD = 0.0f;												//ミップマップ最小値
//	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
//	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//ピクセルシェーダーからのみ可視
//
//	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
//	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
//	rootSignatureDesc.pParameters = &root.rootParams.front();
//	rootSignatureDesc.NumParameters = root.rootParams.size();
//	//テクスチャ追加
//	rootSignatureDesc.pStaticSamplers = &samplerDesc;
//	rootSignatureDesc.NumStaticSamplers = 1;
//
//	ID3DBlob* rootSigBlob = nullptr;
//	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &Blobs[ShaderCode::error]);
//	assert(SUCCEEDED(result));
//	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
//	assert(SUCCEEDED(result));
//	rootSigBlob->Release();
//
//	pipeline.RootSignature(*rootsignature.Get());
//
//	for (int i = 0; i < _countof(pipelinestate); i++)
//	{
//		int j = i;
//		if (j > 3)j = 0;
//		result = device->CreateGraphicsPipelineState(&pipeline.pipelineDesc[j], IID_PPV_ARGS(&pipelinestate[j]));
//	}
//	assert(SUCCEEDED(result));
// 
//	delete rootSigBlob;
//
//	return result;
//
//}

void IF::Graphic::Initialize(D3D12_DESCRIPTOR_RANGE& descRangeSRV, LPCWSTR vs, LPCWSTR ps, LPCWSTR gs)
{
	HRESULT result;

	RootParam root(descRangeSRV, 1);

	Compiller(vs, ps, gs);

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{// xyz座標
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{// 法線ベクトル
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{// uv座標
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};
	GPipeline pipeline(Blobs[ShaderCode::vs].Get(), Blobs[ShaderCode::ps].Get(), Blobs[ShaderCode::gs].Get(), inputLayout, _countof(inputLayout));

	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//横繰り返し
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//縦繰り返し
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//奥行繰り返し
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//リニア補完
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;												//ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//ピクセルシェーダーからのみ可視

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &root.rootParams.front();
	rootSignatureDesc.NumParameters = root.rootParams.size();
	//テクスチャ追加
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	ID3D12Device* device = DirectX12::Instance()->GetDevice();
	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &Blobs[ShaderCode::error]);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();

	pipeline.RootSignature(*rootsignature.Get());

	for (int i = 0; i < 4; i++)
	{
		int j = i;
		result = device->CreateGraphicsPipelineState(&pipeline.pipelineDesc[j], IID_PPV_ARGS(&pipelinestate[j]));
	}
	assert(SUCCEEDED(result));

	pipeline.pipelineDesc[0].RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
	pipeline.pipelineDesc[0].DepthStencilState.DepthEnable = false;		//深度テストを行わない
	result = device->CreateGraphicsPipelineState(&pipeline.pipelineDesc[0], IID_PPV_ARGS(&pipelinestate[16]));
	pipeline.pipelineDesc->BlendState.RenderTarget[0].BlendEnable = false;
	result = device->CreateGraphicsPipelineState(&pipeline.pipelineDesc[0], IID_PPV_ARGS(&pipelinestate[17]));
	assert(SUCCEEDED(result));
}

void IF::Graphic::InitializeFBX(D3D12_DESCRIPTOR_RANGE& descRangeSRV, LPCWSTR vs, LPCWSTR ps, LPCWSTR gs)
{
	HRESULT result;

	RootParam root(descRangeSRV, 1);

	Compiller(vs, ps, gs);

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{// xyz座標
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{// 法線ベクトル
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{// uv座標
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"BONEINDICES",0,DXGI_FORMAT_R32G32B32A32_UINT,0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"BONEWEIGHTS",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		}
	};
	GPipeline pipeline(Blobs[ShaderCode::vsA].Get(), Blobs[ShaderCode::psA].Get(), Blobs[ShaderCode::gsA].Get(), inputLayout, _countof(inputLayout));
	for (int i = 0; i < 4; i++)
		pipeline.pipelineDesc[i].RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//横繰り返し
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//縦繰り返し
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//奥行繰り返し
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//リニア補完
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;												//ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//ピクセルシェーダーからのみ可視

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &root.rootParams.front();
	rootSignatureDesc.NumParameters = root.rootParams.size();
	//テクスチャ追加
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	ID3D12Device* device = DirectX12::Instance()->GetDevice();
	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &Blobs[ShaderCode::error]);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();

	pipeline.RootSignature(*rootsignature.Get());

	for (int i = 0; i < 4; i++)
	{
		int j = i + 12;
		result = device->CreateGraphicsPipelineState(&pipeline.pipelineDesc[i], IID_PPV_ARGS(&pipelinestate[j]));
	}
	assert(SUCCEEDED(result));
}

void IF::Graphic::Initialize2D(D3D12_DESCRIPTOR_RANGE& descRangeSRV, LPCWSTR vs, LPCWSTR ps)
{
	HRESULT result;

	//RootParam root(descRangeSRV, 1);
	vector<D3D12_ROOT_PARAMETER> rootParams;

	D3D12_ROOT_PARAMETER rootParamSeed;
	//定数用
	rootParamSeed.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;				//種類
	rootParamSeed.Descriptor.ShaderRegister = 0;								//デスクリプタレンジ
	rootParamSeed.Descriptor.RegisterSpace = 0;									//デスクリプタレンジ数
	rootParamSeed.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//すべてのシェーダーから見える
	rootParams.push_back(rootParamSeed);

	D3D12_ROOT_PARAMETER rootParamSeed2;
	rootParamSeed2.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		//種類
	rootParamSeed2.DescriptorTable.pDescriptorRanges = &descRangeSRV;				//デスクリプタレンジ
	rootParamSeed2.DescriptorTable.NumDescriptorRanges = 1;							//デスクリプタレンジ数
	rootParamSeed2.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;					//すべてのシェーダーから見える
	rootParams.push_back(rootParamSeed2);
	rootParamSeed.Descriptor.ShaderRegister = 1;
	rootParams.push_back(rootParamSeed);

	Compiller(vs, ps, 0, ShaderCompile::vsps);

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{// xyz座標
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{// uv座標
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};
	GPipeline pipeline(Blobs[ShaderCode::vs2d].Get(), Blobs[ShaderCode::ps2d].Get(), nullptr, inputLayout, _countof(inputLayout));

	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//横繰り返し
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//縦繰り返し
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//奥行繰り返し
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//リニア補完
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;												//ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//ピクセルシェーダーからのみ可視

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParams.front();
	rootSignatureDesc.NumParameters = rootParams.size();
	//テクスチャ追加
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	ID3D12Device* device = DirectX12::Instance()->GetDevice();
	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &Blobs[ShaderCode::error]);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();

	pipeline.RootSignature(*rootsignature.Get());

	for (int i = 4; i < 8; i++)
	{
		int j = i;
		result = device->CreateGraphicsPipelineState(&pipeline.pipelineDesc[j], IID_PPV_ARGS(&pipelinestate[j]));
	}
	assert(SUCCEEDED(result));
}

void IF::Graphic::InitializeParticle(D3D12_DESCRIPTOR_RANGE& descRangeSRV)
{
	HRESULT result;

	RootParam root(descRangeSRV, 1);

	Compiller(L"Data/Shaders/ParticleVS.hlsl", L"Data/Shaders/ParticlePS.hlsl", L"Data/Shaders/ParticleGS.hlsl");

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{// xyz座標
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	GPipeline pipeline(Blobs[ShaderCode::vsP].Get(), Blobs[ShaderCode::psP].Get(), Blobs[ShaderCode::gsP].Get(), inputLayout, _countof(inputLayout));

	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//横繰り返し
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//縦繰り返し
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//奥行繰り返し
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//リニア補完
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;												//ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//ピクセルシェーダーからのみ可視

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &root.rootParams.front();
	rootSignatureDesc.NumParameters = root.rootParams.size();
	//テクスチャ追加
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	ID3D12Device* device = DirectX12::Instance()->GetDevice();
	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &Blobs[ShaderCode::error]);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();

	pipeline.RootSignature(*rootsignature.Get());

	for (int i = 8; i < 12; i++)
	{
		int j = i;
		result = device->CreateGraphicsPipelineState(&pipeline.pipelineDesc[j], IID_PPV_ARGS(&pipelinestate[j]));
	}
	assert(SUCCEEDED(result));
}

void IF::Graphic::InitializeMetaball(D3D12_DESCRIPTOR_RANGE& descRangeSRV)
{
	HRESULT result;

	RootParam root(descRangeSRV, 1);

	Compiller(L"Data/Shaders/MetaballVS.hlsl", L"Data/Shaders/MetaballPS.hlsl", L"Data/Shaders/MetaballGS.hlsl");

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{//
			"BLENDINDICES", 0, DXGI_FORMAT_R32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{// xyz座標
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		}
	};

	GPipeline pipeline(Blobs[ShaderCode::vsM].Get(), Blobs[ShaderCode::psM].Get(), Blobs[ShaderCode::gsM].Get(), inputLayout, _countof(inputLayout));

	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//横繰り返し
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//縦繰り返し
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//奥行繰り返し
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//ボーダーの時は黒
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//リニア補完
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//ミップマップ最大値
	samplerDesc.MinLOD = 0.0f;												//ミップマップ最小値
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;			//全てのシェーダーから可視

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &root.rootParams.front();
	rootSignatureDesc.NumParameters = root.rootParams.size();
	//テクスチャ追加
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	ID3D12Device* device = DirectX12::Instance()->GetDevice();
	ID3DBlob* rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &Blobs[ShaderCode::error]);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	assert(SUCCEEDED(result));
	rootSigBlob->Release();

	pipeline.RootSignature(*rootsignature.Get());

	pipeline.pipelineDesc[0].BlendState.RenderTarget[0] = {};
	pipeline.pipelineDesc[0].RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	pipeline.pipelineDesc[0].RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // 背面をカリングしない

	result = device->CreateGraphicsPipelineState(&pipeline.pipelineDesc[0], IID_PPV_ARGS(&pipelinestate[18]));
	assert(SUCCEEDED(result));
}

void IF::Graphic::DrawBlendMode(Blend::Blend blend)
{
	DirectX12::Instance()->GetCmdList()->SetPipelineState(pipelinestate[blend].Get());
}
