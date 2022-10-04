#include "DebugCamera.h"
#include "Input.h"
#include "IFMath.h"
#include "imgui.h"

void IF::DebugCamera::Update()
{
	Input* input = Input::Instance();
	static bool flag = false;
	if (input->MMTriggere())flag = !flag;
	if (flag)
	{
		Mouse move = input->GetMouse3D();
		bool dirty = false;
		float angleX = 0;
		float angleY = 0;

		if (input->MLPush())
		{
			float dy = move.x * scaleY;
			float dx = move.y * scaleX;

			angleX = -dx * M_PI;
			angleY = -dy * M_PI;
			dirty = true;
		}

		if (input->MRPush())
		{
			float dx = move.x / 100.0f;
			float dy = move.y / 100.0f;

			Vector3 move2 = { -dx, +dy, 0 };
			move2 = Vector3Transform(move2, matRot);

			Float3 eye = matView->eye;
			Float3 target = matView->target;

			eye.x += move2.x;
			eye.y += move2.y;
			eye.z += move2.z;

			target.x += move2.x;
			target.y += move2.y;
			target.z += move2.z;

			SetEye(eye);
			SetTarget(target);

			dirty = true;
		}

		if (move.z != 0)
		{
			distance -= move.z / 100.0f;
			distance = distance > 1.0f ? distance : 1.0f;
			dirty = true;
		}

		if (dirty) {
			Matrix newMatRot;
			newMatRot *= MatrixRotationX(-angleX);
			newMatRot *= MatrixRotationY(-angleY);

			matRot = newMatRot * matRot;


			Vector3 newTargetEye = { 0.0f, 0.0f, -distance };
			Vector3 newUp = { 0.0f, 1.0f, 0.0f };

			newTargetEye = Vector3Transform(newTargetEye, matRot);
			newUp = Vector3Transform(newUp, matRot);

			const Float3& newTarget = matView->target;
			SetEye({ newTarget.x + newTargetEye.x, newTarget.y + newTargetEye.y, newTarget.z + newTargetEye.z });
			SetUp({ newUp.x, newUp.y, newUp.z });
		}
	}
	matView->Update();
}

#ifdef _DEBUG
void IF::DebugCamera::GUI()
{
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