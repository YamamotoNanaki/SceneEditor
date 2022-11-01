#include "ObjectManager.h"
#include "ModelManager.h"
#include <cassert>
#include "imgui.h"
#include "Debug.h"


using namespace IF;
using namespace std;

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
		if (com->WeightSaving(800))com->Draw();
	}
}

void IF::ObjectManager::Update()
{
	auto buff = objList;
	for (auto com : buff)
	{
		if (com->WeightSaving(800))com->Update();
	}
	objListSize = objList.size();
}

void IF::ObjectManager::CollisionInitialize()
{
	for (auto com : objList)
	{
		com->CollisionUpdate();
	}
}

Primitive* IF::ObjectManager::GetPrimitive(std::string tag)
{
	for (auto com : objList)
	{
		if (com->tag == tag)
		{
			return com->GetPrimitive();
		}
	}
	return nullptr;
}

Primitive* IF::ObjectManager::GetPrimitiveName(std::string objName)
{
	for (auto com : objList)
	{
		if (com->GetObjName() == objName)
		{
			return com->GetPrimitive();
		}
	}
	return nullptr;
}

Primitive* IF::ObjectManager::GetPrimitiveNumber(int& num, std::string objName)
{
	if (num >= objList.size())return nullptr;
	auto itr = objList.begin();
	for (int i = 0; i < num; i++)itr++;
	auto com = *(itr);
	if (com->GetObjName() != objName)
	{
		num++;
		return GetPrimitiveNumber(num, objName);
	}
	return com->GetPrimitive();
}

