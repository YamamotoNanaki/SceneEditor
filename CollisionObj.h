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
		//���K�����

	public:
		//�N���X�Œ�`���������̂�����(�������珟��ɌĂ΂�܂�)
		void ClassInitialize()override;
		void ClassUpdate()override;
	};
}
