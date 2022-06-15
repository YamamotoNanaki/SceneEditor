#pragma once
#include <Windows.h>

namespace IF
{
	class Window
	{
	public:
		WNDCLASSEX w{}; // ウィンドウクラスの設定
		HWND hwnd;
	private:
		MSG msg{}; // メッセージ
		Window() {}
		Window(const Window&) {}
		Window& operator=(const Window&) {}
		~Window() {}

	public:
		static Window* Instance();
		static void DeleteInstance();
		void Initialize(int window_width, int window_height, LPCWSTR windowName = L"DirectXGame");
		void Unregister();
		bool Message();

		//ウィンドウプロシージャ
		static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	};
}