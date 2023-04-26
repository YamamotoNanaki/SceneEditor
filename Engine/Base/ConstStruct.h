#pragma once
#include "IFMath.h"

namespace IF
{
	static const int MAX_BONES = 128;
	struct ConstBufferDataColor
	{
		Float4 color;	//色(RGBA)
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
		unsigned int lightFlag = true;
		unsigned int toonFlag = false;
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
		//0~0.005くらいで
		float shift;
		bool kadai;
		float pad2;
		float pad3;
	};
	struct ConstBufferDataSkin
	{
		Matrix bones[MAX_BONES];
	};
}