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
			// ���`����Z�b�g
			shapeType = COLLISIONSHAPE_PLANE;
		}

		/// <summary>
		/// �X�V
		/// </summary>
		void Update() override;

		inline void SetDistance(float distance) { this->distance = distance; }

	private:
		// �I�u�W�F�N�g���S����̃I�t�Z�b�g
		Vector3 offset;
		// ���a
		float distance;
	};
}
