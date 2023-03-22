#pragma once
//#include "Particle.h"
#include "ComponentObj.h"

namespace IF
{
	class Metaball/* :public Particle*/
	{
	private:
		uint16_t numMarchingSegments = 30;  // �Z���̕�����
		uint16_t margingSpaceSize = 64;     // �}�[�`���O�L���[�u�̃X�y�[�X�̃T�C�Y
		uint16_t numSpheres = 6;            // ���^�{�[���̐�
		uint16_t smoothUnionValue = 6;            // ���^�{�[���̌����̓x����
		Float3 sphereColor = { 255, 0, 0 };  // ���^�{�[���̐F

	public:
		void Initialize()/*override*/;
		static void StaticInitialize();

	private:
		static Vector3 VertexInterp(float isolevel, Vector3 p1, Vector3 p2, float valp1, float valp2);
		static void CreateTexture();
	};
}
