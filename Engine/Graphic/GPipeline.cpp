#include "GPipeline.h"


using namespace IF;

GPipeline::GPipeline(ID3DBlob* vsBlob, ID3DBlob* psBlob, ID3DBlob* gsBlob, D3D12_INPUT_ELEMENT_DESC* inputLayout, int layoutCount)
{
	for (int i = 0; i < _countof(pipelineDesc); i++)
	{
		pipelineDesc[i].VS.pShaderBytecode = vsBlob->GetBufferPointer();
		pipelineDesc[i].VS.BytecodeLength = vsBlob->GetBufferSize();
		if (gsBlob != nullptr)
		{
			pipelineDesc[i].GS.pShaderBytecode = gsBlob->GetBufferPointer();
			pipelineDesc[i].GS.BytecodeLength = gsBlob->GetBufferSize();
		}
		pipelineDesc[i].PS.pShaderBytecode = psBlob->GetBufferPointer();
		pipelineDesc[i].PS.BytecodeLength = psBlob->GetBufferSize();
		//デプスステンシルステートの設定
		pipelineDesc[i].DepthStencilState.DepthEnable = true;		//深度テストを行う
		pipelineDesc[i].DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		pipelineDesc[i].DSVFormat = DXGI_FORMAT_D32_FLOAT;		//深度値フォーマット

		pipelineDesc[i].SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
		pipelineDesc[i].RasterizerState.CullMode = D3D12_CULL_MODE_BACK;  // 背面をカリング
		pipelineDesc[i].RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // ポリゴン内塗りつぶし
		pipelineDesc[i].RasterizerState.DepthClipEnable = true; // 深度クリッピングを有効に

		D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc[i].BlendState.RenderTarget[0];
		blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

		blenddesc.BlendEnable = true;						//ブレンドを有効にする
		blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;			//加算
		blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;			//ソースの値を100%使う
		blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;			//デストの値を  0%使う

		if (i % 4 == 1)
		{
			blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//加算
			blenddesc.SrcBlend = D3D12_BLEND_ONE;				//ソースの値を100%使う
			blenddesc.DestBlend = D3D12_BLEND_ONE;				//ソースの値を100%使う
			pipelineDesc[i].DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		}
		else if (i % 4 == 2)
		{
			blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;		//デストからソースを減算
			blenddesc.SrcBlend = D3D12_BLEND_ONE;				//ソースの値を100%使う
			blenddesc.DestBlend = D3D12_BLEND_ONE;				//ソースの値を100%使う
			pipelineDesc[i].DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		}
		else if (i % 4 == 3)
		{
			blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//加算
			blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;		//1.0f-デストカラーの値
			blenddesc.DestBlend = D3D12_BLEND_ZERO;				//使わない
			pipelineDesc[i].DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
		}
		else
		{
			blenddesc.BlendOp = D3D12_BLEND_OP_ADD;				//加算
			blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			//ソースのアルファ値
			blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;		//1.0f-ソースのアルファ値
			pipelineDesc[i].DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	//書き込み許可
		}

		pipelineDesc[i].InputLayout.pInputElementDescs = inputLayout;
		pipelineDesc[i].InputLayout.NumElements = layoutCount;

		pipelineDesc[i].PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		pipelineDesc[i].NumRenderTargets = 1; // 描画対象は1つ
		pipelineDesc[i].RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0～255指定のRGBA
		pipelineDesc[i].SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

		if (i > 3 && i < 8)
		{
			pipelineDesc[i].DepthStencilState.DepthEnable = false;		//深度テストを行わない
			pipelineDesc[i].RasterizerState.CullMode = D3D12_CULL_MODE_NONE;  // 背面をカリングしない
		}
		if (i > 7)pipelineDesc[i].PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
	}
}

void GPipeline::RootSignature(ID3D12RootSignature& rootsignature)
{
	for (int i = 0; i < _countof(pipelineDesc); i++)
	{
		// パイプラインにルートシグネチャをセット
		pipelineDesc[i].pRootSignature = &rootsignature;
	}
}
