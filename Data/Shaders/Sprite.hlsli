cbuffer cbuff0:register(b0)
{
	float4 color;	//�F(RGBA)
};

cbuffer ConstBufferTransform : register(b1)
{
	matrix mat;
};

struct VSOutput
{
	float4 svpos : SV_POSITION;
	float2 uv : TEXCORD;
};
