#pragma once
#include "View.h"
#include "Projection.h"
#include "ICamera.h"

namespace IF
{
	class Camera :ICamera
	{
	private:
		//ƒJƒƒ‰
		View* matView = nullptr;
		Projection* matPro = nullptr;

	public:
		inline Camera(float winWidth, float winHeight)
		{
			matPro = new Projection(45.0f, winWidth, winHeight);
			matView = new View;
			matView->eye = { 1,1,-5.0f };
			matView->Update();
		}
		inline void ViewUpdate()
		{
			matView->Update();
		}
		inline void SetEye(Float3 eye)
		{
			matView->eye = eye;
		}
		inline void SetTarget(Float3 target)
		{
			matView->target = target;
		}
		inline void SetUp(Float3 up)
		{
			matView->up = up;
		}
		inline void SetFarZ(float farZ)
		{
			matPro->farZ = farZ;
		}
		inline void SetAngle(float fovAngle)
		{
			matPro->fovAngle = fovAngle;
		}
		inline void SetNearZ(float nearZ)
		{
			matPro->nearZ = nearZ;
		}
		inline void SetHeight(float winHeight)
		{
			matPro->winHeight = winHeight;
		}
		inline void SetWidth(float winWidth)
		{
			matPro->winWidth = winWidth;
		}
		inline ~Camera()
		{
			delete matView;
			delete matPro;
		}
	};
}
