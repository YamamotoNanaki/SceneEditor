#include "Emitter.h"
#include "ImGui.h"
#include "Texture.h"
#include "ObjectManager.h"
#include "DirectX12.h"
#include "Rand.h"
#include "Graphic.h"

using namespace IF;
using namespace std;

IF::Emitter::Emitter()
{
	postEffect = DEBUG_NEW PostEffect;
	postEffect->Initialize();
}

void IF::Emitter::StaticInitialize()
{
	Particle::VIInitialize();
}

void IF::Emitter::Add()
{
	if (generateF && DrawF)
	{
		timer.SafeUpdate();
		if ((maxnum == 0 || num < maxnum) && timer.IsEnd())
		{
			timer.Set(addTime);
			if (generateTime > 0)
			{
				if (gtimer.GetEndTime() == 0)gtimer.Set(generateTime);
			}
			for (int i = 0; i < addnum; i++)
			{
				if (maxnum != 0 && num > maxnum)break;
				particles.push_back(make_unique<Particle>());
				unique_ptr<Particle>& buff = particles.back();

				float spe[3];
				float sca[3];
				float end[3];
				buff->speedF = speedF;
				buff->colorF = colorF;
				for (int i = 0; i < 3; i++)
				{
					spe[i] = Rand::GetRandF(-speedRange[i], speedRange[i]) + speed[i];
					buff->speed[i] = spe[i];
					spe[i] = Rand::GetRandF(-speedRange[i], speedRange[i]) + startSpeed[i];
					buff->endSpeed[i] = endSpeed[i];
					buff->startSpeed[i] = spe[i];
					buff->speedEase[i] = speedEase[i];
					end[i] = Rand::GetRandF(-endPosRange[i], endPosRange[i]) + endposition[i];
					buff->endposition[i] = end[i];
					buff->posEase[i] = posEase[i];
					sca[i] = Rand::GetRandF(-scaleRange[i], scaleRange[i]) + scale[i];
					buff->scale[i] = sca[i];
					sca[i] = Rand::GetRandF(-scaleRange[i], scaleRange[i]) + startScale[i];
					buff->endScale[i] = endScale[i];
					buff->startScale[i] = sca[i];
					buff->scaleEase[i] = scaleEase[i];
					buff->color[i] = color[i];
					buff->startcolor[i] = startcolor[i];
					buff->endcolor[i] = endcolor[i];
					buff->colorEase[i] = colorEase[i];
				}
				if (scaleRangeFlag)
				{
					sca[0] = Rand::GetRandF(-scaleRange[0], scaleRange[0]) + scale[i];
					for (int i = 0; i < 3; i++)
					{
						buff->scale[i] = sca[0];
					}
				}
				buff->color[3] = color[3];
				buff->startcolor[3] = startcolor[3];
				buff->endcolor[3] = endcolor[3];
				buff->colorEase[3] = colorEase[3];

				//‰ñ“]
				buff->rota = rota + Rand::GetRandF(0, rotarandom);
				buff->rotaF = rotaF;
				buff->rotaspeed = rotaspeed;
				buff->rotastart = rotastart + Rand::GetRandF(0, rotarandom);
				buff->rotaend = rotaend;
				buff->rotaEase = rotaEase;

				if (follow)
				{
					Float3 p = ObjectManager::Instance()->GetPosition(followtag);
					if (posRangeF)
					{
						Float3 s = ObjectManager::Instance()->GetScale(followtag);
						addPosRange[0] = s.x + followRangeOffset[0];
						addPosRange[1] = s.y + followRangeOffset[1];
						addPosRange[2] = s.z + followRangeOffset[2];
					}
					p.x = Rand::GetRandF(-addPosRange[0], addPosRange[0]) + p.x + followPosOffset[0];
					p.y = Rand::GetRandF(-addPosRange[1], addPosRange[1]) + p.y + followPosOffset[1];
					p.z = Rand::GetRandF(-addPosRange[2], addPosRange[2]) + p.z + followPosOffset[2];
					buff->position = p;
				}
				else
				{
					float pos[3];
					for (int i = 0; i < 3; i++)
					{
						pos[i] = Rand::GetRandF(-addPosRange[i], addPosRange[i]) + position[i];
						buff->startposition[i] = pos[i];
					}
					buff->position = pos;
				}
				buff->posF = posF;

				buff->scaleF = scaleF;

				buff->deleteTime = deleteTime;
				buff->setSpeedPosFlag = setSpeedPosFlag;
				buff->timer.Set(deleteTime);

				buff->isGravity = isGravity;
				buff->gravity = gravity;

				buff->Initialize();
			}
		}
		if (generateTime > 0)
		{
			gtimer.SafeUpdate();
			if (gtimer.IsEnd())
			{
				gtimer.Set(0);
				timer.Set(0);
				generateF = false;
			}
		}
	}
}

