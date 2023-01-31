#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"

namespace IF
{
	class RayCollider : public BaseCollider, public Ray
	{
	public:
		RayCollider(Vector3 offset = { 0,0,0 }, Vector3 dir = { 0,-1,0 }) :
			offset(offset), dir(dir)
		{
			// 球形状をセット
			shapeType = COLLISIONSHAPE_RAY;
		}

		/// <summary>
		/// 更新
		/// </summary>
		void Update() override;

	private:
		Vector3 start;
		// オブジェクト中心からのオフセット
		Vector3 offset;
		Vector3 dir;
	};
}
