#pragma once
//#include <d3d12.h>
//#include <DirectXMath.h>
//#include <wrl.h>
//#include <vector>
//#include "ParticleVI.h"
//#include "ConstStruct.h"
//
//#pragma comment(lib,"d3d12.lib") 
//
//namespace IF
//{
//	class Particle
//	{
//		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
//		template<class T> using vector = std::vector<T>;
//		using XMFLOAT3 = DirectX::XMFLOAT3;
//		using XMMATRIX = DirectX::XMMATRIX;
//	public:
//	enum BillBoardMode
//	{
//		NOON,
//		BILLBOARD,
//		YBOARD
//	};
//	private:
//		static PV* vi;
//		static ID3D12Device* device;
//		static ID3D12GraphicsCommandList commandList;
//
//	public:
//		//定数バッファ
//		ComPtr<ID3D12Resource> constBuffTransform;
//		//定数バッファマップ
//		ConstBufferBillboard* constMapTransform;
//		//アフィン変換情報
//		XMFLOAT3 position = { 0,0,0 };
//		//ワールド変換行列
//		XMMATRIX matWorld;
//		float scale = 1;
//		unsigned int flame = 0;
//		bool flag = 0;
//		float targetY;
//		float startY;
//
//	public:
//		static void SetDeviceCommand(ID3D12Device* device,ID3D12GraphicsCommandList* commandList);
//		void Initialize();
//		static void VIInitialize();
//		void Update(XMMATRIX matView, XMMATRIX matProjection, XMMATRIX matBillBoard = DirectX::XMMatrixIdentity()); 
//		void DrawBefore(ID3D12RootSignature* root, ID3D12DescriptorHeap* srvHeap, D3D12_GPU_VIRTUAL_ADDRESS GPUAddress,
//			D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//		void Draw(vector<D3D12_VIEWPORT> viewport);
//		~Particle();
//	};
//}