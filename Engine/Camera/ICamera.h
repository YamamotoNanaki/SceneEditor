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
		virtual Vector3& GetFront() = 0;
		virtual Float3& GetSpeed() = 0;
		virtual Matrix* GetMatView() = 0;
		virtual Matrix* GetMatPro() = 0;
		virtual float& GetRota() = 0;
		virtual short GetType() = 0;
		virtual void SetType(short type) = 0;
		virtual ~ICamera() {}
#ifdef _DEBUG
		virtual void GUI() = 0;
#endif
	};
}
