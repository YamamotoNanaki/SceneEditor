#include "Metaball.hlsli"

//��`�̒��_��
static const uint vnum = 4;

//���_�̃I�t�Z�b�g�l
static const float4 offset_array[vnum] =
{
    float4(-5, -5, 0, 0),
	float4(-5, +5, 0, 0),
	float4(+5, -5, 0, 0),
	float4(+5, +5, 0, 0)
};

//main�֐�
[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : POSITION, //�_�̓���
	inout TriangleStream<GSOutput> output //�O�p�`���o��
)
{
    GSOutput element;
    for (uint i = 0; i < vnum; i++)
    {
        float4 offset = mul(matBillboard, offset_array[i]); //�r���{�[�h��
        element.svpos = input[0].pos + offset; //���_�����炷
        element.svpos = mul(mat, element.svpos); //���[���h�ϊ�
        element.vDiscard = input[0].vDiscard;
        output.Append(element); //�o��
    }
}