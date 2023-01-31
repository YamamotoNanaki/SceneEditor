#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <vector>
#include <string>
#include "Light.h"
#include "ModelVI.h"
#include "ConstStruct.h"
#include "Model.h"
#include "FBXModel.h"
#include "ConstBuff.h"

#pragma comment(lib,"d3d12.lib") 

namespace IF
{
	namespace BillBoard
	{
		enum BillBoardMode
		{
			NOON,
			BILLBOARD,
			YBOARD
		};
	}

	class Object
	{
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		template<class T> using vector = std::vector<T>;
	private:
		Model* model = nullptr;
		ConstBuff cb;
		ConstBuff outLineCb;
		static LightManager* lightPtr;

	protected:
		int billbord = 0;

	public:
		FBXModel* fmodel = nullptr;
		float animTimer = 0;
		//�萔�o�b�t�@
		ComPtr<ID3D12Resource> constBuffTransform;
		ComPtr<ID3D12Resource> constBuffOutLine;
		ComPtr<ID3D12Resource> constBuffSkin;
		//�萔�o�b�t�@�}�b�v
		ConstBufferDataTransform* constMapTransform = nullptr;
		ConstBufferDataTransform* OutLineTransform = nullptr;
		ConstBufferDataSkin* constMapSkin = nullptr;
		//�A�t�B���ϊ����
		Float3 scale = { 1,1,1 };
		Float3 rotation = { 0,0,0 };
		Float3 position = { 0,0,0 };
		//���[���h�ϊ��s��
		Matrix matWorld{};
		//�e�I�u�W�F�N�g�ւ̃|�C���^
		Object* parent = nullptr;
		bool lightFlag = true;
		bool toonFlag = false;

	public:
		float outLineWidth = 0.2f;
		Float4 outLineColor = { 0,0,0,1 };
		bool outLineFlag = true;
		float polygonsize = 1;
		float explosion = 0;
		float polygonRota = 0;

	public:
		void Initialize(Model* model);
		void Initialize(FBXModel* fmodel);
		void SetModel(Model* model);
		inline Model* GetModel()
		{
			return model;
		}
		static void DrawBefore(ID3D12RootSignature* root, D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		void Update(Matrix matView, Matrix matProjection, Float3 comeraPos, int mode = BillBoard::NOON);
		void UpdateWorldMatrix(int mode = BillBoard::NOON);
		void MatWorldUpdate();
		void Draw();
		void FBXDraw();
		void OutLineDraw();
		void Draw(unsigned short texNum);
		~Object();
		static void StaticInitialize();
		void SetColor(int r, int g, int b, int a);
		void SetColorF(float r, float g, float b, float a);
		inline Float4 GetColor()
		{
			return cb.GetColor();
		}
		void SetBright(int r, int g, int b);
		void SetAlpha(int a);

		inline std::string GetModelTag()
		{
			return model->GetTag();
		}
	};
}