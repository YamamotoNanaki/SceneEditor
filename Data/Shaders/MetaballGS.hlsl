#include "Metaball.hlsli"

[maxvertexcount(3)]
void main(
	triangle VSOutput input[3] : POSITION,
	inout TriangleStream<GSOutput> output
)
{
    for (uint i = 0; i < 3; i++)
    {
        GSOutput element;
        element.svpos = mul(mat, input[i].pos);
        element.vDiscard = input[i].vDiscard;
        output.Append(element);
    }
}