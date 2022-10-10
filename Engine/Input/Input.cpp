#include "Input.h"
#include "Debug.h"
#include <cassert>

using namespace IF;
using namespace KEY;

#define DIRECTINPUT_VERSION	0x0800		//DirectInputのバージョン指定
static LPDIRECTINPUTDEVICE8 g_GamePadDevice;					//!< DIRECTINPUTDEVICE8のポインタ

#pragma warning(disable : 4995)

Input::Input()
{
	for (int i = 0; i < 256; i++)
	{
		key[i] = 0;
		oldkey[i] = 0;
	}
}

Input* IF::Input::Instance()
{
	static Input* inst = DEBUG_NEW Input;
	return inst;
}

void IF::Input::DeleteInstance()
{
	XInputEnable(false); // バイブレーション解除
	delete Input::Instance();
}

void IF::Input::InputFlag(char flag)
{
	INPUT_FLAG = flag;
}

//ゲームパッドデバイスの作成-デバイス列挙の結果を受け取る構造体
struct DeviceEnumParameter
{
	LPDIRECTINPUTDEVICE8* GamePadDevice;
	int FindCount;
};

BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef)
{
	return DIENUM_CONTINUE;
}

void IF::Input::Initialize(HINSTANCE& hInstance, HWND& hwnd)
{
	HRESULT result = DirectInput8Create(
		hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));
	//キーボードデバイスの生成
	if (INPUT_FLAG & USE_INPUT_KEY)
	{
		result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
		assert(SUCCEEDED(result));

		//入力データ形式のセット
		result = keyboard->SetDataFormat(&c_dfDIKeyboard);//標準形式
		assert(SUCCEEDED(result));

		//排他制御レベルのセット
		result = keyboard->SetCooperativeLevel(
			hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		assert(SUCCEEDED(result));
	}
	//マウスデバイスの生成
	if (INPUT_FLAG & USE_INPUT_MOUSE)
	{
		result = directInput->CreateDevice(GUID_SysMouse, &devMouse, NULL);
		assert(SUCCEEDED(result));

		// 入力データ形式のセット
		result = devMouse->SetDataFormat(&c_dfDIMouse2); // 標準形式
		assert(SUCCEEDED(result));
		// 排他制御レベルのセット
		result = devMouse->SetCooperativeLevel(
			hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
		assert(SUCCEEDED(result));

	}
	//コントローラデバイスの生成
	if (INPUT_FLAG & USE_INPUT_CONTROLLER)
	{
		XInputGetState(
			0,       // DWORD         dwUserIndex
			&pad); // XINPUT_STATE* pState
	}
}

void Input::Update()
{
	HRESULT result = S_OK;
	if (INPUT_FLAG & USE_INPUT_KEY)
	{
		for (int i = 0; i < 256; i++)
		{
			oldkey[i] = key[i];
		}
		keyboard->Acquire();
		result = keyboard->GetDeviceState(sizeof(key), key);
	}

	if (INPUT_FLAG & USE_INPUT_MOUSE)
	{
		// 前回の入力を保存
		oldmouse = mouse;
		// マウス
		result = devMouse->Acquire();	// マウス動作開始
		// マウスの入力
		result = devMouse->GetDeviceState(sizeof(mouse), &mouse);
	}

	if (INPUT_FLAG & USE_INPUT_CONTROLLER)
	{
		oldpad = pad;
		XInputGetState(
			0,       // DWORD         dwUserIndex
			&pad); // XINPUT_STATE* pState
	}
}

bool Input::KeyTriggere(KeyCode keyCode)
{
	//if (!(INPUT_FLAG & USE_INPUT_KEY))return false;
	return key[keyCode] && !oldkey[keyCode];
}

bool Input::KeyDown(KeyCode keyCode)
{
	//if (!(INPUT_FLAG & USE_INPUT_KEY))return false;
	return key[keyCode];
}

bool Input::KeyRelease(KeyCode keyCode)
{
	//if (!(INPUT_FLAG & USE_INPUT_KEY))return false;
	return !key[keyCode] && oldkey[keyCode];
}

bool IF::Input::MLPush()
{
	//if (!(INPUT_FLAG & USE_INPUT_MOUSE))return false;
	if (mouse.rgbButtons[0]) {
		return true;
	}

	return false;
}

bool IF::Input::MRPush()
{
	//if (!(INPUT_FLAG & USE_INPUT_MOUSE))return false;
	if (mouse.rgbButtons[1]) {
		return true;
	}

	return false;
}

bool IF::Input::MMPush()
{
	//if (!(INPUT_FLAG & USE_INPUT_MOUSE))return false;
	if (mouse.rgbButtons[2]) {
		return true;
	}

	return false;
}

bool IF::Input::MLTriggere()
{
	//if (!(INPUT_FLAG & USE_INPUT_MOUSE))return false;
	if (mouse.rgbButtons[0] && !oldmouse.rgbButtons[0]) {
		return true;
	}

	return false;
}

bool IF::Input::MRTriggere()
{
	//if (!(INPUT_FLAG & USE_INPUT_MOUSE))return false;
	if (mouse.rgbButtons[1] && !oldmouse.rgbButtons[1]) {
		return true;
	}

	return false;
}

bool IF::Input::MMTriggere()
{
	//if (!(INPUT_FLAG & USE_INPUT_MOUSE))return false;
	if (mouse.rgbButtons[2] && !oldmouse.rgbButtons[2]) {
		return true;
	}

	return false;
}

bool IF::Input::MLRelease()
{
	//if (!(INPUT_FLAG & USE_INPUT_MOUSE))return false;
	if (!mouse.rgbButtons[0] && oldmouse.rgbButtons[0]) {
		return true;
	}

	return false;
}

bool IF::Input::MRRelease()
{
	//if (!(INPUT_FLAG & USE_INPUT_MOUSE))return false;
	if (!mouse.rgbButtons[1] && oldmouse.rgbButtons[1]) {
		return true;
	}

	return false;
}

bool IF::Input::MMRelease()
{
	//if (!(INPUT_FLAG & USE_INPUT_MOUSE))return false;
	if (!mouse.rgbButtons[2] && oldmouse.rgbButtons[2]) {
		return true;
	}

	return false;
}

Float2 IF::Input::GetLAnalog(int unresponsive_range)
{
	return Float2(GetLXAnalog(unresponsive_range), GetLYAnalog(unresponsive_range));
}

Float2 IF::Input::GetRAnalog(int unresponsive_range)
{
	return Float2(GetRXAnalog(unresponsive_range), GetRYAnalog(unresponsive_range));
}

float IF::Input::GetLXAnalog(int unresponsive_range)
{
	float x = pad.Gamepad.sThumbLX;
	if (x < unresponsive_range && x > -unresponsive_range)
	{
		x = 0;
	}
	if (x == -32768)x = -32767;
	return x / 32767.0f;
}

float IF::Input::GetRXAnalog(int unresponsive_range)
{
	float x = pad.Gamepad.sThumbRX;
	if (x < unresponsive_range && x > -unresponsive_range)
	{
		x = 0;
	}
	if (x == -32768)x = -32767;
	return x / 32767.0f;
}

float IF::Input::GetLYAnalog(int unresponsive_range)
{
	float y = pad.Gamepad.sThumbLY;
	if (y < unresponsive_range && y > -unresponsive_range)
	{
		y = 0;
	}
	if (y == -32768)y = -32767;
	return y / 32767.0f;
}

float IF::Input::GetRYAnalog(int unresponsive_range)
{
	float y = pad.Gamepad.sThumbRY;
	if (y < unresponsive_range && y > -unresponsive_range)
	{
		y = 0;
	}
	if (y == -32768)y = -32767;
	return y / 32767.0f;
}

bool IF::Input::PadPush(PAD::PadCode pad)
{
	if (this->pad.Gamepad.wButtons & pad)return true;
	return false;
}

bool IF::Input::PadTriggere(PAD::PadCode pad)
{
	if ((this->pad.Gamepad.wButtons & pad) && !(this->oldpad.Gamepad.wButtons & pad))return true;
	return false;
}

bool IF::Input::PadRelease(PAD::PadCode pad)
{
	if (!(this->pad.Gamepad.wButtons & pad) && (this->oldpad.Gamepad.wButtons & pad))return true;
	return false;
}

void IF::Input::PadVibrationStart(WORD L, WORD R)
{
	vibration.wLeftMotorSpeed = L;
	vibration.wRightMotorSpeed = R;
	XInputSetState(0, &vibration);
}

void IF::Input::PadVibrationStop()
{
	vibration.wLeftMotorSpeed = 0;
	vibration.wRightMotorSpeed = 0;
	XInputSetState(0, &vibration);
}

Mouse IF::Input::GetMouse3D()
{
	Mouse m{};
	if (!(INPUT_FLAG & USE_INPUT_MOUSE))return m;
	m.x = mouse.lX;
	m.y = mouse.lY;
	m.z = mouse.lZ;
	return m;
}

bool Input::Judge(const KeyCode a[], int Type)
{
	if (a == nullptr)return false;
	if (Type != AND && Type != OR)return false;
	int b = sizeof(a);
	int c = sizeof(a[0]);
	for (int i = 0; i < b / c; i++)
	{
		if (KeyDown(a[i]))
		{
			if (Type == OR)return true;
		}
		else
		{
			if (Type == AND)return false;
		}
	}
	if (Type == OR)return false;
	return true;
}
