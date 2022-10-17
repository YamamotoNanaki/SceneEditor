#pragma once
#include "View.h"
#include "Projection.h"
#include "ICamera.h"
#include "IFMath.h"
#include "Debug.h"

namespace IF
{
	class DebugCamera : public ICamera
	{
		static const std::string cameraName;
		//ƒJƒƒ‰
		float distance = 20;
		float scaleX = 1.0f / (float)1280;
		float scaleY = 1.0f / (float)720;
		Matrix matRot;

	public:
		DebugCamera() {}
		inline void Initialize(float fovAngle, float winWidth, float winHeight)
		{
			matPro = DEBUG_NEW Projection(fovAngle, winWidth, winHeight);
			matView = DEBUG_NEW View;
			matView->eye = { 1,1,-5.0f };
			matView->Update();
		}
		void Update();
		inline ~DebugCamera() {}
#ifdef _DEBUG
		void GUI();
#endif
	};
}