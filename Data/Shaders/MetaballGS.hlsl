#include "Metaball.hlsli"

//矩形の頂点数
static const uint vnum = 4;

//頂点のオフセット値
static const float4 offset_array[vnum] =
{
    float4(-5, -5, 0, 0),
	float4(-5, +5, 0, 0),
	float4(+5, -5, 0, 0),
	float4(+5, +5, 0, 0)
};

//main関数
[maxvertexcount(vnum)]
void main(
	point VSOutput input[1] : POSITION, //点の入力
	inout TriangleStream<GSOutput> output //三角形を出力
)
{
    GSOutput element;
    for (uint i = 0; i < vnum; i++)
    {
        float4 offset = mul(matBillboard, offset_array[i]); //ビルボード化
        element.svpos = input[0].pos + offset; //頂点をずらす
        element.svpos = mul(mat, element.svpos); //ワールド変換
        element.vDiscard = input[0].vDiscard;
        output.Append(element); //出力
    }
}