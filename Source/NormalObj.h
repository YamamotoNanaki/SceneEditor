#pragma once
#include "ComponentObj.h"
namespace IF
{
	class Normal : public CObject
	{
	private:
		static const std::string objName;

	public:
		void Initialize(Model* model, bool prefab)override;
		void Update()override;
		void Draw()override;
		const std::string GetObjName()override
		{
			return objName;
		}
		inline ~Normal() {};
	};
}
