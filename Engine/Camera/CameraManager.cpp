#include "CameraManager.h"
#include "ImGui.h"

using namespace ImGui;

#ifdef _DEBUG
void IF::CameraManager::GUI()
{
	auto buff = cameraList;
	for (auto com : buff)
	{
		com->GUI();
	}
}

void IF::CameraManager::OutputJson(nlohmann::json& j)
{
	int i = 0;
	for (auto com : cameraList)
	{
		j["camera"][i]["tag"] = com->tag;
		j["camera"][i]["eye"]["x"] = com->GetEye()->x;
		j["camera"][i]["eye"]["y"] = com->GetEye()->y;
		j["camera"][i]["eye"]["z"] = com->GetEye()->z;
		j["camera"][i]["target"]["x"] = com->GetTarget().x;
		j["camera"][i]["target"]["y"] = com->GetTarget().y;
		j["camera"][i]["target"]["z"] = com->GetTarget().z;
		Camera* buff = dynamic_cast<Camera*>(com);
		if (buff != nullptr)
		{
			j["camera"][i]["type"] = true;
			j["camera"][i]["cameratype"] = com->GetType();
		}
		else j["camera"][i]["type"] = false;
		i++;
	}
}
#endif