#ifdef _DEBUG
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
		static int type = NotPri;
		if (ImGui::TreeNode("Collision"))
		{
			ImGui::RadioButton("Ray", &type, RayPri);
			ImGui::SameLine();
			ImGui::RadioButton("Sphere", &type, SpherePri);
			ImGui::RadioButton("Plane", &type, PlanePri);
			ImGui::SameLine();
			ImGui::RadioButton("Box", &type, BoxPri);
			ImGui::RadioButton("CircleXY", &type, CircleXYPri);
			ImGui::SameLine();
			ImGui::RadioButton("Not", &type, NotPri);
			ImGui::TreePop();
		}
		int old = objn;
		static bool flag = true;
		if (ImGui::TreeNode("ObjectName"))
		{
			//ImGui::RadioButton("Normal", &objn, tagName::Normal);
			//ImGui::RadioButton("Player", &objn, tagName::Player);
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
			//if (objn == tagName::Normal)tag = "Normal";
			//if (objn == tagName::Player)tag = "Player";
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
				if (objn == 0)
				{
					Normal* b = Add<Normal>(ModelManager::Instance()->GetModel(model), tag, board);
					if (b == nullptr)
					{
						error = 2;
					}
					flag = true;
					if (type != NotPri)
					{
						b->SetCollision(type);
					}
					b->SetPos(startpos);
				}
				//if (objn == tagName::Player) {
				//	Player* b = Add<Player>(ModelManager::Instance()->GetModel(model), tag, board);
				//	if (b == nullptr)
				//	{
				//		error = 2;
				//	}
				//	flag = true;
				//	if (type != NotPri)
				//	{
				//		b->SetCollision(type);
				//	}
				//	b->SetPos(startpos);
				//}
				//if (objn == tagName::Enemy) {
				//	Enemy* b = Add<Enemy>(ModelManager::Instance()->GetModel(model), tag, board);
				//	if (b == nullptr)
				//	{
				//		error = 2;
				//	}
				//	flag = true;
				//	if (type != NotPri)
				//	{
				//		b->SetCollision(type);
				//	}
				//	b->SetPos(startpos);
				//}
				//if (objn == tagName::Wall) {
				//	Wall* b = Add<Wall>(ModelManager::Instance()->GetModel(model), tag, board);
				//	if (b == nullptr)
				//	{
				//		error = 2;
				//	}
				//	flag = true;
				//	if (type != NotPri)
				//	{
				//		b->SetCollision(type);
				//	}
				//	b->SetPos(startpos);
				//}
				//if (objn == tagName::NoBreakWall) {
				//	NoBreakWall* b = Add<NoBreakWall>(ModelManager::Instance()->GetModel(model), tag, board);
				//	if (b == nullptr)
				//	{
				//		error = 2;
				//	}
				//	flag = true;
				//	if (type != NotPri)
				//	{
				//		b->SetCollision(type);
				//	}
				//	b->SetPos(startpos);
				//}
				//if (objn == tagName::JumpPad) {
				//	JumpPad* b = Add<JumpPad>(ModelManager::Instance()->GetModel(model), tag, board);
				//	if (b == nullptr)
				//	{
				//		error = 2;
				//	}
				//	flag = true;
				//	if (type != NotPri)
				//	{
				//		b->SetCollision(type);
				//	}
				//	b->SetPos(startpos);
				//}
				//if (objn == tagName::DashArea) {
				//	DashArea* b = Add<DashArea>(ModelManager::Instance()->GetModel(model), tag, board);
				//	if (b == nullptr)
				//	{
				//		error = 2;
				//	}
				//	flag = true;
				//	if (type != NotPri)
				//	{
				//		b->SetCollision(type);
				//	}
				//	b->SetPos(startpos);
				//}
				//if (objn == tagName::ReSpownWall) {
				//	ReSpownWall* b = Add<ReSpownWall>(ModelManager::Instance()->GetModel(model), tag, board);
				//	if (b == nullptr)
				//	{
				//		error = 2;
				//	}
				//	flag = true;
				//	if (type != NotPri)
				//	{
				//		b->SetCollision(type);
				//	}
				//	b->SetPos(startpos);
				//}
				//if (objn == tagName::Item) {
				//	Item* b = Add<Item>(ModelManager::Instance()->GetModel(model), tag, board);
				//	if (b == nullptr)
				//	{
				//		error = 2;
				//	}
				//	flag = true;
				//	if (type != NotPri)
				//	{
				//		b->SetCollision(type);
				//	}
				//	b->SetPos(startpos);
				//}
				//if (objn == tagName::GoleArea) {
				//	GoleArea* b = Add<GoleArea>(ModelManager::Instance()->GetModel(model), tag, board);
				//	if (b == nullptr)
				//	{
				//		error = 2;
				//	}
				//	flag = true;
				//	if (type != NotPri)
				//	{
				//		b->SetCollision(type);
				//	}
				//	b->SetPos(startpos);
				//}
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
		//if (a == "Item")
		//{
		//	Item* buff = dynamic_cast<Item*>(com);
		//	j["object"]["object"][i]["Item"] = buff->itemNum;
		//}
		//if (a == "Wall")
		//{
		//	Wall* buff = dynamic_cast<Wall*>(com);
		//	buff->OutputJson(j, i);
		//}
		//if (a == "NoBreakWall")
		//{
		//	NoBreakWall* buff = dynamic_cast<NoBreakWall*>(com);
		//	buff->OutputJson(j, i);
		//}
		//if (a == "ReSpownWall")
		//{
		//	ReSpownWall* buff = dynamic_cast<ReSpownWall*>(com);
		//	buff->OutputJson(j, i);
		//}
		//if (a == "GoleArea")
		//{
		//	GoleArea* buff = dynamic_cast<GoleArea*>(com);
		//	buff->OutputJson(j, i);
		//}
		j["object"]["object"][i]["tag"] = com->tag;
		j["object"]["object"][i]["model"] = com->GetModelTag();
		j["object"]["object"][i]["pos"]["x"] = com->GetPos().x;
		j["object"]["object"][i]["pos"]["y"] = com->GetPos().y;
		j["object"]["object"][i]["pos"]["z"] = com->GetPos().z;
		j["object"]["object"][i]["rot"]["x"] = com->GetRota().x;
		j["object"]["object"][i]["rot"]["y"] = com->GetRota().y;
		j["object"]["object"][i]["rot"]["z"] = com->GetRota().z;
		j["object"]["object"][i]["sca"]["x"] = com->GetScale().x;
		j["object"]["object"][i]["sca"]["y"] = com->GetScale().y;
		j["object"]["object"][i]["sca"]["z"] = com->GetScale().z;
		j["object"]["object"][i]["color"]["x"] = com->GetColor().x;
		j["object"]["object"][i]["color"]["y"] = com->GetColor().y;
		j["object"]["object"][i]["color"]["z"] = com->GetColor().z;
		j["object"]["object"][i]["color"]["w"] = com->GetColor().w;
		j["object"]["object"][i]["BillBoard"] = (int)com->GetBillBoard();
		j["object"]["object"][i]["collision"] = (int)com->GetCollision();
		j["object"]["object"][i]["prefab"] = com->GetPrefab();
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
#endif

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
