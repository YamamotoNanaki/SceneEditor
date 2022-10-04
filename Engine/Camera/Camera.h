#pragma once
#include "View.h"
#include "Projection.h"
#include "ICamera.h"
#include "Timer.h"

namespace IF
{
	class Camera :public ICamera
	{
	private:
		//ƒJƒƒ‰
		View* matView = nullptr;
		Projection* matPro = nullptr;
		Vector3 frontvec{};
		Float3 speed{};
		float rota = 90;
		float fspeed = 0;
		Timer timer;
		bool Tflag = true;
		short flag = -1;
		short cameratype = 0;

	public:
		inline short GetType()
		{
			return cameratype;
		}
		inline void SetType(short type)
		{
			cameratype = type;
		}
		inline float& GetRota()
		{
			return rota;
		}
		inline Vector3& GetFront()
		{
			return frontvec;
		}
		inline Float3& GetSpeed()
		{
			return speed;
		}
		Camera() {}
		inline void Initialize(float fovAngle, float winWidth, float winHeight)
		{
			matPro = new Projection(fovAngle, winWidth, winHeight);
			matView = new View;
			//matView->target = { 0,2,0 };
			//matView->eye = { 0,1,-10 };
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
		inline ~Camera()
		{
			delete matView;
			delete matPro;
		}

#ifdef _DEBUG
		void GUI();
#endif
	};
}
