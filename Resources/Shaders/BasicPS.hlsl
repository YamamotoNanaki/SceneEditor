#include "Basic.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(GSOutput input) : SV_TARGET
{
	float3 light = normalize(float3(1,-1,1));
	float Ldiffuse = saturate(dot(-light, input.normal));
	float3 shade_color;
	shade_color = ambient;
	shade_color += diffuse * Ldiffuse;
	float4 texcolor = float4(tex.Sample(smp, input.uv));
	return float4(texcolor.rgb * shade_color, texcolor.a * alpha) * color;
}