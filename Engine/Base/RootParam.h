#pragma once
#include <vector>
#include <d3dx12.h>

#pragma comment(lib,"d3d12.lib") 

namespace IF
{
	class RootParam
	{
		template <class T>using vector = std::vector<T>;

	private:
		void ConstRoot(int num);
		void TexRoot(D3D12_DESCRIPTOR_RANGE& descRangeSRV, int texMax);

	public:
		vector<D3D12_ROOT_PARAMETER> rootParams = {};
		RootParam(D3D12_DESCRIPTOR_RANGE& descRangeSRV, int texMax);
	};
}