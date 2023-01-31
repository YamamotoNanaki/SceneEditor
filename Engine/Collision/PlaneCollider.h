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
			// ���`����Z�b�g
			shapeType = COLLISIONSHAPE_PLANE;
		}

		/// <summary>
		/// �X�V
		/// </summary>
		void Update() override;

		inline void SetDistance(float distance) { this->distance = distance; }

	private:
		// ���a
		float distance;
	};
}
