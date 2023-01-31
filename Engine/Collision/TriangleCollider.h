#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"

namespace IF
{
	class TriangleCollider : public BaseCollider, public Triangle
	{
	public:
		TriangleCollider(Vector3 p0, Vector3 p1, Vector3 p2,Vector3 offset = { 0,0,0 }) :
			offset(offset)
		{
			// ���`����Z�b�g
			shapeType = COLLISIONSHAPE_TRIANGLE;
			this->p0 = p0;
			this->p1 = p1;
			this->p2 = p2;
		}

		/// <summary>
		/// �X�V
		/// </summary>
		void Update() override;

	private:
		Vector3 objPos;
		// �I�u�W�F�N�g���S����̃I�t�Z�b�g
		Vector3 offset;
		Vector3 p0;
		Vector3 p1;
		Vector3 p2;
	};
}
