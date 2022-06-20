#pragma once
#include "ComponentObj.h"

namespace IF
{
	class PlayerObj : public ComponentObj
	{
	private:
		Object obj;
		unsigned short texNum = 0;
		Matrix* matView, * matProjection;
		Float3* cameraPos;
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
		inline ~PlayerObj()override {};
	};
}