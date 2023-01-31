#pragma once
#include "Particle.h"
#include <memory>
#include <list>
#include <string>
#include <d3d12.h>
#include "nlohmann/json.hpp"
#include "Graphic.h"
#include "PostEffect.h"

namespace IF
{
	class Emitter
	{
	public:
		std::string tag;
		std::string followtag;
		//生成フラグ
		bool generateF = true;
		//描画フラグ
		bool DrawF = true;
	protected:
		std::list<std::unique_ptr<Particle>>particles;
	protected:
		//重力
		bool isGravity = false;
		float gravity = 1;
		//描画方法
		int Blendmode = Blend::PADD;
		//生成数
		int maxnum = 100;
		int addnum = 1;
		int num = 0;
		//スピード
		float endSpeed[3]{};
		float startSpeed[3]{};
		float speed[3]{};
		bool speedF = false;
		int speedEase[3] = {};
		//色
		float color[4] = { 1,1,1,1 };
		float startcolor[4] = { 1,1,1,1 };
		float endcolor[4]{};
		bool colorF = false;
		int colorEase[4] = {};
		//ポジション
		float position[3]{};
		float endposition[3]{};
		bool posF = false;
		bool posRangeF = false;
		bool setSpeedPosFlag = false;
		int posEase[3] = {};
		//スケール
		float scale[3]{};
		float endScale[3]{};
		float startScale[3]{};
		bool scaleF = false;
		int scaleEase[3] = {};
		bool scaleRangeFlag = false;
		//回転
		float rota = 0;
		float rotastart = 0;
		float rotaend = 0;
		float rotaspeed = 0;
		float rotarandom = 0;
		bool rotaF = false;
		int rotaEase = LERP;
		//生成時
		float addPosRange[3]{};
		float endPosRange[3]{};
		float speedRange[3]{};
		float addspeed[3]{};
		float scaleRange[3]{};
		float addscale[3]{};
		//追加
		int addTime;
		bool flag;
		int generateTime;
		//削除条件
		int deleteTime = 60;
		float deletePositionMax[3]{};
		float deletePositionMin[3]{};
		//オブジェクト追従
		bool follow;
		float followPosOffset[3];
		float followRangeOffset[3];
		//画像
		int tex = 1;
		Timer timer;
		Timer gtimer;

	public:
		bool emitterDeleteFlag = false;

	protected:
		PostEffect* postEffect = nullptr;

	public:
		Emitter();
		static void StaticInitialize();
		virtual void Add();
		virtual void Update();
		virtual void DrawPostEffect();
		virtual void Draw();
		virtual void SetPositionFollow();
		virtual void SetPosition(const Float3& pos);
		virtual void SetPositionRange(const Float3& pos);
		virtual void SetEndPosition(const Float3& pos);
		virtual void SetEndPositionRange(const Float3& pos);
		virtual void SetRota(const float& rota);
		virtual void SetColor(const Float4& color);
		virtual void DrawBefore(ID3D12RootSignature* root);
		virtual void DrawAfter();
		virtual ~Emitter()
		{
			particles.clear();
			delete postEffect;
		}
		virtual bool WeightSaving(float max = 700);

		virtual void InputJson(nlohmann::json& j);
//#ifdef _DEBUG
		virtual void GUI();
		virtual void OutputJson(nlohmann::json& j);
//#endif
	};
}
