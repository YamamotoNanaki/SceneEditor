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
		EaseIn, EaseOut, EaseInOut, LERP
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

	protected:
		static PV vi;
		ConstBuff cb;

	public:
		static ICamera* camera;
		//�萔�o�b�t�@
		ComPtr<ID3D12Resource> constBuffTransform;
		//�萔�o�b�t�@�}�b�v
		ConstBufferBillboard* constMapTransform;
		//�A�t�B���ϊ����
		Float3 position = { 0,0,0 };
		//���[���h�ϊ��s��
		Matrix matWorld;

	public:
		//�d��
		bool isGravity = false;
		float gravity = 1;
		//�X�s�[�h
		float endSpeed[3]{};
		float startSpeed[3]{};
		float speed[3]{};
		bool speedF = false;
		int speedEase[3] = { LERP };
		//�F
		float color[4]{};
		float startcolor[4]{};
		float endcolor[4]{};
		bool colorF = false;
		int colorEase[4] = { LERP };
		//�|�W�V����
		float endposition[3]{};
		bool posF = false;
		int posEase[3] = { LERP };
		float startposition[3]{};
		bool setSpeedPosFlag = false;
		//�X�P�[��
		float scale[3]{};
		float endScale[3]{};
		float startScale[3]{};
		bool scaleF = false;
		int scaleEase[3] = { LERP };
		//��]
		float rota = 0;
		float rotastart = 0;
		float rotaend = 0;
		float rotaspeed = 0;
		int rotaEase = LERP;
		bool rotaF = false;
		//����
		Timer timer{};
		int deleteTime = 0;
		bool deleteFlag = false;

	public:
		virtual void Initialize();
		static void VIInitialize();
		void MatUpdate();
		virtual void Update();
		static void DrawBefore(ID3D12RootSignature* root,
			D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
		void Draw();
		~Particle();
	};
}