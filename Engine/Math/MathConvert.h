#pragma once
#include <DirectXMath.h>
#include "IFMath.h"

namespace IF
{
	DirectX::XMMATRIX MatrixConvert(Matrix m);
	Matrix MatrixConvert(DirectX::XMMATRIX xm);
	DirectX::XMFLOAT3 Float3Convert(Float3 f);
	Float3 Float3Convert(DirectX::XMFLOAT3 xf);
}