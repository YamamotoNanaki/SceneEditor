#pragma once
#include "Sprite.h"
#include "Camera.h"
#include "Debug.h"
#include "nlohmann/json.hpp"
#include <list>

namespace IF
{
	class SpriteManager
	{
	public:
		std::list<Sprite*>backgroundList;
		std::list<Sprite*>foregroundList;
		Sprite load;
		Sprite load2;
	private:
		SpriteManager()
		{
			load.Initialize(1);
			load.scale = { 20,15 };
			load.position = { 1920 / 2,1080 / 2 };
			for (int i = 0; i < 4; i++)load.color[i] = 0;
			load2.Initialize(1);
			load2.scale = { 2,2 };
			load2.position = { 1280 / 2,720 / 2 };
			for (int i = 0; i < 4; i++)load2.color[i] = 1;
		}
		SpriteManager(const Sprite&) {}
		SpriteManager& operator=(const SpriteManager&) {}
		~SpriteManager();
		Float2 anchorpoint = { 0.5f,0.5f };
	public:
		static SpriteManager* Instance();
		static void DeleteInstance();
		inline Sprite* GetAddress(std::string tag)
		{
			for (auto com : foregroundList)
			{
				if (com->tag == tag)
				{
					return com;
				}
			}
			for (auto com : backgroundList)
			{
				if (com->tag == tag)
				{
					return com;
				}
			}
			return nullptr;
		}
		void ForeGroundDraw();
		void BackGroundDraw();
		void Update();
		void DebugUpdate();
		void DrawFlagChange(bool f, std::string tag);
		inline void DeleteSprite()
		{
			auto buff = backgroundList;
			for (auto com : buff)
			{
				if (com->DeleteSprite())
				{
					backgroundList.remove(com);
					delete com;
				}
			}
			auto buff2 = foregroundList;
			for (auto com : buff2)
			{
				if (com->DeleteSprite())
				{
					foregroundList.remove(com);
					delete com;
				}
			}
		}
		inline void SetBright(float r, float g, float b, std::string tag, bool all = false)
		{
			if (all)
			{
				for (auto com : foregroundList)
				{
					com->SetBright(r, g, b);
				}
				for (auto com : backgroundList)
				{
					com->SetBright(r, g, b);
				}
			}
			else
			{
				for (auto com : foregroundList)
				{
					if (com->tag == tag)
					{
						com->SetBright(r, g, b);
						return;
					}
				}
				for (auto com : backgroundList)
				{
					if (com->tag == tag)
					{
						com->SetBright(r, g, b);
						return;
					}
				}
			}
			if ("load" == tag)
			{
				load.SetBright(r, g, b);
			}
		}
		inline void Reset()
		{
			for (auto com : foregroundList)
			{
				delete com;
			}
			for (auto com : backgroundList)
			{
				delete com;
			}
			foregroundList.clear();
			backgroundList.clear();
		}
		inline Sprite* Add(unsigned short texNum, std::string tag, bool back)
		{
			Sprite* spr = DEBUG_NEW Sprite;
			spr->Initialize(texNum, { 100,100 }, anchorpoint);
			spr->tag = tag;
			if (back)backgroundList.push_back(spr);
			else foregroundList.push_back(spr);
			return spr;
		}
		inline Sprite* AddFront(unsigned short texNum, std::string tag, bool back)
		{
			Sprite* spr = DEBUG_NEW Sprite;
			spr->Initialize(texNum);
			spr->tag = tag;
			if (back)backgroundList.push_front(spr);
			else foregroundList.push_front(spr);
			return spr;
		}
		inline Sprite* Add(unsigned short texNum, std::string tag, bool back, Float2 anchorpoint)
		{
			Sprite* spr = DEBUG_NEW Sprite;
			spr->Initialize(texNum, { 100,100 }, anchorpoint);
			spr->tag = tag;
			if (back)backgroundList.push_back(spr);
			else foregroundList.push_back(spr);
			return spr;
		}
		inline void SetTexture(unsigned short texNum, std::string tag)
		{
			for (auto com : foregroundList)
			{
				if (com->tag == tag)
				{
					com->texNum = texNum;
					return;
				}
			}
			for (auto com : backgroundList)
			{
				if (com->tag == tag)
				{
					com->texNum = texNum;
					return;
				}
			}
		}
		inline unsigned short GetTexture(std::string tag)
		{
			unsigned short texNum = 0;
			for (auto com : foregroundList)
			{
				if (com->tag == tag)
				{
					texNum = com->texNum;
					break;
				}
			}
			for (auto com : backgroundList)
			{
				if (com->tag == tag)
				{
					texNum = com->texNum;
					break;
				}
			}
			return texNum;
		}
		inline void SetPosition(Float2 pos, std::string tag)
		{
			for (auto com : foregroundList)
			{
				if (com->tag == tag)
				{
					com->position = pos;
					return;
				}
			}
			for (auto com : backgroundList)
			{
				if (com->tag == tag)
				{
					com->position = pos;
					return;
				}
			}
		}
		inline void SetAlpha(float a, std::string tag, bool all = false)
		{
			if (all)
			{
				for (auto com : foregroundList)
				{
					com->SetAlpha(a);
				}
				for (auto com : backgroundList)
				{
					com->SetAlpha(a);
				}
			}
			else
			{
				for (auto com : foregroundList)
				{
					if (com->tag == tag)
					{
						com->SetAlpha(a);
						return;
					}
				}
				for (auto com : backgroundList)
				{
					if (com->tag == tag)
					{
						com->SetAlpha(a);
						return;
					}
				}
			}
			if ("load" == tag)
			{
				load.SetAlpha(a);
			}
		}
		inline void SetRotation(float rota, std::string tag)
		{
			for (auto com : foregroundList)
			{
				if (com->tag == tag)
				{
					com->rotation = rota;
					return;
				}
			}
			for (auto com : backgroundList)
			{
				if (com->tag == tag)
				{
					com->rotation = rota;
					return;
				}
			}
		}
		inline void SetScale(Float2 scale, std::string tag)
		{
			for (auto com : foregroundList)
			{
				if (com->tag == tag)
				{
					com->scale = scale;
					return;
				}
			}
			for (auto com : backgroundList)
			{
				if (com->tag == tag)
				{
					com->scale = scale;
					return;
				}
			}
		}
		inline void SetAI(short aitype, std::string tag)
		{
			for (auto com : foregroundList)
			{
				if (com->tag == tag)
				{
					return;
				}
			}
			for (auto com : backgroundList)
			{
				if (com->tag == tag)
				{
					return;
				}
			}
		}
	private:
		inline bool GetNum(std::string tag)
		{
			for (auto com : foregroundList)
			{
				if (com->tag == tag)
				{
					return true;
				}
			}
			for (auto com : backgroundList)
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
			std::string t = tag;
			while (GetNum(t))
			{
				i++;
				if (i < 10)
				{
					t = tag + (char)(i + 48);
				}
				else if (i < 100)
				{
					t = tag + (char)(i / 10 + 48) + (char)(i % 10 + 48);
				}
				else if (i < 1000)
				{
					int b = i / 100;
					int c = i / 10;
					c -= b * 10;
					t = tag + (char)(b + 48) + (char)(c + 48) + (char)(i % 10 + 48);
				}
				else
				{
					int b = i / 1000;
					int c = i / 100;
					c -= b * 10;
					int d = i / 10;
					d -= b * 100;
					d -= c * 10;
					t = tag + (char)(b + 48) + (char)(c + 48) + (char)(d + 48) + (char)(i % 10 + 48);
				}
			}
			return i;
		}

#ifdef _DEBUG
		bool drawflag = false;

		void GUI();
		enum typeinfo
		{
			Tag,
			Model
		};
		void OutputJson(nlohmann::json& j);
#endif
	};
}