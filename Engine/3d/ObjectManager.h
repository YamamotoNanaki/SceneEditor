#pragma once
#include "ComponentObj.h"
#include <list>

namespace IF
{
	class ObjectManager
	{
	private:
		std::list<CObject*> objList;
	public:
		ObjectManager() {}
		~ObjectManager();
		void Draw();
		void Update();
		template <class T> inline void Add(Model* model, Matrix* matView, Matrix* matProjection, Float3* cameraPos, std::string tag, int mode = BillBoard::NOON)
		{
			T* obj = new T;
			obj->Initialize(model);
			BillBoard::BillBoardMode a = BillBoard::NOON;
			if (mode == BillBoard::BILLBOARD)
			{
				a = BillBoard::BILLBOARD;
			}
			if (mode == BillBoard::YBOARD)
			{
				a = BillBoard::YBOARD;
			}
			obj->MatInitialize(matView, matProjection, cameraPos, a);
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
		template<class T>inline void SetBillBoard(int mode, const char* tag = 0)
		{
			BillBoard::BillBoardMode a = BillBoard::NOON;
			if (mode == BillBoard::BILLBOARD)
			{
				a = BillBoard::BILLBOARD;
			}
			if (mode == BillBoard::YBOARD)
			{
				a = BillBoard::YBOARD;
			}
			if (strcmp(tag, 0))
			{
				for (auto com : objList)
				{
					com->SetBillBoard(a);
				}
			}
			else
			{
				for (auto com : objList)
				{
					const char* ctag = com->tag.c_str();
					if (!strcmp(ctag, tag))
					{
						com->SetBillBoard(a);
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
		inline int GetTagNum(std::string tag)
		{
			int i = 0;
			for (auto com : objList)
			{
				if (i == 0)
				{
					if (com->tag == tag)
					{
						i++;
					}
				}
				else
				{
					std::string a = tag + (char)(i + 48);
					if (com->tag == a)
					{
						i++;
					}
				}
			}
			return i;
		}

#ifdef _DEBUG
		void GUI();
#endif
	};
}
