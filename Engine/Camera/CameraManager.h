#pragma once
#include "Camera.h"
#include <list>

namespace IF
{
	class CameraManager
	{
	private:
		std::list<ICamera*>cameraList;
	};
}
