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
		ObjectManager() {}
		ObjectManager(const ObjectManager&);
		ObjectManager& operator=(const ObjectManager&) {}
		~ObjectManager();
	public:
		static ObjectManager* Instance();
		static void DeleteInstance();
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
		inline void SetAi(std::string tag, short ai)
		{
			for (auto com : objList)
			{
				if (com->tag == tag)
				{
					com->SetAi(ai);
					return;
				}
			}
		}
		inline void Reset()
		{
			for (auto com : objList)
			{
				delete com;
			}
			objList.clear();
		}
		Primitive* GetPrimitive(std::string tag);
		template<class T>inline T* GetAddress(std::string tag)
		{
			for (auto com : objList)
			{
				if (com->tag == tag)
				{
					T* buff = dynamic_cast<T*>(com);
					return buff;
				}
			}
			return nullptr;
		}
		template <class T> inline T* Add(Model* model, std::string tag, int mode = BillBoard::NOON, bool prefab = false)
		{
			T* obj = new T;
			obj->Initialize(model, prefab);
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
			return obj;
		}
		template <class T> inline T* CopyAdd(std::string copytag, std::string addtag, bool prefab = false)
		{
			T* obj = new T;
			for (auto com : objList)
			{
				if (com->tag == copytag)
				{
					obj->Initialize(com->GetModelAddress(), prefab);
					obj->MatInitialize(camera->GetMatView(), camera->GetMatPro(), camera->GetEye(), com->GetBillBoard());
					obj->tag = addtag;
					obj->SetCollision(com->GetCollision());
					objList.push_back(obj);
					return obj;
				}
			}
			return nullptr;
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
		inline void DeleteObject()
		{
			auto buff = objList;
			for (auto com : buff)
			{
				com->DeleteObj();
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
		inline Float3 GetPosition(std::string tag)
		{
			for (auto com : objList)
			{
				if (com->tag == tag)
				{
					return com->GetPos();
				}
			}
			return Float3(0, 0, 0);
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
		inline Float3 GetRotation(std::string tag)
		{
			for (auto com : objList)
			{
				if (com->tag == tag)
				{
					return com->GetRota();
				}
			}
			return Float3(0, 0, 0);
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
		inline Float3 GetScale(std::string tag)
		{
			for (auto com : objList)
			{
				if (com->tag == tag)
				{
					return com->GetScale();
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
		//inline bool Collision()
		//{
		//	std::vector<Primitive*> c;
		//	std::vector<unsigned short> b;
		//	for (auto com : objList)
		//	{
		//		unsigned short a = com->GetCollision();
		//		if (a == NotPri)continue;
		//		c.push_back(com->GetPrimitive());
		//		b.push_back(a);
		//	}
		//	if (c.size() <= 1)return false;
		//	for (int i = 0; i < c.size() - 1; i++)
		//	{
		//		bool flag = false;
		//		for (int j = i + 1; j < c.size(); j++)
		//		{
		//			if ((b[i] == RayPri && b[j] == SpherePri))
		//			{
		//				Ray* r = dynamic_cast<Ray*>(c[i]);
		//				Sphere* s = dynamic_cast<Sphere*>(c[j]);
		//				if (r == nullptr || s == nullptr)continue;
		//				flag = Collision::CheckRaySphere(*r, *s);
		//			}
		//			if ((b[i] == SpherePri && b[j] == RayPri))
		//			{
		//				Ray* r = dynamic_cast<Ray*>(c[j]);
		//				Sphere* s = dynamic_cast<Sphere*>(c[i]);
		//				if (r == nullptr || s == nullptr)continue;
		//				flag = Collision::CheckRaySphere(*r, *s);
		//			}
		//			if (flag)return flag;
		//		}
		//	}
		//	return false;
		//}
	private:
		inline bool GetNum(std::string tag)
		{
			for (auto com : objList)
			{
				if (com->tag == tag)
				{
					return true;
				}
			}
			return false;
		}
	public:
		inline int GetTagNum(std::string tag)
		{
			int i = 0;
			for (auto com : objList)
			{
				if (i == 0)
				{
					if (GetNum(tag))
					{
						i++;
					}
				}
				else if (i < 10)
				{
					std::string a = tag + (char)(i + 48);
					if (GetNum(a))
					{
						i++;
					}
				}
				else if (i < 100)
				{
					std::string a = tag + (char)(i / 10 + 48) + (char)(i % 10 + 48);
					if (GetNum(a))
					{
						i++;
					}
				}
				else if (i < 1000)
				{
					int b = i / 100;
					int c = i / 10;
					c -= b * 10;
					std::string a = tag + (char)(b + 48) + (char)(c + 48) + (char)(i % 10 + 48);
					if (GetNum(a))
					{
						i++;
					}
				}
				else
				{
					int b = i / 1000;
					int c = i / 100;
					c -= b * 10;
					int d = i / 10;
					d -= b * 100;
					d -= c * 10;
					std::string a = tag + (char)(b + 48) + (char)(c + 48) + (char)(d + 48) + (char)(i % 10 + 48);
					if (GetNum(a))
					{
						i++;
					}
				}
			}
			return i;
		}
		inline void SetAlpha(float a, std::string tag, bool all = false)
		{
			if (all)
			{
				for (auto com : objList)
				{
					com->SetAlpha(a);
				}
			}
			else
			{
				for (auto com : objList)
				{
					if (com->tag == tag)
					{
						com->SetAlpha(a);
						return;
					}
				}
			}
		}
		void Delete(std::string tag);
		void DeleteFlag(std::string tag, bool AllNum = false)
		{
			if (AllNum)
			{
				for (auto com : objList)
				{
					int j = 0;
					for (int i = 0; i < tag.size(); i++)
					{
						if (!strncmp(tag.c_str(), com->tag.c_str(), i))
						{
							j++;
							if (j == tag.size())
							{
								com->deleteFlag = true;
							}
						}
						else
						{
							break;
						}
					}
				}
			}
			else
			{
				for (auto com : objList)
				{
					if (tag == com->tag)
					{
						com->deleteFlag = true;
					}
				}
			}
			}
			void ChangePushback(std::string tag)
			{
				for (auto com : objList)
				{
					if (com->tag == tag)
					{
						objList.remove(com);
						objList.push_back(com);
						return;
					}
				}
			}
			void InputJson(nlohmann::json& j);
#ifdef _DEBUG
			void GUI();
			std::string GUIRadio();
			enum typeinfo
			{
				Tag,
				Model
			};
			void OutputJson(nlohmann::json & j);
			inline void SetDebugFlag(bool flag)
			{
				for (auto com : objList)
				{
					com->SetFlag(flag);
				}
			}
			inline short GetAi(std::string tag)
			{
				for (auto com : objList)
				{
					if (com->tag == tag)
					{
						return com->GetAi();
					}
				}
			}
#endif
		};
	}
