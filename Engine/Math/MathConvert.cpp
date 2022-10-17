#include "MathConvert.h"

using namespace DirectX;
using namespace IF;

DirectX::XMMATRIX IF::MatrixConvert(Matrix m)
{
	return XMMatrixSet(
		m.m[0][0],
		m.m[0][1],
		m.m[0][2],
		m.m[0][3],
		m.m[1][0],
		m.m[1][1],
		m.m[1][2],
		m.m[1][3],
		m.m[2][0],
		m.m[2][1],
		m.m[2][2],
		m.m[2][3],
		m.m[3][0],
		m.m[3][1],
		m.m[3][2],
		m.m[3][3]);
}

Matrix IF::MatrixConvert(DirectX::XMMATRIX xm)
{
	return Matrix(xm.r[0].m128_f32[0], xm.r[0].m128_f32[1], xm.r[0].m128_f32[2], xm.r[0].m128_f32[3],
		xm.r[1].m128_f32[0], xm.r[1].m128_f32[1], xm.r[1].m128_f32[2], xm.r[1].m128_f32[3],
		xm.r[2].m128_f32[0], xm.r[2].m128_f32[1], xm.r[2].m128_f32[2], xm.r[2].m128_f32[3],
		xm.r[3].m128_f32[0], xm.r[3].m128_f32[1], xm.r[3].m128_f32[2], xm.r[3].m128_f32[3]);
}

DirectX::XMFLOAT3 IF::Float3Convert(Float3 m)
{
	return XMFLOAT3(m.x, m.y, m.z);
}

Float3 IF::Float3Convert(DirectX::XMFLOAT3 xm)
{
	return Float3(xm.x, xm.y, xm.z);
}
