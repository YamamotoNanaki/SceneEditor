float3 position;
float vertexId;

uniform float time;
uniform float3 numCells;
uniform float3 cellSize;
uniform sampler2D triTableTexture;

uniform float effectValue;
uniform float smoothUnionValue;

uniform float4x4 modelViewMatrix;
uniform float3x3 normalMatrix;
uniform float4x4 projectionMatrix;
uniform float3 cameraPosition;
uniform float4 randomValues[NUM_SPHERES];

float3 vPos;
float3 vNormal;
float vDiscard;

//#pragma glslify: PI = require('../../_utils/glsl/PI.glsl)
//#pragma glslify: rotateVec3 = require('../../_utils/glsl/rotateVec3.glsl)
static const float PI = acos(-1);
const float PI2 = PI * 2.0;
const float3 AXIS_X = float3(1.0, 0.0, 0.0);
const float3 AXIS_Y = float3(0.0, 1.0, 0.0);
const float3 AXIS_Z = float3(0.0, 0.0, 1.0);

// 球の距離関数
float sphere(float3 p, float r)
{
    return length(p) - r;
}

// メタボールをランダムに動かす
//float randomObj(float3 p, int i, float4 randomValues)
//{
//    float t = fmod(time * 0.002 * (0.2 + randomValues.w) + randomValues.z * 100.0, PI2);
//    float3 translate = (randomValues.xyz * 2.0 - 1.0) * 20.0 * sin(t);
//    float r = 6.0 + randomValues.x * 6.0;
//    float l = cellSize.x;
//    p -= translate;
//    return sphere(p, r);
//}

// Smooth Union
float opSmoothUnion(float d1, float d2, float k)
{
    float h = clamp(0.5 + 0.5 * (d2 - d1) / k, 0.0, 1.0);
    return lerp(d2, d1, h) - k * h * (1.0 - h);
}

// 最終的な距離関数
float getDistance(float3 p)
{
  // 適当に回転
    float theta = fmod(time * 0.001, PI2);
    p = rotateVec3(p, theta, AXIS_Z);
    p = rotateVec3(p, theta, AXIS_X);

    float result = 0.0;
    float d;
    for (int i = 0; i < NUM_SPHERES; i++)
    {
        d = randomObj(p, i, randomValues[i]);
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

void main()
{
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
    cubeIndex = lerp(cubeIndex, float(or(int(cubeIndex), 1)), 1.0 - step(0.0, v0));
    cubeIndex = lerp(cubeIndex, float(or(int(cubeIndex), 2)), 1.0 - step(0.0, v1));
    cubeIndex = lerp(cubeIndex, float(or(int(cubeIndex), 4)), 1.0 - step(0.0, v2));
    cubeIndex = lerp(cubeIndex, float(or(int(cubeIndex), 8)), 1.0 - step(0.0, v3));
    cubeIndex = lerp(cubeIndex, float(or(int(cubeIndex), 16)), 1.0 - step(0.0, v4));
    cubeIndex = lerp(cubeIndex, float(or(int(cubeIndex), 32)), 1.0 - step(0.0, v5));
    cubeIndex = lerp(cubeIndex, float(or(int(cubeIndex), 64)), 1.0 - step(0.0, v6));
    cubeIndex = lerp(cubeIndex, float(or(int(cubeIndex), 128)), 1.0 - step(0.0, v7));

  // 続いて現在の頂点がどの辺上に配置されるかを調べる
  // つまり、ルックアップテーブルのどの値を参照するかのインデックスを求める
    float edgeIndex =

    texture2D( triTableTexture,

    float2
    ((cubeIndex * 16.0 + vertexIdInCell) / 4096.0, 0.0)).
    a * 255.0;
    float3 pos = position;

    vDiscard = 0.0;
    if (edgeIndex == 255.0)
    {
    // edgeIndexが255の場合、頂点は破棄
        vNormal = float3(0.0, 0.0, 1.0);
        pos = position;
        vDiscard = 1.0;
    }
    else if (edgeIndex == 0.0)
    {
        pos = interpolate(c0, c1, v0, v1);
    }
    else if (edgeIndex == 1.0)
    {
        pos = interpolate(c1, c2, v1, v2);
    }
    else if (edgeIndex == 2.0)
    {
        pos = interpolate(c2, c3, v2, v3);
    }
    else if (edgeIndex == 3.0)
    {
        pos = interpolate(c3, c0, v3, v0);
    }
    else if (edgeIndex == 4.0)
    {
        pos = interpolate(c4, c5, v4, v5);
    }
    else if (edgeIndex == 5.0)
    {
        pos = interpolate(c5, c6, v5, v6);
    }
    else if (edgeIndex == 6.0)
    {
        pos = interpolate(c6, c7, v6, v7);
    }
    else if (edgeIndex == 7.0)
    {
        pos = interpolate(c4, c7, v4, v7);
    }
    else if (edgeIndex == 8.0)
    {
        pos = interpolate(c0, c4, v0, v4);
    }
    else if (edgeIndex == 9.0)
    {
        pos = interpolate(c1, c5, v1, v5);
    }
    else if (edgeIndex == 10.0)
    {
        pos = interpolate(c2, c6, v2, v6);
    }
    else if (edgeIndex == 11.0)
    {
        pos = interpolate(c3, c7, v3, v7);
    }

    vNormal = getNormal(pos);

  // エフェクト
    vec3 effectSize = cellSize * 1.5;
    pos = lerp(pos, floor(pos / effectSize + 0.5) * effectSize, effectValue);

    vPos = pos;

    gl_Position = projectionMatrix * modelViewMatrix * float4(pos, 1.0);
}