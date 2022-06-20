#pragma once
#include "ComponentObj.h"
#include <list>

namespace IF
{
	class ObjectManager
	{
	private:
		std::list<ComponentObj*> objList;
	public:
		ObjectManager() {}
		~ObjectManager();
		void Draw();
		void Update();
		template <class T> inline void Add(Model* model, Matrix* matView, Matrix* matProjection, Float3* cameraPos, std::string tag, BillBoard::BillBoardMode mode = BillBoard::NOON)
		{
			T* obj = new T;
			obj->Initialize(model);
			obj->MatInitialize(matView, matProjection, cameraPos, mode);
			obj->tag = tag;
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
		template<class T>inline void SetView(Matrix* matView, const char* tag = 0)
		{
			if (strcmp(tag, 0))
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
					const char* ctag = com->tag.c_str();
					if (!strcmp(ctag, tag))
					{
						com->SetView(matView);
					}
				}
			}
		}
		template<class T>inline void SetProjection(Matrix* matProjection, const char* tag = 0)
		{
			if (strcmp(tag, 0))
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
					const char* ctag = com->tag.c_str();
					if (!strcmp(ctag, tag))
					{
						com->SetProjection(matProjection);
					}
				}
			}
		}
		template<class T>inline void SetCamera(Float3* cameraPos, const char* tag = 0)
		{
			if (strcmp(tag, 0))
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
					const char* ctag = com->tag.c_str();
					if (!strcmp(ctag, tag))
					{
						com->SetCamera(cameraPos);
					}
				}
			}
		}
		template<class T>inline void SetBillBoard(BillBoard::BillBoardMode mode, const char* tag = 0)
		{
			if (strcmp(tag, 0))
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
					const char* ctag = com->tag.c_str();
					if (!strcmp(ctag, tag))
					{
						com->SetBillBoard(mode);
					}
				}
			}
		}
		inline void SetViewport(std::vector<D3D12_VIEWPORT>viewport)
		{
			for (auto com : objList)
			{
				com->SetViewport(viewport);
			}
		}
		template<class T>inline void SetTexture(unsigned short texNum, const char* tag = 0)
		{
			if (strcmp(tag, 0))
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
					const char* ctag = com->tag.c_str();
					if (!strcmp(ctag, tag))
					{
						com->SetTexture(texNum);
					}
				}
			}
		}
		inline void SetPosition(Float3 pos, const char* tag)
		{
			for (auto com : objList)
			{
				const char* ctag = com->tag.c_str();
				if (!strcmp(ctag, tag))
				{
					com->SetPos(pos);
					return;
				}
			}
		}

#ifdef _DEBUG
		void GUI();
#endif
	};
}
