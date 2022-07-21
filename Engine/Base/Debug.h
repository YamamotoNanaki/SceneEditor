#pragma once
#include <d3d12.h>

#ifdef _DEBUG
//デバッグレイヤーを使用
void Debug(ID3D12Device* device);
#endif
