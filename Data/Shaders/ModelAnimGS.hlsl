#include "ModelAnim.hlsli"


[maxvertexcount(3)]
void main(
	triangle VSOutput input[3] : SV_POSITION,
	inout TriangleStream<GSOutput> output
)
{
    float3 offsetPos = ((input[0].normal + input[1].normal + input[2].normal) / 3) * explosion;
    float3 center = (input[0].svpos.xyz + input[1].svpos.xyz + input[2].svpos.xyz) / 3;
    for (uint i = 0; i < 3; i++)
    {
        GSOutput element;
        element.svpos = input[i].svpos;
        element.svpos.xyz = (input[i].svpos.xyz - center) * (polygonSize) + center;
        element.svpos.xyz += offsetPos;
        element.svpos.y -= gravity;
        element.worldpos = input[i].worldpos;
        element.normal = input[i].normal;
        element.uv = input[i].uv;
        output.Append(element);
    }
}