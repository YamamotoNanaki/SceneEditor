#pragma once
#include "ComponentObj.h"
#include <list>

namespace IF
{
	class ObjectManager
	{
	public:
		std::list<ComponentObj*> objList;
	public:
		ObjectManager() {}
		~ObjectManager();
		void Draw();
		void Update();
		template <class T> inline void Add(Model* model, Matrix* matView, Matrix* matProjection, Float3* cameraPos, std::string tag, BillBoard::BillBoardMode mode = BillBoard::NOON)
		{
			T* obj = new T(this);
			obj.Initialize(model);
			obj.MatInitialize(matView, matProjection, cameraPos, mode);
			objList.push_back(obj);
		}
		template<class T> inline T* GetComponent()
		{
			for (auto com : objList) {
				T* buff = dynamic_cast<T*>(com);
				if (buff != nullptr)
					return buff;
			}
			return nullptr;
		}
		template<class T>inline void SetView(Matrix* matView, std::string tag = 0)
		{
			if (tag == 0)
			{
				for (auto com : objList)
				{
					com->SetView(matView);
				}
			}
			else
			{
				for (auto com : objList)
				{
					if (com.GetTag() == tag)
					{
						com->SetView(matView);
					}
				}
			}
		}
		template<class T>inline void SetProjection(Matrix* matProjection)
		{
			if (tag == 0)
			{
				for (auto com : objList)
				{
					com->SetProjection(matProjection);
				}
			}
			else
			{
				for (auto com : objList)
				{
					if (com.GetTag() == tag)
					{
						com->SetProjection(matProjection);
					}
				}
			}
		}
		template<class T>inline void SetCamera(Float3* cameraPos)
		{
			if (tag == 0)
			{
				for (auto com : objList)
				{
					com->SetCamera(cameraPos);
				}
			}
			else
			{
				for (auto com : objList)
				{
					if (com.GetTag() == tag)
					{
						com->SetCamera(cameraPos);
					}
				}
			}
		}
		template<class T>inline void SetBillBoard(BillBoard::BillBoardMode mode)
		{
			if (tag == 0)
			{
				for (auto com : objList)
				{
					com->SetBillBoard(mode);
				}
			}
			else
			{
				for (auto com : objList)
				{
					if (com.GetTag() == tag)
					{
						com->SetBillBoard(mode);
					}
				}
			}
		}
		template<class T>inline void SetViewport(std::vector<D3D12_VIEWPORT>viewport)
		{
			if (tag == 0)
			{
				for (auto com : objList)
				{
					com->SetViewport(viewport);
				}
			}
			else
			{
				for (auto com : objList)
				{
					if (com.GetTag() == tag)
					{
						com->SetViewport(viewport);
					}
				}
			}
		}
		template<class T>inline void SetTexture(unsigned short texNum)
		{
			if (tag == 0)
			{
				for (auto com : objList)
				{
					com->SetTexture(texNum);
				}
			}
			else
			{
				for (auto com : objList)
				{
					if (com.GetTag() == tag)
					{
						com->SetTexture(texNum);
					}
				}
			}
		}
	};
}
