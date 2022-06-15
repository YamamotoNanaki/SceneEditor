#pragma once
#include "IFMath.h"

namespace IF
{
	class DLight
	{
	public:
		struct ConstDLightData
		{
			Vector3 lightv;
			float pad;
			Float3 lightColor;
			unsigned int active;
		};

	private:
		Vector3 lightDir = { 1,0,0 };
		Float3 lightColor = { 1,1,1 };
		bool active = false;

	public:
		inline void SetLightDir(const Vector3& lightDir) 
		{ 
			this->lightDir = lightDir;
			this->lightDir = Vector3Normalize(this->lightDir);
		}
		inline const Vector3& GetLightDir() { return lightDir; }
		inline void SetLightColor(const Float3& lightColor) { this->lightColor = lightColor; }
		inline const Float3& GetLightColor() { return lightColor; }
		inline void SetActive(bool active) { this->active = active; }
		inline bool IsActive() { return active; }
	};
}