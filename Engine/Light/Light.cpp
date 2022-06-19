#include "Light.h"
#include <cassert>

using namespace IF;
using namespace Microsoft::WRL;

ComPtr<ID3D12Device> IF::LightManager::device = nullptr;
ComPtr<ID3D12GraphicsCommandList> IF::LightManager::commandList = nullptr;

void IF::LightManager::SetDeviceCommand(ID3D12Device* device, ID3D12GraphicsCommandList* commandList)
{
	assert(!LightManager::device && "device == nullptr");
	assert(device && "device ‰Šú‰»Ï‚Ý");
	LightManager::device = device;
	assert(!LightManager::commandList && "commandList == nullptr");
	assert(commandList && "commandList ‰Šú‰»Ï‚Ý");
	LightManager::commandList = commandList;
}

void IF::LightManager::Initialize()
{
	HRESULT result;
	D3D12_HEAP_PROPERTIES a = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC b = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff);

	result = device->CreateCommittedResource(&a, D3D12_HEAP_FLAG_NONE, &b, D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&constBuff));

	assert(SUCCEEDED(result));

	TransferConstBuffer();
}

void IF::LightManager::TransferConstBuffer()
{
	HRESULT result;
	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap);
	assert(SUCCEEDED(result));
	constMap->ambientColor = ambientColor;
	for (int i = 0; i < DLightNum; i++) {
		if (dLight[i].IsActive()) {
			constMap->dLights[i].active = 1;
			constMap->dLights[i].lightv = -dLight[i].GetLightDir();
			constMap->dLights[i].lightColor = dLight[i].GetLightColor();
		}
		else {
			constMap->dLights[i].active = 0;
		}
	}
	for (int i = 0; i < PLightNum; i++) {
		if (pLight[i].IsActive()) {
			constMap->pLights[i].active = 1;
			constMap->pLights[i].lightPos = pLight[i].GetLightPos();
			constMap->pLights[i].lightColor = pLight[i].GetLightColor();
			constMap->pLights[i].lightAtten = pLight[i].GetLightAtten();
		}
		else {
			constMap->pLights[i].active = 0;
		}
	}
	for (int i = 0; i < SLightNum; i++) {
		if (sLight[i].IsActive()) {
			constMap->sLights[i].active = 1;
			constMap->sLights[i].lightv = -sLight[i].GetLightDir();
			constMap->sLights[i].lightPos = sLight[i].GetLightPos();
			constMap->sLights[i].lightColor = sLight[i].GetLightColor();
			constMap->sLights[i].lightAtten = sLight[i].GetLightAtten();
			constMap->sLights[i].lightFactorAngleCos = sLight[i].GetLightFactorAngle();
		}
		else {
			constMap->sLights[i].active = 0;
		}
	}
	for (int i = 0; i < CShadowNum; i++) {
		if (cShadow[i].IsActive()) {
			constMap->cShadows[i].active = 1;
			constMap->cShadows[i].shadowv = -cShadow[i].GetShadowDir();
			constMap->cShadows[i].casterPos = cShadow[i].GetCasterPos();
			constMap->cShadows[i].distanceCasterLight = cShadow[i].GetDistanceCasterLight();
			constMap->cShadows[i].shadowAtten = cShadow[i].GetShadowAtten();
			constMap->cShadows[i].shadowFactorAngleCos = cShadow[i].GetShadowFactorAngle();
		}
		else {
			constMap->sLights[i].active = 0;
		}
	}
}

void IF::LightManager::DefaultLightSetting()
{
	dLight[0].SetActive(true);
	dLight[0].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dLight[0].SetLightDir({ 0.0f, -1.0f, 0.0f });

	dLight[1].SetActive(true);
	dLight[1].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dLight[1].SetLightDir({ +0.5f, +0.1f, +0.2f });

	dLight[2].SetActive(true);
	dLight[2].SetLightColor({ 1.0f, 1.0f, 1.0f });
	dLight[2].SetLightDir({ -0.5f, +0.1f, -0.2f });
}

void IF::LightManager::SetAmbientColor(const Float3& color)
{
	ambientColor = color;
	dirty = true;
}

void IF::LightManager::SetDirLightActive(int index, bool active)
{
	assert(0 <= index && index < DLightNum);

	dLight[index].SetActive(active);
}

void IF::LightManager::SetDirLightDir(int index, const Vector3& lightdir)
{
	assert(0 <= index && index < DLightNum);

	dLight[index].SetLightDir({ lightdir.x,lightdir.y,lightdir.z });
	dirty = true;
}

