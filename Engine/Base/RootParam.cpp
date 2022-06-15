#include "RootParam.h"


using namespace IF;

void RootParam::ConstRoot(int num)
{
	D3D12_ROOT_PARAMETER rootParamSeed;
	//定数用
	rootParamSeed.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;				//種類
	rootParamSeed.Descriptor.ShaderRegister = num;								//デスクリプタレンジ
	rootParamSeed.Descriptor.RegisterSpace = 0;									//デスクリプタレンジ数
	rootParamSeed.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				//すべてのシェーダーから見える
	rootParams.push_back(rootParamSeed);
}

void RootParam::TexRoot(D3D12_DESCRIPTOR_RANGE& descRangeSRV, int texMax)
{
	for (int i = 0; i < texMax; i++)
	{
		D3D12_ROOT_PARAMETER rootParamSeed;
		rootParamSeed.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;		//種類
		rootParamSeed.DescriptorTable.pDescriptorRanges = &descRangeSRV;				//デスクリプタレンジ
		rootParamSeed.DescriptorTable.NumDescriptorRanges = 1;							//デスクリプタレンジ数
		rootParamSeed.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;					//すべてのシェーダーから見える
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
