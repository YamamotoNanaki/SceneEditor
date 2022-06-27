#include "Model.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(GSOutput input) : SV_TARGET
{
	float4 texcolor = float4(tex.Sample(smp, input.uv));
	const float shininess = 4.0f;
	float3 amb = ambient;
	float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
	float4 shadecolor = float4(ambientColor * amb , alpha);

	// 平行光源
	for (int i = 0; i < DLIGHT_NUM; i++) {
		if (dLights[i].active) {
			float3 dotlightnormal = dot(dLights[i].lightv, input.normal);
			float3 reflect = normalize(-dLights[i].lightv + 2 * dotlightnormal * input.normal);
			float3 diff = dotlightnormal * diffuse;
			float3 spe = pow(saturate(dot(reflect, eyedir)), shininess) * specular;

			shadecolor.rgb += (diff + spe) * dLights[i].lightcolor;
		}
	}

	// 点光源
	for (i = 0; i < PLIGHT_NUM; i++) {
		if (pLights[i].active) {
			// ライトへの方向ベクトル
			float3 lightv = pLights[i].lightpos - input.worldpos.xyz;
			float d = length(lightv);
			lightv = normalize(lightv);
			float atten = 1.0f / (pLights[i].lightatten.x + pLights[i].lightatten.y * d + pLights[i].lightatten.z * d * d);
			float3 dotlightnormal = dot(lightv, input.normal);
			float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			float3 diff = dotlightnormal * diffuse;
			float3 spe = pow(saturate(dot(reflect, eyedir)), shininess) * specular;

			// 全て加算する
			shadecolor.rgb += atten * (diff + spe) * pLights[i].lightcolor;
		}
	}

	// スポットライト
	for (i = 0; i < SLIGHT_NUM; i++) {
		if (sLights[i].active) {
			// ライトへの方向ベクトル
			float3 lightv = sLights[i].lightpos - input.worldpos.xyz;
			float d = length(lightv);
			lightv = normalize(lightv);

			float atten = saturate(1.0f / (sLights[i].lightatten.x + sLights[i].lightatten.y * d + sLights[i].lightatten.z * d * d));
			float cos = dot(lightv, sLights[i].lightv);
			float angleatten = smoothstep(sLights[i].lightFactorAngleCos.y, sLights[i].lightFactorAngleCos.x, cos);
			atten *= angleatten;

			float3 dotlightnormal = dot(lightv, input.normal);
			float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
			float3 diff = dotlightnormal * diffuse;
			float3 spe = pow(saturate(dot(reflect, eyedir)), shininess) * specular;

			// 全て加算する
			shadecolor.rgb += atten * (diff + spe) * sLights[i].lightcolor;
		}
	}

	// 丸影
	for (i = 0; i < CSHADOW_NUM; i++) {
		if (cShadows[i].active) {
			// オブジェクト表面からキャスターへのベクトル
			float3 casterv = cShadows[i].casterpos - input.worldpos.xyz;
			float d = dot(casterv,cShadows[i].shadowv);

			float atten = saturate(1.0f / (cShadows[i].shadowatten.x + cShadows[i].shadowatten.y * d + cShadows[i].shadowatten.z * d * d));
			atten *= step(0, d);

			float3 lightpos = cShadows[i].casterpos + cShadows[i].shadowv * cShadows[i].distanceCasterLight;
			float3 lightv = normalize(lightpos - input.worldpos.xyz);
			float cos = dot(lightv, cShadows[i].shadowv);
			float angleatten = smoothstep(cShadows[i].shadowFactorAngleCos.y,cShadows[i].shadowFactorAngleCos.x, cos);
			atten *= angleatten;

			// 全て減算する
			shadecolor.rgb -= atten;
		}
	}

	return shadecolor * texcolor * color;
}