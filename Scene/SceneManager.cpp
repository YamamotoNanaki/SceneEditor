#include "SceneManager.h"

using namespace IF;
using namespace std;

void IF::SceneManager::Initialize(int winWidth, int winHeight, ID3D12Device* device, ID3D12GraphicsCommandList* commandList, std::vector<D3D12_VIEWPORT> viewport, HWND& hwnd)
{
	this->winWidth = winWidth;
	this->winHeight = winHeight;
	this->device = device;
	this->commandList = commandList;
	this->viewport = viewport;
	this->hwnd = hwnd;
	scene = new Scene;
	scene->Initialize(winWidth, winHeight, device, commandList, viewport, hwnd);
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
