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

		uint16_t numMarchingSegments = 30;  // セルの分割数
		float margingSpaceSize = 64;     // マーチングキューブのスペースのサイズ
		uint16_t numSpheres = 6;            // メタボールの数
		uint16_t smoothUnionValue = 6;            // メタボールの結合の度合い
		Float3 sphereColor = { 255, 0, 0 };  // メタボールの色
		PV vi;
		ConstBuff cb;
		ICamera* camera;
		//定数バッファとマップ
		Microsoft::WRL::ComPtr<ID3D12Resource> matrixTransform;
		ConstBufferMatrix* constMapMatrix;

		//定数バッファとマップ
		Microsoft::WRL::ComPtr<ID3D12Resource> margingCubesSpaceTransform;
		ConstBufferMargingCubesSpace* constMapMargingCubesSpace;

		//定数バッファとマップ
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
