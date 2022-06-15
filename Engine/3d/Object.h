#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <vector>
#include <string>
#include "Light.h"
#include "ModelVI.h"
#include "ConstStruct.h"
#include "Model.h"
#include "IFMath.h"

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
		static LightManager* light;

	public:
		//�萔�o�b�t�@
		ComPtr<ID3D12Resource> constBuffTransform;
		//�萔�o�b�t�@�}�b�v
		ConstBufferDataTransform* constMapTransform = nullptr;
		//�A�t�B���ϊ����
		Float3 scale = { 1,1,1 };
		Float3 rotation = { 0,0,0 };
		Float3 position = { 0,0,0 };
		//���[���h�ϊ��s��
		Matrix matWorld{};
		//�e�I�u�W�F�N�g�ւ̃|�C���^
		Object* parent = nullptr;

	public:
		void Initialize(ID3D12Device* device, Model* model);
		void SetModel(Model* model);
		void DrawBefore(ID3D12GraphicsCommandList* commandList, ID3D12RootSignature* root, D3D12_GPU_VIRTUAL_ADDRESS GPUAddress,
			D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		void Update(Matrix matView, Matrix matProjection, Float3 comeraPos, BillBoard::BillBoardMode mode = BillBoard::NOON);
		void Draw(ID3D12GraphicsCommandList* commandList, vector<D3D12_VIEWPORT> viewport);
		void Draw(ID3D12GraphicsCommandList* commandList, vector<D3D12_VIEWPORT> viewport, unsigned short texNum);
		~Object();
		static void SetLight(LightManager* light)
		{
			Object::light = light;
		}
	};
}