void IF::Emitter::DrawBefore(ID3D12RootSignature* root)
{
	Particle::DrawBefore(root);
	postEffect->DrawBefore();
	Graphic::Instance()->DrawBlendMode((Blend::Blend)Blendmode);
}

void IF::Emitter::DrawAfter()
{
	postEffect->DrawAfter();
}

void IF::Emitter::Update()
{
	if (this->tag == "Particle32")this->DrawF = true;
	Add();
	for (unique_ptr<Particle>& buff : particles)
	{
		buff->Update();
		buff->MatUpdate();
	}
	particles.remove_if([](std::unique_ptr<Particle>& p) {return p->deleteFlag; });
}

void IF::Emitter::DrawPostEffect()
{
	Texture::Instance()->SetTexture(tex);
	for (unique_ptr<Particle>& buff : particles)
	{
		buff->Draw();
	}
}

void IF::Emitter::Draw()
{
	if (particles.size() == 0)return;
	postEffect->Draw(true);
}

void IF::Emitter::SetPositionFollow()
{
	if (!follow)return;
	CObject* o = ObjectManager::Instance()->GetAddress<CObject>(followtag);
	if (o == nullptr)
	{
		return;
	}
	Float3 p = o->GetPos();
	o->emitter = this;
	if (p.x == 0 && p.y == 0 && p.z == 0)return;
	position[0] = p.x;
	position[1] = p.y;
	position[2] = p.z;
}

void IF::Emitter::SetPosition(const Float3& pos)
{
	position[0] = pos.x;
	position[1] = pos.y;
	position[2] = pos.z;
}

void IF::Emitter::SetPositionRange(const Float3& posRange)
{
	addPosRange[0] = posRange.x;
	addPosRange[1] = posRange.y;
	addPosRange[2] = posRange.z;
}

void IF::Emitter::SetEndPosition(const Float3& pos)
{
	endposition[0] = pos.x;
	endposition[1] = pos.y;
	endposition[2] = pos.z;
}

void IF::Emitter::SetEndPositionRange(const Float3& posRange)
{
	endPosRange[0] = posRange.x;
	endPosRange[1] = posRange.y;
	endPosRange[2] = posRange.z;
}

void IF::Emitter::SetRota(const float& rota)
{
	this->rota = rota;
}

void IF::Emitter::SetColor(const Float4& color)
{
	this->color[0] = color.x;
	this->color[1] = color.y;
	this->color[2] = color.z;
	this->color[3] = color.w;
}

bool IF::Emitter::WeightSaving(float max)
{
	float cx = ObjectManager::Instance()->GetCamera()->cpos.x;
	float a = cx - position[0];
	if (a > 0)
	{
		if (a < max)return true;
	}
	else
	{
		if (a > -max)return true;
	}
	return false;
}

