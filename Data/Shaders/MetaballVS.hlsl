#include "Metaball.hlsli"

Texture2D<float> tex : register(t0);
SamplerState smp : register(s0);

float3 rotateVec3(float3 p, float angle, float3 axis)
{
    float3 a = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float r = 1.0 - c;
    float3x3 m = float3x3(
    a.x * a.x * r + c,
    a.y * a.x * r + a.z * s,
    a.z * a.x * r - a.y * s,
    a.x * a.y * r - a.z * s,
    a.y * a.y * r + c,
    a.z * a.y * r + a.x * s,
    a.x * a.z * r + a.y * s,
    a.y * a.z * r - a.x * s,
    a.z * a.z * r + c
  );
    return mul(m, p);
}


// 球の距離関数
float sphere_func(float3 p, float r)
{
    return length(p) - r;
}

// メタボールをランダムに動かす
float randomObj(float3 p, int i, float4 randomValues)
{
    static const float PI = acos(-1);
    static const float PI2 = PI * 2.0;
    float t = fmod(time * 0.002 * (0.2 + randomValues.w) + randomValues.z * 100.0, PI2);
    float3 translate = (randomValues.xyz * 2.0 - 1.0) * 20.0 * sin(t);
    float r = 6.0 + randomValues.x * 6.0;
    float l = cellSize.x;
    p -= translate;
    return sphere_func(p, r);
}

// Smooth Union
float opSmoothUnion(float d1, float d2, float k)
{
    float h = clamp(0.5 + 0.5 * (d2 - d1) / k, 0.0, 1.0);
    return lerp(d2, d1, h) - k * h * (1.0 - h);
}

// 最終的な距離関数
float getDistance(float3 p)
{
    static const float PI = acos(-1);
    static const float PI2 = PI * 2.0;
  // 適当に回転
    float theta = fmod(time * 0.001, PI2);
    p = rotateVec3(p, theta, float3(0.0, 0.0, 1.0));
    p = rotateVec3(p, theta, float3(1.0, 0.0, 0.0));

    float result = 0.0;
    float d;
    for (int i = 0; i < numSpheres; i++)
    {
        d = randomObj(p, i, sphere[i].randomValues);
        if (result == 0.0)
            result = d;
        result = opSmoothUnion(result, d, smoothUnionValue);
    }

    return result;
}

// 法線を算出
float3 getNormal(float3 p)
{
    float d = 0.1;
    return normalize(float3(
    getDistance(p + float3(d, 0.0, 0.0)) - getDistance(p - float3(d, 0.0, 0.0)),
    getDistance(p + float3(0.0, d, 0.0)) - getDistance(p - float3(0.0, d, 0.0)),
    getDistance(p + float3(0.0, 0.0, d)) - getDistance(p - float3(0.0, 0.0, d))
  ));
}

// v0, v1の値をもとにp0, p1を補間した値を返す
float3 interpolate(float3 p0, float3 p1, float v0, float v1)
{
    return lerp(p0, p1, -v0 / (v1 - v0));
}

// 論理演算用
int modi(int x, int y)
{
    return x - y * (x / y);
}

// 論理和
int or(int a, int b)
{
    int result = 0;
    int n = 1;
    for (int i = 0; i < 8; i++)
    {
        if ((modi(a, 2) == 1) || (modi(b, 2) == 1))
            result += n;
        a = a / 2;
        b = b / 2;
        n = n * 2;
        if (!(a > 0 || b > 0))
            break;
    }
    return result;
}

