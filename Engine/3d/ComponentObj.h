#pragma once
#include "Object.h"

namespace IF
{
	class CObject
	{
	public:
		std::string tag;
	public:
		virtual void Update() = 0;
		virtual void Draw() = 0;
		virtual void Initialize(Model* model) = 0;
		virtual void SetView(Matrix* matView) = 0;
		virtual void SetProjection(Matrix* matProjection) = 0;
		virtual void SetCamera(Float3* cameraPos) = 0;
		virtual void SetBillBoard(BillBoard::BillBoardMode mode) = 0;
		virtual void SetViewport(std::vector<D3D12_VIEWPORT>viewport) = 0;
		virtual void MatInitialize(Matrix* matView, Matrix* matProjection, Float3* cameraPos, BillBoard::BillBoardMode mode = BillBoard::NOON) = 0;
		virtual void SetTexture(unsigned short texNum) = 0;
		virtual void SetPos(Float3 pos) = 0;
		virtual void SetRota(Float3 pos) = 0;
		virtual void SetScale(Float3 pos) = 0;
		virtual Float3 GetPos() = 0;
		virtual Float3 GetRota() = 0;
		virtual Float3 GetScale() = 0;
		virtual ~CObject() {};
		virtual std::string GetModelTag() = 0;
		virtual BillBoard::BillBoardMode GetBillBoard() = 0;
	};
}
