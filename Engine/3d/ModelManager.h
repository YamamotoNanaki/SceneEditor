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
			Model* model = new Model;
			if (!model->LoadModel(name, smoothing))return false;
			model->SetTag(tag);
			modelList.push_back(model);
			return true;
		}
		inline void Create(std::string tag, std::string mode, bool smoothing)
		{
			Model* model = new Model;
			model->CreateCube(smoothing);
			model->SetTag(tag);
			modelList.push_back(model);
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
#ifdef _DEBUG
		void GUI();
		std::string GUIRadio();
		void OutputJson(nlohmann::json& j);
#endif
	};
}
