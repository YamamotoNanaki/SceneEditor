#include "ObjectManager.h"
#include "ModelManager.h"
#include <cassert>
#include "imgui.h"
#include "Debug.h"
#include "CollisionObj.h"
#include "SphereObject.h"
#include "PlaneObj.h"
#include "TriangleObj.h"


using namespace IF;
using namespace std;

enum tagName
{
	Normal, CollisionObj, SphereObject, PlaneObj, TriangleObj
};

IF::ObjectManager::~ObjectManager()
{
	for (auto com : objList)delete com;
	objList.clear();
	assert(objList.empty() && "インスタンスを破棄できません");
}

IF::ObjectManager* IF::ObjectManager::Instance()
{
	static ObjectManager* inst = DEBUG_NEW ObjectManager;
	return inst;
}

void IF::ObjectManager::DeleteInstance()
{
	delete ObjectManager::Instance();
}

void IF::ObjectManager::Draw()
{
	for (auto com : objList)
	{
		if (com->WeightSaving(150))com->Draw();
	}
}

void IF::ObjectManager::OutLineDraw()
{
	for (auto com : objList)
	{
		if (com->WeightSaving(800))com->OutLineDraw();
	}
}

void IF::ObjectManager::Update()
{
	auto buff = objList;
	for (auto com : buff)
	{
		if (com->WeightSaving(150))com->Update();
	}
	objListSize = objList.size();
}

void IF::ObjectManager::IntputJson(nlohmann::json& j)
{
	for (auto i : j["object"]["object"])
	{
		CObject* ptr = nullptr;
		if ("Normal" == i["ObjectName"])ptr = Add<Normal>(ModelManager::Instance()->GetModel(i["model"]), i["tag"], i["BillBoard"], 0);
		else if ("CollisionObj" == i["ObjectName"])ptr = Add<CollisionObj>(ModelManager::Instance()->GetModel(i["model"]), i["tag"], i["BillBoard"], 0);
		else if ("SphereObject" == i["ObjectName"])ptr = Add<SphereObject>(ModelManager::Instance()->GetModel(i["model"]), i["tag"], i["BillBoard"], 0);
		else if ("PlaneObj" == i["ObjectName"])ptr = Add<PlaneObj>(ModelManager::Instance()->GetModel(i["model"]), i["tag"], i["BillBoard"], 0);
		else if ("TriangleObj" == i["ObjectName"])ptr = Add<TriangleObj>(ModelManager::Instance()->GetModel(i["model"]), i["tag"], i["BillBoard"], 0);

		if (ptr != nullptr)
		{
			ptr->InputJson(i);
			ptr->prefab = i["prefab"];
		}
	}
}

