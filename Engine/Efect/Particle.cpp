#include "Particle.h"
#include "View.h"
#include "Ease.h"
#include "DirectX12.h"
#include <cassert>

using namespace IF;
using namespace std;

ICamera* Particle::camera = nullptr;
PV Particle::vi{};

void Particle::Initialize()
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
	ID3D12Device* device = DirectX12::Instance()->GetDevice();
	result = device->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE, &resdesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffTransform));
	assert(SUCCEEDED(result));
	//�萔�o�b�t�@�̃}�b�s���O
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
	assert(SUCCEEDED(result));

	cb.Initialize();
}

void Particle::VIInitialize()
{
	VertexPos vertices[] = {
		// x   y   z        u    v
		//�O
		{{0, 0, 0}},	//����
	};

	ID3D12Device* device = DirectX12::Instance()->GetDevice();
	vi.SetVerticleIndex(vertices, _countof(vertices));
	vi.Initialize();
}

void Particle::MatUpdate()
{
	Matrix matScale, matTrams, matRot;

	//�X�P�[���A��]�A���s�ړ�
	matScale = MatrixScaling(scale[0], scale[1], scale[2]);
	matTrams = MatrixTranslation(position.x, position.y, position.z);
	matRot = MatrixRotationZ(ConvertToRadians(rota));
	//���[���h�s��̍���
	matWorld = MatrixIdentity();
	matWorld *= matScale;
	matWorld *= matRot;
	matWorld *= matTrams;

	//�萔�o�b�t�@�ւ̃f�[�^�]��
	constMapTransform->mat = matWorld * *camera->GetMatView() * *camera->GetMatPro();
	constMapTransform->matBillboard = View::matBillBoard;
}

void IF::Particle::Update()
{
	timer.SafeUpdate();
	if (timer.IsEnd())
	{
		deleteFlag = true;
		return;
	}
	//�F
	double (*ease)(double, double, double, double);
	ease = Ease::Lerp;
	if (colorF)
	{
		for (int i = 0; i < 4; i++)
		{
			if (colorEase[i] == Lerp)ease = Ease::Lerp;
			else if (colorEase[i] == EaseIn)ease = Ease::InQuad;
			else if (colorEase[i] == EaseOut)ease = Ease::OutQuad;
			else if (colorEase[i] == EaseInOut)ease = Ease::InOutQuad;
			color[i] = ease(startcolor[i], endcolor[i], deleteTime, timer.NowTime());
		}
	}
	cb.SetColor(color[0], color[1], color[2], color[3]);

	//��]
	if (rotaF)
	{
		if (rotaEase == Lerp)ease = Ease::Lerp;
		else if (rotaEase == EaseIn)ease = Ease::InQuad;
		else if (rotaEase == EaseOut)ease = Ease::OutQuad;
		else if (rotaEase == EaseInOut)ease = Ease::InOutQuad;
		rota = ease(rotastart, rotaend, deleteTime, timer.NowTime());
	}
	else
	{
		rota += rotaspeed;
	}

	//�X�P�[��
	if (scaleF)
	{
		for (int i = 0; i < 3; i++)
		{
			if (scaleEase[i] == Lerp)ease = Ease::Lerp;
			else if (scaleEase[i] == EaseIn)ease = Ease::InQuad;
			else if (scaleEase[i] == EaseOut)ease = Ease::OutQuad;
			else if (scaleEase[i] == EaseInOut)ease = Ease::InOutQuad;
			scale[i] = ease(startScale[i], endScale[i], deleteTime, timer.NowTime());
		}
	}

	if (setSpeedPosFlag)
	{
		//�X�s�[�h
		if (speedF)
		{
			for (int i = 0; i < 3; i++)
			{
				if (speedEase[i] == Lerp)ease = Ease::Lerp;
				else if (speedEase[i] == EaseIn)ease = Ease::InQuad;
				else if (speedEase[i] == EaseOut)ease = Ease::OutQuad;
				else if (speedEase[i] == EaseInOut)ease = Ease::InOutQuad;
				speed[i] = ease(startSpeed[i], endSpeed[i], deleteTime, timer.NowTime());
			}
		}
		position.x += speed[0];
		position.y += speed[1];
		position.z += speed[2];
		if (isGravity)position.y += gravity;
	}
	else
	{
		//�|�W�V����
		if (posF)
		{
			float pos[3];
			for (int i = 0; i < 3; i++)
			{
				if (posEase[i] == Lerp)ease = Ease::Lerp;
				else if (posEase[i] == EaseIn)ease = Ease::InQuad;
				else if (posEase[i] == EaseOut)ease = Ease::OutQuad;
				else if (posEase[i] == EaseInOut)ease = Ease::InOutQuad;
				pos[i] = ease(startposition[i], endposition[i], deleteTime, timer.NowTime());
			}
			position = pos;
		}
	}
}

void IF::Particle::DrawBefore(ID3D12RootSignature* root, D3D_PRIMITIVE_TOPOLOGY topology)
{
	ID3D12GraphicsCommandList* commandList = DirectX12::Instance()->GetCmdList();
	commandList->SetGraphicsRootSignature(root);
	commandList->IASetPrimitiveTopology(topology);
}

void Particle::Draw()
{
	ID3D12GraphicsCommandList* commandList = DirectX12::Instance()->GetCmdList();
	commandList->SetGraphicsRootConstantBufferView(0, cb.GetGPUAddress());
	//���_�o�b�t�@�̐ݒ�
	commandList->IASetVertexBuffers(0, 1, &vi.GetVertexView());
	//�萔�o�b�t�@�r���[�̐ݒ�
	commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
	//�`��R�}���h
	commandList->DrawInstanced((UINT)vi.GetSize(), 1, 0, 0);
}

Particle::~Particle()
{
	constBuffTransform->Unmap(0, nullptr);
}
