#include "MathConvert.h"

using namespace DirectX;
using namespace IF;

DirectX::XMMATRIX IF::MatrixConvert(Matrix m)
{
	return XMMatrixSet(m._1_1, m._1_2, m._1_3, m._1_4, m._2_1, m._2_2, m._2_3, m._2_4,
		m._3_1, m._3_2, m._3_3, m._3_4, m._4_1, m._4_2, m._4_3, m._4_4);
}

Matrix IF::MatrixConvert(DirectX::XMMATRIX xm)
{
	return Matrix(xm.r[0].m128_f32[0],xm.r[0].m128_f32[1],xm.r[0].m128_f32[2],xm.r[0].m128_f32[3],
		xm.r[1].m128_f32[0],xm.r[1].m128_f32[1],xm.r[1].m128_f32[2],xm.r[1].m128_f32[3],
		xm.r[2].m128_f32[0],xm.r[2].m128_f32[1],xm.r[2].m128_f32[2],xm.r[2].m128_f32[3],
		xm.r[3].m128_f32[0],xm.r[3].m128_f32[1],xm.r[3].m128_f32[2],xm.r[3].m128_f32[3]);
}

DirectX::XMFLOAT3 IF::Float3Convert(Float3 m)
{
	return XMFLOAT3(m.x, m.y, m.z);
}

Float3 IF::Float3Convert(DirectX::XMFLOAT3 xm)
{
	return Float3(xm.x, xm.y, xm.z);
}
