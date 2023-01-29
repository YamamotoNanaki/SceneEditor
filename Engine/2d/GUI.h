#pragma once
#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"
#include "IFMath.h"
#include <Windows.h>
#include <dxgi1_6.h>
#include <d3dx12.h>
#include <functional>
#include <string>

namespace IF
{
	class GUI
	{
	private:
		static int const NUM_FRAMES_IN_FLIGHT = 3;
	public:
		void Initialize();
		static void NewGui(std::function<void(void)> guiFunc, std::string guiname, bool open = false, ImGuiWindowFlags flags = 0);
		static void DragFloatAuto(std::string label, float* f, float dragspeed = 1, float min = 0, float max = 0,
			const char* format = "%.3f", ImGuiSliderFlags flag = 0);
		static void DragIntAuto(std::string label, int* i, float dragspeed = 1, int min = 0, int max = 0,
			const char* format = "%.3f", ImGuiSliderFlags flag = 0);
		static void RaguiArray(std::string label, int* returnnum, int maxnum);
		static void CheckBox(std::string label, bool* check);
		static void NewCollapsingHeader(std::function<void(void)> guiFunc, std::string headername, ImGuiTreeNodeFlags flag = 0);
		static void NewTreeNode(std::function<void(void)> guiFunc, std::string headername);
		static void InputTextString(std::string label, std::string inputtext, ImGuiInputTextFlags flag = 0);
		static void DrawImage(unsigned short texnum, Float2 size, Float2 uv1, Float2 uv2, Float4 tint_col, Float4 border_col);
		static bool DrawImageButton(unsigned short texnum, Float2 size, Float2 uv1, Float2 uv2, Float4 tint_col, Float4 border_col);
		static bool ButtonString(std::string label, Float2 size = { 0,0 });
	};
}