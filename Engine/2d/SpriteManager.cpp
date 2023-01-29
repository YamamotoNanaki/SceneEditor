#include "SpriteManager.h"
#include "imgui.h"
#include "Texture.h"

using namespace IF;

IF::SpriteManager::~SpriteManager()
{
	Reset();
}

SpriteManager* IF::SpriteManager::Instance()
{
	static SpriteManager* inst = DEBUG_NEW SpriteManager;
	return inst;
}

void IF::SpriteManager::DeleteInstance()
{
	delete SpriteManager::Instance();
}

void IF::SpriteManager::ForeGroundDraw()
{
	for (auto com : foregroundList)
	{
		com->Draw();
	}
	load.Draw();
}

void IF::SpriteManager::BackGroundDraw()
{
	for (auto com : backgroundList)
	{
		com->Draw();
	}
}

void IF::SpriteManager::Update()
{
	for (auto com : foregroundList)
	{
		com->Update();
	}
	for (auto com : backgroundList)
	{
		com->Update();
	}
	load.Update();
}

#ifdef _DEBUG

void IF::SpriteManager::DebugUpdate()
{
	for (auto com : foregroundList)
	{
		com->DebugUpdate();
	}
	for (auto com : backgroundList)
	{
		com->DebugUpdate();
	}
}

#endif

void IF::SpriteManager::DrawFlagChange(bool f, std::string tag)
{
	for (auto buff : backgroundList)
	{
		if (buff->tag == tag)
		{
			buff->drawFlag = f;
			return;
		}
	}
	for (auto buff : foregroundList)
	{
		if (buff->tag == tag)
		{
			buff->drawFlag = f;
			return;
		}
	}
}

#ifdef _DEBUG
void IF::SpriteManager::GUI()
{
	ImGui::Begin("SpriteManager");
	static bool d = false;
	ImGui::Checkbox("DeleteMode", &d);
	if (ImGui::CollapsingHeader("fore"))
	{
		auto buff = foregroundList;
		for (auto com : buff)
		{
			if (ImGui::TreeNode(com->tag.c_str())) {
				if (d)
				{
					if (ImGui::Button("Delete"))
					{
						foregroundList.remove(com);
						delete com;
						ImGui::TreePop();
						continue;
					}
				}
				com->GUI();
				if (ImGui::TreeNode("change background"))
				{
					if (ImGui::Button("Change"))
					{
						backgroundList.push_back(com);
						foregroundList.remove(com);
					}
					ImGui::TreePop();
				}
				ImGui::TreePop();
			}
		}
	}
	if (ImGui::CollapsingHeader("back"))
	{
		auto buff2 = backgroundList;
		for (auto com : buff2)
		{
			if (ImGui::TreeNode(com->tag.c_str())) {
				if (d)
				{
					if (ImGui::Button("Delete"))
					{
						backgroundList.remove(com);
						delete com;
						ImGui::TreePop();
						continue;
					}
				}
				com->GUI();
				if (ImGui::TreeNode("change foregroundList"))
				{
					if (ImGui::Button("Change"))
					{
						foregroundList.push_back(com);
						backgroundList.remove(com);
					}
					ImGui::TreePop();
				}
				ImGui::TreePop();
			}
		}
	}
	static char tagC[256];
	if (ImGui::CollapsingHeader("NewSprite"))
	{
		static int tex = 1;
		static int back = false;
		static int cla = 0;
		int old = cla;
		static bool flag = true;
		if (ImGui::TreeNode("class"))
		{
			ImGui::RadioButton("Sprite", &cla, 0);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Texture"))
		{
			Texture::Instance()->TexNum(&tex);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("back or fore"))
		{
			ImGui::RadioButton("back", &back, 1);
			ImGui::RadioButton("fore", &back, 0);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("anchorpoint"))
		{
			static int num = 0;
			ImGui::RadioButton("Center", &num, 0);
			ImGui::RadioButton("left  top", &num, 1);
			ImGui::RadioButton("left  bottom", &num, 2);
			ImGui::RadioButton("right top", &num, 3);
			ImGui::RadioButton("right bottom", &num, 4);
			if (num == 0)anchorpoint = { 0.5f,0.5f };
			else if (num == 1)anchorpoint = { 0.f,0.f };
			else if (num == 2)anchorpoint = { 0.f,1.f };
			else if (num == 3)anchorpoint = { 1.f,0.f };
			else anchorpoint = { 1.f,1.f };
			ImGui::TreePop();
		}
		if (old != cla)
		{
			flag = true;
		}
		if (flag == true)
		{
			std::string tag;
			if (cla == 0)tag = "Sprite";
			int num = this->GetTagNum(tag);
			int d = 0;
			for (int i = 1; i <= num; i *= 10)
			{
				d++;
			}
			int digit = d;
			for (d; d > 0; d--)
			{
				int a = 1;
				for (int i = 0; i < d - 1; i++)a *= 10;
				int b = num / a;
				tag += (b + 48);
				num -= b * a;
			}
			strcpy_s(tagC, tag.c_str());
			flag = false;
		}
		ImGui::InputText("Tag", tagC, sizeof(tagC));
		if (ImGui::Button("Add"))
		{
			Sprite* spr = Add(tex, tagC, back);
			spr->SetPosition({ 1280 / 2,720 / 2 });
		}
		if (ImGui::Button("AddFront"))
		{
			Sprite* spr = AddFront(tex, tagC, back);
			spr->SetPosition({ 1280 / 2,720 / 2 });
		}
	}
	ImGui::End();
}

void IF::SpriteManager::OutputJson(nlohmann::json& j)
{
	int i = 0;
	for (auto com : foregroundList)
	{
		j["sprite"][i]["tag"] = com->tag;
		j["sprite"][i]["tex"] = com->texNum;
		j["sprite"][i]["pos"]["x"] = com->position.x;
		j["sprite"][i]["pos"]["y"] = com->position.y;
		j["sprite"][i]["rot"] = com->rotation;
		j["sprite"][i]["sca"]["x"] = com->scale.x;
		j["sprite"][i]["sca"]["y"] = com->scale.y;
		j["sprite"][i]["anchorpoint"]["x"] = com->anchorpoint.x;
		j["sprite"][i]["anchorpoint"]["y"] = com->anchorpoint.y;
		j["sprite"][i]["color"]["r"] = com->color[0];
		j["sprite"][i]["color"]["g"] = com->color[1];
		j["sprite"][i]["color"]["b"] = com->color[2];
		j["sprite"][i]["color"]["a"] = com->color[3];
		j["sprite"][i]["type"] = false;
		i++;
	}
	for (auto com : backgroundList)
	{
		j["sprite"][i]["tag"] = com->tag;
		j["sprite"][i]["tex"] = com->texNum;
		j["sprite"][i]["pos"]["x"] = com->position.x;
		j["sprite"][i]["pos"]["y"] = com->position.y;
		j["sprite"][i]["rot"] = com->rotation;
		j["sprite"][i]["sca"]["x"] = com->scale.x;
		j["sprite"][i]["sca"]["y"] = com->scale.y;
		j["sprite"][i]["anchorpoint"]["x"] = com->anchorpoint.x;
		j["sprite"][i]["anchorpoint"]["y"] = com->anchorpoint.y;
		j["sprite"][i]["color"]["r"] = com->color[0];
		j["sprite"][i]["color"]["g"] = com->color[1];
		j["sprite"][i]["color"]["b"] = com->color[2];
		j["sprite"][i]["color"]["a"] = com->color[3];
		j["sprite"][i]["type"] = true;
		i++;
	}
}
#endif
