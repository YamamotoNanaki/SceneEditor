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

// ���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����\����
struct VSOutput
{
    float4 svpos : SV_POSITION; // �V�X�e���p���_���W
    float2 uv : TEXCOORD; // uv�l
};
