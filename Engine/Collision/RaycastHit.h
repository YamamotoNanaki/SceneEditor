#pragma once
#include "BaseCollider.h"


namespace IF
{
	class CObject;
	struct RaycastHit
	{
		// 衝突相手のオブジェクト
		CObject* object = nullptr;
		// 衝突相手のコライダー
		BaseCollider* collider = nullptr;

		// 衝突相手のオブジェクト
		std::list<CObject*> allobject;
		// 衝突相手のコライダー
		std::forward_list<BaseCollider*> allcollider;
		// 衝突点
		Vector3 inter;
		// 衝突点までの距離
		float distance = 0.0f;
	};
}
