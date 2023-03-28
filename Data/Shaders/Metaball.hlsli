cbuffer ConstBufferBillboard : register(b0)
{
    matrix mat; //行列
    matrix matBillboard;
};

cbuffer ConstBufferMargingCubesSpace : register(b1)
{
    float3 numCells;
    float numVertices;
    float3 cellSize;
}

struct MetaballSpheres
{
    float3 pos;
    bool activ;
    float4 color;
    float3 scale;
};

static const int MAX_METABALL = 128;

cbuffer ConstBufferNumSpheres : register(b2)
{
    MetaballSpheres sphere[MAX_METABALL];
}

struct VSOutput
{
    float4 pos : POSITION; //頂点座標
};

struct GSOutput
{
    float4 svpos : SV_POSITION; //頂点座標
    float2 uv : TEXCOORD; //uv値
};
