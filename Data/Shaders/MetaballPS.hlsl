#include "Metaball.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> tex2 : register(t1);
SamplerState smp : register(s0);

float4 main(GSOutput input) : SV_TARGET
{
    return tex.Sample(smp, input.uv);
}