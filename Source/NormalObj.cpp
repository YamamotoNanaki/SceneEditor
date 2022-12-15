#include "NormalObj.h"
#include "imgui.h"
#include "ObjectManager.h"
#include "CameraManager.h"
#include "Collision.h"
#include "Sound.h"
#include "Input.h"

using namespace IF;
using namespace std;

//中に作ったクラスと同じ名前を書く
const std::string Normal::objName = "Normal";

//? 初期化処理
void IF::Normal::ClassInitialize()
{
}

//? アップデート
void IF::Normal::ClassUpdate()
{
	if (Input::Instance()->KeyDown(KEY::RIGHT))	obj.position.x += 0.5;
	if (Input::Instance()->KeyDown(KEY::LEFT))	obj.position.x -= 0.5;
	if (Input::Instance()->KeyDown(KEY::DOWN))	obj.position.y -= 0.5;
	if (Input::Instance()->KeyDown(KEY::UP))	obj.position.y += 0.5;
}

