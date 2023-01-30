#include "Model.hlsli"
#include "Util.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);



float4 main(GSOutput input) : SV_TARGET
{
    float4 texcolor = float4(tex.Sample(smp, input.uv));
    const float shininess = 4.0f;
    float3 amb = ambient;
    float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
    float4 shadecolor = float4(ambientColor * amb, alpha);

    if (lightFlag)
    {
	// 平行光源
        for (int i = 0; i < DLIGHT_NUM; i++)
        {
            if (dLights[i].active)
            {
                float3 dotlightnormal = dot(dLights[i].lightv, input.normal);
                float3 reflect = normalize(-dLights[i].lightv + 2 * dotlightnormal * input.normal);
                float3 diff = dotlightnormal * diffuse;
                diff = diff <= 0 ? 0 : diff;
                float3 spe = pow(saturate(dot(reflect, eyedir)), shininess) * specular;

                shadecolor.rgb += (diff + spe) * dLights[i].lightcolor;
            }
        }

	// 点光源
        for (i = 0; i < PLIGHT_NUM; i++)
        {
            if (pLights[i].active)
            {
			// ライトへの方向ベクトル
                float3 lightv = pLights[i].lightpos - input.worldpos.xyz;
                float d = length(lightv);
                lightv = normalize(lightv);
                float atten = 1.0f / (pLights[i].lightatten.x + pLights[i].lightatten.y * d + pLights[i].lightatten.z * d * d);
                float3 dotlightnormal = dot(lightv, input.normal);
                float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
                reflect = reflect <= 0 ? 0 : reflect;
                float3 diff = dotlightnormal * diffuse;
                diff = diff <= 0 ? 0 : diff;
                float3 spe = pow(saturate(dot(reflect, eyedir)), shininess) * specular;

			// 全て加算する
                shadecolor.rgb += atten * (diff + spe) * pLights[i].lightcolor;
            }
        }

	// スポットライト
        for (i = 0; i < SLIGHT_NUM; i++)
        {
            if (sLights[i].active)
            {
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
                diff = diff <= 0 ? 0 : diff;
                float3 spe = pow(saturate(dot(reflect, eyedir)), shininess) * specular;

			// 全て加算する
                shadecolor.rgb += atten * (diff + spe) * sLights[i].lightcolor;
            }
        }

	// 丸影
        for (i = 0; i < CSHADOW_NUM; i++)
        {
            if (cShadows[i].active)
            {
			// オブジェクト表面からキャスターへのベクトル
                float3 casterv = cShadows[i].casterpos - input.worldpos.xyz;
                float d = dot(casterv, cShadows[i].shadowv);

                float atten = saturate(1.0f / (cShadows[i].shadowatten.x + cShadows[i].shadowatten.y * d + cShadows[i].shadowatten.z * d * d));
                atten *= step(0, d);

                float3 lightpos = cShadows[i].casterpos + cShadows[i].shadowv * cShadows[i].distanceCasterLight;
                float3 lightv = normalize(lightpos - input.worldpos.xyz);
                float cos = dot(lightv, cShadows[i].shadowv);
                float angleatten = smoothstep(cShadows[i].shadowFactorAngleCos.y, cShadows[i].shadowFactorAngleCos.x, cos);
                atten *= angleatten;

			// 全て減算する
                shadecolor.rgb -= atten;
            }
        }
        if (toonFlag)
        {
            float4 toon = shadecolor;
            toon.r = step(0.4, shadecolor.r);
            if (toon.r == 1)
            {
                if (shadecolor.r < 0.5)
                {
                    toon.r = Lerp(0, 0.5, 0.1, shadecolor.r - 0.4);
                }
                else
                {
                    toon.r = step(0.8, shadecolor.r);
                    if (toon.r == 0)
                    {
                        toon.r = 0.5;
                    }
                    else if (shadecolor.r < 0.9)
                    {
                        toon.r = Lerp(0.5, 1, 0.1, shadecolor.r - 0.8);
                    }
                }
            }
            toon.g = step(0.4, shadecolor.g);
            if (toon.g == 1)
            {
                if (shadecolor.g < 0.5)
                {
                    toon.g = Lerp(0, 0.5, 0.1, shadecolor.g - 0.4);
                }
                else
                {
                    toon.g = step(0.8, shadecolor.g);
                    if (toon.g == 0)
                    {
                        toon.g = 0.5;
                    }
                    else if (shadecolor.g < 0.9)
                    {
                        toon.g = Lerp(0.5, 1, 0.1, shadecolor.g - 0.8);
                    }
                }
            }
            toon.b = step(0.4, shadecolor.b);
            if (toon.b == 1)
            {
                if (shadecolor.b < 0.5)
                {
                    toon.b = Lerp(0, 0.5, 0.1, shadecolor.b - 0.4);
                }
                else
                {
                    toon.b = step(0.8, shadecolor.b);
                    if (toon.b == 0)
                    {
                        toon.b = 0.5;
                    }
                    else if (shadecolor.b < 0.9)
                    {
                        toon.b = Lerp(0.5, 1, 0.1, shadecolor.b - 0.8);
                    }
                }
            }
            shadecolor = toon;
        }
    }

    return shadecolor * texcolor * color;
}