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
		Float4 rimColor;
		Float4 difColor;
		Float4 speColor;
		Float3 cameraPos;
		float explosion;
		float polygonSize;
		float gravity;
		float rotation;
		unsigned int lightFlag = true;
		unsigned int toonFlag = false;
		unsigned int rimFlag = false;
		unsigned int mixFlag = false;
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
		unsigned int kadai;
		unsigned int gaussianBlur;
		unsigned int gray = false;
		float sepia = 0;
		unsigned int bloom;
		unsigned int cross;
		unsigned int depth;
		unsigned int depth2;
		float pad;
		float pad1;
		float pad2;
	};
	struct ConstBufferDataSkin
	{
		Matrix bones[MAX_BONES];
	};
}