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
		static LightManager* lightPtr;

	public:
		FBXModel* fmodel = nullptr;
		//定数バッファ
		ComPtr<ID3D12Resource> constBuffTransform;
		ComPtr<ID3D12Resource> constBuffSkin;
		//定数バッファマップ
		ConstBufferDataTransform* constMapTransform = nullptr;
		ConstBufferDataSkin* constMapSkin = nullptr;
		//アフィン変換情報
		Float3 scale = { 1,1,1 };
		Float3 rotation = { 0,0,0 };
		Float3 position = { 0,0,0 };
		//ワールド変換行列
		Matrix matWorld{};
		//親オブジェクトへのポインタ
		Object* parent = nullptr;

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
		void Draw();
		void FBXDraw();
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