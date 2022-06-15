#include "SpriteVI.h"
#include <cassert>

void IF::SV::Initialize(ID3D12Device* device)
{
	HRESULT result;

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));

	//---------------------------

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};   // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

	D3D12_RESOURCE_DESC resDesc{};  // ���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���_�o�b�t�@�̐���

	result = device->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE, &resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	//---------------------------

	// GPU��̃o�b�t�@�ɑΉ��������z���������擾
	Vertex2D* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	// �S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];   // ���W���R�s�[
	}

	//-----------------------------

	// ���_�o�b�t�@�r���[�̍쐬

	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

}

D3D12_VERTEX_BUFFER_VIEW& IF::SV::GetVertexView()
{
	return vbView;
}

void IF::SV::SetVerticle(Vertex2D* vertices)
{
	for (int i = 0; i < _countof(this->vertices); i++)
	{
		this->vertices[i] = vertices[i];
	}
}

unsigned int IF::SV::GetSize()
{
	return _countof(vertices);
}

void IF::SV::Transfer()
{
	Vertex2D* vertMap = nullptr;
	HRESULT result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	// �S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++)
	{
		vertMap[i] = vertices[i];   // ���W���R�s�[
	}
}

IF::SV::~SV()
{
	// �}�b�v������
	vertBuff->Unmap(0, nullptr);
}
