#include "CameraManager.h"
#include "ImGui.h"

using namespace ImGui;

void IF::CameraManager::GUI()
{
	for (auto com : cameraList)
	{
		if (TreeNode(com->tag.c_str()))
		{
			if (ImGui::TreeNode("Eye"))
			{
				Float3 eye = *com->GetEye();
				float e[3] = { eye.x,eye.y,eye.z };
				ImGui::InputFloat3("", e);
				com->SetEye({ e[0],e[1],e[2] });
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Target"))
			{
				Float3 target = com->GetTarget();
				float t[3] = { ConvertToDegrees(target.x),ConvertToDegrees(target.y),ConvertToDegrees(target.z) };
				ImGui::InputFloat3("", t);
				com->SetTarget({ ConvertToRadians(t[0]),ConvertToRadians(t[1]), ConvertToRadians(t[2]) });
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Up"))
			{
				Float3 up = com->GetUp();
				float u[3] = { up.x,up.y,up.z };
				ImGui::InputFloat3("", u);
				com->SetUp({ u[0], u[1], u[2] });
				ImGui::TreePop();
			}
			TreePop();
		}
	}
}
