#include "PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0); // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1); // 1番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 Blur1Pixel(float2 uv, Texture2D<float4> tex)
{
    uv = clamp(uv, 0, 1);
    float4 col = float4(0, 0, 0, 1);
    float u = 1.f / 1280.f;
    float v = 1.f / 720.f;
    
    for (int x = 0; x < 3; x++)
    {
        for (int y = 0; y < 3; y++)
        {
            col += tex.Sample(smp, uv + float2(u * (x - 1), v * (y - 1)));
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

float4 GaussianBlur(float2 uv, Texture2D<float4> tex)
{
    uv = clamp(uv, 0, 1);
    float totalWeight = 0, _sigma = 0.005, _stepWidth = 0.001;
    float4 col = float4(0, 0, 0, 1);
    
    for (float py = -_sigma * 2; py <= _sigma * 2; py += _stepWidth)
    {
        for (float px = -_sigma * 2; px <= _sigma * 2; px += _stepWidth)
        {
            float2 pickuv = uv + float2(px, py);
            float weight = Gaussian(uv, pickuv, _sigma);
            col += tex.Sample(smp, pickuv) * weight;
            totalWeight += weight;
        }
    }
    col.rgb = col.rgb / totalWeight;
    return col;
}

float4 GaussianDepthBlur(float2 uv, Texture2D<float4> tex, float focusWidth,float _FocusDepth ,float _sigma = 0.005)
{
    float totalWeight = 0, _stepWidth = 0.001;
    float4 col = float4(0, 0, 0, 1);

    for (float py = -_sigma * 2; py <= _sigma * 2; py += _stepWidth)
    {
        for (float px = -_sigma * 2; px <= _sigma * 2; px += _stepWidth)
        {
            float2 pickuv = uv + float2(px, py);
            float pickDepth = tex1.Sample(smp, pickuv).r;
            float pickFocus = smoothstep(0, focusWidth, abs(pickDepth - _FocusDepth));
            float weight = Gaussian(uv, pickuv, _sigma) * pickFocus;
            col += tex.Sample(smp, pickuv) * weight;
            totalWeight += weight;
        }
    }
    col.rgb = col.rgb / totalWeight;
    return col;
}

float4 DotFilter(float2 uv, Texture2D<float4> tex)
{
    uv = clamp(uv, 0, 1);
    float4 col = tex.Sample(smp, uv);
    float2 st = uv / 1280 * 30;
    st = frac(st * float2(1280, 720));
    float l = distance(st, float2(0.5, 0.5));
    return col * (float4(1, 1, 1, 1) * 1 - step(0.3, l));
}

float4 GaussianAngleBlur(float2 uv, Texture2D<float4> tex, float angleDeg)
{
    float totalWeight = 0;
    float4 col = float4(0, 0, 0, 0);
    float2 pickuv;
    float pickRange = 0.06;
    float angleRad = angleDeg * 3.141592 / 180;

	[loop]
    for (float j = -pickRange; j <= pickRange; j += 0.005)
    {
        float x = cos(angleRad) * j;
        float y = sin(angleRad) * j;
        pickuv = uv + float2(x, y);

        float weight = Gaussian(uv, pickuv, pickRange);
        col += DotFilter(pickuv,tex) * weight;
        totalWeight += weight;
    }
    col.rgb = col.rgb / totalWeight;
    return col;
}

float4 CrossFilter(float2 uv, Texture2D<float4> tex)
{
    float4 col;
    for (int i = 0; i < 3 ;i++)
    {
        col += GaussianAngleBlur(uv, tex, i * 60);
    }
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
    float4 texcolor1;
    
    color = texcolor;
    if (kadai)
    {
        texcolor.rgb = 1 - texcolor.rgb;
        texcolor1 = Blur1Pixel(input.uv, tex1);
        float num = 0;
    
        color = texcolor;
        if (fmod(input.uv.y, 0.1f) < 0.05f)
        {
            color = texcolor1;
        }
    }
    if (gaussianBlur)
    {
        color = GaussianBlur(input.uv, tex0);
    }
    if (gray)
    {
        float gray = color.r * 0.299 + color.g * 0.587 + color.b * 0.114;

        color = float4(gray + sepia, gray, gray - sepia, color.a);
    }
    if (bloom)
    {
        color += GaussianBlur(input.uv, tex1);
    }
    if (cross)
    {
        color += CrossFilter(input.uv, tex1);
    }
    if(depth2)
    {        
        float depth = tex1.Sample(smp, input.uv).r;
        
        float inFocus = 1 - smoothstep(0, _NFocusWidth, abs(depth - _FocusDepth));
        float outFocus = smoothstep(_NFocusWidth, _FFocusWidth, abs(depth - _FocusDepth));
        float middleFocus = 1 - inFocus - outFocus;
        float4 inFocusColor;
        float4 middleFocusColor;
        float4 outFocusColor;

        if (_DepthTexFlag == 0)
        {
            inFocusColor = float4(1, 0, 0, 1);
            middleFocusColor = float4(0, 1, 0, 1);
            outFocusColor = float4(0, 0, 1, 1);
        }
        else
        {
            inFocusColor = tex0.Sample(smp, input.uv);
            middleFocusColor = GaussianDepthBlur(input.uv, tex0, _NFocusWidth, _FocusDepth, 0.001);
            outFocusColor = GaussianDepthBlur(input.uv, tex0, _FFocusWidth, _FocusDepth);
        }
        color = inFocus * inFocusColor + middleFocus * middleFocusColor + outFocus * outFocusColor;
    }
    return color;
}