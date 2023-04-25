#include "PostEffect.h"
#include "Window.h"
#include "DirectX12.h"
//#include "Input.h"
#include <cassert>
#include <d3dx12.h>
#include <d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

using namespace IF;

const float PostEffect::clearColor[4] = { 0,0,0,0.0f };
unsigned int PostEffect::PostEfectTexNum = 900;

PostEffect::PostEffect() :Sprite() {}

IF::PostEffect::~PostEffect()
{
	constBuffPostEffect->Unmap(0, nullptr);
}

void PostEffect::Draw(bool add)
{
	ID3D12GraphicsCommandList* cmdList = DirectX12::Instance()->GetCmdList();
	if (add)
	{
		cmdList->SetPipelineState(pipelineStateAdd.Get());
	}
	else
	{
		cmdList->SetPipelineState(pipelineState.Get());
	}
	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//if (Input::Instance()->KeyTriggere(KEY::num0))
	//{
	//	static int tex = 0;
	//	tex = (tex + 1) % 2;

	//	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	//	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	//	srvDesc.Texture2D.MipLevels = 1;

	ID3D12Device* device = DirectX12::Instance()->GetDevice();
	//	device->CreateShaderResourceView(texBuff[tex].Get(), &srvDesc, descHeapSRV->GetCPUDescriptorHandleForHeapStart());
	//}
	// 頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &vi->GetVertexView());

	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	//デスクリプタヒープをセット
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffPostEffect.Get()->GetGPUVirtualAddress());
	//cmdList->SetGraphicsRootConstantBufferView(0, this->constBuffTransform->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1, CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeapSRV->GetGPUDescriptorHandleForHeapStart(), 0, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	cmdList->SetGraphicsRootDescriptorTable(2, CD3DX12_GPU_DESCRIPTOR_HANDLE(descHeapSRV->GetGPUDescriptorHandleForHeapStart(), 1, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	// 描画コマンド
	cmdList->DrawInstanced(4, 1, 0, 0);
}

void IF::PostEffect::Update()
{
	Sprite::Update();
}

void PostEffect::Initialize()
{
	if (PostEfectTexNum > 1023)
	{
		assert(false && "これ以上画像を作成できません");
	}
	HRESULT result;

	ID3D12Device* device = DirectX12::Instance()->GetDevice();
	float w = Window::Instance()->winWidth;
	float h = Window::Instance()->winHeight;

	Sprite::Initialize(PostEfectTexNum);
	PostEfectTexNum++;

	Vertex2D vertices[4] = {
		{{-1,-1,0},{0,1}},
		{{-1,+1,0},{0,0}},
		{{+1,-1,0},{1,1}},
		{{+1,+1,0},{1,0}},
	};

	vi->SetVerticle(vertices);
	vi->Initialize();

	D3D12_HEAP_PROPERTIES a = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC b = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferMatrix) + 0xff) & ~0xff);
	result = device->CreateCommittedResource(
		&a, D3D12_HEAP_FLAG_NONE, &b,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constBuffTransform));
	assert(SUCCEEDED(result));

	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM, w, (UINT)h,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	D3D12_HEAP_PROPERTIES c = CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0);

	D3D12_CLEAR_VALUE d = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor);

	for (int i = 0; i < 2; i++)
	{
		result = device->CreateCommittedResource(
			&c, D3D12_HEAP_FLAG_NONE, &texresDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&d, IID_PPV_ARGS(&texBuff[i]));
		assert(SUCCEEDED(result));
	}

	const UINT pixelCount = w * h;
	const UINT rowPitch = sizeof(UINT) * w;
	const UINT depthPitch = rowPitch * h;

	UINT* img = new UINT[pixelCount];
	for (int i = 0; i < pixelCount; i++)img[i] = 0xff0000ff;

	for (int i = 0; i < 2; i++)
	{
		result = texBuff[i]->WriteToSubresource(0, nullptr, img, rowPitch, depthPitch);
		assert(SUCCEEDED(result));
	}
	delete[] img;

	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 2;

	result = device->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	for (int i = 0; i < 2; i++)
	{
		device->CreateShaderResourceView(texBuff[i].Get(), &srvDesc, CD3DX12_CPU_DESCRIPTOR_HANDLE(
			descHeapSRV->GetCPUDescriptorHandleForHeapStart(), i, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
	}
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 2;

	result = device->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&descHeapRTV));
	assert(SUCCEEDED(result));
	for (int i = 0; i < 2; i++)
	{
		device->CreateRenderTargetView(texBuff[i].Get(), nullptr, CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)));
	}

	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT, w, h, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	D3D12_HEAP_PROPERTIES e = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3D12_CLEAR_VALUE f = CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0);

	result = device->CreateCommittedResource(&e, D3D12_HEAP_FLAG_NONE,
		&depthResDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &f, IID_PPV_ARGS(&depthBuff));

	assert(SUCCEEDED(result));

	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;

	result = device->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&descHeapDSV));
	assert(SUCCEEDED(result));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	device->CreateDepthStencilView(depthBuff.Get(), &dsvDesc, descHeapDSV->GetCPUDescriptorHandleForHeapStart());

	CreateGraphicsPipelineState();

	//ヒープ設定
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//リソース設定
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferPostEffect) + 0xff) & ~0xff;
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//GPUリソースの生成
	device = DirectX12::Instance()->GetDevice();
	result = device->CreateCommittedResource(
		&cbHeapProp,	//ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc,		//リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffPostEffect)
	);

	result = constBuffPostEffect->Map(0, nullptr, (void**)&constMapPostEffect);	//マッピング
	assert(SUCCEEDED(result));
}

