#pragma once
#include "ComponentObj.h"
namespace IF
{
	class Normal : public CObject
	{
	private:
		static const std::string objName;
		bool explosionF = false;
		bool polygonSizeF = false;

	public:
		bool moveF = false;
		const std::string GetObjName()override
		{
			return objName;
		}
		void ClassUpdate()override;
		void ClassInputJson(nlohmann::json& j)override;
#ifdef _DEBUG
		void ClassUI()override;
		void ClassOutputJson(nlohmann::json& j)override;
#endif
	};
}
