#pragma once
#include "Object.h"
#include "CollisionPrimitive.h"
#include "Model.h"

namespace IF
{
	class CObject
	{
	public:
		std::string tag;
		bool deleteFlag = false;
	public:
		virtual void Update() = 0;
		virtual void Draw() = 0;
		virtual void Initialize(Model* model, bool prefab) = 0;
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
		virtual void SetAlpha(float a) = 0;
		virtual bool GetPrefab() = 0;
		virtual Float3 GetPos() = 0;
		virtual Float3 GetRota() = 0;
		virtual Float3 GetScale() = 0;
		virtual ~CObject() {};
		virtual std::string GetModelTag() = 0;
		virtual Model* GetModelAddress() = 0;
		virtual BillBoard::BillBoardMode GetBillBoard() = 0;
		virtual void SetCollision(unsigned short type) = 0;
		virtual unsigned short GetCollision() = 0;
		virtual Primitive* GetPrimitive() = 0;
		virtual void SetAi(unsigned short ai) = 0;
		virtual void DeleteObj() = 0;
		virtual const std::string GetObjName() = 0;
		virtual Float4 GetColor() = 0;
		virtual void SetColor(Float4 color) = 0;
#ifdef _DEBUG
		virtual void SetFlag(bool flag) = 0;
		virtual void GUI() = 0;
		virtual unsigned short GetAi() = 0;
#endif
	};
}
