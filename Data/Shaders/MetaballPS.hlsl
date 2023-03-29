#include "Metaball.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
    float4 rColor;
    if (input.vDiscard == 1.0)
    {
        discard;
    }
    else
    {
        rColor = float4(sphere[0].color.xyz, 1.0);
    }
    return rColor;
}