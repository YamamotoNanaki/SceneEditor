#pragma once
#include <d3d12.h>
#include <forward_list>
#include "CollisionPrimitive.h"
#include "RaycastHit.h"
#include "QueryCallback.h"

namespace IF
{
	class BaseCollider;
	class CollisionManager
	{
	public:
		static CollisionManager* Instance();

	public:
		inline void AddCollider(BaseCollider* collider)
		{
			colliders.push_front(collider);
		}

		inline void RemoveCollider(BaseCollider*collider)
		{
			colliders.remove(collider);
		}

		void CheckAllCollisions();

		bool Raycast(const Ray& ray, RaycastHit* hitInfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);

		bool Raycast(const Ray& ray, unsigned short attribute, RaycastHit* hitInfo = nullptr, float maxDistance = D3D12_FLOAT32_MAX);

		void QuerySphere(const Sphere& sphere, QueryCallback* callback, unsigned short attribute = (unsigned short)0xffffffff);

	private:
		CollisionManager() = default;
		CollisionManager(const CollisionManager&) = delete;
		~CollisionManager() = default;
		CollisionManager& operator=(const CollisionManager&) = delete;

		std::forward_list<BaseCollider*>colliders;
	};
}
