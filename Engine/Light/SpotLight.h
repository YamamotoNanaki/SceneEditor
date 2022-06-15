#pragma once
#include "IFMath.h"

namespace IF
{
	class SLight
	{
	public:
		struct ConstSLightData
		{
			Vector3 lightv;
			float pad;
			Float3 lightPos;
			float pad1;
			Float3 lightColor;
			float pad2;
			Float3 lightAtten;
			float pad3;
			Float2 lightFactorAngleCos;
			unsigned int active = false;
			float pad4;
		};

	private:
		Vector3 lightDir = { 1,0,0 };
		Float3 lightPos = { 0,0,0 };
		Float3 lightColor = { 1,1,1 };
		Float3 lightAtten = { 1.0f,1.0f,1.0f };
		Float2 lightFactorAngleCos = { 0.5f,0.2f };
		bool active = false;

	public:
		inline void SetLightDir(const Vector3& lightDir)
		{
			this->lightDir = lightDir;
			this->lightDir = Vector3Normalize(this->lightDir);
		}
		inline const Vector3& GetLightDir() { return lightDir; }
		inline void SetLightPos(const Float3& lightPos) { this->lightPos = lightPos; }
		inline const Float3& GetLightPos() { return lightPos; }
		inline void SetLightColor(const Float3& lightColor) { this->lightColor = lightColor; }
		inline const Float3& GetLightColor() { return lightColor; }
		inline void SetLightAtten(const Float3& lightAtten) { this->lightAtten = lightAtten; }
		inline const Float3& GetLightAtten() { return lightAtten; }
		inline void SetLightFactorAngle(const Float2& lightFactorAngle)
		{
			this->lightFactorAngleCos.x = cosf(ConvertToRadians(lightFactorAngle.x));
			this->lightFactorAngleCos.y = cosf(ConvertToRadians(lightFactorAngle.y));
		}
		inline const Float2& GetLightFactorAngle() { return lightFactorAngleCos; }
		inline void SetActive(bool active) { this->active = active; }
		inline bool IsActive() { return active; }
	};
}