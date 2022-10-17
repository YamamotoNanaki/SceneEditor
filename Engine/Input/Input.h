#pragma once
#include <dinput.h>
#include <wrl.h>
#include "Float2.h"
#include <Xinput.h>

#pragma comment (lib, "xinput.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

namespace IF
{
	enum KEY_INPUT_FLAG
	{
		USE_INPUT_KEY = 0b001,
		USE_INPUT_MOUSE = 0b010,
		USE_INPUT_CONTROLLER = 0b100
	};
	namespace KEY
	{
		typedef char KeyCode;
		enum Type
		{
			OR,
			AND,
		};

	}
	namespace PAD
	{
		typedef unsigned short PadCode;
	}
	struct Mouse
	{
		LONG x;
		LONG y;
		LONG z;
	};

	class Input
	{
		using KeyCode = KEY::KeyCode;
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	private:
		char key[256]{}, oldkey[256]{};
		ComPtr < IDirectInput8> directInput = nullptr;
		ComPtr < IDirectInputDevice8> keyboard = nullptr;
		ComPtr < IDirectInputDevice8> devMouse;
		DIMOUSESTATE2 mouse = {};
		DIMOUSESTATE2 oldmouse = {};
		XINPUT_STATE pad{};
		XINPUT_STATE oldpad{};
		XINPUT_VIBRATION vibration{};
	private:
		/// <summary>
		/// コンストラクタ
		/// キーボード入力の初期化
		/// </summary>
		Input();
		Input(const Input&);
		Input& operator=(const Input&) {}
		~Input() {}

		char INPUT_FLAG;
	public:
		static Input* Instance();
		static void DeleteInstance();
		void InputFlag(char flag);

		void Initialize(HINSTANCE& hInstance, HWND& hwnd);

		/// <summary>
		/// キーボード情報のアップデート
		/// </summary>
		/// <param name="result"></param>
		void Update();

		/// <summary>
		/// トリガー判定
		/// </summary>
		/// <param name="keyCode">判定を取りたいキーのマクロ</param>
		/// <returns></returns>
		bool KeyTriggere(KeyCode keyCode);
		/// <summary>
		/// 入力判定
		/// </summary>
		/// <param name="keyCode">判定を取りたいキーのマクロ</param>
		/// <returns></returns>
		bool KeyDown(KeyCode keyCode);
		/// <summary>
		/// リリース判定
		/// </summary>
		/// <param name="keyCode">判定を取りたいキーのマクロ</param>
		/// <returns></returns>
		bool KeyRelease(KeyCode keyCode);

		bool MLPush();
		bool MRPush();
		bool MMPush();
		bool MLTriggere();
		bool MRTriggere();
		bool MMTriggere();
		bool MLRelease();
		bool MRRelease();
		bool MMRelease();

		Float2 GetLAnalog(int unresponsive_range = 200);
		Float2 GetRAnalog(int unresponsive_range = 200);
		float GetLXAnalog(int unresponsive_range = 200);
		float GetRXAnalog(int unresponsive_range = 200);
		float GetLYAnalog(int unresponsive_range = 200);
		float GetRYAnalog(int unresponsive_range = 200);

		bool PadPush(PAD::PadCode pad);
		bool PadTriggere(PAD::PadCode pad);
		bool PadRelease(PAD::PadCode pad);

		void PadVibrationStart(WORD L, WORD R);
		void PadVibrationStop();

		Mouse GetMouse3D();

		bool Judge(const KeyCode a[], int Type);
	};
	namespace PAD
	{
		enum PADCODE
		{
			UP = XINPUT_GAMEPAD_DPAD_UP,
			DOWN = XINPUT_GAMEPAD_DPAD_DOWN,
			LEFT = XINPUT_GAMEPAD_DPAD_LEFT,
			RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT,
			START = XINPUT_GAMEPAD_START,
			BACK = XINPUT_GAMEPAD_BACK,
			LTHUMB = XINPUT_GAMEPAD_LEFT_THUMB,
			RTHUMB = XINPUT_GAMEPAD_RIGHT_THUMB,
			LSHOULDER = XINPUT_GAMEPAD_LEFT_SHOULDER,
			RSHOULDER = XINPUT_GAMEPAD_RIGHT_SHOULDER,
			A = XINPUT_GAMEPAD_A,
			B = XINPUT_GAMEPAD_B,
			X = XINPUT_GAMEPAD_X,
			Y = XINPUT_GAMEPAD_Y,
			ALL = UP | DOWN | LEFT | RIGHT | START | BACK | LTHUMB | RTHUMB | LSHOULDER | RSHOULDER | A | B | X | Y
		};
	}
	namespace KEY
	{
		const KeyCode Arrow[4] = { DIK_LEFT,DIK_RIGHT,DIK_UP,DIK_DOWN };
		const KeyCode WASD[4] = { DIK_W,DIK_A,DIK_S,DIK_D };
		enum KEYCODE
		{
			num0 = DIK_NUMPAD0,
			num1 = DIK_NUMPAD1,
			num2 = DIK_NUMPAD2,
			num3 = DIK_NUMPAD3,
			num4 = DIK_NUMPAD4,
			num5 = DIK_NUMPAD5,
			num6 = DIK_NUMPAD6,
			num7 = DIK_NUMPAD7,
			num8 = DIK_NUMPAD8,
			num9 = DIK_NUMPAD9,
			SPACE = DIK_SPACE,
			ENTER = DIK_RETURN,
			ESC = DIK_ESCAPE,
			SHIFT = DIK_LSHIFT,
			CTRL = DIK_LCONTROL,
			TAB = DIK_TAB,
			UP = DIK_UP,
			DOWN = DIK_DOWN,
			LEFT = DIK_LEFT,
			RIGHT = DIK_RIGHT,
			BACK = DIK_BACKSPACE,
			ALT = DIK_LALT,
			CLOCK = DIK_CAPSLOCK,
			RALT = DIK_RALT,
			RSHIFT = DIK_RSHIFT,
			RCTRL = DIK_RCONTROL,
			F1 = DIK_F1,
			F2 = DIK_F2,
			F3 = DIK_F3,
			F4 = DIK_F4,
			F5 = DIK_F5,
			F6 = DIK_F6,
			F7 = DIK_F7,
			F8 = DIK_F8,
			F9 = DIK_F9,
			F10 = DIK_F10,
			F11 = DIK_F11,
			F12 = DIK_F12
		};
	}
}