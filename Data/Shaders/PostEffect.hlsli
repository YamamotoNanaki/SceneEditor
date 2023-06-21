cbuffer cbuff0 : register(b0)
{
    float shift;
    bool kadai;
    bool gaussianBlur;
    bool gray;
    float sepia;
    bool bloom;
    bool cross;
    bool depth;
    bool depth2;
    float _NFocusWidth;
    float _FFocusWidth;
    float _FocusDepth;
    bool _DepthTexFlag;
};

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float2 uv : TEXCOORD; // uv値
};