void IF::Emitter::InputJson(nlohmann::json& j)
{
	maxnum = j["maxnum"];
	addnum = j["addnum"];
	speedF = j["speedF"];
	posF = j["posF"];
	setSpeedPosFlag = j["setSpeedPosFlag"];
	scaleF = j["scaleF"];
	addTime = j["addTime"];
	flag = j["flag"];
	followtag = j["followtag"];
	follow = j["follow"];
	tex = j["tex"];
	tag = j["tag"];
	colorF = j["colorF"];
	generateF = j["generateF"];
	generateTime = j["generateTime"];
	Blendmode = j["Blendmode"];
	posRangeF = j["posRangeF"];
	isGravity = j["isGravity"];
	gravity = j["gravity"];
	rotarandom = j["rotarandom"];
	rota = j["rota"];
	rotastart = j["rotastart"];
	rotaend = j["rotaend"];
	rotaspeed = j["rotaspeed"];
	rotaF = j["rotaF"];
	rotaEase = j["rotaEase"];
	scaleRangeFlag = j["scaleRangeFlag"];
	deleteTime = j["deleteTime"];
	for (int i = 0; i < 3; i++)
	{
		endSpeed[i] = j["endSpeed"][i];
		startSpeed[i] = j["startSpeed"][i];
		speed[i] = j["speed"][i];
		speedEase[i] = j["speedEase"][i];
		position[i] = j["position"][i];
		endposition[i] = j["endposition"][i];
		endPosRange[i] = j["endPosRange"][i];
		posEase[i] = j["posEase"][i];
		scale[i] = j["scale"][i];
		endScale[i] = j["endScale"][i];
		startScale[i] = j["startScale"][i];
		scaleEase[i] = j["scaleEase"][i];
		addPosRange[i] = j["addPosRange"][i];
		speedRange[i] = j["speedRange"][i];
		addspeed[i] = j["addspeed"][i];
		scaleRange[i] = j["scaleRange"][i];
		addscale[i] = j["addscale"][i];
		deletePositionMax[i] = j["deletePositionMax"][i];
		deletePositionMin[i] = j["deletePositionMin"][i];
		followRangeOffset[i] = j["followRangeOffset"][i];
		followPosOffset[i] = j["followPosOffset"][i];
	}
	for (int i = 0; i < 4; i++)
	{
		startcolor[i] = j["startcolor"][i];
		endcolor[i] = j["endcolor"][i];
		colorEase[i] = j["colorEase"][i];
		color[i] = j["color"][i];
	}
}

