#include "FBXModel.h"
#include "DirectX12.h"
#include <cassert>

using namespace IF;
using namespace std;

void IF::FBXModel::Draw(ID3D12Resource* address)
{
	for (unique_ptr<Node>& node : nodes)
	{
		for (auto mesh : node->meshes)
		{
			mesh->Draw(address);
		}
	}
}

void IF::FBXModel::Draw(ID3D12Resource* address, unsigned short texNum)
{
	for (unique_ptr<Node>& node : nodes)
	{
		for (auto mesh : node->meshes)
		{
			mesh->Draw(address, texNum);
		}
	}
}
