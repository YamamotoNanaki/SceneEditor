#include "DebugCamera.h"
#include "Input.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "IFMath.h"

void IF::DebugCamera::Update()
{
	Input* input = Input::Instance();
	static bool flag;
#ifdef _DEBUG
	flag = false;
#else
	flag = true;
#endif
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