void IF::LightManager::SetDirLightColor(int index, const Float3& lightcolor)
{
	assert(0 <= index && index < DLightNum);

	dLight[index].SetLightColor(lightcolor);
	dirty = true;
}

void IF::LightManager::SetPointLightActive(int index, bool active)
{
	assert(0 <= index && index < PLightNum);

	pLight[index].SetActive(active);
}

void IF::LightManager::SetPointLightPos(int index, const Float3& lightpos)
{
	assert(0 <= index && index < PLightNum);

	pLight[index].SetLightPos(lightpos);
	dirty = true;
}

void IF::LightManager::SetPointLightColor(int index, const Float3& lightcolor)
{
	assert(0 <= index && index < PLightNum);

	pLight[index].SetLightColor(lightcolor);
	dirty = true;
}

void IF::LightManager::SetPointLightAtten(int index, const Float3& lightAtten)
{
	assert(0 <= index && index < PLightNum);

	pLight[index].SetLightAtten(lightAtten);
	dirty = true;
}

void IF::LightManager::SetSpotLightActive(int index, bool active)
{
	assert(0 <= index && index < SLightNum);

	sLight[index].SetActive(active);
}

void IF::LightManager::SetSpotLightDir(int index, const Vector3& lightdir)
{
	assert(0 <= index && index < SLightNum);

	sLight[index].SetLightDir(lightdir);
	dirty = true;
}

void IF::LightManager::SetSpotLightPos(int index, const Float3& lightpos)
{
	assert(0 <= index && index < SLightNum);

	sLight[index].SetLightPos(lightpos);
	dirty = true;
}

void IF::LightManager::SetSpotLightColor(int index, const Float3& lightcolor)
{
	assert(0 <= index && index < SLightNum);

	sLight[index].SetLightColor(lightcolor);
	dirty = true;
}

void IF::LightManager::SetSpotLightAtten(int index, const Float3& lightAtten)
{
	assert(0 <= index && index < SLightNum);

	sLight[index].SetLightAtten(lightAtten);
	dirty = true;
}

void IF::LightManager::SetSpotLightFactorAngle(int index, const Float2& lightFactorAngle)
{
	assert(0 <= index && index < SLightNum);

	sLight[index].SetLightFactorAngle(lightFactorAngle);
	dirty = true;
}

void IF::LightManager::SetCircleShadowActive(int index, bool active)
{
	assert(0 <= index && index < CShadowNum);

	cShadow[index].SetActive(active);
}

void IF::LightManager::SetCircleShadowDir(int index, const Vector3& lightdir)
{
	assert(0 <= index && index < CShadowNum);

	cShadow[index].SetShadowDir(lightdir);
	dirty = true;
}

void IF::LightManager::SetCircleShadowCasterPos(int index, const Float3& lightpos)
{
	assert(0 <= index && index < CShadowNum);

	cShadow[index].SetCasterPos(lightpos);
	dirty = true;
}

void IF::LightManager::SetCircleShadowDistanceCasterLight(int index, float distanceCasterLight)
{
	assert(0 <= index && index < CShadowNum);

	cShadow[index].SetDistanceCasterLight(distanceCasterLight);
	dirty = true;
}

void IF::LightManager::SetCircleShadowAtten(int index, const Float3& lightAtten)
{
	assert(0 <= index && index < CShadowNum);

	cShadow[index].SetShadowAtten(lightAtten);
	dirty = true;
}

void IF::LightManager::SetCircleShadowFactorAngle(int index, const Float2& lightFactorAngle)
{
	assert(0 <= index && index < CShadowNum);

	cShadow[index].SetShadowFactorAngle(lightFactorAngle);
	dirty = true;
}


void IF::LightManager::Update()
{
	if (dirty)
	{
		TransferConstBuffer();
		dirty = false;
	}
}

void IF::LightManager::Draw(UINT rootParameterIndex)
{
	commandList->SetGraphicsRootConstantBufferView(rootParameterIndex, constBuff->GetGPUVirtualAddress());
}

LightManager* IF::LightManager::Instance()
{
	static LightManager* instance = new LightManager;
	return instance;
}

void IF::LightManager::DeleteInstance()
{
	delete LightManager::Instance();
}

void IF::LightManager::UnMap()
{
	constBuff->Unmap(0, nullptr);
}
