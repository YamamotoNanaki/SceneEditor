#include "CameraManager.h"
#include "ObjectManager.h"
#include "ImGui.h"

using namespace ImGui;

void IF::CameraManager::AutoUpdate()
{
	std::string t = ObjectManager::Instance()->GetCamera()->tag;
	for (auto com : cameraList)
	{
		if (com->tag == t)
		{
			com->Update();
			return;
		}
	}
}

//#ifdef _DEBUG
void IF::CameraManager::GUI()
{
	ImGui::Begin("Camera");
	auto buff = cameraList;
	for (auto com : buff)
	{
		com->GUI();
	}
	ImGui::End();
}

void IF::CameraManager::OutputJson(nlohmann::json& j)
{
	int i = 0;
	for (auto com : cameraList)
	{
		j["camera"][i]["tag"] = com->tag;
		j["camera"][i]["CameraName"] = com->GetName();
		j["camera"][i]["eye"]["x"] = com->GetEye()->x;
		j["camera"][i]["eye"]["y"] = com->GetEye()->y;
		j["camera"][i]["eye"]["z"] = com->GetEye()->z;
		//j["camera"][i]["rota"] = com->GetRota();
		j["camera"][i]["target"]["x"] = com->GetTarget().x;
		j["camera"][i]["target"]["y"] = com->GetTarget().y;
		j["camera"][i]["target"]["z"] = com->GetTarget().z;
		i++;
	}
}
//#endif