#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"

namespace IF
{
	class PlaneCollider : public BaseCollider, public Plane
	{
	public:
		PlaneCollider(Vector3 offset = { 0,0,0 }, float distance = 1.0f) :
			offset(offset),
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
		// オブジェクト中心からのオフセット
		Vector3 offset;
		// 半径
		float distance;
	};
}
