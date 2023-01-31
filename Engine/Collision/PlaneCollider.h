#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"

namespace IF
{
	class PlaneCollider : public BaseCollider, public Plane
	{
	public:
		PlaneCollider(float distance = 1.0f) :
			distance(distance)
		{
			// 球形状をセット
			shapeType = COLLISIONSHAPE_PLANE;
		}

		/// <summary>
		/// 更新
		/// </summary>
		void Update() override;

		inline void SetDistance(float distance) { this->distance = distance; }

	private:
		// 半径
		float distance;
	};
}
