#include "SceneManager.h"

using namespace IF;
using namespace std;

void IF::SceneManager::Initialize(int winWidth, int winHeight, ID3D12Device* device, ID3D12GraphicsCommandList* commandList, std::vector<D3D12_VIEWPORT> viewport)
{
	this->winWidth = winWidth;
	this->winHeight = winHeight;
	this->device = device;
	this->commandList = commandList;
	this->viewport = viewport;
	scene = new Scene(winWidth, winHeight, device, commandList, viewport);
}

void IF::SceneManager::Update()
{
	scene->Update();
}

void IF::SceneManager::Draw()
{
	scene->Draw();
}

IF::SceneManager::~SceneManager()
{
	delete scene;
}
