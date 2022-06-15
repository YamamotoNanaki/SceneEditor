#pragma once
#include <Windows.h>

namespace IF
{
	class Window
	{
	public:
		WNDCLASSEX w{}; // �E�B���h�E�N���X�̐ݒ�
		HWND hwnd;
	private:
		MSG msg{}; // ���b�Z�[�W
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

		//�E�B���h�E�v���V�[�W��
		static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	};
}