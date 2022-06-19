#pragma once
#include "ComponentObj.h"
namespace IF
{
	class NormalObj : public ComponentObj
	{
	private:
		Object obj;
		unsigned short texNum = 0;
		Matrix* matView, * matProjection;
		Float3* cameraPos;
		BillBoard::BillBoardMode mode = BillBoard::NOON;
		static std::vector<D3D12_VIEWPORT>viewport;
		std::string tag;
	public:
		void MatInitialize(Matrix* matView, Matrix* matProjection, Float3* cameraPos, BillBoard::BillBoardMode mode = BillBoard::NOON);
		void Initialize(Model* model);
		void Update();
		void Draw();
		inline void SetView(Matrix* matView)
		{
			this->matView = matView;
		}
		inline void SetProjection(Matrix* matProjection)
		{
			this->matProjection = matProjection;
		}
		inline void SetCamera(Float3* cameraPos)
		{
			this->cameraPos = cameraPos;
		}
		inline void SetBillBoard(BillBoard::BillBoardMode mode)
		{
			this->mode = mode;
		}
		inline void SetViewport(std::vector<D3D12_VIEWPORT>viewport)
		{
			NormalObj::viewport = viewport;
		}
		inline void SetTexture(unsigned short texNum)
		{
			this->texNum = texNum;
		}
		inline void SetTag(std::string tag)
		{
			this->tag = tag;
		}
	};
}
