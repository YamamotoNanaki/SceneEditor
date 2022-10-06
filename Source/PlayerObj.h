#pragma once
#include "ComponentObj.h"
#include "Debug.h"
#include "CollisionPrimitive.h"

namespace IF
{
	class PlayerObj : public CObject
	{
	private:
		Object obj{};
		unsigned short texNum = 0;
		Matrix* matView = nullptr, * matProjection = nullptr;
		Float3* cameraPos = nullptr;
		BillBoard::BillBoardMode mode = BillBoard::NOON;
		static std::vector<D3D12_VIEWPORT>viewport;
		Primitive* colision;
		unsigned short ptype;
	public:
		inline void SetCollision(unsigned short type)
		{
			if (type == RayPri)
			{
				colision = DEBUG_NEW Ray;
				ptype = RayPri;
			}
			else if (type == PlanePri)
			{
				colision = DEBUG_NEW Plane;
				ptype = PlanePri;
			}
			else if (type == SpherePri)
			{
				colision = DEBUG_NEW Sphere;
				ptype = SpherePri;
			}
			else
			{
				if (colision != nullptr)delete colision;
				ptype = NotPri;
			}
		}
		inline unsigned short GetCollision()
		{
			return ptype;
		}
		inline Primitive* GetPrimitive()
		{
			return colision;
		}
		void MatInitialize(Matrix* matView, Matrix* matProjection, Float3* cameraPos, BillBoard::BillBoardMode mode = BillBoard::NOON)override;
		//void Initialize(Model* model)override;
		void Update()override;
		void Draw()override;
		inline void SetView(Matrix* matView)override
		{
			this->matView = matView;
		}
		inline void SetProjection(Matrix* matProjection)override
		{
			this->matProjection = matProjection;
		}
		inline void SetCamera(Float3* cameraPos)override
		{
			this->cameraPos = cameraPos;
		}
		inline void SetBillBoard(BillBoard::BillBoardMode mode)override
		{
			this->mode = mode;
		}
		inline BillBoard::BillBoardMode GetBillBoard()override
		{
			return mode;
		}
		inline void SetViewport(std::vector<D3D12_VIEWPORT>viewport)override
		{
			PlayerObj::viewport = viewport;
		}
		inline void SetTexture(unsigned short texNum)override
		{
			this->texNum = texNum;
		}
		inline Float3 GetPos()override
		{
			return obj.position;
		}
		inline void SetPos(Float3 pos)
		{
			this->obj.position = pos;
		}
		inline Float3 GetRota()override
		{
			return obj.rotation;
		}
		inline void SetRota(Float3 rotation)
		{
			this->obj.rotation = rotation;
		}
		inline Float3 GetScale()override
		{
			return obj.scale;
		}
		inline void SetScale(Float3 scale)
		{
			this->obj.scale = scale;
		}
		inline ~PlayerObj()override {};

		inline std::string GetModelTag()override
		{
			return obj.GetModelTag();
		}
	};
}
