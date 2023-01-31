#include "ICamera.h"
#include "imgui.h"
#include "ObjectManager.h"

//#ifdef _DEBUG
void IF::ICamera::GUI()
{
	if (ImGui::TreeNode(tag.c_str())) {
		if (ImGui::TreeNode("Position"))
		{
			float e[3] = { matView->eye.x,matView->eye.y,matView->eye.z };
			ImGui::DragFloat3("", e, 0.05f);
			matView->eye = { e[0],e[1],e[2] };
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Rotate"))
		{
			float e[3] = { matView->eye.x,matView->eye.y,matView->eye.z };
			ImGui::DragFloat3("", e, 0.05f);
			matView->eye = { e[0],e[1],e[2] };
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Target"))
		{
			float t[3] = { matView->target.x,matView->target.y,matView->target.z };
			ImGui::DragFloat3("", t, 0.05f);
			matView->target = { t[0],t[1],t[2] };
			ImGui::TreePop();
		}
		if (ImGui::Button("Chenge"))
		{
			ObjectManager::Instance()->SetCamera(this);
		}
		ImGui::TreePop();
	}
}
//#endif
