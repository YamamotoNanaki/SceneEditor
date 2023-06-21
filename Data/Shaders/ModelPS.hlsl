#include "Model.hlsli"
#include "Util.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);



PSOutput main(GSOutput input) : SV_TARGET
{
    float4 texcolor = float4(tex.Sample(smp, input.uv));
    const float shininess = 4.0f;
    float3 amb = color.xyz * 0.2f * ambient * texcolor.xyz * color.xyz;
    float3 eyedir = normalize(cameraPos - input.worldpos.xyz);
    float4 shadecolor = float4(amb, alpha * texcolor.a);

    if (lightFlag)
    {
	// 平行光源
        for (int i = 0; i < DLIGHT_NUM; i++)
        {
            if (dLights[i].active)
            {
                float3 dotlightnormal = dot(dLights[i].lightv, input.normal);
                float3 reflect = normalize(-dLights[i].lightv + 2 * dotlightnormal * input.normal);
                if (toonFlag)
                {
                    dotlightnormal = smoothstep(0.3, 0.35, dotlightnormal);
                }
                
                float3 diff = dotlightnormal * diffuse * texcolor.xyz * color.xyz;
                diff = diff <= 0 ? 0 : diff;
                float3 spe;
                if (toonFlag)
                {
                    spe = smoothstep(0.4, 0.45, pow(saturate(dot(reflect, eyedir)), shininess) * specular);
                }
                else
                {
                    spe = pow(saturate(dot(reflect, eyedir)), shininess) * specular;
                }

                if (!mixFlag && toonFlag)
                {
                    float3 a = (1 - dotlightnormal - spe) * ambientColor;
                    a = a < 0 ? 0 : a;
                    shadecolor.rgb = a + (1 - spe) * dotlightnormal * difColor.xyz + spe * speColor.xyz;
                }
                else
                {
                    shadecolor.rgb += (diff + spe) * dLights[i].lightcolor;
                }
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
                
                float3 dotlightnormal = dot(dLights[i].lightv, input.normal);
                float3 reflect = normalize(-dLights[i].lightv + 2 * dotlightnormal * input.normal);
                if (toonFlag)
                {
                    dotlightnormal = smoothstep(0.3, 0.35, dotlightnormal);
                }
                float3 diff = dotlightnormal * diffuse * texcolor.xyz * color.xyz;
                diff = diff <= 0 ? 0 : diff;
                float3 spe;
                if (toonFlag)
                {
                    spe = smoothstep(0.4, 0.45, pow(saturate(dot(reflect, eyedir)), shininess) * specular);
                }
                else
                {
                    spe = pow(saturate(dot(reflect, eyedir)), shininess) * specular;
                }

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
                
                float3 dotlightnormal = dot(dLights[i].lightv, input.normal);
                float3 reflect = normalize(-dLights[i].lightv + 2 * dotlightnormal * input.normal);
                if (toonFlag)
                {
                    dotlightnormal = smoothstep(0.3, 0.35, dotlightnormal);
                }
                float3 diff = dotlightnormal * diffuse * texcolor.xyz * color.xyz;
                diff = diff <= 0 ? 0 : diff;
                float3 spe;
                if (toonFlag)
                {
                    spe = smoothstep(0.4, 0.45, pow(saturate(dot(reflect, eyedir)), shininess) * specular);
                }
                else
                {
                    spe = pow(saturate(dot(reflect, eyedir)), shininess) * specular;
                }

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
    }
    
    float rim = 0;
    if (rimFlag)
    {
        if (toonFlag)
        {
            rim = smoothstep(0.3, 0.35, pow(1 - dot(eyedir, input.normal), 3));
        }
        else
        {
            rim = pow(1 - dot(eyedir, input.normal), 3);
        }
    }

    PSOutput o;
    o.target0 = (1 - rim) * shadecolor + rim * rimColor;
    
    float4 col = float4(0,0,0,0);
    if (depth)
    {
        col.r = input.svpos.z / input.svpos.w;
        col.w = 1;
    }
    else
    {
        col = o.target0;
        float grayScale = col.r * 0.299 + col.g * 0.587 * col.b * 0.114;
        float extract = smoothstep(0.2, 0.3, grayScale);
        col *= extract;
    }
    o.target1 = col;
    return o;
}