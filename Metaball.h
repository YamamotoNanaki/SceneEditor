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
		Vector3 scale = { 1,1,1 };
		Vector3 rota;

		uint16_t numMarchingSegments = 16;  // セルの分割数
		float margingSpaceSize = 32;     // マーチングキューブのスペースのサイズ
		uint16_t numSpheres = 3;            // メタボールの数
		uint16_t smoothUnionValue = 6;            // メタボールの結合の度合い
		Float3 sphereColor = { 255, 0, 0 };  // メタボールの色
		MV vi;
		ICamera* camera;
		//定数バッファとマップ
		Microsoft::WRL::ComPtr<ID3D12Resource> matrixTransform;
		ConstBufferBillboard* constMapMatrix;

		//定数バッファとマップ
		Microsoft::WRL::ComPtr<ID3D12Resource> margingCubesSpaceTransform;
		ConstBufferMargingCubesSpace* constMapMargingCubesSpace;

		//定数バッファとマップ
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
