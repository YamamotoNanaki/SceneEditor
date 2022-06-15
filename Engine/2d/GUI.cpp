#include "GUI.h"

void IF::GUI::Initialize(HWND hwnd, ID3D12Device* device, ID3D12DescriptorHeap* SrvDescHeap, IDXGISwapChain4* swapchain)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX12_Init(device, NUM_FRAMES_IN_FLIGHT,
        DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, SrvDescHeap,
        SrvDescHeap->GetCPUDescriptorHandleForHeapStart(),
        SrvDescHeap->GetGPUDescriptorHandleForHeapStart());
}
