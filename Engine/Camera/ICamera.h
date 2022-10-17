#pragma once
#include <string>
#include "IFMath.h"
#include "View.h"
#include "Projection.h"

namespace IF
{
	class ICamera
	{
	public:
		std::string tag;
		//ƒJƒƒ‰
		View* matView = nullptr;
		Projection* matPro = nullptr;
		float distance = 20;
		Vector3 cpos = {0, 0, -1};
		Vector3 ctar = {0, 0, 0};
		Vector3 cupv = {0, 1, 0};
	public:
		virtual void Initialize(float fovAngle, float winWidth, float winHeight) = 0;
		virtual void Update() = 0;
		inline void SetTag(std::string tag)
		{
			this->tag = tag;
		}
		inline void SetEye(Float3 eye)
		{
			matView->eye = eye;
			cpos = SetVector3(matView->eye);
		}
		inline void SetTarget(Float3 target)
		{
			matView->target = target;
			ctar = SetVector3(matView->target);
		}
		inline void SetUp(Float3 up)
		{
			matView->up = up;
			cupv = SetVector3(matView->up);
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
		virtual std::string GetName() = 0;
		virtual ~ICamera()
		{
			delete matView;
			delete matPro;
		}
#ifdef _DEBUG
		void GUI();
#endif
	};
}
