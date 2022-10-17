#include "Camera.h"
#include "Input.h"
#include "imgui.h"
#include "Timer.h"
#include "Ease.h"
#include "Rand.h"

using namespace IF;

const std::string Camera::cameraName = "camera";

static enum camerainfo
{
	Start, Boss, BossFront
};

void IF::Camera::Update()
{
	matView->Update();
}

static const int starttime = 70;

#ifdef _DEBUG
void IF::Camera::GUI()
{
	static float distance = 10.0f;
	if (ImGui::TreeNode(tag.c_str())) {
		if (ImGui::TreeNode("Eye"))
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
		ImGui::TreePop();
	}
}
#endif