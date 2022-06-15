//#include "Particle.h"
//#include "View.h"
//#include "MathConvert.h"
//
//using namespace DirectX;
//using namespace IF;
//
//ID3D12Device* Particle::device = nullptr;
//ID3D12GraphicsCommandList* Particle::commandList = nullptr;
//PV* Particle::vi = nullptr;
//
//void Particle::Initialize()
//{
//	HRESULT result;
//	//�萔�o�b�t�@�̃q�[�v�ݒ�
//	D3D12_HEAP_PROPERTIES heapProp{};
//	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
//	//�萔�o�b�t�@�̃��\�[�X�ݒ�
//	D3D12_RESOURCE_DESC resdesc{};
//	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	resdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
//	resdesc.Height = 1;
//	resdesc.DepthOrArraySize = 1;
//	resdesc.MipLevels = 1;
//	resdesc.SampleDesc.Count = 1;
//	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//	//�萔�o�b�t�@�̐���
//	result = device->CreateCommittedResource(
//		&heapProp, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
//		IID_PPV_ARGS(&constBuffTransform));
//	assert(SUCCEEDED(result));
//	//�萔�o�b�t�@�̃}�b�s���O
//	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
//	assert(SUCCEEDED(result));
//
//}
//
//void Particle::VIInitialize()
//{
//	VertexPos vertices[] = {
//		// x   y   z        u    v
//		//�O
//		{{0, 0, 0}},	//����
//	};
//
//	vi->SetVerticleIndex(vertices, _countof(vertices));
//	vi->Initialize(device);
//}
//
//void IF::Particle::SetDeviceCommand(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
//{
//	Particle::device = device;
//	Particle::commandList = commandList;
//}
//
//void Particle::Update(XMMATRIX matView, XMMATRIX matProjection, XMMATRIX matBillBoard)
//{
//	XMMATRIX matScale, matTrams;
//
//	//�X�P�[���A��]�A���s�ړ�
//	matScale = XMMatrixScaling(scale, scale, scale);
//	matTrams = XMMatrixTranslation(position.x, position.y, position.z);
//	//���[���h�s��̍���
//	matWorld = XMMatrixIdentity();
//	matWorld *= matScale;
//	matWorld *= matTrams;
//
//	//�萔�o�b�t�@�ւ̃f�[�^�]��
//	constMapTransform->mat = MatrixConvert(matWorld * matView * matProjection);
//	constMapTransform->matBillboard = MatrixConvert(matBillBoard);
//}
//
//void IF::Particle::DrawBefore(ID3D12RootSignature* root, ID3D12DescriptorHeap* srvHeap, D3D12_GPU_VIRTUAL_ADDRESS GPUAddress, D3D_PRIMITIVE_TOPOLOGY topology)
//{
//	commandList->SetGraphicsRootSignature(root);
//	commandList->IASetPrimitiveTopology(topology);
//	commandList->SetGraphicsRootConstantBufferView(0, GPUAddress);
//}
//
//void Particle::Draw(vector<D3D12_VIEWPORT> viewport)
//{
//	for (int i = 0; i < viewport.size(); i++)
//	{
//		commandList->RSSetViewports(1, &viewport[i]);
//		//���_�o�b�t�@�̐ݒ�
//		commandList->IASetVertexBuffers(0, 1, &vi->GetVertexView());
//		//�萔�o�b�t�@�r���[�̐ݒ�
//		commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
//		//�`��R�}���h
//		commandList->DrawInstanced((UINT)vi->GetSize(), 1, 0, 0);
//	}
//}
//
//Particle::~Particle()
//{
//	constBuffTransform->Unmap(0, nullptr);
//	delete vi;
//}
