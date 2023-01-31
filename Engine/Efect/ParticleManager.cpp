#include "ParticleManager.h"
#include "ObjectManager.h"
#include "ImGui.h"

using namespace IF;
using namespace std;

int IF::ParticleManager::GetTagNum(string tag)
{
	int i = 0;
	for (std::unique_ptr<Emitter>& com : emitterList)
	{
		if (i == 0)
		{
			if (GetNum(tag))
			{
				i++;
			}
		}
		else if (i < 10)
		{
			std::string a = tag + (char)(i + 48);
			if (GetNum(a))
			{
				i++;
			}
		}
		else if (i < 100)
		{
			std::string a = tag + (char)(i / 10 + 48) + (char)(i % 10 + 48);
			if (GetNum(a))
			{
				i++;
			}
		}
		else if (i < 1000)
		{
			int b = i / 100;
			int c = i / 10;
			c -= b * 10;
			std::string a = tag + (char)(b + 48) + (char)(c + 48) + (char)(i % 10 + 48);
			if (GetNum(a))
			{
				i++;
			}
		}
		else
		{
			int b = i / 1000;
			int c = i / 100;
			c -= b * 10;
			int d = i / 10;
			d -= b * 100;
			d -= c * 10;
			std::string a = tag + (char)(b + 48) + (char)(c + 48) + (char)(d + 48) + (char)(i % 10 + 48);
			if (GetNum(a))
			{
				i++;
			}
		}
	}
	return i;
}

void IF::ParticleManager::Add()
{
	emitterList.push_back(make_unique<Emitter>());
}

void IF::ParticleManager::Update()
{
	Particle::camera = ObjectManager::Instance()->GetCamera();
	for (unique_ptr<Emitter>& buff : emitterList)
	{
		buff->SetPositionFollow();
		/*if(buff->WeightSaving(800))*/buff->Update();
	}
}

void IF::ParticleManager::Draw()
{
	for (unique_ptr<Emitter>& buff : emitterList)
	{
		//if (buff->WeightSaving(800))
		{
			buff->Draw();
		}
	}
}

void IF::ParticleManager::DrawPostEffect(ID3D12RootSignature* root)
{
	for (unique_ptr<Emitter>& buff : emitterList)
	{
		//if (buff->WeightSaving(800))
		{
			buff->DrawBefore(root);
			buff->DrawPostEffect();
			buff->DrawAfter();
		}
	}
}

void IF::ParticleManager::Reset()
{
	emitterList.clear();
}

void IF::ParticleManager::Start(std::string tag)
{
	for (unique_ptr<Emitter>& buff : emitterList)
	{
		if (buff->tag == tag)
		{
			buff->generateF = true;
			return;
		}
	}
}

void IF::ParticleManager::End(std::string tag)
{
	for (unique_ptr<Emitter>& buff : emitterList)
	{
		if (buff->tag == tag)
		{
			buff->generateF = false;
			return;
		}
	}
}

void IF::ParticleManager::SetObject(std::string ptag, std::string& otag)
{
	if (&otag == nullptr)return;
	for (unique_ptr<Emitter>& buff : emitterList)
	{
		if (buff->tag == ptag)
		{
			buff->followtag = otag;
			return;
		}
	}
}

ParticleManager* IF::ParticleManager::Instance()
{
	static ParticleManager inst;
	return &inst;
}

void IF::ParticleManager::InputJson(nlohmann::json& j)
{
	for (auto i : j["Particle"])
	{
		Add();
		unique_ptr<Emitter>& buff = emitterList.back();
		buff->InputJson(i);
	}
}

//#ifdef _DEBUG
void IF::ParticleManager::GUI()
{
	static bool d = false;
	ImGui::Begin("Particle");
	ImGui::Checkbox("delete", &d);
	if (debug)
	{
		if (ImGui::Button("DebugStop"))debug = false;
	}
	else
	{
		if (ImGui::Button("DebugStart"))debug = true;
	}
	for (unique_ptr<Emitter>& buff : emitterList)
	{
		if (ImGui::TreeNode(buff->tag.c_str()))
		{
			if (d)
			{
				if (ImGui::Button("Delete"))
				{
					buff->emitterDeleteFlag = true;
				}
			}
			buff->GUI();
			ImGui::TreePop();
		}
	}
	emitterList.remove_if([](std::unique_ptr<Emitter>& p) {return p->emitterDeleteFlag; });
	if (ImGui::CollapsingHeader("NewParticle"))
	{
		static char tagC[256];
		static string tag;
		static bool flag = true;
		if (flag == true)
		{
			tag = "Particle";
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
			Add();
			unique_ptr<Emitter>& buff = emitterList.back();
			buff->tag = tag;
			flag = true;
			debug = false;
		}
	}
	ImGui::End();
}

void IF::ParticleManager::DebugUpdate()
{
	if (debug)
	{
		this->Update();
	}
}

void IF::ParticleManager::OutputJson(nlohmann::json& j)
{
	int i = 0;
	for (unique_ptr<Emitter>& buff : emitterList)
	{
		buff->OutputJson(j["Particle"][i]);
		i++;
	}
}
//#endif
