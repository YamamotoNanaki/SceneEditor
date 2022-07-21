#include "Debug.h"
#include <d3d12.h>
#include <wrl.h>

#ifdef _DEBUG
void Debug()
{
	// デバックレイヤーをオンに
	Microsoft::WRL::ComPtr<ID3D12Debug1> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
}
#endif
