#pragma once
//#include "Particle.h"
//#include <vector>
//
//class Fire
//{
//	//�l�[���X�y�[�X���g�p
//	template <class T> using vector = std::vector<T>;
//	using XMFLOAT3 = DirectX::XMFLOAT3;
//	using XMMATRIX = DirectX::XMMATRIX;
//private:
//	unsigned int maxFlame;		//�ő�t���[����
//	float scale;				//�傫��
//	bool moveOffset = 0;		//2�t���[����1�t���[���ōX�V����
//
//public:
//	XMFLOAT3 pos;				//�|�W�V����
//	IF::Particle particle[500];	//�p�[�e�B�N��
//
//private:
//	//�p�[�e�B�N����ǉ�
//	void AddParticle();
//
//public:
//	//�ݒ菉����
//	Fire(XMFLOAT3 pos, float scale = 1.0f, unsigned int maxFlame = 100);
//	//�p�[�e�B�N��������
//	void Initialize(ID3D12Device* device);
//	void Update(XMMATRIX matView, XMMATRIX matProjection, XMMATRIX matBillBoard = DirectX::XMMatrixIdentity());
//	void Draw(ID3D12
// sCommandList* commandList, vector<D3D12_VIEWPORT>viewport);
//};