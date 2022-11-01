#include "GUI.h"
#include "Util.h"
#include "DirectX12.h"
#include "Window.h"
#include "Texture.h"
#include <typeinfo>

void IF::GUI::Initialize()
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
	ImGui_ImplWin32_Init(Window::Instance()->hwnd);
	ID3D12DescriptorHeap* SrvDescHeap = Texture::Instance()->srvHeap.Get();
	ImGui_ImplDX12_Init(DirectX12::Instance()->GetDevice(), NUM_FRAMES_IN_FLIGHT,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, SrvDescHeap,
		SrvDescHeap->GetCPUDescriptorHandleForHeapStart(),
		SrvDescHeap->GetGPUDescriptorHandleForHeapStart());
}

void IF::GUI::NewGui(std::function<void(void)> guiF, std::string guiname, bool open, ImGuiWindowFlags flags)
{
	ImGui::Begin(guiname.c_str(), &open, flags);
	guiF();
	ImGui::End();
}

void IF::GUI::DragFloatAuto(std::string label, float* f, float dragspeed, float min, float max, const char* format, ImGuiSliderFlags flag)
{
	int s = GetSize(f);
	if (s == 1)ImGui::DragFloat(label.c_str(), f, dragspeed);
	else if (s == 2)ImGui::DragFloat2(label.c_str(), f, dragspeed);
	else if (s == 3)ImGui::DragFloat3(label.c_str(), f, dragspeed);
	else if (s == 4)ImGui::DragFloat4(label.c_str(), f, dragspeed);
	else ImGui::Text("Error array");
}

void IF::GUI::DragIntAuto(std::string label, int* i, float dragspeed, int min, int max, const char* format, ImGuiSliderFlags flag)
{
	int s = GetSize(i);
	if (s == 1)ImGui::DragInt(label.c_str(), i, dragspeed);
	else if (s == 2)ImGui::DragInt2(label.c_str(), i, dragspeed);
	else if (s == 3)ImGui::DragInt3(label.c_str(), i, dragspeed);
	else if (s == 4)ImGui::DragInt4(label.c_str(), i, dragspeed);
	else ImGui::Text("Error array");
}

void IF::GUI::RaguiArray(std::string label, int* returnnum, int maxnum)
{
	for (int i = 0; i < maxnum; i++)
	{
		ImGui::RadioButton(label.c_str(), returnnum, i);
	}
}

void IF::GUI::CheckBox(std::string label, bool* check)
{
	ImGui::Checkbox(label.c_str(), check);
}

void IF::GUI::NewCollapsingHeader(std::function<void(void)> guiF, std::string headername, ImGuiTreeNodeFlags flag)
{
	if (ImGui::CollapsingHeader(headername.c_str(), flag))
	{
		guiF();
	}
}

void IF::GUI::NewTreeNode(std::function<void(void)> guiFunc, std::string headername)
{
	if (ImGui::TreeNode(headername.c_str()))
	{
		guiFunc();
		ImGui::TreePop();
	}
}

void IF::GUI::InputTextString(std::string label, std::string inputtext, ImGuiInputTextFlags flag)
{
	char c[256];
	strcpy_s(c, inputtext.c_str());
	ImGui::InputText(label.c_str(), c, flag);
	inputtext = c;
}

void IF::GUI::DrawImage(unsigned short texnum, Float2 size, Float2 uv1, Float2 uv2, Float4 tint_col, Float4 border_col)
{
	ImVec2 s = { size.x,size.y };
	ImVec2 u = { uv1.x,uv1.y };
	ImVec2 v = { uv2.x,uv2.y };
	ImVec4 t = { tint_col.x,tint_col.y,tint_col.z,tint_col.w };
	ImVec4 b = { border_col.x,border_col.y,border_col.z,border_col.w };
	ImGui::Image((ImTextureID)texnum, s, u, v, t, b);
}

bool IF::GUI::DrawImageButton(unsigned short texnum, Float2 size, Float2 uv1, Float2 uv2, Float4 tint_col, Float4 border_col)
{
	ImVec2 s = { size.x,size.y };
	ImVec2 u = { uv1.x,uv1.y };
	ImVec2 v = { uv2.x,uv2.y };
	ImVec4 t = { tint_col.x,tint_col.y,tint_col.z,tint_col.w };
	ImVec4 b = { border_col.x,border_col.y,border_col.z,border_col.w };
	return ImGui::ImageButton((ImTextureID)texnum, s, u, v, -1, t, b);
}

bool IF::GUI::ButtonString(std::string label, Float2 size)
{
	ImVec2 s = { size.x,size.y };
	return ImGui::Button(label.c_str(), s);
}
