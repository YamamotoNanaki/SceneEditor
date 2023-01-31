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
			// ���`����Z�b�g
			shapeType = COLLISIONSHAPE_RAY;
		}

		/// <summary>
		/// �X�V
		/// </summary>
		void Update() override;

	private:
		Vector3 start;
		// �I�u�W�F�N�g���S����̃I�t�Z�b�g
		Vector3 offset;
		Vector3 dir;
	};
}
