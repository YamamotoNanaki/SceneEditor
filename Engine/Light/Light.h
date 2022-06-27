#pragma once
#include <wrl.h>
#include <d3dx12.h>
#include "IFMath.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CircleShadow.h"

namespace IF
{
	class LightManager
	{
		template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

	private:
		static const int PLightNum = 3;
		static const int DLightNum = 3;
		static const int SLightNum = 3;
		static const int CShadowNum = 1;

	public:
		struct ConstBufferData
		{
			Float3 ambientColor;
			float pad1;
			DLight::ConstDLightData dLights[DLightNum];
			PLight::ConstPLightData pLights[PLightNum];
			SLight::ConstSLightData sLights[PLightNum];
			CShadow::ConstCShadowData cShadows[CShadowNum];
		};

	private:
		static ComPtr < ID3D12Device> device;
		static ComPtr < ID3D12GraphicsCommandList> commandList;
		ComPtr<ID3D12Resource> constBuff;
		Float3 ambientColor = { 1,1,1 };
		DLight dLight[DLightNum];
		PLight pLight[PLightNum];
		SLight sLight[SLightNum];
		CShadow cShadow[CShadowNum];
		bool dirty = false;
		LightManager() {};
		LightManager(const LightManager&) {};
		LightManager& operator=(const LightManager&) {};
		inline ~LightManager() {}

	public:
		static void SetDeviceCommand(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
		void Initialize();
		void TransferConstBuffer();
		void DefaultLightSetting();
		void SetAmbientColor(const Float3& color);
		void SetDirLightActive(int index, bool active);
		void SetDirLightDir(int index, const Vector3& lightdir);
		void SetDirLightColor(int index, const Float3& lightcolor);
		void SetPointLightActive(int index, bool active);
		void SetPointLightPos(int index, const Float3& lightpos);
		void SetPointLightColor(int index, const Float3& lightcolor);
		void SetPointLightAtten(int index, const Float3& lightAtten);
		void SetSpotLightActive(int index, bool active);
		void SetSpotLightDir(int index, const Vector3& lightdir);
		void SetSpotLightPos(int index, const Float3& lightpos);
		void SetSpotLightColor(int index, const Float3& lightcolor);
		void SetSpotLightAtten(int index, const Float3& lightAtten);
		void SetSpotLightFactorAngle(int index, const Float2& lightFactorAngle);
		void SetCircleShadowActive(int index, bool active);
		void SetCircleShadowDir(int index, const Vector3& shadowdir);
		void SetCircleShadowCasterPos(int index, const Float3& shadowpos);
		void SetCircleShadowDistanceCasterLight(int index, float distanceCasterLight);
		void SetCircleShadowAtten(int index, const Float3& shadowAtten);
		void SetCircleShadowFactorAngle(int index, const Float2& shadowFactorAngle);
		void Update();
		void Draw(UINT rootParameterIndex);
		static LightManager* Instance();
		static void DeleteInstance();
		void UnMap();
	};
}