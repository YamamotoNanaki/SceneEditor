#include "Camera.h"
#include "Input.h"
#include "Quaternion.h"
#include "ObjectManager.h"
#include "Ease.h"
#include "NormalObj.h"

using namespace IF;

const std::string Camera::cameraName = "Camera";

static float rot = 0.01f;
static Normal* n = nullptr;
void IF::Camera::Update()
{
	if (camera)
	{
		Float3 p = n->GetPos();
		matView->target = { p.x,1,p.z };
		matView->eye = { p.x,10,p.z - 15 };
		matView->Update();
	}
	else
	{
		if (!once)
		{
			once = true;
			spos = cpos;
			star = ctar;
			supv = cupv;
		}
		upAngle = 0;
		rightAngle = 0;
#ifdef _DEBUG
#else
		if (Input::Instance()->KeyDown(KEY::RIGHT))rightAngle = -rot;
		if (Input::Instance()->KeyDown(KEY::LEFT))rightAngle = rot;
		if (Input::Instance()->KeyDown(KEY::UP))upAngle = rot;
		if (Input::Instance()->KeyDown(KEY::DOWN))upAngle = -rot;
		if (Input::Instance()->KeyDown(DIK_C))
		{
			cpos = spos;
			ctar = star;
			cupv = supv;
		}
#endif

		Quaternion qRAngle = quaternion(cupv, rightAngle);
		Vector3 side = cupv.Cross(ctar - cpos);
		side.Normalize();
		Quaternion qUAngle = quaternion(side, upAngle);

		Quaternion q = qRAngle * qUAngle;

		Quaternion qpos = quaternion(cpos.x, cpos.y, cpos.z, 0.0f);
		Quaternion qq = Conjugate(q);

		qpos = q * qpos * qq;
		cpos = { qpos.x,qpos.y,qpos.z };

		Quaternion qup = quaternion(cupv.x, cupv.y, cupv.z, 0.0f);
		qup = q * qup * qq;

		cupv = GetAxis(qup);

		matView->eye = SetFloat3(cpos);
		matView->target = SetFloat3(ctar);
		matView->up = SetFloat3(cupv);
		matView->Update();
	}
}

IF::Camera::~Camera()
{
	n = nullptr;
}

void IF::Camera::CameraInitialize(Normal* normal)
{
	matView->target = { 0,3,0 };
	matView->eye = { 0,10,-7 };
	matView->Update();
	cpos = SetVector3(matView->eye);
	ctar = SetVector3(matView->target);
	cupv = SetVector3(matView->up);
	n = normal;
	camera = true;
}

static const int starttime = 70;
