#pragma once
#include "View.h"
#include "Projection.h"
#include "ICamera.h"
#include "IFMath.h"

namespace IF
{
	class DebugCamera : public ICamera
	{
		//ƒJƒƒ‰
		View* matView = nullptr;
		Projection* matPro = nullptr;
		float distance = 20;
		float scaleX = 1.0f / (float)1280;
		float scaleY = 1.0f / (float)720;
		Matrix matRot;

	public:
		DebugCamera() {}
		inline void Initialize(float fovAngle, float winWidth, float winHeight)
		{
			matPro = new Projection(fovAngle, winWidth, winHeight);
			matView = new View;
			matView->eye = { 1,1,-5.0f };
			matView->Update();
		}
		void Update();
		inline void SetTag(std::string tag)
		{
			this->tag = tag;
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
		inline Float3* GetEye()
		{
			return &matView->eye;
		}
		inline Float3& GetTarget()
		{
			return matView->target;
		}
		inline Float3& GetUp()
		{
			return matView->up;
		}
		inline Matrix* GetMatView()
		{
			return matView->GetAddressOf();
		}
		inline Matrix* GetMatPro()
		{
			return matPro->GetAddressOf();
		}
		inline ~DebugCamera()
		{
			delete matView;
			delete matPro;
		}
	};
}