VSOutput main(float vertexId : BLENDINDICES, float4 pos : POSITION)
{
    VSOutput vsout;
    float cellId = floor(vertexId / 15.0); // セルのID
    float vertexIdInCell = fmod(vertexId, 15.0); // セル内での頂点のID

  // セルの基準点を算出
    float3 cellIndices = float3(
    fmod(cellId, numCells.x),
    fmod(cellId, (numCells.x * numCells.y)) / numCells.x,
    cellId / (numCells.x * numCells.y)
  );
    cellIndices = floor(cellIndices);

  // セルの基準点 (立方体の向かって左下奥)
    float3 c0 = (0.5 * numCells - cellIndices) * cellSize;

  // セルの各頂点
    float3 c1 = c0 + cellSize * float3(1.0, 0.0, 0.0);
    float3 c2 = c0 + cellSize * float3(1.0, 0.0, 1.0);
    float3 c3 = c0 + cellSize * float3(0.0, 0.0, 1.0);
    float3 c4 = c0 + cellSize * float3(0.0, 1.0, 0.0);
    float3 c5 = c0 + cellSize * float3(1.0, 1.0, 0.0);
    float3 c6 = c0 + cellSize * float3(1.0, 1.0, 1.0);
    float3 c7 = c0 + cellSize * float3(0.0, 1.0, 1.0);

  // セルの各頂点のボクセル値を求める (ボクセル値はメタボールまでの距離)
    float v0 = getDistance(c0);
    float v1 = getDistance(c1);
    float v2 = getDistance(c2);
    float v3 = getDistance(c3);
    float v4 = getDistance(c4);
    float v5 = getDistance(c5);
    float v6 = getDistance(c6);
    float v7 = getDistance(c7);

  // ルックアップテーブルの参照

  // まずはポリゴンの張り方の256通りのうちどのパターンになるか調べる
    float cubeIndex = 0.0;
    //float cubeIndex = 16.0;
    //cubeIndex = lerp(cubeIndex, float(or(int(cubeIndex), 1)), 1.0 - step(0.0, v0));
    //cubeIndex = lerp(cubeIndex, float(or(int(cubeIndex), 2)), 1.0 - step(0.0, v1));
    //cubeIndex = lerp(cubeIndex, float(or(int(cubeIndex), 4)), 1.0 - step(0.0, v2));
    //cubeIndex = lerp(cubeIndex, float(or(int(cubeIndex), 8)), 1.0 - step(0.0, v3));
    //cubeIndex = lerp(cubeIndex, float(or(int(cubeIndex), 16)), 1.0 - step(0.0, v4));
    //cubeIndex = lerp(cubeIndex, float(or(int(cubeIndex), 32)), 1.0 - step(0.0, v5));
    //cubeIndex = lerp(cubeIndex, float(or(int(cubeIndex), 64)), 1.0 - step(0.0, v6));
    //cubeIndex = lerp(cubeIndex, float(or(int(cubeIndex), 128)), 1.0 - step(0.0, v7));

  // 続いて現在の頂点がどの辺上に配置されるかを調べる
  // つまり、ルックアップテーブルのどの値を参照するかのインデックスを求める
    //float edgeIndex = tex[float2((cubeIndex * 16.0 + vertexIdInCell) / 4096.0, 0.0)] * 255.0;
    //float edgeIndex = float4(tex.SampleLevel(smp, float2(cubeIndex / 4096, 0.5), 0.0)).a * 255.0;
    float edgeIndex = tex.SampleLevel(smp, float2(cubeIndex / 4096.0, 0.0), 0).r;
    edgeIndex *= 255;
    float3 fpos = pos.xyz;

    vsout.vDiscard = 0.0;
    if (edgeIndex == 255.0)
    {
        fpos.x += 7;
        vsout.pos = float4(fpos, 1.0);
        return vsout;
    // edgeIndexが255の場合、頂点は破棄
        //vNormal = float3(0.0, 0.0, 1.0);
        fpos = pos.xyz;
        vsout.vDiscard = 1.0;
    }
    else
    {
        fpos.x -= 7;
        vsout.pos = float4(fpos, 1.0);
        return vsout;
    }
    //else if (edgeIndex == 0.0)
    //{
    //    fpos = interpolate(c0, c1, v0, v1);
    //}
    //else if (edgeIndex == 1.0)
    //{
    //    fpos = interpolate(c1, c2, v1, v2);
    //}
    //else if (edgeIndex == 2.0)
    //{
    //    fpos = interpolate(c2, c3, v2, v3);
    //}
    //else if (edgeIndex == 3.0)
    //{
    //    fpos = interpolate(c3, c0, v3, v0);
    //}
    //else if (edgeIndex == 4.0)
    //{
    //    fpos = interpolate(c4, c5, v4, v5);
    //}
    //else if (edgeIndex == 5.0)
    //{
    //    fpos = interpolate(c5, c6, v5, v6);
    //}
    //else if (edgeIndex == 6.0)
    //{
    //    fpos = interpolate(c6, c7, v6, v7);
    //}
    //else if (edgeIndex == 7.0)
    //{
    //    fpos = interpolate(c4, c7, v4, v7);
    //}
    //else if (edgeIndex == 8.0)
    //{
    //    fpos = interpolate(c0, c4, v0, v4);
    //}
    //else if (edgeIndex == 9.0)
    //{
    //    fpos = interpolate(c1, c5, v1, v5);
    //}
    //else if (edgeIndex == 10.0)
    //{
    //    fpos = interpolate(c2, c6, v2, v6);
    //}
    //else if (edgeIndex == 11.0)
    //{
    //    fpos = interpolate(c3, c7, v3, v7);
    //}

    //vNormal = getNormal(pos);

    // エフェクト
    float3 effectSize = cellSize * 1.5;
    fpos = lerp(fpos, floor(fpos / effectSize + 0.5) * effectSize, effectValue);

    //vPos = pos;

    vsout.pos = mul(mat, float4(fpos, 1.0));
    //vsout.pos = float4(fpos, 1.0);
    return vsout;
}