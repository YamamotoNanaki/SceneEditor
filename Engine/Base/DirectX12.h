#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include <wrl.h>
#include "IFMath.h"

#pragma comment(lib,"d3d12.lib") 
#pragma comment(lib,"dxgi.lib")


namespace IF
{
	class DirectX12
	{
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		template<class T> using vector = std::vector<T>;

	public:
		ComPtr < ID3D12Device> device = nullptr;
		ComPtr < IDXGIFactory6> dxgiFactory = nullptr;
		ComPtr < IDXGISwapChain4> swapchain = nullptr;
		ComPtr < ID3D12CommandAllocator> commandAllocator = nullptr;
		ComPtr < ID3D12GraphicsCommandList> commandList = nullptr;
		ComPtr < ID3D12CommandQueue> commandQueue = nullptr;
		ComPtr < ID3D12DescriptorHeap> rtvHeaps = nullptr;
		// 裏表の2つ分について
		vector< ComPtr<ID3D12Resource>> backBuffers = vector<ComPtr<ID3D12Resource>>(2);
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
		// フェンスの生成
		ComPtr<ID3D12Fence> fence = nullptr;
		UINT64 fenceVal = 0;
		vector<D3D12_VIEWPORT> viewport{};

	private:
		ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;
		ComPtr < ID3D12Resource> depthBuffer = nullptr;

		D3D12_RESOURCE_BARRIER barrierDesc{};
		D3D12_RECT scissorrect{};

		// 3．画面クリア           R     G     B    A
		float clearColor[4] = { 0.1f ,0.25f ,0.5f ,1.0f };

	public:
		static DirectX12* Instance();
		static void DeleteInstance();
		void Initialize(HWND hwnd, int window_width, int window_height);
		void DrawBefore();
		void DrawAfter();
		void SetNewViewPort(float width, float height, float topX, float topY, float minDepth = 0.0f, float maxDepth = 1.0f);
		void SetScissorrect(float left, float right, float top, float bottom);

	public:
		void SetClearColor(Float4 color);
		void SetClearColor(float Red, float Green, float Bule);

	private:
		DirectX12() {};
		DirectX12(const DirectX12&) {};
		~DirectX12() {};
		const DirectX12& operator=(const DirectX12&) {};
	};
}