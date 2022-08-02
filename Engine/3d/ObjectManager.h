#pragma once
#include "ComponentObj.h"
#include "NormalObj.h"
#include "PlayerObj.h"
#include "ICamera.h"
#include "Camera.h"
#include "nlohmann/json.hpp"
#include "Collision.h"
#include <list>

namespace IF
{
	class ObjectManager
	{
	private:
		std::list<CObject*> objList;
		ICamera* camera = nullptr;
	public:
		ObjectManager() {}
		~ObjectManager();
		void Draw();
		void Update();
		inline void SetCamera(ICamera* camera)
		{
			this->camera = camera;
			if (objList.size() == 0)return;
			for (auto com : objList)
			{
				com->SetView(camera->GetMatView());
				com->SetProjection(camera->GetMatPro());
				com->SetCamera(camera->GetEye());
			}
		}
		inline void Reset()
		{
			objList.clear();
		}
		template <class T> inline void Add(Model* model, std::string tag, int mode = BillBoard::NOON)
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
			obj->MatInitialize(camera->GetMatView(), camera->GetMatPro(), camera->GetEye(), a);
			obj->tag = tag;
			obj->SetCollision(NotPri);
			objList.push_back(obj);
		}
		template<class T>inline void SetView(Matrix* matView, std::string tag = "\0")
		{
			if (tag == "\0")
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
					if (com->tag == tag)
					{
						com->SetView(matView);
					}
				}
			}
		}
		template<class T>inline void SetProjection(Matrix* matProjection, std::string tag = "\0")
		{
			if (tag == "\0")
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
					if (com->tag == tag)
					{
						com->SetProjection(matProjection);
					}
				}
			}
		}
		template<class T>inline void SetCamera(Float3* cameraPos, std::string tag = "\0")
		{
			if (tag == "\0")
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
		template<class T>inline void SetBillBoard(int mode, std::string tag = "\0")
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
			if (tag == "\0")
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
					if (com->tag == tag)
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
		template<class T>inline void SetTexture(unsigned short texNum, std::string tag = "\0")
		{
			if (tag == "\0")
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
					if (com->tag == tag)
					{
						com->SetTexture(texNum);
					}
				}
			}
		}
		inline void SetPosition(Float3 pos, std::string tag)
		{
			for (auto com : objList)
			{
				if (com->tag == tag)
				{
					com->SetPos(pos);
					return;
				}
			}
		}
		inline void SetRotation(Float3 rota, std::string tag)
		{
			for (auto com : objList)
			{
				if (com->tag == tag)
				{
					com->SetRota(rota);
					return;
				}
			}
		}
		inline void SetScale(Float3 scale, std::string tag)
		{
			for (auto com : objList)
			{
				if (com->tag == tag)
				{
					com->SetScale(scale);
					return;
				}
			}
		}
		inline void SetCollision(unsigned short ptype, std::string tag)
		{
			for (auto com : objList)
			{
				if (com->tag == tag)
				{
					com->SetCollision(ptype);
					return;
				}
			}
		}
		inline bool Collision()
		{
			std::vector<Primitive*> c;
			std::vector<unsigned short> b;
			for (auto com : objList)
			{
				unsigned short a = com->GetCollision();
				if (a == NotPri)continue;
				c.push_back(com->GetPrimitive());
				b.push_back(a);
			}
			if (c.size() <= 1)return false;
			for (int i = 0; i < c.size() - 1; i++)
			{
				bool flag = false;
				for (int j = i + 1; j < c.size(); j++)
				{
					if ((b[i] == RayPri && b[j] == SpherePri))
					{
						Ray* r = dynamic_cast<Ray*>(c[i]);
						Sphere* s = dynamic_cast<Sphere*>(c[j]);
						if (r == nullptr || s == nullptr)continue;
						flag = Collision::CheckRaySphere(*r, *s);
					}
					if ((b[i] == SpherePri && b[j] == RayPri))
					{
						Ray* r = dynamic_cast<Ray*>(c[j]);
						Sphere* s = dynamic_cast<Sphere*>(c[i]);
						if (r == nullptr || s == nullptr)continue;
						flag = Collision::CheckRaySphere(*r, *s);
					}
					if (flag)return flag;
				}
			}
			return false;
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
		void OutputJson(nlohmann::json& j);

		//#ifdef _DEBUG
		void GUI();
		enum typeinfo
		{
			Tag,
			Model
		};
		//#endif
	};
}
