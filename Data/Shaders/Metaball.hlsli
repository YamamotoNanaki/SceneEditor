cbuffer ConstBufferBillboard : register(b0)
{
    matrix mat; //ïœä∑çsóÒ
    matrix matBillboard;
};

cbuffer ConstBufferMargingCubesSpace : register(b1)
{
    float3 numCells;
    float3 cellSize;
}

struct MetaballSpheres
{
    float3 pos;
    bool activ;
    float4 color;
    float4 randomValues;
    float3 scale;
    float pad;
};

static const int MAX_METABALL = 128;

cbuffer ConstBufferNumSpheres : register(b2)
{
    MetaballSpheres sphere[MAX_METABALL];
    float time;
    float effectValue;
    float smoothUnionValue;
    float numSpheres;
}

struct VSOutput
{
    float4 pos : POSITION; //í∏ì_ç¿ïW
    float vDiscard : PSIZE;
};

struct GSOutput
{
    float4 svpos : SV_POSITION;
    float vDiscard : PSIZE;
};

