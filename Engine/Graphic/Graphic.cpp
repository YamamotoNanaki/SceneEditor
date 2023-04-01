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

	// �V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(fillname,  // �V�F�[�_�t�@�C����
		nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", s[num], // �G���g���[�|�C���g���A�V�F�[�_�[���f��	�w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0, &Blobs[Blobs.size() - 1], &Blobs[ShaderCode::error]);

	//-------------------------

	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		string error;
		error.resize(Blobs[ShaderCode::error]->GetBufferSize());

		copy_n((char*)Blobs[ShaderCode::error]->GetBufferPointer(),
			Blobs[ShaderCode::error]->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
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
//		{// xyz���W
//			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
//		},
//		{// �@���x�N�g��
//			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
//		},
//		{// uv���W
//			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
//		},
//	};
//	GPipeline pipeline(Blobs[ShaderCode::vs].Get(), Blobs[ShaderCode::ps].Get(), Blobs[ShaderCode::gs].Get(), inputLayout, _countof(inputLayout));
//
//	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
//	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���J��Ԃ�
//	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//�c�J��Ԃ�
//	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���s�J��Ԃ�
//	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//�{�[�_�[�̎��͍�
//	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//���j�A�⊮
//	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//�~�b�v�}�b�v�ő�l
//	samplerDesc.MinLOD = 0.0f;												//�~�b�v�}�b�v�ŏ��l
//	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
//	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//�s�N�Z���V�F�[�_�[����̂݉�
//
//	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
//	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
//	rootSignatureDesc.pParameters = &root.rootParams.front();
//	rootSignatureDesc.NumParameters = root.rootParams.size();
//	//�e�N�X�`���ǉ�
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
		{// xyz���W
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{// �@���x�N�g��
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{// uv���W
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};
	GPipeline pipeline(Blobs[ShaderCode::vs].Get(), Blobs[ShaderCode::ps].Get(), Blobs[ShaderCode::gs].Get(), inputLayout, _countof(inputLayout));

	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���J��Ԃ�
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//�c�J��Ԃ�
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���s�J��Ԃ�
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//���j�A�⊮
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;												//�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//�s�N�Z���V�F�[�_�[����̂݉�

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &root.rootParams.front();
	rootSignatureDesc.NumParameters = root.rootParams.size();
	//�e�N�X�`���ǉ�
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
	pipeline.pipelineDesc[0].DepthStencilState.DepthEnable = false;		//�[�x�e�X�g���s��Ȃ�
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
		{// xyz���W
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{// �@���x�N�g��
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{// uv���W
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
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���J��Ԃ�
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//�c�J��Ԃ�
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���s�J��Ԃ�
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//���j�A�⊮
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;												//�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//�s�N�Z���V�F�[�_�[����̂݉�

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &root.rootParams.front();
	rootSignatureDesc.NumParameters = root.rootParams.size();
	//�e�N�X�`���ǉ�
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
	//�萔�p
	rootParamSeed.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;				//���
	rootParamSeed.Descriptor.ShaderRegister = 0;								//�f�X�N���v�^�����W
	rootParamSeed.Descriptor.RegisterSpace = 0;									//�f�X�N���v�^�����W��
	rootParamSeed.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//���ׂẴV�F�[�_�[���猩����
	rootParams.push_back(rootParamSeed);

	D3D12_ROOT_PARAMETER rootParamSeed2;
	rootParamSeed2.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		//���
	rootParamSeed2.DescriptorTable.pDescriptorRanges = &descRangeSRV;				//�f�X�N���v�^�����W
	rootParamSeed2.DescriptorTable.NumDescriptorRanges = 1;							//�f�X�N���v�^�����W��
	rootParamSeed2.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;					//���ׂẴV�F�[�_�[���猩����
	rootParams.push_back(rootParamSeed2);
	rootParamSeed.Descriptor.ShaderRegister = 1;
	rootParams.push_back(rootParamSeed);

	Compiller(vs, ps, 0, ShaderCompile::vsps);

	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{// xyz���W
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{// uv���W
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};
	GPipeline pipeline(Blobs[ShaderCode::vs2d].Get(), Blobs[ShaderCode::ps2d].Get(), nullptr, inputLayout, _countof(inputLayout));

	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���J��Ԃ�
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//�c�J��Ԃ�
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���s�J��Ԃ�
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//���j�A�⊮
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;												//�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//�s�N�Z���V�F�[�_�[����̂݉�

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParams.front();
	rootSignatureDesc.NumParameters = rootParams.size();
	//�e�N�X�`���ǉ�
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
		{// xyz���W
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	GPipeline pipeline(Blobs[ShaderCode::vsP].Get(), Blobs[ShaderCode::psP].Get(), Blobs[ShaderCode::gsP].Get(), inputLayout, _countof(inputLayout));

	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���J��Ԃ�
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//�c�J��Ԃ�
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���s�J��Ԃ�
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//���j�A�⊮
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;												//�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			//�s�N�Z���V�F�[�_�[����̂݉�

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &root.rootParams.front();
	rootSignatureDesc.NumParameters = root.rootParams.size();
	//�e�N�X�`���ǉ�
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
		{// xyz���W
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		}
	};

	GPipeline pipeline(Blobs[ShaderCode::vsM].Get(), Blobs[ShaderCode::psM].Get(), Blobs[ShaderCode::gsM].Get(), inputLayout, _countof(inputLayout));

	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���J��Ԃ�
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//�c�J��Ԃ�
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					//���s�J��Ԃ�
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	//�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					//���j�A�⊮
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									//�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;												//�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;			//�S�ẴV�F�[�_�[�����

	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &root.rootParams.front();
	rootSignatureDesc.NumParameters = root.rootParams.size();
	//�e�N�X�`���ǉ�
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
	pipeline.pipelineDesc[0].RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // �w�ʂ��J�����O���Ȃ�

	result = device->CreateGraphicsPipelineState(&pipeline.pipelineDesc[0], IID_PPV_ARGS(&pipelinestate[18]));
	assert(SUCCEEDED(result));
}

void IF::Graphic::DrawBlendMode(Blend::Blend blend)
{
	DirectX12::Instance()->GetCmdList()->SetPipelineState(pipelinestate[blend].Get());
}
