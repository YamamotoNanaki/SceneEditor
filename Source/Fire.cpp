//#include "Fire.h"
//#include "Rand.h"
//#include "Ease.h"
//
//using namespace IF;
//using namespace DirectX;
//
//Fire::Fire(XMFLOAT3 pos, float scale, unsigned int maxFlame) :pos(pos), scale(scale), maxFlame(maxFlame) {};
//
//void Fire::Initialize(ID3D12Device* device)
//{
//	HRESULT result;
//	for (int i = 0; i < _countof(particle); i++)
//	{
//		particle[i].Initialize();
//	}
//	Particle::VIInitialize();
//}
//
//void Fire::AddParticle()
//{
//	const float offset = 500;
//	int j = 0;
//	for (int i = 0; i < _countof(particle); i++)
//	{
//		if (particle[i].flag == false && j < 3)
//		{
//			particle[i].targetY = pos.y + scale * 25 + (float)Rand::GetRand(0, offset) / 50;
//			particle[i].startY = pos.y + (float)Rand::GetRand(0, offset) / 100;
//			particle[i].flag = true;
//			particle[i].flame = 0;
//			particle[i].position = { pos.x,pos.y,pos.z };
//			j++;
//		}
//		if (j == 3)break;
//	}
//}
//
//void Fire::Update(XMMATRIX matView, XMMATRIX matProjection, XMMATRIX matBillBoard)
//{
//	int offset = 100;
//	if (moveOffset == false)
//	{
//		AddParticle();
//		for (int i = 0; i < _countof(particle); i++)
//		{
//			if (particle[i].flag == false)continue;
//
//			particle[i].position.y = Ease::InQuad(particle[i].startY, particle[i].targetY, maxFlame, particle[i].flame);
//
//			particle[i].position.x += (float)Rand::GetRand(-offset, +offset) / offset / 3;
//
//			particle[i].position.z += (float)Rand::GetRand(-offset, +offset) / offset / 3;
//
//			particle[i].scale = Ease::Lerp(scale, 0, maxFlame, particle[i].flame);
//
//			particle[i].flame++;
//			if (particle[i].flame == maxFlame)particle[i].flag = false;
//
//			particle[i].Update(matView, matProjection, matBillBoard);
//		}
//		moveOffset = true;
//	}
//	else moveOffset = false;
//}
//
//void Fire::Draw(ID3D12GraphicsCommandList* commandList, vector<D3D12_VIEWPORT>viewport)
//{
//	for (int i = 0; i < _countof(particle); i++)
//	{
//		if (particle[i].flag == false)continue;
//		particle[i].Draw(viewport);
//	}
//}
