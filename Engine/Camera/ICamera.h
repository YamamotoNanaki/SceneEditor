#pragma once

namespace IF
{
	class ICamera
	{
	public:
		virtual void ViewUpdate() = 0;
		virtual void SetEye(Float3 eye) = 0;
		virtual void SetTarget(Float3 target) = 0;
		virtual void SetUp(Float3 up) = 0;
		virtual void SetFarZ(float farZ) = 0;
		virtual void SetAngle(float fovAngle) = 0;
		virtual void SetNearZ(float nearZ) = 0;
		virtual void SetHeight(float winHeight) = 0;
		virtual void SetWidth(float winWidth) = 0;
		virtual ~ICamera() {}
	};
}