//#ifdef _DEBUG
void IF::Emitter::GUI()
{
	static float dragspeed = 0.5f;
	ImGui::DragFloat("DragSpeed", &dragspeed, 0.5f);
	if (dragspeed < 0.5f)dragspeed = 0.5f;
	if (ImGui::TreeNode("generate"))
	{
		ImGui::DragInt("time to next spawn", &addTime);
		ImGui::DragInt("Time to finish generation", &generateTime);
		ImGui::Text("finish generation == 0 : Infinity");
		if (generateTime > 0)generateF = false;
		else generateF = true;
		ImGui::Text("generate flag : %s", generateF == true ? "true" : "false");
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("delete"))
	{
		ImGui::DragInt("delete time", &deleteTime);
		ImGui::DragFloat3("delete max position", deletePositionMax, dragspeed);
		ImGui::DragFloat3("delete min position", deletePositionMin, dragspeed);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("tex"))
	{
		Texture::Instance()->TexNum(&tex);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Blendmode"))
	{
		ImGui::RadioButton("Normal", &Blendmode, (int)Blend::PNORMAL);
		ImGui::RadioButton("Add", &Blendmode, (int)Blend::PADD);
		ImGui::RadioButton("Invsrc", &Blendmode, (int)Blend::PINVSRC);
		ImGui::RadioButton("Sub", &Blendmode, (int)Blend::PSUB);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Generated number"))
	{
		ImGui::Text("max number == 0 : infinite");
		ImGui::DragInt("max number", &maxnum);
		ImGui::DragInt("add number", &addnum);
		ImGui::Text("now particle num%d", num);
		ImGui::TreePop();
	}
	ImGui::Text("Choose speed and position priority");
	ImGui::Checkbox("speed : check , position : not", &setSpeedPosFlag);
	ImGui::Text("now : %s", setSpeedPosFlag == false ? "position" : "speed");
	if (ImGui::TreeNode("position"))
	{
		ImGui::Checkbox("follow position", &follow);
		if (follow)
		{
			ImGui::Checkbox("follow range", &posRangeF);
			string t;
			t = ObjectManager::Instance()->GUIGetTag();
			ImGui::DragFloat3("followPosOffset", followPosOffset, dragspeed);
			if (t != "\0" && t != followtag)followtag = t;
			if (!posRangeF)ImGui::DragFloat3("range", addPosRange, dragspeed);
			else ImGui::DragFloat3("followRangeOffset", followRangeOffset, dragspeed);
		}
		else
		{
			ImGui::Checkbox("use end pos", &posF);
			ImGui::DragFloat3("pos", position, dragspeed);
			if (posF)
			{
				ImGui::DragFloat3("endposition", endposition, dragspeed);
				ImGui::DragFloat3("endPosRange", endPosRange, dragspeed);
				if (ImGui::CollapsingHeader("Easing set"))
				{
					if (ImGui::TreeNode("Ease Set x"))
					{
						ImGui::RadioButton("Lerp", &posEase[0], LERP);
						ImGui::RadioButton("EaseIn", &posEase[0], EaseIn);
						ImGui::RadioButton("EaseOut", &posEase[0], EaseOut);
						ImGui::RadioButton("EaseInOut", &posEase[0], EaseInOut);
						ImGui::TreePop();
					}
					if (ImGui::TreeNode("Ease Set y"))
					{
						ImGui::RadioButton("Lerp", &posEase[1], LERP);
						ImGui::RadioButton("EaseIn", &posEase[1], EaseIn);
						ImGui::RadioButton("EaseOut", &posEase[1], EaseOut);
						ImGui::RadioButton("EaseInOut", &posEase[1], EaseInOut);
						ImGui::TreePop();
					}
					if (ImGui::TreeNode("Ease Set z"))
					{
						ImGui::RadioButton("Lerp", &posEase[2], LERP);
						ImGui::RadioButton("EaseIn", &posEase[2], EaseIn);
						ImGui::RadioButton("EaseOut", &posEase[2], EaseOut);
						ImGui::RadioButton("EaseInOut", &posEase[2], EaseInOut);
						ImGui::TreePop();
					}
				}
			}
			ImGui::DragFloat3("range", addPosRange, dragspeed);
		}
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("scale"))
	{
		ImGui::Checkbox("scaleType", &scaleF);
		if (scaleF)
		{
			ImGui::DragFloat3("start scale", startScale, dragspeed);
			ImGui::DragFloat3("end scale", endScale, dragspeed);
			if (ImGui::CollapsingHeader("Easing set"))
			{
				if (ImGui::TreeNode("Ease Set x"))
				{
					ImGui::RadioButton("Lerp", &scaleEase[0], LERP);
					ImGui::RadioButton("EaseIn", &scaleEase[0], EaseIn);
					ImGui::RadioButton("EaseOut", &scaleEase[0], EaseOut);
					ImGui::RadioButton("EaseInOut", &scaleEase[0], EaseInOut);
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Ease Set y"))
				{
					ImGui::RadioButton("Lerp", &scaleEase[1], LERP);
					ImGui::RadioButton("EaseIn", &scaleEase[1], EaseIn);
					ImGui::RadioButton("EaseOut", &scaleEase[1], EaseOut);
					ImGui::RadioButton("EaseInOut", &scaleEase[1], EaseInOut);
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Ease Set z"))
				{
					ImGui::RadioButton("Lerp", &scaleEase[2], LERP);
					ImGui::RadioButton("EaseIn", &scaleEase[2], EaseIn);
					ImGui::RadioButton("EaseOut", &scaleEase[2], EaseOut);
					ImGui::RadioButton("EaseInOut", &scaleEase[2], EaseInOut);
					ImGui::TreePop();
				}
			}
		}
		else ImGui::DragFloat3("scale", scale, dragspeed);
		ImGui::Checkbox("start scale xy flag", &scaleRangeFlag);
		if (scaleRangeFlag) ImGui::DragFloat("start scale range xy", &scaleRange[0], dragspeed);
		else ImGui::DragFloat3("start scale range", scaleRange, dragspeed);
		ImGui::DragFloat3("generate", addscale, dragspeed);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Speed"))
	{
		ImGui::Checkbox("gravity flag", &isGravity);
		if (isGravity)
		{
			ImGui::DragFloat("gravity", &gravity, dragspeed);
		}
		ImGui::Checkbox("speedType", &speedF);
		if (speedF)
		{
			ImGui::DragFloat3("start speed", startSpeed, dragspeed);
			ImGui::DragFloat3("end speed", endSpeed, dragspeed);
			if (ImGui::CollapsingHeader("Easing set"))
			{
				if (ImGui::TreeNode("Ease Set x"))
				{
					ImGui::RadioButton("Lerp", &speedEase[0], LERP);
					ImGui::RadioButton("EaseIn", &speedEase[0], EaseIn);
					ImGui::RadioButton("EaseOut", &speedEase[0], EaseOut);
					ImGui::RadioButton("EaseInOut", &speedEase[0], EaseInOut);
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Ease Set y"))
				{
					ImGui::RadioButton("Lerp", &speedEase[1], LERP);
					ImGui::RadioButton("EaseIn", &speedEase[1], EaseIn);
					ImGui::RadioButton("EaseOut", &speedEase[1], EaseOut);
					ImGui::RadioButton("EaseInOut", &speedEase[1], EaseInOut);
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Ease Set z"))
				{
					ImGui::RadioButton("Lerp", &speedEase[2], LERP);
					ImGui::RadioButton("EaseIn", &speedEase[2], EaseIn);
					ImGui::RadioButton("EaseOut", &speedEase[2], EaseOut);
					ImGui::RadioButton("EaseInOut", &speedEase[2], EaseInOut);
					ImGui::TreePop();
				}
			}
		}
		else ImGui::DragFloat3("speed", speed, dragspeed);
		ImGui::DragFloat3("start speed range", speedRange, dragspeed);
		ImGui::DragFloat3("generate", addspeed, dragspeed);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Rota"))
	{
		ImGui::Checkbox("start and end mode", &rotaF);
		if (rotaF)
		{
			ImGui::DragFloat("start rota", &rotastart, dragspeed);
			ImGui::DragFloat("end rota", &rotaend, dragspeed);
			if (ImGui::CollapsingHeader("Ease"))
			{
				ImGui::RadioButton("Lerp", &rotaEase, LERP);
				ImGui::RadioButton("EaseIn", &rotaEase, EaseIn);
				ImGui::RadioButton("EaseOut", &rotaEase, EaseOut);
				ImGui::RadioButton("EaseInOut", &rotaEase, EaseInOut);
			}
		}
		else
		{
			ImGui::DragFloat("start rota", &rota, dragspeed);
			ImGui::DragFloat("rota speed", &rotaspeed, dragspeed);
		}
		ImGui::DragFloat("rota start random", &rotarandom, dragspeed);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("color"))
	{
		ImGui::Checkbox("colorType", &colorF);
		if (colorF)
		{
			ImGui::ColorEdit4("start color", startcolor, dragspeed);
			ImGui::ColorEdit4("end color", endcolor, dragspeed);
			if (ImGui::CollapsingHeader("Easing set"))
			{
				if (ImGui::TreeNode("Ease Set r"))
				{
					ImGui::RadioButton("Lerp", &colorEase[0], LERP);
					ImGui::RadioButton("EaseIn", &colorEase[0], EaseIn);
					ImGui::RadioButton("EaseOut", &colorEase[0], EaseOut);
					ImGui::RadioButton("EaseInOut", &colorEase[0], EaseInOut);
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Ease Set g"))
				{
					ImGui::RadioButton("Lerp", &colorEase[1], LERP);
					ImGui::RadioButton("EaseIn", &colorEase[1], EaseIn);
					ImGui::RadioButton("EaseOut", &colorEase[1], EaseOut);
					ImGui::RadioButton("EaseInOut", &colorEase[1], EaseInOut);
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Ease Set b"))
				{
					ImGui::RadioButton("Lerp", &colorEase[2], LERP);
					ImGui::RadioButton("EaseIn", &colorEase[2], EaseIn);
					ImGui::RadioButton("EaseOut", &colorEase[2], EaseOut);
					ImGui::RadioButton("EaseInOut", &colorEase[2], EaseInOut);
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Ease Set a"))
				{
					ImGui::RadioButton("Lerp", &colorEase[3], LERP);
					ImGui::RadioButton("EaseIn", &colorEase[3], EaseIn);
					ImGui::RadioButton("EaseOut", &colorEase[3], EaseOut);
					ImGui::RadioButton("EaseInOut", &colorEase[3], EaseInOut);
					ImGui::TreePop();
				}
			}
		}
		else ImGui::ColorEdit4("color", color, dragspeed);
		ImGui::TreePop();
	}
}

void IF::Emitter::OutputJson(nlohmann::json& j)
{
	j["maxnum"] = maxnum;
	j["addnum"] = addnum;
	j["speedF"] = speedF;
	j["posF"] = posF;
	j["setSpeedPosFlag"] = setSpeedPosFlag;
	j["scaleF"] = scaleF;
	j["addTime"] = addTime;
	j["flag"] = flag;
	j["deleteTime"] = deleteTime;
	j["followtag"] = followtag;
	j["follow"] = follow;
	j["tex"] = tex;
	j["tag"] = tag;
	j["colorF"] = colorF;
	j["generateF"] = generateF;
	j["generateTime"] = generateTime;
	j["Blendmode"] = Blendmode;
	j["posRangeF"] = posRangeF;
	j["isGravity"] = isGravity;
	j["gravity"] = gravity;
	j["rota"] = rota;
	j["rotaF"] = rotaF;
	j["rotaspeed"] = rotaspeed;
	j["rotastart"] = rotastart;
	j["rotaend"] = rotaend;
	j["rotaEase"] = rotaEase;
	j["rotarandom"] = rotarandom;
	j["scaleRangeFlag"] = scaleRangeFlag;
	j["deleteTime"] = deleteTime;

	for (int i = 0; i < 3; i++)
	{
		j["endSpeed"][i] = endSpeed[i];
		j["startSpeed"][i] = startSpeed[i];
		j["speed"][i] = speed[i];
		j["speedEase"][i] = speedEase[i];
		j["position"][i] = position[i];
		j["endposition"][i] = endposition[i];
		j["endPosRange"][i] = endPosRange[i];
		j["posEase"][i] = posEase[i];
		j["scale"][i] = scale[i];
		j["endScale"][i] = endScale[i];
		j["startScale"][i] = startScale[i];
		j["scaleEase"][i] = scaleEase[i];
		j["addPosRange"][i] = addPosRange[i];
		j["speedRange"][i] = speedRange[i];
		j["addspeed"][i] = addspeed[i];
		j["scaleRange"][i] = scaleRange[i];
		j["addscale"][i] = addscale[i];
		j["deletePositionMax"][i] = deletePositionMax[i];
		j["deletePositionMin"][i] = deletePositionMin[i];
		j["followPosOffset"][i] = followPosOffset[i];
		j["followRangeOffset"][i] = followRangeOffset[i];
	}
	for (int i = 0; i < 4; i++)
	{
		j["startcolor"][i] = startcolor[i];
		j["endcolor"][i] = endcolor[i];
		j["colorEase"][i] = colorEase[i];
		j["color"][i] = color[i];
	}
}
//#endif
