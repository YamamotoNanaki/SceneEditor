#pragma once
#include "ComponentObj.h"
namespace IF
{
	class Normal : public CObject
	{
		//���K�����
	private:
		static const std::string objName;
	public:
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
