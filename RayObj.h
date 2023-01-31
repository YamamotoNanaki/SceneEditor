#pragma once
#include "ComponentObj.h"

namespace IF
{
	class RayObj : public CObject
	{
		//↓必ず作る
	private:
		static const std::string objName;

	public:
		const std::string GetObjName()override
		{
			return objName;
		}
		//↑必ず作る

	public:
		//クラスで定義したいものを書く(書いたら勝手に呼ばれます)
		void ClassInitialize()override;
		void ClassUpdate()override;
		void OnCollision(const CollisionInfo& info)override;
	};
}
