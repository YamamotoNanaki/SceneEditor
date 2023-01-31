#pragma once
#include "IFMath.h"


namespace IF
{
	class CObject;
	class BaseCollider;
	/// <summary>
	/// 衝突情報
	/// </summary>
	struct CollisionInfo
	{
	public:
		CollisionInfo(CObject* object, BaseCollider* collider, const Vector3& inter, float* distance = nullptr)
		{
			this->object = object;
			this->collider = collider;
			this->inter = inter;
			if (distance)this->distance = *distance;
		}

		// 衝突相手のオブジェクト
		CObject* object = nullptr;
		// 衝突相手のコライダー
		BaseCollider* collider = nullptr;
		// 衝突点
		Vector3 inter;
		float distance;
	};
}
