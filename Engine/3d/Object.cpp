#include "Object.h"
#include "View.h"
#include <fstream>
#include <sstream>
#include <string>
#include <cassert>

using namespace IF;
using namespace std;
using namespace IF::BillBoard;
using namespace Microsoft::WRL;

LightManager* Object::light = nullptr;
ComPtr<ID3D12Device> Object::device = nullptr;
ComPtr<ID3D12GraphicsCommandList> Object::commandList = nullptr;

void IF::Object::DrawBefore(ID3D12RootSignature* root, D3D_PRIMITIVE_TOPOLOGY topology)
{
	commandList->SetGraphicsRootSignature(root);
	commandList->IASetPrimitiveTopology(topology);
}

void IF::Object::SetModel(Model* model)
{
	this->model = model;
}

void IF::Object::Initialize(Model* model)
{
	HRESULT result;
	//�萔�o�b�t�@�̃q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//�萔�o�b�t�@�̃��\�[�X�ݒ�
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffTransform));
	assert(SUCCEEDED(result));


	//�萔�o�b�t�@�̃}�b�s���O
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));

	this->model = model;

	cb.Initialize(device.Get());
}

void Object::Update(Matrix matView, Matrix matProjection, Float3 cameraPos, int mode)
{
	Matrix matScale, matRot, matTrams;

	//�X�P�[���A��]�A���s�ړ�
	matScale = MatrixScaling(scale.x, scale.y, scale.z);
	matRot = MatrixIdentity();
	matRot *= MatrixRotationZ(rotation.z);
	matRot *= MatrixRotationX(rotation.x);
	matRot *= MatrixRotationY(rotation.y);
	matTrams = MatrixTranslation(position.x, position.y, position.z);
	//���[���h�s��̍���
	matWorld = MatrixIdentity();
	if (mode == BILLBOARD)matWorld *= View::matBillBoard;
	else if (mode == YBOARD)matWorld *= View::matBillBoardY;
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrams;
	//�e�I�u�W�F�N�g�������
	if (parent != nullptr)
	{
		matWorld *= parent->matWorld;
	}

	//�萔�o�b�t�@�ւ̃f�[�^�]��
	constMapTransform->viewPro = matView * matProjection;
	constMapTransform->world = matWorld;
	constMapTransform->cameraPos = cameraPos;
}

void Object::Draw(vector<D3D12_VIEWPORT> viewport)
{
	if (model == nullptr)
	{
		assert(0 && "���f�����Z�b�g����Ă��܂���");
		return;
	}

	light->Draw(4);
	commandList->SetGraphicsRootConstantBufferView(0, cb.GetGPUAddress());
	model->Draw(Object::commandList.Get(), viewport, constBuffTransform.Get());
}

void IF::Object::Draw(vector<D3D12_VIEWPORT> viewport, unsigned short texNum)
{
	if (model == nullptr)
	{
		assert(0 && "���f�����Z�b�g����Ă��܂���");
		return;
	}

	light->Draw(4);

	model->Draw(Object::commandList.Get(), viewport, constBuffTransform.Get(), texNum);
}

Object::~Object()
{
	constBuffTransform->Unmap(0, nullptr);
}

void IF::Object::SetColor(int r, int g, int b, int a)
{
	cb.SetColor(r, g, b, a);
}

void IF::Object::SetBright(int r, int g, int b)
{
	cb.SetBright(r, g, b);
}

void IF::Object::SetAlpha(int alpha)
{
	cb.SetAlpha(alpha);
}
