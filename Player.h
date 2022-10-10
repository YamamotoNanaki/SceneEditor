#pragma once
#include "ComponentObj.h"
#include "CollisionPrimitive.h"
#include "Debug.h"
#include <string>
namespace IF
{
	class Player : public CObject
	{
	private:
		static const std::string objName;
		Object obj{};
		unsigned short texNum = 0;
		Matrix* matView = nullptr, * matProjection = nullptr;
		Float3* cameraPos = nullptr;
		BillBoard::BillBoardMode mode = BillBoard::NOON;
		static std::vector<D3D12_VIEWPORT>viewport;
		Primitive* colision;
		unsigned short ptype;
		bool prefab = false;
		bool deleteFlag = false;
	public:
		//アップデート関数
		void GameUpdate();
		void SetCollision();
		void MatrixUpdate();
		//当たった時の処理
		void IsEnemyHitNotWeak();
		void IsEnemyHitWakPoint();
		void CollisionObject();



		inline void SetAlpha(float a)
		{
			obj.SetAlpha(a);
		}
		const std::string GetObjName()
		{
			return objName;
		}
		static const std::string GetObjectName() { return objName; };
		inline void SetColor(Float4 color)override
		{
			obj.SetColorF(color.x, color.y, color.z, color.w);
		}
		void DeleteObj();
		inline void SetColor(int r, int g, int b, int a)
		{
			obj.SetColor(r, g, b, a);
		}
		inline void SetAi(unsigned short ai)
		{
			//this->ai = ai;
		}
		inline bool GetPrefab()
		{
			return prefab;
		}
		inline Model* GetModelAddress()
		{
			return obj.GetModel();
		}
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
				colision = DEBUG_NEW Ray;
				delete colision;
				colision = nullptr;
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
		void Initialize(Model* model, bool prefab)override;
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
			Player::viewport = viewport;
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
		inline ~Player()override
		{
			if (colision != nullptr)delete colision;
		};
		inline std::string GetModelTag()override
		{
			return obj.GetModelTag();
		}
		inline Float4 GetColor()override
		{
			return obj.GetColor();
		}
#ifdef _DEBUG
		bool flag = false;
		inline void SetFlag(bool flag)
		{
			this->flag = flag;
		}
		void GUI();
		inline unsigned short GetAi()
		{
			return 0;
		}
#endif
	};
}


