#include "Camera.h"
#include "Input.h"
#include "Quaternion.h"

using namespace IF;

const std::string Camera::cameraName = "Camera";

static enum camerainfo
{
	Start, Boss, BossFront
};
static float rot = 0.01f;
void IF::Camera::Update()
{
	//upAngle = 0;
	//rightAngle = 0;

	//Quaternion qRAngle = quaternion(cupv, rightAngle);
	//Vector3 side = cupv.Cross(ctar - cpos);
	//side.Normalize();
	//Quaternion qUAngle = quaternion(side, upAngle);

	//Quaternion q = qRAngle * qUAngle;

	//Quaternion qpos = quaternion(cpos.x, cpos.y, cpos.z, 0.0f);
	//Quaternion qq = Conjugate(q);

	//qpos = q * qpos * qq;
	//cpos = { qpos.x,qpos.y,qpos.z };

	//Quaternion qup = quaternion(cupv.x, cupv.y, cupv.z, 0.0f);
	//qup = q * qup * qq;

	//cupv = GetAxis(qup);

	if (Input::Instance()->KeyDown(DIK_A))
	{
		matView->eye.x -= 0.5f;
		matView->target.x -= 0.5f;
	}
	if (Input::Instance()->KeyDown(DIK_D))
	{
		matView->eye.x += 0.5f;
		matView->target.x += 0.5f;
	}
	if (Input::Instance()->KeyDown(DIK_W))
	{
		matView->eye.y += 0.5f;
		matView->target.y += 0.5f;
	}
	if (Input::Instance()->KeyDown(DIK_S))
	{
		matView->eye.y -= 0.5f;
		matView->target.y -= 0.5f;
	}
	//matView->eye = SetFloat3(cpos);
	//matView->target = SetFloat3(ctar);
	//matView->up = SetFloat3(cupv);
	matView->Update();
}

static const int starttime = 70;
