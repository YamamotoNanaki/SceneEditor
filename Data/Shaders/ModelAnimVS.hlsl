#include "ModelAnim.hlsli"

SkinOutput ComputeSkin(VSInput input)
{
    SkinOutput output = (SkinOutput) 0;

    uint iBone;
    float weight;
    matrix m;

    iBone = input.boneIndices.x;
    weight = input.boneWeights.x;
    m = matSkinning[iBone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);

    iBone = input.boneIndices.y;
    weight = input.boneWeights.y;
    m = matSkinning[iBone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);

    iBone = input.boneIndices.z;
    weight = input.boneWeights.z;
    m = matSkinning[iBone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);

    iBone = input.boneIndices.w;
    weight = input.boneWeights.w;
    m = matSkinning[iBone];
    output.pos += weight * mul(m, input.pos);
    output.normal += weight * mul((float3x3) m, input.normal);

    return output;
}

VSOutput main(VSInput input)
{
    SkinOutput skinned = ComputeSkin(input);
    float4 wnormal = normalize(mul(world, float4(skinned.normal, 0)));
    float4 wpos = mul(world, skinned.pos);

	VSOutput output;//ピクセルシェーダーに渡す値
    output.svpos = mul(mul(viewPro, world), skinned.pos); //座標に行列を乗算
	output.worldpos = wpos;
	output.normal = wnormal.xyz;
	output.uv = input.uv;
	return output;
}