void PostEffect::DrawBefore()
{
	ID3D12Device* device = DirectX12::Instance()->GetDevice();
	ID3D12GraphicsCommandList* cmdList = DirectX12::Instance()->GetCmdList();
	for (int i = 0; i < 2; i++)
	{
		D3D12_RESOURCE_BARRIER a = CD3DX12_RESOURCE_BARRIER::Transition(texBuff[i].Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
		cmdList->ResourceBarrier(1, &a);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE rtvH[2];
	for (int i = 0; i < 2; i++)
	{
		rtvH[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapRTV->GetCPUDescriptorHandleForHeapStart(), i, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	}
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = descHeapDSV->GetCPUDescriptorHandleForHeapStart();

	cmdList->OMSetRenderTargets(2, rtvH, false, &dsvH);

	float w = Window::Instance()->winWidth;
	float h = Window::Instance()->winHeight;

	CD3DX12_VIEWPORT viewport[2];
	CD3DX12_RECT rect[2];
	for (int i = 0; i < 2; i++)
	{
		viewport[i] = CD3DX12_VIEWPORT(0.f, 0.f, w, h);
		rect[i] = CD3DX12_RECT(0, 0, w, h);
	}

	//D3D12_RECT b = CD3DX12_RECT(0, 0, w, h);

	//D3D12_VIEWPORT viewport{};

	//viewport.Width = w;
	//viewport.Height = h;
	//viewport.TopLeftX = 0.0f;
	//viewport.TopLeftY = 0.0f;
	//viewport.MinDepth = 0;
	//viewport.MaxDepth = 1;

	cmdList->RSSetViewports(2, viewport);
	cmdList->RSSetScissorRects(2, rect);

	for (int i = 0; i < 2; i++)
	{
		cmdList->ClearRenderTargetView(rtvH[i], clearColor, 0, nullptr);
	}
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}

void PostEffect::DrawAfter()
{
	for (int i = 0; i < 2; i++)
	{
		D3D12_RESOURCE_BARRIER a = CD3DX12_RESOURCE_BARRIER::Transition(texBuff[i].Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		DirectX12::Instance()->GetCmdList()->ResourceBarrier(1, &a);
	}
}

void IF::PostEffect::SetRGBShift(float shift)
{
	constMapPostEffect->shift = shift;
}

void PostEffect::CreateGraphicsPipelineState()
{
	ID3D12Device* device = DirectX12::Instance()->GetDevice();

	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob>vsBlob;
	ComPtr<ID3DBlob>psBlob;
	ComPtr<ID3DBlob>errorBlob;

	result = D3DCompileFromFile(
		L"Data/Shaders/PostEffectVS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());

		assert(0);
	}
	result = D3DCompileFromFile(
		L"Data/Shaders/PostEffectPS.hlsl", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());

		assert(0);
	}


	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv座標(1行で書いたほうが見やすい)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS; // 常に上書きルール

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 2;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.RTVFormats[1] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV0;
	descRangeSRV0.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV1;
	descRangeSRV1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1); // t1 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[3];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV1, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT); // s0 レジスタ

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));

	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = rootSignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//加算
	blenddesc.SrcBlend = D3D12_BLEND_ONE;				//ソースの値を100%使う
	blenddesc.DestBlend = D3D12_BLEND_ONE;				//ソースの値を100%使う
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;			//加算
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;			//ソースの値を100%使う
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;			//デストの値を  0%使う
	gpipeline.BlendState.RenderTarget[0] = blenddesc;
	gpipeline.BlendState.RenderTarget[1] = blenddesc;
	gpipeline.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineStateAdd));
	assert(SUCCEEDED(result));
}
