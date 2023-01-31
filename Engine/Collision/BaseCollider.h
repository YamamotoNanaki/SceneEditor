#pragma once
#include "CollisionTypes.h"
#include "CollisionInfo.h"
#include "ComponentObj.h"

namespace IF
{
	/// <summary>
	/// コライダー基底クラス
	/// </summary>
	class BaseCollider
	{
	public:
		friend class CollisionManager;
		BaseCollider() = default;
		virtual ~BaseCollider() = default;

		inline void SetObject(CObject* object) {
			this->object = object;
		}

		inline CObject* GetObject3d() {
			return object;
		}

		virtual void Update() = 0;

		inline CollisionShapeType GetShapeType() { return shapeType; }

		/// <summary>
		/// 衝突時コールバック関数
		/// </summary>
		/// <param name="info">衝突情報</param>
		inline void OnCollision(const CollisionInfo& info) {
			object->OnCollision(info);
		}

		inline void SetAttribute(unsigned short attribute) {
			this->attribute = attribute;
		}

		inline void AddAttribute(unsigned short attribute) {
			this->attribute |= attribute;
		}

		inline void RemoveAttribute(unsigned short attribute) {
			this->attribute &= !attribute;
		}

	protected:
		CObject* object = nullptr;
		// 形状タイプ
		CollisionShapeType shapeType = SHAPE_UNKNOWN;

		unsigned short attribute = 0b1111111111111111;
	};
}

