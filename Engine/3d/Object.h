#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <vector>
#include <string>
#include "Light.h"
#include "ModelVI.h"
#include "ConstStruct.h"
#include "Model.h"
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
		static LightManager* light;
		static ComPtr<ID3D12Device> device;
		static ComPtr<ID3D12GraphicsCommandList> commandList;

	public:
		//定数バッファ
		ComPtr<ID3D12Resource> constBuffTransform;
		//定数バッファマップ
		ConstBufferDataTransform* constMapTransform = nullptr;
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
		void SetModel(Model* model);
		static void DrawBefore(ID3D12RootSignature* root, D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		void Update(Matrix matView, Matrix matProjection, Float3 comeraPos, int mode = BillBoard::NOON);
		void Draw(vector<D3D12_VIEWPORT> viewport);
		void Draw(vector<D3D12_VIEWPORT> viewport, unsigned short texNum);
		~Object();
		static inline void StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList, LightManager* light)
		{
			Object::device = device;
			Object::commandList = commandList;
			Object::light = light;
		}
		void SetColor(int r, int g, int b, int a);
		void SetBright(int r, int g, int b);
		void SetAlpha(int a);

		inline std::string GetModelTag()
		{
			return model->GetTag();
		}
	};
}