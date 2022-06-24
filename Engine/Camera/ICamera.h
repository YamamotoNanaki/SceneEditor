#pragma once
#include <string>
#include <IFMath.h>

namespace IF
{
	class ICamera
	{
	public:
		std::string tag;
	public:
		virtual void Initialize(float fovAngle, float winWidth, float winHeight) = 0;
		virtual void Update() = 0;
		virtual void SetTag(std::string tag) = 0;
		virtual void SetEye(Float3 eye) = 0;
		virtual void SetTarget(Float3 target) = 0;
		virtual void SetUp(Float3 up) = 0;
		virtual void SetFarZ(float farZ) = 0;
		virtual void SetAngle(float fovAngle) = 0;
		virtual void SetNearZ(float nearZ) = 0;
		virtual void SetHeight(float winHeight) = 0;
		virtual void SetWidth(float winWidth) = 0;
		virtual Float3* GetEye() = 0;
		virtual Float3& GetTarget() = 0;
		virtual Float3& GetUp() = 0;
		virtual Matrix* GetMatView() = 0;
		virtual Matrix* GetMatPro() = 0;
		virtual ~ICamera() {}
	};
}
