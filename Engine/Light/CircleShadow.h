#pragma once
#include "IFMath.h"

namespace IF
{
	class CShadow
	{
	public:
		struct ConstCShadowData
		{
			Vector3 shadowv;
			float pad;
			Float3 casterPos;
			float distanceCasterLight;
			Float3 shadowAtten;
			float pad3;
			Float2 shadowFactorAngleCos;
			unsigned int active = false;
			float pad4;
		};

	private:
		Vector3 shadowDir = { 1,0,0 };
		float distanceCasterLight = 100.0f;
		Float3 casterPos = { 0,0,0 };
		Float3 shadowAtten = { 0.5f,0.6f,0.0f };
		Float2 shadowFactorAngleCos = { 0.2f,0.5f };
		bool active = false;

	public:
		inline void SetShadowDir(const Vector3& shadowDir)
		{
			this->shadowDir = shadowDir;
			this->shadowDir = Vector3Normalize(this->shadowDir);
		}
		inline const Vector3& GetShadowDir() { return shadowDir; }
		inline void SetCasterPos(const Float3& casterPos) { this->casterPos = casterPos; }
		inline const Float3& GetCasterPos() { return casterPos; }
		inline void SetDistanceCasterLight(float distanceCasterLight) { this->distanceCasterLight = distanceCasterLight; }
		inline const float GetDistanceCasterLight() { return distanceCasterLight; }
		inline void SetShadowAtten(const Float3& shadowAtten) { this->shadowAtten = shadowAtten; }
		inline const Float3& GetShadowAtten() { return shadowAtten; }
		inline void SetShadowFactorAngle(const Float2& shadowAngle)
		{
			this->shadowFactorAngleCos.x = cosf(ConvertToRadians(shadowAngle.x));
			this->shadowFactorAngleCos.y = cosf(ConvertToRadians(shadowAngle.y));
		}
		inline const Float2& GetShadowFactorAngle() { return shadowFactorAngleCos; }
		inline void SetActive(bool active) { this->active = active; }
		inline bool IsActive() { return active; }
	};
}