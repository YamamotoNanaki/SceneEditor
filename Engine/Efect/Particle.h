#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <vector>
#include "ParticleVI.h"
#include "ConstStruct.h"
#include "IFMath.h"
#include "ICamera.h"
#include "ConstBuff.h"
#include "Timer.h"

#pragma comment(lib,"d3d12.lib")

namespace IF
{
	enum easetype
	{
		EaseIn, EaseOut, EaseInOut, Lerp
	};
	class Particle
	{
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		template<class T> using vector = std::vector<T>;
	public:
		enum BillBoardMode
		{
			NOON,
			BILLBOARD,
			YBOARD
		};

		static ID3D12GraphicsCommandList* commandList;
	protected:
		static PV vi;
		static ID3D12Device* device;
		static vector<D3D12_VIEWPORT> viewport;
		ConstBuff cb;

	public:
		static ICamera* camera;
		//定数バッファ
		ComPtr<ID3D12Resource> constBuffTransform;
		//定数バッファマップ
		ConstBufferBillboard* constMapTransform;
		//アフィン変換情報
		Float3 position = { 0,0,0 };
		//ワールド変換行列
		Matrix matWorld;

	public:
		//重力
		bool isGravity = false;
		float gravity = 1;
		//スピード
		float endSpeed[3]{};
		float startSpeed[3]{};
		float speed[3]{};
		bool speedF = false;
		int speedEase[3] = { Lerp };
		//色
		float color[4]{};
		float startcolor[4]{};
		float endcolor[4]{};
		bool colorF = false;
		int colorEase[4] = { Lerp };
		//ポジション
		float endposition[3]{};
		bool posF = false;
		int posEase[3] = { Lerp };
		float startposition[3]{};
		bool setSpeedPosFlag = false;
		//スケール
		float scale[3]{};
		float endScale[3]{};
		float startScale[3]{};
		bool scaleF = false;
		int scaleEase[3] = { Lerp };
		//回転
		float rota = 0;
		float rotastart = 0;
		float rotaend = 0;
		float rotaspeed = 0;
		int rotaEase = Lerp;
		bool rotaF = false;
		//時間
		Timer timer{};
		int deleteTime = 0;
		bool deleteFlag = false;

	public:
		static void StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, vector<D3D12_VIEWPORT> viewport);
		void Initialize();
		static void VIInitialize();
		void MatUpdate();
		virtual void Update();
		static void DrawBefore(ID3D12RootSignature* root,
			D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
		void Draw();
		~Particle();
	};
}