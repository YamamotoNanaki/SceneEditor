#include "Debug.h"
#include <d3d12.h>

#ifdef _DEBUG
void Debug()
{
	// �f�o�b�N���C���[���I����
	ID3D12Debug* debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
}
#endif
