#include "NormalObj.h"
#include "imgui.h"
#include "ObjectManager.h"
#include "CameraManager.h"
#include "Collision.h"
#include "Sound.h"
#include "Input.h"

using namespace IF;
using namespace std;

//���ɍ�����N���X�Ɠ������O������
const std::string Normal::objName = "Normal";

//? ����������
void IF::Normal::ClassInitialize()
{
}

//? �A�b�v�f�[�g
void IF::Normal::ClassUpdate()
{
	if (Input::Instance()->KeyDown(KEY::RIGHT))	obj.position.x += 0.5;
	if (Input::Instance()->KeyDown(KEY::LEFT))	obj.position.x -= 0.5;
	if (Input::Instance()->KeyDown(KEY::DOWN))	obj.position.y -= 0.5;
	if (Input::Instance()->KeyDown(KEY::UP))	obj.position.y += 0.5;
}

