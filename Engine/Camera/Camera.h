#pragma once
#include "ICamera.h"
#include "Debug.h"

namespace IF
{
	class Camera :public ICamera
	{
	private:
		static const std::string cameraName;

		Vector3 spos;
		Vector3 star;
		Vector3 supv;
		bool once = false;
	public:
		Camera() {}
		inline void Initialize(float fovAngle, float winWidth, float winHeight)
		{
			matPro = DEBUG_NEW Projection(fovAngle, winWidth, winHeight);
			matView = DEBUG_NEW View;
			matView->target = { 0,2,0 };
			matView->eye = { 0,1,-10 };
			matView->Update();
			cpos = SetVector3(matView->eye);
			ctar = SetVector3(matView->target);
			cupv = SetVector3(matView->up);

		}
		void Update();
		inline ~Camera() {}
		inline std::string GetName()
		{
			return cameraName;
		}

	};
}
