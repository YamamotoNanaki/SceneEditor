#pragma once
#include "Model.h"
#include "nlohmann/json.hpp"
#include "Debug.h"

namespace IF
{
	class ModelManager
	{
	private:
		std::list <Model*> modelList;
		~ModelManager();
		ModelManager() {}
		ModelManager(const ModelManager&) {}
		ModelManager& operator=(const ModelManager&) {}

	public:
		static ModelManager* Instance();
		static void DeleteInstance();
		inline bool Load(std::string tag, bool smoothing, std::string name)
		{
			for (auto com : modelList)
			{
				if (com->GetTag() == tag)
				{
					return true;
				}
			}
			Model* model = DEBUG_NEW Model;
			if (!model->LoadModel(name, smoothing))return false;
			model->SetTag(tag);
			modelList.push_back(model);
			return true;
		}
		inline void Create(std::string tag, bool smoothing, unsigned short texNum, unsigned short mode)
		{
			Model* model = DEBUG_NEW Model;
			if (mode == CREATE_CUBE)model->CreateCube(texNum, smoothing);
			else if (mode == CREATE_TRIANGLE)model->CreateTriangle(texNum, smoothing);
			else if (mode == CREATE_CIRCLE)model->CreateCircle(texNum, smoothing);
			else if (mode == CREATE_SPHERE)model->CreateSphere(texNum, smoothing);
			else if (mode == CREATE_RAY)model->CreateRay();
			else if (mode == CREATE_POLYGON_SQUARE)model->CreatePolygonSquare(texNum, smoothing);
			model->SetTag(tag);
			modelList.push_back(model);
		}
		inline void Reset()
		{
			for (auto com : modelList)
			{
				delete com;
			}
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
		void GUI();
		std::string GUIRadio();
//#endif
	};
}
