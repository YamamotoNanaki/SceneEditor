#pragma once
//#include "Particle.h"
#include "ComponentObj.h"

namespace IF
{
	class Metaball/* :public Particle*/
	{
	private:
		uint16_t numMarchingSegments = 30;  // セルの分割数
		uint16_t margingSpaceSize = 64;     // マーチングキューブのスペースのサイズ
		uint16_t numSpheres = 6;            // メタボールの数
		uint16_t smoothUnionValue = 6;            // メタボールの結合の度合い
		Float3 sphereColor = { 255, 0, 0 };  // メタボールの色

	public:
		void Initialize()/*override*/;
		static void StaticInitialize();

	private:
		static Vector3 VertexInterp(float isolevel, Vector3 p1, Vector3 p2, float valp1, float valp2);
		static void CreateTexture();
	};
}
