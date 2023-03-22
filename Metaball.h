#pragma once
//#include "Particle.h"
#include "ComponentObj.h"
#include <wrl.h>

namespace IF
{
	class Metaball/* :public Particle*/
	{
	private:
		Vector3 pos;
		Vector3 scale;
		Vector3 rota;

		uint16_t numMarchingSegments = 30;  // �Z���̕�����
		float margingSpaceSize = 64;     // �}�[�`���O�L���[�u�̃X�y�[�X�̃T�C�Y
		uint16_t numSpheres = 6;            // ���^�{�[���̐�
		uint16_t smoothUnionValue = 6;            // ���^�{�[���̌����̓x����
		Float3 sphereColor = { 255, 0, 0 };  // ���^�{�[���̐F
		PV vi;
		ConstBuff cb;
		ICamera* camera;
		//�萔�o�b�t�@�ƃ}�b�v
		Microsoft::WRL::ComPtr<ID3D12Resource> matrixTransform;
		ConstBufferMatrix* constMapMatrix;

		//�萔�o�b�t�@�ƃ}�b�v
		Microsoft::WRL::ComPtr<ID3D12Resource> margingCubesSpaceTransform;
		ConstBufferMargingCubesSpace* constMapMargingCubesSpace;

		//�萔�o�b�t�@�ƃ}�b�v
		Microsoft::WRL::ComPtr<ID3D12Resource> numSpheresTransform;
		ConstBufferNumSpheres* constMapNumSpheres;

	public:
		void Initialize()/*override*/;
		void Update();
		~Metaball();
		void Draw();

	private:
		void UpdateMargingCubesSpace();
		void UpdateNumSpheres();
		void TransferConstBuffer();
		void DataTransferGPU();
	};
}
