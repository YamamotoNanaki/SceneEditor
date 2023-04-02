#include "Metaball.hlsli"

float4 main(GSOutput input) : SV_TARGET
{
    float4 rColor;
    if (input.vDiscard == 1.0)
    {
        discard;
    }
    else
    {
        return float4(1, 1, 1, 1.0);
    }
    return rColor;
    return float4(1, 1, 1, 1.0);
}