#pragma once
//#include "Particle.h"
//#include <vector>
//
//class Fire
//{
//	//ネームスペースを使用
//	template <class T> using vector = std::vector<T>;
//	using XMFLOAT3 = DirectX::XMFLOAT3;
//	using XMMATRIX = DirectX::XMMATRIX;
//private:
//	unsigned int maxFlame;		//最大フレーム数
//	float scale;				//大きさ
//	bool moveOffset = 0;		//2フレーム中1フレームで更新する
//
//public:
//	XMFLOAT3 pos;				//ポジション
//	IF::Particle particle[500];	//パーティクル
//
//private:
//	//パーティクルを追加
//	void AddParticle();
//
//public:
//	//設定初期化
//	Fire(XMFLOAT3 pos, float scale = 1.0f, unsigned int maxFlame = 100);
//	//パーティクル初期化
//	void Initialize(ID3D12Device* device);
//	void Update(XMMATRIX matView, XMMATRIX matProjection, XMMATRIX matBillBoard = DirectX::XMMatrixIdentity());
//	void Draw(ID3D12
// sCommandList* commandList, vector<D3D12_VIEWPORT>viewport);
//};