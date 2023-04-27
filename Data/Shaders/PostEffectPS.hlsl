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

float Gaussian(float2 drawuv, float2 pickuv, float sigma)
{
    float d = distance(drawuv, pickuv);
    return exp(-(d * d) / (2 * sigma * sigma));
}

float4 GaussianBlur(float2 uv)
{
    float totalWeight = 0, _sigma = 0.005, _stepWidth = 0.001;
    float4 col = float4(0, 0, 0, 1);
    
    for (float py = -_sigma * 2; py <= _sigma * 2; py += _stepWidth)
    {
        for (float px = -_sigma * 2; px <= _sigma * 2; px += _stepWidth)
        {
            float2 pickuv = uv + float2(px, py);
            float weight = Gaussian(uv, pickuv, _sigma);
            col += tex0.Sample(smp, pickuv) * weight;
            totalWeight += weight;
        }
    }
    col.rgb = col.rgb / totalWeight;
    return col;
}


float4 main(VSOutput input) : SV_TARGET
{
    float4 color;
    float4 texcolor = tex0.Sample(smp, input.uv);
    float s = shift;
    texcolor.r = tex0.Sample(smp, input.uv + float2(-s, s)).r;
    texcolor.g = tex0.Sample(smp, input.uv + float2(0, -s)).g;
    texcolor.b = tex0.Sample(smp, input.uv + float2(s, 0)).b;
    if(gray)
    {
        float gray = texcolor.r * 0.299 + texcolor.g * 0.587 + texcolor.b * 0.114;

        texcolor = float4(gray + sepia, gray, gray - sepia, texcolor.a);
    }
    float4 texcolor1;
    
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
    if (gaussianBlur)
    {
        color = GaussianBlur(input.uv);
    }
    return color;
}