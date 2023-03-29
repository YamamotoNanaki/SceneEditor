#pragma once
//#include "Particle.h"
#include "Texture.h"
#include "ComponentObj.h"
#include <wrl.h>

namespace IF
{
	class Metaball/* :public Particle*/
	{
	private:
		Vector3 pos;
		Vector3 scale = { 1,1,0 };
		Vector3 rota;

		uint16_t numMarchingSegments = 30;  // �Z���̕�����
		float margingSpaceSize = 64;     // �}�[�`���O�L���[�u�̃X�y�[�X�̃T�C�Y
		uint16_t numSpheres = 6;            // ���^�{�[���̐�
		uint16_t smoothUnionValue = 6;            // ���^�{�[���̌����̓x����
		Float3 sphereColor = { 255, 0, 0 };  // ���^�{�[���̐F
		PV vi;
		ICamera* camera;
		//�萔�o�b�t�@�ƃ}�b�v
		Microsoft::WRL::ComPtr<ID3D12Resource> matrixTransform;
		ConstBufferBillboard* constMapMatrix;

		//�萔�o�b�t�@�ƃ}�b�v
		Microsoft::WRL::ComPtr<ID3D12Resource> margingCubesSpaceTransform;
		ConstBufferMargingCubesSpace* constMapMargingCubesSpace;

		//�萔�o�b�t�@�ƃ}�b�v
		Microsoft::WRL::ComPtr<ID3D12Resource> numSpheresTransform;
		ConstBufferNumSpheres* constMapNumSpheres;

		Texture::Tex* tex;
		const short texNum = 1023;

	public:
		void GetCamera();
		void Initialize()/*override*/;
		void Update();
		~Metaball();
		void Draw();
		void DrawBefore();
		void DrawAfter();

	private:
		void UpdateMargingCubesSpace();
		void UpdateNumSpheres();
		void TransferConstBuffer();
		void DataTransferGPU();
	};
}
