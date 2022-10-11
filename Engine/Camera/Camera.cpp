#include "Camera.h"
#include "Input.h"
#include "imgui.h"
#include "Timer.h"
#include "Ease.h"
#include "Rand.h"

using namespace IF;

#define seconds 50

static enum camerainfo
{
	Start, Boss, BossFront
};

void IF::Camera::Update()
{
	static float distance = 10.0f;
	matView->target = { matView->eye.x + cosf(ConvertToRadians(rota)) * distance,  2.0f,
		matView->eye.z + sinf(ConvertToRadians(rota)) * distance };
	frontvec = { cosf(ConvertToRadians(rota)),0,sinf(ConvertToRadians(rota)) };
	frontvec.Normalize();

	matView->Update();
}

static const int starttime = 70;

#ifdef _DEBUG
void IF::Camera::GUI()
{
	if (ImGui::TreeNode(tag.c_str())) {
		if (ImGui::TreeNode("Eye"))
		{
			float e[3] = { matView->eye.x,matView->eye.y,matView->eye.z };
			ImGui::DragFloat3("", e, 0.05f);
			matView->eye = { e[0],e[1],e[2] };
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Rotation"))
		{
			float t[2] = { rota,0 };
			ImGui::DragFloat2("", t, 0.05f);
			rota = t[0];
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