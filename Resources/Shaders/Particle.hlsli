cbuffer cbuff0:register(b0)
{
	float4 color;				//�F(RGBA)
};

cbuffer ConstBufferDataTransform : register(b1)
{
	matrix mat;					//3D�ϊ��s��
	matrix matBillboard;		//�r���{�[�h�s��
};

struct VSOutput
{
	float4 pos : POSITION;	//���_���W
};

struct GSOutput
{
	float4 svpos : SV_POSITION;	//���_���W
	float2 uv :TEXCOORD;		//uv�l
};
