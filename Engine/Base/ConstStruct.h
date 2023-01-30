#pragma once
#include "IFMath.h"

namespace IF
{
	static const int MAX_BONES = 128;
	struct ConstBufferDataColor
	{
		Float4 color;	//êF(RGBA)
	};

	struct ConstBufferDataTransform
	{
		Matrix viewPro;
		Matrix world;
		Float3 cameraPos;
		float explosion;
		float polygonSize;
		float gravity;
		float rotation;
		unsigned int lightFlag = 1;
		unsigned int toonFlag = 0;
		float pad;
		float pad1;
		float pad2;
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
	struct ConstBufferPostEffect
	{
		//0~0.005Ç≠ÇÁÇ¢Ç≈
		float shift;
		unsigned int nega = 0;
		unsigned int grayscale = 0;
		float sepia = 0;
	};
	struct ConstBufferDataSkin
	{
		Matrix bones[MAX_BONES];
	};
}