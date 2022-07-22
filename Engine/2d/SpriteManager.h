#pragma once
#include "Sprite.h"
#include "Camera.h"
#include "nlohmann/json.hpp"
#include <list>

namespace IF
{
	class SpriteManager
	{
	private:
		std::list<Sprite*>spriteList;

	public:
		SpriteManager() {}
		~SpriteManager();
		void Draw();
		void Update();
		inline void Reset()
		{
			spriteList.clear();
		}
		inline void Add(unsigned short texNum, std::string tag)
		{
			Sprite* spr = new Sprite;
			spr->Initialize(texNum);
			spr->tag = tag;
			spriteList.push_back(spr);
		}
		inline void SetViewport(std::vector<D3D12_VIEWPORT>viewport)
		{
			for (auto com : spriteList)
			{
				com->SetViewport(viewport);
			}
		}
		inline void SetTexture(unsigned short texNum, std::string tag)
		{
			for (auto com : spriteList)
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
			for (auto com : spriteList)
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
			for (auto com : spriteList)
			{
				if (com->tag == tag)
				{
					com->position = pos;
					return;
				}
			}
		}
		inline void SetRotation(float rota, std::string tag)
		{
			for (auto com : spriteList)
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
			for (auto com : spriteList)
			{
				if (com->tag == tag)
				{
					com->scale = scale;
					return;
				}
			}
		}
		inline int GetTagNum(std::string tag)
		{
			int i = 0;
			for (auto com : spriteList)
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

//#ifdef _DEBUG
		bool drawflag = false;

		void GUI(bool* flag,std::string* tag);
		enum typeinfo
		{
			Tag,
			Model
		};
//#endif
		void OutputJson(nlohmann::json& j);
	};
}