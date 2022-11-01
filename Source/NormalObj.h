#pragma once
#include "ComponentObj.h"
namespace IF
{
	class Normal : public CObject
	{
	private:
		static const std::string objName;

	public:
		const std::string GetObjName()override
		{
			return objName;
		}
	};
}
