#pragma once
#include "ICamera.h"
#include "Camera.h"
#include "nlohmann/json.hpp"
#include <list>
#include <vector>

namespace IF
{
	class CameraManager
	{
	private:
		std::list<ICamera*>cameraList;

	public:
		template <class T> inline void Add(std::string tag, float fovAngle, float winWidth, float winHeight)
		{
			for (auto com : cameraList)
			{
				if (com->tag == tag)return;
			}
			T* camera = new T;
			camera->Initialize(fovAngle, winWidth, winHeight);
			camera->SetTag(tag);
			cameraList.push_back(camera);
		}
		inline ICamera* GetCamera(std::string tag)
		{
			for (auto com : cameraList)
			{
				if (com->tag == tag)
				{
					return com;
				}
			}
		}
		inline void Update(std::string tag)
		{
			for (auto com : cameraList)
			{
				if (com->tag == tag)
				{
					com->Update();
					return;
				}
			}
		}
		inline void Update()
		{
			for (auto com : cameraList)
			{
				com->Update();
			}
		}
		inline void SetEye(Float3 eye, std::string tag)
		{
			for (auto com : cameraList)
			{
				if (com->tag == tag)
				{
					com->SetEye(eye);
					return;
				}
			}
		}
		inline void SetTarget(Float3 target, std::string tag)
		{
			for (auto com : cameraList)
			{
				if (com->tag == tag)
				{
					com->SetTarget(target);
					return;
				}
			}
		}
		inline void SetUp(Float3 up, std::string tag)
		{
			for (auto com : cameraList)
			{
				if (com->tag == tag)
				{
					com->SetUp(up);
					return;
				}
			}
		}
		inline void SetFarZ(float farZ, std::string tag)
		{
			for (auto com : cameraList)
			{
				if (com->tag == tag)
				{
					com->SetFarZ(farZ);
					return;
				}
			}
		}
		inline void SetAngle(float fovAngle, std::string tag)
		{
			for (auto com : cameraList)
			{
				if (com->tag == tag)
				{
					com->SetAngle(fovAngle);
					return;
				}
			}
		}
		inline void SetNearZ(float nearZ, std::string tag)
		{
			for (auto com : cameraList)
			{
				if (com->tag == tag)
				{
					com->SetNearZ(nearZ);
					return;
				}
			}
		}
		inline void SetHeight(float winHeight, std::string tag)
		{
			for (auto com : cameraList)
			{
				if (com->tag == tag)
				{
					com->SetHeight(winHeight);
					return;
				}
			}
		}
		inline void SetWidth(float winWidth, std::string tag)
		{
			for (auto com : cameraList)
			{
				if (com->tag == tag)
				{
					com->SetWidth(winWidth);
					return;
				}
			}
		}
	public:
		void OutputJson(nlohmann::json& j);
		inline void Reset()
		{
			cameraList.clear();
		}

//#ifdef _DEBUG
		void GUI();
//#endif
	};
}
