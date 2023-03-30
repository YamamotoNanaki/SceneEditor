#pragma once
#include "IFMath.h"

namespace IF
{
	static const int MAX_BONES = 128;
	static const int MAX_METABALL = 128;
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
		bool lightFlag = true;
		bool toonFlag = false;
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
		float pad1;
		float pad2;
		float pad3;
	};
	struct ConstBufferDataSkin
	{
		Matrix bones[MAX_BONES];
	};
	struct ConstBufferMatrix
	{
		Matrix mat;
	};
	struct ConstBufferMargingCubesSpace
	{
		Vector3 numCells;
		float pad;
		Vector3 cellSize;
		float pad1;
	};

	struct MetaballSpheres
	{
		Float3 pos = { 0,0,0 };
		bool activ = false;
		Float4 color = { 1,1,1,1 };
		Float4 randomValues;
		Float3 scale = { 1,1,1 };
		float pad;
	};

	struct ConstBufferNumSpheres
	{
		MetaballSpheres sphere[MAX_METABALL];
		float time;
		float effectValue;
		float smoothUnionValue;
		float numSpheres;
	};
}