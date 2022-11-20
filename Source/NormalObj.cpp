#include "NormalObj.h"
#include "imgui.h"
#include "ObjectManager.h"
#include "CameraManager.h"
#include "Collision.h"
#include "Sound.h"
#include "Input.h"
#include "Ease.h"

using namespace IF;
using namespace std;

const std::string Normal::objName = "Normal";

void IF::Normal::ClassUpdate()
{
	static bool f = false;
	static bool f2 = false;
	if (Input::Instance()->KeyDown(DIK_R))
	{
		obj.explosion = 0;
		obj.polygonSize = 1;
		obj.gravity = 0;
		f = false;
		f2 = false;
	}
	if (explosionF)
	{
		static Timer t;
		if (Input::Instance()->KeyTriggere(DIK_1) || Input::Instance()->KeyTriggere(DIK_0))
		{
			f = true;
			t.Set(60);
		}
		if (f)
		{
			t.SafeUpdate();
			obj.explosion += Ease::Lerp(1, 0.7, t.GetEndTime(), t.NowTime());
			obj.gravity += Ease::InQuad(0, 2, t.GetEndTime(), t.NowTime());
		}
	}
	if (polygonSizeF)
	{
		static Timer t;
		if (Input::Instance()->KeyTriggere(DIK_2) || Input::Instance()->KeyTriggere(DIK_0))
		{
			f2 = true;
			t.Set(60);
		}
		if (f2)
		{
			t.SafeUpdate();
			obj.polygonSize = Ease::InQuad(1, 0, t.GetEndTime(), t.NowTime());
		}
	}
}

void IF::Normal::ClassInputJson(nlohmann::json& j)
{
	explosionF = j["explosionF"];
	polygonSizeF = j["polygonSizeF"];
}

#ifdef _DEBUG
void IF::Normal::ClassUI()
{
	if (ImGui::CollapsingHeader("ClassOptions"))
	{
		ImGui::Checkbox("explosionF", &explosionF);
		ImGui::Checkbox("polygonSizeF", &polygonSizeF);
	}
}

void IF::Normal::ClassOutputJson(nlohmann::json& j)
{
	j["explosionF"] = explosionF;
	j["polygonSizeF"] = polygonSizeF;
}
#endif
