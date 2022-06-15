#include "Debug.h"
#include <d3d12.h>

#ifdef _DEBUG
void Debug()
{
	// デバックレイヤーをオンに
	ID3D12Debug* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
}
#endif
