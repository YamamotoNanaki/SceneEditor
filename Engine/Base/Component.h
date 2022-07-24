#pragma once
#include <string>

namespace IF
{
	class Component
	{
		virtual void Start() = 0;
		virtual void Update() = 0;
		template<class T> T Get(std::string tag) = 0;

#ifdef _DEBUG
		virtual void GUI() = 0;
#endif
	};
}
