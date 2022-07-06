#include "SpriteManager.h"
#include "imgui.h"

IF::SpriteManager::~SpriteManager()
{
	Reset();
}

void IF::SpriteManager::Draw()
{
	for (auto com : spriteList)
	{
		com->Draw();
	}
}

void IF::SpriteManager::Update()
{
	for (auto com : spriteList)
	{
		com->Update();
	}
}

#ifdef _DEBUG
void IF::SpriteManager::GUI()
{
	for (auto com : spriteList)
	{
		if (ImGui::TreeNode(com->tag.c_str())) {
			if (ImGui::TreeNode("Position"))
			{
				float p[2] = { com->position.x,com->position.y };
				ImGui::DragFloat2("", p);
				com->position = { p[0],p[1] };
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Rotation"))
			{
				float r[2] = { ConvertToDegrees(com->rotation) };
				ImGui::DragFloat2("", r, 0.5f);
				if (r[0] >= 360)r[0] -= 360;
				if (r[0] < 0)r[0] += 360;
				com->rotation = ConvertToRadians(r[0]);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Scale"))
			{
				float s[2] = { com->scale.x,com->scale.y };
				ImGui::DragFloat2("", s, 0.05f);
				com->scale = { s[0],s[1] };
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}
	}
}

void IF::SpriteManager::OutputJson(nlohmann::json& j)
{
}

#endif
