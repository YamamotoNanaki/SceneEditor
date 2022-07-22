#pragma once
#include "Model.h"
#include "nlohmann/json.hpp"

namespace IF
{
	class ModelManager
	{
	private:
		std::list <Model*> modelList;

	public:
		~ModelManager();
		inline bool Load(std::string tag, bool smoothing, std::string name)
		{
			for (auto com : modelList)
			{
				if (com->GetTag() == tag)
				{
					return true;
				}
			}
			Model* model = new Model;
			if (!model->LoadModel(name, smoothing))return false;
			model->SetTag(tag);
			modelList.push_back(model);
			return true;
		}
		inline void Create(std::string tag, bool smoothing, unsigned short texNum, unsigned short mode)
		{
			Model* model = new Model;
			if (mode == CREATE_CUBE)model->CreateCube(texNum, smoothing);
			else if (mode == CREATE_TRIANGLE)model->CreateTriangle(texNum, smoothing);
			else if (mode == CREATE_CIRCLE)model->CreateCircle(texNum, smoothing);
			else if (mode == CREATE_SPHERE)model->CreateSphere(texNum, smoothing);
			model->SetTag(tag);
			modelList.push_back(model);
		}
		inline void Reset()
		{
			modelList.clear();
		}
		inline Model* GetModel(std::string tag)
		{
			for (auto com : modelList)
			{
				if (com->GetTag() == tag)
				{
					return com;
				}
			}
			return nullptr;
		}
		inline int GetTagNum(std::string tag)
		{
			int i = 0;
			for (auto com : modelList)
			{
				if (i == 0)
				{
					if (com->GetTag() == tag)
					{
						i++;
					}
				}
				else
				{
					std::string a = tag + (char)(i + 48);
					if (com->GetTag() == a)
					{
						i++;
					}
				}
			}
			return i;
		}
		inline void SetTexture(unsigned short texNum, std::string tag)
		{
			for (auto com : modelList)
			{
				if (com->GetTag() == tag)
				{
					com->SetTexNum(texNum);
					return;
				}
			}
		}
		inline unsigned short GetTexture(std::string tag)
		{
			unsigned short texNum = 0;
			for (auto com : modelList)
			{
				if (com->GetTag() == tag)
				{
					texNum = com->GetTexNum();
					break;
				}
			}
			return texNum;
		}
		void OutputJson(nlohmann::json& j);
//#ifdef _DEBUG
		void GUI(bool* flag, std::string* tag);
		std::string GUIRadio();
//#endif
	};
}
