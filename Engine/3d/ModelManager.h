#pragma once
#include "Model.h"

namespace IF
{
	class ModelManager
	{
	private:
		std::list <Model*> modelList;

	public:
		~ModelManager();
		inline void Load(std::string tag, bool smoothing, std::string name)
		{
			Model* model = new Model;
			model->LoadModel(name, smoothing);
			model->SetTag(tag);
			modelList.push_back(model);
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
#ifdef _DEBUG
		void GUI();
		std::string GUIRadio();
#endif
	};
}
