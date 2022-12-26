#include "Mesh.h"
#include "DirectX12.h"
#include "Texture.h"
#include <cassert>

using namespace std;

void IF::Mesh::Draw(ID3D12Resource* address)
{
	static ID3D12GraphicsCommandList* commandList = DirectX12::Instance()->GetCmdList();
	if (material.tex == true)Texture::Instance()->SetTexture(material.texNum);
	//���_�o�b�t�@�̐ݒ�
	commandList->IASetVertexBuffers(0, 1, &GetVertexView());
	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	commandList->IASetIndexBuffer(&GetIndexView());
	//�萔�o�b�t�@�r���[�̐ݒ�
	commandList->SetGraphicsRootConstantBufferView(2, address->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(3, constBuffTransform1->GetGPUVirtualAddress());
	//�`��R�}���h
	commandList->DrawIndexedInstanced((UINT)GetSize(), 1, 0, 0, 0);
}

void IF::Mesh::Draw(ID3D12Resource* address, unsigned short texNum)
{
	static ID3D12GraphicsCommandList* commandList = DirectX12::Instance()->GetCmdList();
	Texture::Instance()->SetTexture(texNum);
	//���_�o�b�t�@�̐ݒ�
	commandList->IASetVertexBuffers(0, 1, &GetVertexView());
	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	commandList->IASetIndexBuffer(&GetIndexView());
	//�萔�o�b�t�@�r���[�̐ݒ�
	commandList->SetGraphicsRootConstantBufferView(2, address->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(3, constBuffTransform1->GetGPUVirtualAddress());
	//�`��R�}���h
	commandList->DrawIndexedInstanced((UINT)GetSize(), 1, 0, 0, 0);
}

void IF::Mesh::SetVerticleIndex(std::vector<VertexBone> vertices, size_t vertexCount, std::vector<UINT> indices, size_t indexCount)
{
	for (int i = 0; i < vertexCount; i++)
	{
		this->vertices.push_back(vertices[i]);
	}
	for (int i = 0; i < indexCount; i++)
	{
		this->indices.push_back(indices[i]);
	}
}

void IF::Mesh::SetVerticleIndex(VertexBone* vertices, size_t vertexCount, UINT* indices, size_t indexCount)
{
	for (int i = 0; i < vertexCount; i++)
	{
		this->vertices.push_back(vertices[i]);
	}
	for (int i = 0; i < indexCount; i++)
	{
		this->indices.push_back(indices[i]);
	}
}

void IF::Mesh::Initialize()
{
	HRESULT result;

	//�萔�o�b�t�@�̃q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp2{};
	heapProp2.Type = D3D12_HEAP_TYPE_UPLOAD;
	//�萔�o�b�t�@�̃��\�[�X�ݒ�
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferMaterial) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	result = DirectX12::Instance()->GetDevice()->CreateCommittedResource(
		&heapProp2, D3D12_HEAP_FLAG_NONE, &resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffTransform1));
	assert(SUCCEEDED(result));

	result = constBuffTransform1->Map(0, nullptr, (void**)&constMapMaterial);
	assert(SUCCEEDED(result));

	constMapMaterial->ambient = material.ambient;
	constMapMaterial->diffuse = material.diffuse;
	constMapMaterial->specular = material.specular;
	constMapMaterial->alpha = material.alpha;

	constBuffTransform1->Unmap(0, nullptr);

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * vertices.size());


#pragma endregion ���_�f�[�^�[

	//---------------------------

#pragma region ���_�o�b�t�@�̊m��
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
	ID3D12Device* device = DirectX12::Instance()->GetDevice();
	result = device->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE, &resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

#pragma endregion ���_�o�b�t�@�̊m��

	//---------------------------

#pragma region ���_�o�b�t�@�ւ̃f�[�^�]��
// GPU��̃o�b�t�@�ɑΉ��������z���������擾
	VertexBone* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));

	// �S���_�ɑ΂���
	for (int i = 0; i < vertices.size(); i++)
	{
		vertMap[i] = vertices[i];   // ���W���R�s�[
	}

	// �}�b�v������
	vertBuff->Unmap(0, nullptr);

#pragma endregion ���_�o�b�t�@�ւ̃f�[�^�]��

	//-----------------------------

#pragma region ���_�o�b�t�@�r���[�̍쐬
// ���_�o�b�t�@�r���[�̍쐬

	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

#pragma endregion ���_�o�b�t�@�փr���[�̍쐬

	//-------------------------------

	if (!indices.size() == 0)
	{

#pragma region �C���f�b�N�X�o�b�t�@�̐���

		// �C���f�b�N�X�f�[�^�S�̂̃T�C�Y
		UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * indices.size());
		// �C���f�b�N�X�o�b�t�@�̐ݒ�
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Width = sizeIB;//�C���f�b�N�X�o�b�t�@�̐���
		resDesc.Height = 1;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		result = device->CreateCommittedResource(
			&heapProp,				//�q�[�v�ݒ�
			D3D12_HEAP_FLAG_NONE,
			&resDesc,				//���\�[�X�ݒ�
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&indexBuff));

#pragma endregion �C���f�b�N�X�o�b�t�@�̐���

		//----------------------------

#pragma region �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
//GPU��̃o�b�t�@�ɑΉ��������z���������擾
		uint16_t* indexMap = nullptr;
		result = indexBuff->Map(0, nullptr, (void**)&indexMap);

		//�S�C���f�b�N�X�ɑ΂���
		for (int i = 0; i < indices.size(); i++)
		{
			indexMap[i] = indices[i];	//�C���f�b�N�X���R�s�[
		}

		//�Ȃ��������
		indexBuff->Unmap(0, nullptr);

		ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
		ibView.Format = DXGI_FORMAT_R16_UINT;
		ibView.SizeInBytes = sizeIB;

#pragma endregion �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��

	}

}

D3D12_VERTEX_BUFFER_VIEW& IF::Mesh::GetVertexView()
{
	return vbView;
}

D3D12_INDEX_BUFFER_VIEW& IF::Mesh::GetIndexView()
{
	return ibView;
}

size_t IF::Mesh::GetSize()
{
	return indices.size();
}
