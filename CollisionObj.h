#pragma once
#include "TouchableObject.h"

namespace IF
{
	class CollisionObj : public TouchableObject
	{
	private:
		static const std::string objName;
		const std::string GetObjName()override
		{
			return objName;
		}
		//↑必ず作る

	public:
		//クラスで定義したいものを書く(書いたら勝手に呼ばれます)
		void ClassInitialize()override;
		void ClassUpdate()override;
	};
}
