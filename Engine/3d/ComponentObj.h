#pragma once
#include "Object.h"
#include "CollisionPrimitive.h"
#include "Model.h"
#include "ICamera.h"
#include "Debug.h"
#include "ParticleManager.h"

namespace IF
{
	class CObject
	{
	public:
		std::string tag;
		bool deleteFlag = false;
		ICamera* cameraPtr = nullptr;
		bool particleFlag = false;
		Emitter* emitter = nullptr;
	protected:
		Object obj{};
		unsigned short texNum = 0;
		Matrix* matView = nullptr, * matProjection = nullptr;
		Float3* cameraPos = nullptr;
		int mode = BillBoard::NOON;
		static std::vector<D3D12_VIEWPORT>viewport;
		unsigned short ptype;
		bool prefab = false;
		Primitive* collision = nullptr;

		//!↓ワンボタン用で作ったやつ。後で消しとけ！
		bool isActive = true;
		//!↑ワンボタン用で作ったやつ。後で消しとけ！
	public:
		virtual void Initialize(Model* model, bool prefab) = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;
		virtual ~CObject() = 0;
		virtual void DebugUpdate();

	public:
		virtual void CollisionUpdate();
		virtual bool WeightSaving(float max = 700);
		//!↓ワンボタン用で作ったやつ。後で消しとけ！
		inline bool IsActive() { return isActive; }
		virtual void SetActive(bool active);
		//!↑ワンボタン用で作ったやつ。後で消しとけ！
		inline CObject* GetAdrres() { return this; }
		inline void MatUpdate()
		{
			obj.Update(*matView, *matProjection, *cameraPos, mode);
		}
		void MatInitialize(Matrix* matView, Matrix* matProjection, Float3* cameraPos, int mode = BillBoard::NOON);
		virtual const std::string GetObjName() = 0;
		inline void SetAlpha(float a)
		{
			obj.SetAlpha(a);
		}
		bool DeleteObj();
		inline void SetColor(Float4 color)
		{
			obj.SetColorF(color.x, color.y, color.z, color.w);
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
			if (collision != nullptr)
			{
				delete collision;
				collision = nullptr;
			}
			if (type == RayPri)
			{
				collision = DEBUG_NEW Ray;
				ptype = RayPri;
			}
			else if (type == PlanePri)
			{
				collision = DEBUG_NEW Plane;
				ptype = PlanePri;
			}
			else if (type == SpherePri)
			{
				collision = DEBUG_NEW Sphere;
				ptype = SpherePri;
			}
			else if (type == BoxPri)
			{
				collision = DEBUG_NEW Box;
				ptype = BoxPri;
			}
			else if (type == CircleXYPri)
			{
				collision = DEBUG_NEW CircleXY;
				ptype = CircleXYPri;
			}
			else
			{
				ptype = NotPri;
			}
		}
		inline unsigned short GetCollision()
		{
			return ptype;
		}
		inline Primitive* GetPrimitive()
		{
			return collision;
		}
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
		inline void SetBillBoard(int mode)
		{
			this->mode = mode;
		}
		inline int GetBillBoard()
		{
			return mode;
		}
		inline void SetViewport(std::vector<D3D12_VIEWPORT>viewport)
		{
			CObject::viewport = viewport;
		}
		inline void SetTexture(unsigned short texNum)
		{
			this->texNum = texNum;
		}
		inline Float3 GetPos()
		{
			return obj.position;
		}
		inline void SetPos(Float3 pos)
		{
			this->obj.position = pos;
		}
		inline Float3 GetRota()
		{
			return obj.rotation;
		}
		inline void SetRota(Float3 rotation)
		{
			this->obj.rotation = rotation;
		}
		inline Float3 GetScale()
		{
			return obj.scale;
		}
		inline void SetScale(Float3 scale)
		{
			this->obj.scale = scale;
		}
		inline void CObjDelete()
		{
			if (collision != nullptr)delete collision;
		};
		inline std::string GetModelTag()
		{
			return obj.GetModelTag();
		}
		inline Float4 GetColor()
		{
			return obj.GetColor();
		}
#ifdef _DEBUG
		bool flag = false;
		inline void SetFlag(bool flag)
		{
			this->flag = flag;
		}
		virtual void GUI();
#endif
	};
}
