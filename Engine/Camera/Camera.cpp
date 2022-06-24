#include "Camera.h"
#include "Input.h"

void IF::Camera::Update()
{
	Input* input = Input::Instance();
	if (input->KDown(KEY::UP))
	{
		matView->eye.z += 0.5f;
		matView->target.z += 0.5f;
	}
	if (input->KDown(KEY::DOWN))
	{
		matView->eye.z -= 0.5f;
		matView->target.z -= 0.5f;
	}
	if (input->KDown(KEY::RIGHT))
	{
		matView->eye.x += 0.5f;
		matView->target.x += 0.5f;
	}
	if (input->KDown(KEY::LEFT))
	{
		matView->eye.x -= 0.5f;
		matView->target.x -= 0.5f;
	}
	matView->Update();
}
