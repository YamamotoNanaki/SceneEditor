cbuffer cbuff0:register(b0)
{
	float4 color;				//色(RGBA)
};

cbuffer ConstBufferDataTransform : register(b1)
{
	matrix mat;					//3D変換行列
	matrix matBillboard;		//ビルボード行列
};

struct VSOutput
{
	float4 pos : POSITION;	//頂点座標
};

struct GSOutput
{
	float4 svpos : SV_POSITION;	//頂点座標
	float2 uv :TEXCOORD;		//uv値
};
