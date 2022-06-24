#include "Input.h"
#include <cassert>

using namespace IF;
using namespace KEY;

#define DIRECTINPUT_VERSION	0x0800		//DirectInputのバージョン指定

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
	static Input* inst = new Input;
	return inst;
}

void IF::Input::DeleteInstance()
{
	delete Input::Instance();
}

void IF::Input::Initialize(HINSTANCE& hInstance, HWND& hwnd)
{
	HRESULT result = DirectInput8Create(
		hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(result));

	//キーボードデバイスの生成
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	assert(SUCCEEDED(result));

	// マウスデバイスの生成	
	result = directInput->CreateDevice(GUID_SysMouse, &devMouse, NULL);
	assert(SUCCEEDED(result));

	//入力データ形式のセット
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);//標準形式
	assert(SUCCEEDED(result));

	// 入力データ形式のセット
	result = devMouse->SetDataFormat(&c_dfDIMouse2); // 標準形式
	assert(SUCCEEDED(result));

	//排他制御レベルのセット
	result = keyboard->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));

	// 排他制御レベルのセット
	result = devMouse->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	assert(SUCCEEDED(result));
}

void Input::Update()
{
	for (int i = 0; i < 256; i++)
	{
		oldkey[i] = key[i];
	}
	keyboard->Acquire();
	HRESULT result = keyboard->GetDeviceState(sizeof(key), key);

	// 前回の入力を保存
	oldmouse = mouse;
	// マウス
	result = devMouse->Acquire();	// マウス動作開始
	// マウスの入力
	result = devMouse->GetDeviceState(sizeof(mouse), &mouse);
}

bool Input::KTriggere(KeyCode keyCode)
{
	return key[keyCode] && !oldkey[keyCode];
}

bool Input::KDown(KeyCode keyCode)
{
	return key[keyCode];
}

bool Input::KRelease(KeyCode keyCode)
{
	return !key[keyCode] && oldkey[keyCode];
}

bool IF::Input::MLPush()
{
	if (mouse.rgbButtons[0]) {
		return true;
	}

	return false;
}

bool IF::Input::MRPush()
{
	if (mouse.rgbButtons[1]) {
		return true;
	}

	return false;
}

bool IF::Input::MMPush()
{
	if (mouse.rgbButtons[2]) {
		return true;
	}

	return false;
}

bool IF::Input::MLTriggere()
{
	if (mouse.rgbButtons[0] && !oldmouse.rgbButtons[0]) {
		return true;
	}

	return false;
}

bool IF::Input::MRTriggere()
{
	if (mouse.rgbButtons[1] && !oldmouse.rgbButtons[1]) {
		return true;
	}

	return false;
}

bool IF::Input::MMTriggere()
{
	if (mouse.rgbButtons[2] && !oldmouse.rgbButtons[2]) {
		return true;
	}

	return false;
}

bool IF::Input::MLRelease()
{
	if (!mouse.rgbButtons[0] && oldmouse.rgbButtons[0]) {
		return true;
	}

	return false;
}

bool IF::Input::MRRelease()
{
	if (!mouse.rgbButtons[1] && oldmouse.rgbButtons[1]) {
		return true;
	}

	return false;
}

bool IF::Input::MMRelease()
{
	if (!mouse.rgbButtons[2] && oldmouse.rgbButtons[2]) {
		return true;
	}

	return false;
}

Mouse IF::Input::GetMouse3D()
{
	Mouse m;
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
		if (KDown(a[i]))
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
