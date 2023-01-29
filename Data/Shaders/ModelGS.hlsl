#include "Model.hlsli"
#include "Util.hlsli"


[maxvertexcount(3)]
void main(
	triangle VSOutput input[3] : SV_POSITION,
	inout TriangleStream<GSOutput> output
)
{
    float3 offsetPos = ((input[0].normal + input[1].normal + input[2].normal) / 3) * explosion;
    float3 center = (input[0].svpos.xyz + input[1].svpos.xyz + input[2].svpos.xyz) / 3;
    float r = 2.0 * (Rand(center.xy) - 0.5);
    float3 r3 = r.xxx;
    for (uint i = 0; i < 3; i++)
    {
        GSOutput element;
        element.svpos = input[i].svpos;
        //スケール変化
        element.svpos.xyz = (input[i].svpos.xyz - center) * (polygonSize) + center;
        //乱数を用いた回転
        element.svpos.xyz = Rotate(element.svpos.xyz - center, r3 * rotation) + center;
        //爆発の処理
        element.svpos.xyz += offsetPos;
        //通常の処理
        element.svpos.y -= gravity;
        element.worldpos = input[i].worldpos;
        element.normal = input[i].normal;
        element.uv = input[i].uv;
        output.Append(element);
    }
}