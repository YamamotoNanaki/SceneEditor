#include "GPipeline.h"


using namespace IF;

GPipeline::GPipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, ID3DBlob* gsBlob, D3D12_INPUT_ELEMENT_DESC* inputLayout, int layoutCount)
{
	for (int i = 0; i < _countof(pipelineDesc); i++)
	{
		pipelineDesc[i].VS.pShaderBytecode = vsBlob->GetBufferPointer();
		pipelineDesc[i].VS.BytecodeLength = vsBlob->GetBufferSize();
		if (gsBlob != nullptr)
		{
			pipelineDesc[i].GS.pShaderBytecode = gsBlob->GetBufferPointer();
			pipelineDesc[i].GS.BytecodeLength = gsBlob->GetBufferSize();
		}
		pipelineDesc[i].PS.pShaderBytecode = psBlob->GetBufferPointer();
		pipelineDesc[i].PS.BytecodeLength = psBlob->GetBufferSize();
		//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
		pipelineDesc[i].DepthStencilState.DepthEnable = true;		//�[�x�e�X�g���s��
		pipelineDesc[i].DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		pipelineDesc[i].DSVFormat = DXGI_FORMAT_D32_FLOAT;		//�[�x�l�t�H�[�}�b�g

		pipelineDesc[i].SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�
		pipelineDesc[i].RasterizerState.CullMode = D3D12_CULL_MODE_BACK;  // �w�ʂ��J�����O
		pipelineDesc[i].RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
		pipelineDesc[i].RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

		D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc[i].BlendState.RenderTarget[0];
		blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

		blenddesc.BlendEnable = true;						//�u�����h��L���ɂ���
		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;			//���Z
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;			//�\�[�X�̒l��100%�g��
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;			//�f�X�g�̒l��  0%�g��

		if (i % 4 == 1)
		{
			blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//���Z
			blenddesc.SrcBlend = D3D12_BLEND_ONE;				//�\�[�X�̒l��100%�g��
			blenddesc.DestBlend = D3D12_BLEND_ONE;				//�\�[�X�̒l��100%�g��
			pipelineDesc[i].DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		}
		else if (i % 4 == 2)
		{
			blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;		//�f�X�g����\�[�X�����Z
			blenddesc.SrcBlend = D3D12_BLEND_ONE;				//�\�[�X�̒l��100%�g��
			blenddesc.DestBlend = D3D12_BLEND_ONE;				//�\�[�X�̒l��100%�g��
			pipelineDesc[i].DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		}
		else if (i % 4 == 3)
		{
			blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//���Z
			blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;		//1.0f-�f�X�g�J���[�̒l
			blenddesc.DestBlend = D3D12_BLEND_ZERO;				//�g��Ȃ�
			pipelineDesc[i].DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		}
		else
		{
			blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//���Z
			blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//�\�[�X�̃A���t�@�l
			blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;		//1.0f-�\�[�X�̃A���t�@�l
			pipelineDesc[i].DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//�������݋���
		}

		pipelineDesc[i].InputLayout.pInputElementDescs = inputLayout;
		pipelineDesc[i].InputLayout.NumElements = layoutCount;

		pipelineDesc[i].PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		pipelineDesc[i].NumRenderTargets = 1; // �`��Ώۂ�1��
		pipelineDesc[i].RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
		pipelineDesc[i].SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

		if (i > 3 && i < 8)
		{
			pipelineDesc[i].DepthStencilState.DepthEnable = false;		//�[�x�e�X�g���s��Ȃ�
			pipelineDesc[i].RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // �w�ʂ��J�����O���Ȃ�
		}
		if (i > 7)pipelineDesc[i].PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	}
}

void GPipeline::RootSignature(ID3D12RootSignature& rootsignature)
{
	for (int i = 0; i < _countof(pipelineDesc); i++)
	{
		// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
		pipelineDesc[i].pRootSignature = &rootsignature;
	}
}
