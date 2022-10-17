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
		float rota = 90;
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
		inline float& GetRota()
		{
			return rota;
		}
		inline void SetRota(float rota)
		{
			this->rota = rota;
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
		virtual ~ICamera()
		{
			delete matView;
			delete matPro;
		}
#ifdef _DEBUG
		virtual void GUI() = 0;
#endif
	};
}
