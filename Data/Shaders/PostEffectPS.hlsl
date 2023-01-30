#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    float s = shift;
    float4 texcolor = tex.Sample(smp, input.uv);
    texcolor.r = tex.Sample(smp, input.uv + float2(-s, s)).r;
    texcolor.g = tex.Sample(smp, input.uv + float2(0, -s)).g;
    texcolor.b = tex.Sample(smp, input.uv + float2(s, 0)).b;


    if (grayscale == 1)
    {
        float gray = texcolor.r * 0.299 + texcolor.g * 0.587 + texcolor.b * 0.114;

        texcolor = float4(gray + sepia, gray, gray - sepia, texcolor.a);
    }
    if (nega == 1)
    {
        texcolor = float4(1 - texcolor.r, 1 - texcolor.g, 1 - texcolor.b, texcolor.a);
    }
    return texcolor;
}