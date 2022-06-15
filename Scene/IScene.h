#pragma once
#include <vector>

namespace IF
{
	class IScene
	{
	public:
		virtual void Initialize(int winWidth, int winHeight, ID3D12Device* device, ID3D12GraphicsCommandList* commandList, std::vector<D3D12_VIEWPORT> viewport, HWND& hwnd) = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;
		virtual ~IScene() = 0;
	};
}