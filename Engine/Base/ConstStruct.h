#pragma once
#include "IFMath.h"

namespace IF
{
	struct ConstBufferDataColor
	{
		Float4 color;	//�F(RGBA)
	};

	struct ConstBufferDataTransform
	{
		Matrix viewPro;
		Matrix world;
		Float3 cameraPos;
		float explosion;
		float polygonSize;
		float gravity;
		float pad2;
		float pad3;
	};

	struct ConstBufferBillboard
	{
		Matrix mat;
		Matrix matBillboard;
	};
	struct ConstBufferMaterial
	{
		Float3 ambient;
		float pad1;
		Float3 diffuse;
		float pad2;
		Float3 specular;
		float alpha;
	};
}