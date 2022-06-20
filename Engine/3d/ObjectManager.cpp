#include "ObjectManager.h"
#include <cassert>
#include "imgui.h"

IF::ObjectManager::~ObjectManager()
{
	objList.clear();
	assert(objList.empty() && "�C���X�^���X��j���ł��܂���");
}

void IF::ObjectManager::Draw()
{
	for (auto com : objList)
	{
		com->Draw();
	}
}

void IF::ObjectManager::Update()
{
	auto buff = objList;
	for (auto com : buff)
	{
		com->Update();
	}
}

#ifdef _DEBUG
void IF::ObjectManager::GUI()
{
	for (auto com : objList)
	{
		com->GUI();
	}
}

#endif