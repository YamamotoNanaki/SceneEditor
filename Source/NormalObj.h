#pragma once
#include "ComponentObj.h"
namespace IF
{
	class UsuallyObj : public CObject
	{
	private:
		Object obj{};
		unsigned short texNum = 0;
		Matrix* matView = nullptr, * matProjection = nullptr;
		Float3* cameraPos = nullptr;
		BillBoard::BillBoardMode mode = BillBoard::NOON;
		static std::vector<D3D12_VIEWPORT>viewport;
	public:
		void MatInitialize(Matrix* matView, Matrix* matProjection, Float3* cameraPos, BillBoard::BillBoardMode mode = BillBoard::NOON)override;
		void Initialize(Model* model)override;
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
			UsuallyObj::viewport = viewport;
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
		inline ~UsuallyObj()override {};
		inline std::string GetModelTag()override
		{
			return obj.GetModelTag();
		}
	};
}