//#ifdef _DEBUG
void IF::ObjectManager::GUI()
{
	auto buff = objList;
	static bool d = false;
	ImGui::Begin("Object");
	ImGui::Checkbox("DeleteFlag", &d);
	for (auto com : buff)
	{
		if (ImGui::TreeNode(com->tag.c_str())) {
			com->GUI();
			if (d)
			{
				if (ImGui::Button("Delete"))
				{
					objList.remove(com);
					delete com;
				}
			}
			ImGui::TreePop();
		}
	}
	if (ImGui::CollapsingHeader("NewObject"))
	{
		static char tagC[256];
		static string tag;
		static string model = ModelManager::Instance()->GUIRadio();
		static int board = 0;
		static int objn = 0;
		static int error = 0;
		if (error)ImGui::Text("error : %s", error == 1 ? "tag == \0" : "model == nullptr");
		if (ImGui::TreeNode("setModel"))
		{
			model = ModelManager::Instance()->GUIRadio();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("BillBoard"))
		{
			ImGui::RadioButton("NotBillBoard", &board, 0);
			ImGui::RadioButton("BillBoard", &board, 1);
			ImGui::RadioButton("YBillBoard", &board, 2);
			ImGui::TreePop();
		}
		int old = objn;
		static bool flag = true;

		if (ImGui::TreeNode("ObjectName"))
		{
			ImGui::RadioButton("Normal", &objn, tagName::Normal);
			ImGui::RadioButton("CollisionObj", &objn, tagName::CollisionObj);
			ImGui::RadioButton("PlaneObj", &objn, tagName::PlaneObj);
			ImGui::RadioButton("SphereObject", &objn, tagName::SphereObject);
			ImGui::RadioButton("TriangleObj", &objn, tagName::TriangleObj);
			//ImGui::RadioButton("Enemy", &objn, tagName::Enemy);
			//ImGui::RadioButton("Wall", &objn, tagName::Wall);
			//ImGui::RadioButton("NoBreakWall", &objn, tagName::NoBreakWall);
			//ImGui::RadioButton("JumpPad", &objn, tagName::JumpPad);
			//ImGui::RadioButton("DashArea", &objn, tagName::DashArea);
			//ImGui::RadioButton("ReSpownWall", &objn, tagName::ReSpownWall);
			//ImGui::RadioButton("Item", &objn, tagName::Item);
			//ImGui::RadioButton("GoleArea", &objn, tagName::GoleArea);
			ImGui::TreePop();
		}
		if (old != objn)
		{
			flag = true;
		}
		if (flag == true)
		{
			if (objn == tagName::Normal)tag = "Normal";
			if (objn == tagName::CollisionObj)tag = "CollisionObj";
			if (objn == tagName::PlaneObj)tag = "PlaneObj";
			if (objn == tagName::SphereObject)tag = "SphereObject";
			if (objn == tagName::TriangleObj)tag = "TriangleObj";

			//if (objn == tagName::Enemy)tag = "Enemy";
			//if (objn == tagName::Wall)tag = "Wall";
			//if (objn == tagName::NoBreakWall)tag = "NoBreakWall";
			//if (objn == tagName::JumpPad)tag = "JumpPad";
			//if (objn == tagName::DashArea)tag = "DashArea";
			//if (objn == tagName::ReSpownWall)tag = "ReSpownWall";
			//if (objn == tagName::Item)tag = "Item";
			//if (objn == tagName::GoleArea)tag = "GoleArea";
			int num = this->GetTagNum(tag);
			int d = 0;
			for (int i = 1; i <= num; i *= 10)
			{
				d++;
			}
			int digit = d;
			for (d; d > 0; d--)
			{
				int a = 1;
				for (int i = 0; i < d - 1; i++)a *= 10;
				int b = num / a;
				tag += (b + 48);
				num -= b * a;
			}
			strcpy_s(tagC, tag.c_str());
			flag = false;
		}

		ImGui::InputText("Tag", tagC, sizeof(tagC));
		tag = tagC;
		if (ImGui::Button("Add"))
		{
			string s = tag;
			if (s == "\0")error = 1;
			else
			{
				Float3 startpos = camera->GetTarget();
				startpos.z = 0;
				CObject* b = nullptr;
				if (objn == tagName::Normal)
				{
					b = Add<Normal>(ModelManager::Instance()->GetModel(model), tag, board);
				}
				else if (objn == tagName::CollisionObj)
				{
					b = Add<CollisionObj>(ModelManager::Instance()->GetModel(model), tag, board);
				}
				else if (objn == tagName::PlaneObj)
				{
					b = Add<PlaneObj>(ModelManager::Instance()->GetModel(model), tag, board);
				}
				else if (objn == tagName::SphereObject)
				{
					b = Add<SphereObject>(ModelManager::Instance()->GetModel(model), tag, board);
				}
				else if (objn == tagName::TriangleObj)
				{
					b = Add<TriangleObj>(ModelManager::Instance()->GetModel(model), tag, board);
				}
				if (b == nullptr)
				{
					error = 2;
				}
				flag = true;
				b->SetPos(startpos);
			}
		}
	}
	if (d)
	{
		if (ImGui::TreeNode("AllDelete"))
		{
			if (ImGui::Button("AllDelete"))
			{
				Reset();
				Add<Normal>(ModelManager::Instance()->GetModel("dome"), "normal");
			}
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

std::string IF::ObjectManager::GUIRadio()
{
	int i = 0;
	static int mode = 0;
	std::string r;
	for (auto com : objList)
	{
		if (i % 3)ImGui::SameLine();
		ImGui::RadioButton(com->tag.c_str(), &mode, i);
		if (mode == i)
		{
			r = com->tag;
		}
		i++;
	}
	return r;
}

void IF::ObjectManager::OutputJson(nlohmann::json& j)
{
	int i = 0;
	//if (camera->tag == "debug")j["object"]["camera"] = "mainCamera";
	j["object"]["camera"] = camera->tag;
	for (auto com : objList)
	{
		string a = com->GetObjName();
		j["object"]["object"][i]["ObjectName"] = a;
		com->OutputJson(j["object"]["object"][i]);
		i++;
	}
}

void IF::ObjectManager::DebugUpdate()
{
	auto buff = objList;
	for (auto com : buff)
	{
		com->DebugUpdate();
	}
	objListSize = objList.size();
}

string IF::ObjectManager::GUIGetTag()
{
	int i = 0;
	static int num = 0;
	static string tag;
	if (ImGui::CollapsingHeader("SetObject"))
	{
		for (auto com : objList)
		{
			ImGui::RadioButton(com->tag.c_str(), &num, i);
			if (num == i)tag = com->tag;
			i++;
		}
	}
	return tag;
}
//#endif

void IF::ObjectManager::Delete(std::string tag)
{
	for (auto com : objList)
	{
		if (com->tag == tag)
		{
			objList.remove(com);
			delete com;
			//com = nullptr;
			return;
		}
	}
}
