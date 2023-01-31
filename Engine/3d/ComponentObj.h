#pragma once
#include "Object.h"
#include "CollisionPrimitive.h"
#include "Model.h"
#include "ICamera.h"
#include "Debug.h"
#include "ParticleManager.h"
#include "CollisionInfo.h"

namespace IF
{
	class BaseCollider;
	class CObject
	{
	public:
		std::string tag;
		bool deleteFlag = false;
		ICamera* cameraPtr = nullptr;
		bool particleFlag = false;
		Emitter* emitter = nullptr;
		bool prefab = false;
	protected:
		Object obj{};
		unsigned short texNum = 0;
		Matrix* matView = nullptr, * matProjection = nullptr;
		Float3* cameraPos = nullptr;
		int mode = BillBoard::NOON;
		BaseCollider* collider = nullptr;
	public:
		virtual void Initialize(Model* model, bool prefab);
		virtual void FBXInitialize(FBXModel* model, bool prefab);
		virtual void Update();
		virtual void Draw();
		virtual void OutLineDraw();
		virtual ~CObject();
		virtual void DebugUpdate();
		void SetCollider(BaseCollider* collider);
		virtual void OnCollision(const CollisionInfo& info) {}

	public:
		virtual void ClassUpdate();
		Matrix GetMatWorld();
		virtual void ClassInitialize();
		virtual bool WeightSaving(float max = 700);
		bool WeightSavingXYZ(float maxX, float maxY, float maxZ);
		inline CObject* GetAdrres() { return this; }
		inline void MatUpdate()
		{
			obj.Update(*matView, *matProjection, *cameraPos, mode);
		}
		inline void UpdateWorldMatrix()
		{
			obj.UpdateWorldMatrix(mode);
		}
		void MatInitialize(Matrix* matView, Matrix* matProjection, Float3* cameraPos, int mode = BillBoard::NOON);
		virtual const std::string GetObjName() = 0;
		inline void SetAlpha(float a)
		{
			obj.SetAlpha(a);
		}
		bool DeleteObj();
		inline void SetToon(bool toon)
		{
			obj.toonFlag = toon;
		}
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
		inline std::string GetModelTag()
		{
			return obj.GetModelTag();
		}
		inline Float4 GetColor()
		{
			return obj.GetColor();
		}
		virtual void ClassInputJson(nlohmann::json& j);
		void InputJson(nlohmann::json& j);
		inline void SetFBXModel(FBXModel* fbxModel)
		{
			obj.fmodel = fbxModel;
		}
//#ifdef _DEBUG
		void OutputJson(nlohmann::json& j);
		bool flag = false;
		inline void SetFlag(bool flag)
		{
			this->flag = flag;
		}
		virtual void GUI();
		virtual void ClassUI();
		virtual void ClassOutputJson(nlohmann::json& j);
//#endif
	};
}
