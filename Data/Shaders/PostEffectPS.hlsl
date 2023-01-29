#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0); // 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float4 main(VSOutput input) : SV_TARGET
{
    float s = shift;
    float4 texcolor = tex.Sample(smp, input.uv);
    texcolor.r = tex.Sample(smp, input.uv + float2(-s, s)).r;
    texcolor.g = tex.Sample(smp, input.uv + float2(0, -s)).g;
    texcolor.b = tex.Sample(smp, input.uv + float2(s, 0)).b;

    return texcolor;
}