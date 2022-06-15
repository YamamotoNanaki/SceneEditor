#include "RootParam.h"


using namespace IF;

void RootParam::ConstRoot(int num)
{
	D3D12_ROOT_PARAMETER rootParamSeed;
	//�萔�p
	rootParamSeed.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;				//���
	rootParamSeed.Descriptor.ShaderRegister = num;								//�f�X�N���v�^�����W
	rootParamSeed.Descriptor.RegisterSpace = 0;									//�f�X�N���v�^�����W��
	rootParamSeed.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//���ׂẴV�F�[�_�[���猩����
	rootParams.push_back(rootParamSeed);
}

void RootParam::TexRoot(D3D12_DESCRIPTOR_RANGE& descRangeSRV, int texMax)
{
	for (int i = 0; i < texMax; i++)
	{
		D3D12_ROOT_PARAMETER rootParamSeed;
		rootParamSeed.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		//���
		rootParamSeed.DescriptorTable.pDescriptorRanges = &descRangeSRV;				//�f�X�N���v�^�����W
		rootParamSeed.DescriptorTable.NumDescriptorRanges = 1;							//�f�X�N���v�^�����W��
		rootParamSeed.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;					//���ׂẴV�F�[�_�[���猩����
		rootParams.push_back(rootParamSeed);
	}
}

RootParam::RootParam(D3D12_DESCRIPTOR_RANGE& descRangeSRV, int texMax)
{
	ConstRoot(0);
	TexRoot(descRangeSRV, texMax);
	ConstRoot(1);
	ConstRoot(2);
	ConstRoot(3);
}
