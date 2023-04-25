#include "PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1); // 1番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 Blur1Pixel(float2 uv)
{
    float4 col = float4(0, 0, 0, 1);
    float u = 1.f / 1280.f;
    float v = 1.f / 720.f;
    
    for (int x = 0; x < 3; x++)
    {
        for (int y = 0; y < 3; y++)
        {
            col += tex1.Sample(smp, uv + float2(u * (x - 1), v * (y - 1)));
        }
    }
    
    col.rgb /= 9;
    return col;
}

float4 main(VSOutput input) : SV_TARGET
{
    float4 color;
    float4 texcolor = tex0.Sample(smp, input.uv);
    float4 texcolor1 = tex1.Sample(smp, input.uv);
    
    color = texcolor;
    if (kadai)
    {
        texcolor.rgb = 1 - texcolor.rgb;
        texcolor1 = Blur1Pixel(input.uv);
        float num = 0;
    
        color = texcolor;
        if (fmod(input.uv.y, 0.1f) < 0.05f)
        {
            color = texcolor1;
        }
    }
    
    return color;
}