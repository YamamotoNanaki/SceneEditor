#pragma once
#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"
#include <Windows.h>
#include <dxgi1_6.h>
#include <d3dx12.h>

namespace IF
{
	class GUI
	{
	private:
		static int const NUM_FRAMES_IN_FLIGHT = 3;
	public:
		void Initialize(HWND hwnd, ID3D12Device* device, ID3D12DescriptorHeap* SrvDescHeap, IDXGISwapChain4* swapchain);
	};
}