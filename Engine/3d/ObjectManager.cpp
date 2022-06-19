#include "ObjectManager.h"
#include <cassert>

IF::ObjectManager::~ObjectManager()
{
	objList.clear();
	assert(objList.empty() && "インスタンスを破棄できません");
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
		if (objList.size() == 1)break;
		if (com == *objList.end())break;
	}
}
