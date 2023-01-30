#pragma once
#include "ComponentObj.h"

namespace IF
{
	class TouchableObject : public CObject
	{
	public:
		void Initialize(Model* model, bool prefab)override;
	};
}
