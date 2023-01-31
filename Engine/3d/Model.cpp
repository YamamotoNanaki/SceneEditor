#include "Model.h"
#include "View.h"
#include "Texture.h"
#include <fstream>
#include <sstream>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Debug.h"
#include "DirectX12.h"

using namespace IF;
using namespace std;

bool Model::LoadModel(string name, bool smoothing)
{
	vi = DEBUG_NEW MVI;
	const string modelname = name;
	const string filename = modelname + ".obj";
	const string directory = "Data/Resources/" + modelname + "/";

	ifstream file;
	file.open(directory + filename);
	if (file.fail())
	{
		return false;
	}

	vector<Float3> position;
	vector<Float3> normal;
	vector<Float2> texcode;

	vector<Vertex> vertices;
	vector<unsigned short> indices;

	short indexCount = 0;

	string line;
	while (getline(file, line))
	{
		istringstream line_stream(line);

		string key;
		getline(line_stream, key, ' ');

		if (key == "v")
		{
			Float3 pos{};
			line_stream >> pos.x;
			line_stream >> pos.y;
			line_stream >> pos.z;
			position.emplace_back(pos);
		}
		if (key == "vn")
		{
			Float3 nor{};
			line_stream >> nor.x;
			line_stream >> nor.y;
			line_stream >> nor.z;
			normal.emplace_back(nor);
		}
		if (key == "vt")
		{
			Float2 tex{};
			line_stream >> tex.x;
			line_stream >> tex.y;
			tex.y = 1 - tex.y;
			texcode.emplace_back(tex);
		}
		if (key == "f")
		{
			int indexCount2 = 0;
			string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				istringstream index_stream(index_string);
				unsigned short inP, inT, inN;
				index_stream >> inP;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> inT;
				index_stream.seekg(1, ios_base::cur);
				index_stream >> inN;

				Vertex vertex{};
				vertex.pos = position[inP - 1];
				vertex.normal = normal[inN - 1];
				vertex.uv = texcode[inT - 1];
				vertices.emplace_back(vertex);

				if (smoothing)
				{
					vi->AddSmoothData(inP, (unsigned short)vertices.size() - 1);
				}

				//四角形ポリゴン
				if (indexCount2 >= 3) {
					indices.emplace_back(indexCount - 1);
					indices.emplace_back(indexCount);
					indices.emplace_back(indexCount - 3);
				}
				else
				{
					indices.emplace_back(indexCount);
				}
				indexCount++;
				indexCount2++;
			}
		}
		if (key == "mtllib")
		{
			string mfilename;
			line_stream >> mfilename;
			ifstream mfile;
			mfile.open(directory + mfilename);
			if (mfile.fail())
				return false;

			string line;
			while (getline(mfile, line))
			{
				istringstream line_stream(line);
				string key;
				getline(line_stream, key, ' ');

				if (key[0] == '\t')
				{
					key.erase(key.begin());
				}
				if (key == "newmtl")
				{
					line_stream >> material.name;
				}
				if (key == "Ka")
				{
					line_stream >> material.ambient.x;
					line_stream >> material.ambient.y;
					line_stream >> material.ambient.z;
				}
				if (key == "Kd")
				{
					line_stream >> material.diffuse.x;
					line_stream >> material.diffuse.y;
					line_stream >> material.diffuse.z;
				}
				if (key == "Ks")
				{
					line_stream >> material.specular.x;
					line_stream >> material.specular.y;
					line_stream >> material.specular.z;
				}
				if (key == "map_Kd")
				{
					line_stream >> material.textureFilename;
					material.texNum = Texture::Instance()->LoadTexture(material.textureFilename);
					material.tex = true;
				}
			}
			mfile.close();
		}
	}

	file.close();

	vi->SetVerticleIndex(vertices, vertices.size(), indices, indices.size());

	//定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resdesc.Width = (sizeof(ConstBufferMaterial) + 0xff) & ~0xff;

	HRESULT result = DirectX12::Instance()->GetDevice()->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE, &resdesc,
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

	VIInitialize(smoothing);

	this->name = name;
	smooth = smoothing;
	type = LOAD_MODEL;
	if (material.tex == false)
	{
		material.texNum = 1;
		material.tex = true;
	}
	return true;
}

void IF::Model::CreateCube(unsigned short texNum, bool smoothing)
{
	vi = DEBUG_NEW MVI;
	type = CREATE_CUBE;
	const float size = 1;

	Vertex vertices[] = {
		// x   y   z        u    v
		//前
		{{-size, -size, -size},{},{0.0f, 1.0f}},	//左下
		{{-size, +size, -size},{},{0.0f, 0.0f}},	//左上
		{{+size, -size, -size},{},{1.0f, 1.0f}},	//右下
		{{+size, +size, -size},{},{1.0f, 0.0f}},	//右上
		//後
		{{+size, -size, +size},{},{1.0f, 1.0f}},	//右下
		{{+size, +size, +size},{},{1.0f, 0.0f}},	//右上
		{{-size, -size, +size},{},{0.0f, 1.0f}},	//左下
		{{-size, +size, +size},{},{0.0f, 0.0f}},	//左上
		//左
		{{-size, -size, -size},{},{0.0f, 1.0f}},	//左下
		{{-size, -size, +size},{},{0.0f, 0.0f}},	//左上
		{{-size, +size, -size},{},{1.0f, 1.0f}},	//右下
		{{-size, +size, +size},{},{1.0f, 0.0f}},	//右上
		//右
		{{+size, +size, -size},{},{1.0f, 1.0f}},	//右下
		{{+size, +size, +size},{},{1.0f, 0.0f}},	//右上
		{{+size, -size, -size},{},{0.0f, 1.0f}},	//左下
		{{+size, -size, +size},{},{0.0f, 0.0f}},	//左上
		//下
		{{-size, +size, +size},{},{1.0f, 1.0f}},	//右下
		{{+size, +size, +size},{},{1.0f, 0.0f}},	//右上
		{{-size, +size, -size},{},{0.0f, 1.0f}},	//左下
		{{+size, +size, -size},{},{0.0f, 0.0f}},	//左上
		//上
		{{-size, -size, -size},{},{0.0f, 1.0f}},	//左下
		{{+size, -size, -size},{},{0.0f, 0.0f}},	//左上
		{{-size, -size, +size},{},{1.0f, 1.0f}},	//右下
		{{+size, -size, +size},{},{1.0f, 0.0f}},	//右上
	};

	//インデックスデータ
	unsigned short indices[] = {
		//前
		0,1,2,
		2,1,3,
		//後
		4,5,6,
		6,5,7,
		//左
		8,9,10,
		10,9,11,
		//右
		12,13,14,
		14,13,15,
		//下
		16,17,18,
		18,17,19,
		//上
		20,21,22,
		22,21,23
	};

	vi->SetVerticleIndex(vertices, _countof(vertices), indices, _countof(indices));

	//定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resdesc.Width = (sizeof(ConstBufferMaterial) + 0xff) & ~0xff;

	HRESULT result = DirectX12::Instance()->GetDevice()->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE, &resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffTransform1));
	assert(SUCCEEDED(result));

	result = constBuffTransform1->Map(0, nullptr, (void**)&constMapMaterial);
	assert(SUCCEEDED(result));

	constMapMaterial->ambient = { 0.8,0.8,0.8 };
	constMapMaterial->diffuse = { 0.8,0.8,0.8 };
	constMapMaterial->specular = { 0.8,0.8,0.8 };
	constMapMaterial->alpha = material.alpha;
	material.texNum = texNum;
	material.tex = true;

	constBuffTransform1->Unmap(0, nullptr);

	VIInitialize(smoothing, true);
}

void IF::Model::CreatePolygonSquare(unsigned short texNum, bool smoothing)
{
	vi = DEBUG_NEW MVI;
	type = CREATE_POLYGON_SQUARE;
	const float size = 1;

	Vertex vertices[] = {
		// x   y   z        u    v
		//前
		{{-size, -size, -size},{},{0.0f, 1.0f}},	//左下
		{{-size, +size, -size},{},{0.0f, 0.0f}},	//左上
		{{+size, -size, -size},{},{1.0f, 1.0f}},	//右下
		{{+size, +size, -size},{},{1.0f, 0.0f}}		//右上
	};

	//インデックスデータ
	unsigned short indices[] = {
		//前
		0,1,2,
		2,1,3
	};

	vi->SetVerticleIndex(vertices, _countof(vertices), indices, _countof(indices));

	//定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resdesc.Width = (sizeof(ConstBufferMaterial) + 0xff) & ~0xff;

	HRESULT result = DirectX12::Instance()->GetDevice()->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE, &resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffTransform1));
	assert(SUCCEEDED(result));

	result = constBuffTransform1->Map(0, nullptr, (void**)&constMapMaterial);
	assert(SUCCEEDED(result));

	constMapMaterial->ambient = { 0.8,0.8,0.8 };
	constMapMaterial->diffuse = { 0.8,0.8,0.8 };
	constMapMaterial->specular = { 0.8,0.8,0.8 };
	constMapMaterial->alpha = material.alpha;
	material.texNum = texNum;
	material.tex = true;

	constBuffTransform1->Unmap(0, nullptr);

	VIInitialize(smoothing, true);
}

void IF::Model::CreateTriangle(unsigned short texNum, bool smoothing)
{
	vi = DEBUG_NEW MVI;
	type = CREATE_TRIANGLE;

	Vertex vertices[] = {
		// x   y   z        u    v
		//前{ -1,1,1 }, { -1, -1, 1 }, { 1, 0, 0 }
		{{ 2,2,2 },{},{0.0f, 0.0f}},	//左下
		{{ 2, -2, 0 },{},{0.0f, 0.0f}},	//上
		{{ -2, 0, -2 },{},{0.0f, 0.0f}},	//右下
	};

	//インデックスデータ
	unsigned short indices[] = {
		//前
		0,1,2
	};

	vi->SetVerticleIndex(vertices, _countof(vertices), indices, _countof(indices));

	//定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resdesc.Width = (sizeof(ConstBufferMaterial) + 0xff) & ~0xff;

	HRESULT result = DirectX12::Instance()->GetDevice()->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE, &resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffTransform1));
	assert(SUCCEEDED(result));

	result = constBuffTransform1->Map(0, nullptr, (void**)&constMapMaterial);
	assert(SUCCEEDED(result));

	constMapMaterial->ambient = { 0.8,0.8,0.8 };
	constMapMaterial->diffuse = { 0.8,0.8,0.8 };
	constMapMaterial->specular = { 0.8,0.8,0.8 };
	constMapMaterial->alpha = material.alpha;
	material.texNum = texNum;
	material.tex = true;

	constBuffTransform1->Unmap(0, nullptr);

	VIInitialize(smoothing, true);
}

void IF::Model::CreateCircle(unsigned short texNum, bool smoothing)
{
	vi = DEBUG_NEW MVI;
	const int DIV = 64;
	const float radius = 0.5f;
	type = CREATE_CIRCLE;

	Vertex vertices[DIV + 1]{};

	for (int i = 0; i < DIV; i++)
	{
		vertices[i] = { {radius * sinf(2 * M_PI / DIV * i),radius * cosf(2 * M_PI / DIV * i),0},{},{0,0} };
	}
	vertices[DIV] = { 0,0,0 };

	unsigned short indices[DIV * 3]{};

	for (int i = 0; i < DIV; i++)
	{
		indices[i * 3] = i;
		indices[i * 3 + 1] = i + 1;
		indices[i * 3 + 2] = DIV;
	}
	indices[DIV * 3 - 2] = 0;

	vi->SetVerticleIndex(vertices, _countof(vertices), indices, _countof(indices));

	//定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resdesc.Width = (sizeof(ConstBufferMaterial) + 0xff) & ~0xff;

	HRESULT result = DirectX12::Instance()->GetDevice()->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE, &resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffTransform1));
	assert(SUCCEEDED(result));

	result = constBuffTransform1->Map(0, nullptr, (void**)&constMapMaterial);
	assert(SUCCEEDED(result));

	constMapMaterial->ambient = { 0.8,0.8,0.8 };
	constMapMaterial->diffuse = { 0.8,0.8,0.8 };
	constMapMaterial->specular = { 0.8,0.8,0.8 };
	constMapMaterial->alpha = material.alpha;
	material.texNum = texNum;
	material.tex = true;

	constBuffTransform1->Unmap(0, nullptr);

	VIInitialize(smoothing, true);
}

void IF::Model::CreateSphere(unsigned short texNum, bool smoothing)
{
	vi = DEBUG_NEW MVI;
	const int DIV = 64;
	const float radius = 1.0f;
	type = CREATE_SPHERE;

	std::vector<Vertex> vertices{};

	vertices.push_back({ { 0, radius, 0 }, {}, {0,0} });

	for (int i = 1; i < DIV; i++)
	{
		float y = cosf(M_PI / DIV * i) * radius;
		float r = sinf(M_PI / DIV * i) * radius;
		for (int j = 0; j < DIV; j++)
		{
			vertices.push_back({ { sinf(2 * M_PI / DIV * j) * r,y,cosf(2 * M_PI / DIV * j) * r},{},{0,0} });
		}
	}
	vertices.push_back({ { 0,-radius,0 },{},{0,0} });

	std::vector< unsigned short> indices{};

	for (int i = 0; i < DIV; i++)
	{
		for (int j = 0; j < DIV; j++)
		{
			if (i == 0)
			{
				indices.push_back(0);
				indices.push_back(j + 1);
				if (j == DIV - 1)
				{
					indices.push_back(1);
					break;
				}
				indices.push_back(j + 2);

			}
			else if (i == DIV - 1)
			{
				indices.push_back((i - 1) * DIV + 1 + j);
				indices.push_back(vertices.size() - 1);
				if (j == DIV - 1)
				{
					indices.push_back((i - 1) * DIV + 1 + j - DIV + 1);
					break;
				}
				indices.push_back((i - 1) * DIV + 1 + j + 1);
			}
			else
			{
				if (j == DIV - 1)
				{
					indices.push_back((i - 1) * DIV + 1 + j);
					indices.push_back((i - 1) * DIV + 1 + j + DIV);
					indices.push_back((i - 1) * DIV + 1 + j + 1);
					indices.push_back((i - 1) * DIV + 1 + j);
					indices.push_back((i - 1) * DIV + 1 + j + 1);
					indices.push_back((i - 1) * DIV + 1 + j - DIV + 1);
					break;
				}
				indices.push_back((i - 1) * DIV + 1 + j);
				indices.push_back((i - 1) * DIV + 1 + j + DIV);
				indices.push_back((i - 1) * DIV + 1 + j + DIV + 1);
				indices.push_back((i - 1) * DIV + 1 + j);
				indices.push_back((i - 1) * DIV + 1 + j + DIV + 1);
				indices.push_back((i - 1) * DIV + 1 + j + 1);
			}
		}
	}

	vi->SetVerticleIndex(vertices, vertices.size(), indices, indices.size());

	//定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resdesc.Width = (sizeof(ConstBufferMaterial) + 0xff) & ~0xff;

	HRESULT result = DirectX12::Instance()->GetDevice()->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE, &resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffTransform1));
	assert(SUCCEEDED(result));

	result = constBuffTransform1->Map(0, nullptr, (void**)&constMapMaterial);
	assert(SUCCEEDED(result));

	constMapMaterial->ambient = { 0.8,0.8,0.8 };
	constMapMaterial->diffuse = { 0.8,0.8,0.8 };
	constMapMaterial->specular = { 0.8,0.8,0.8 };
	constMapMaterial->alpha = material.alpha;
	material.texNum = texNum;
	material.tex = true;

	constBuffTransform1->Unmap(0, nullptr);

	VIInitialize(smoothing, true);
}

void IF::Model::CreateRay()
{
	vi = DEBUG_NEW MVI;
	type = CREATE_RAY;

	Vertex vertices[] =
	{
		{{0, 0, 0},{},{0.0f, 0.0f}},
	};
	unsigned short indices[] =
	{
		0,0,0
	};

	vi->SetVerticleIndex(vertices, _countof(vertices), indices, _countof(indices));

	//定数バッファのヒープ設定
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//定数バッファのリソース設定
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resdesc.Width = (sizeof(ConstBufferMaterial) + 0xff) & ~0xff;

	HRESULT result = DirectX12::Instance()->GetDevice()->CreateCommittedResource(
		&heapProp, D3D12_HEAP_FLAG_NONE, &resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffTransform1));
	assert(SUCCEEDED(result));

	result = constBuffTransform1->Map(0, nullptr, (void**)&constMapMaterial);
	assert(SUCCEEDED(result));

	constMapMaterial->ambient = { 0.8,0.8,0.8 };
	constMapMaterial->diffuse = { 0.8,0.8,0.8 };
	constMapMaterial->specular = { 0.8,0.8,0.8 };
	constMapMaterial->alpha = material.alpha;
	material.tex = true;

	constBuffTransform1->Unmap(0, nullptr);

	VIInitialize(false, true);
}

void Model::VIInitialize(bool smoothing, bool normal)
{
	vi->Initialize(smoothing, normal);
}

void IF::Model::Draw(ID3D12Resource* address)
{
	static ID3D12GraphicsCommandList* commandList = DirectX12::Instance()->GetCmdList();
	if (material.tex == true)Texture::Instance()->SetTexture(material.texNum);
	//頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, &vi->GetVertexView());
	//インデックスバッファの設定
	commandList->IASetIndexBuffer(&vi->GetIndexView());
	//定数バッファビューの設定
	commandList->SetGraphicsRootConstantBufferView(2, address->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(3, constBuffTransform1->GetGPUVirtualAddress());
	//描画コマンド
	commandList->DrawIndexedInstanced((UINT)vi->GetSize(), 1, 0, 0, 0);
}

void IF::Model::Draw(ID3D12Resource* address, unsigned short texNum)
{
	static ID3D12GraphicsCommandList* commandList = DirectX12::Instance()->GetCmdList();
	Texture::Instance()->SetTexture(texNum);
	//頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, &vi->GetVertexView());
	//インデックスバッファの設定
	commandList->IASetIndexBuffer(&vi->GetIndexView());
	//定数バッファビューの設定
	commandList->SetGraphicsRootConstantBufferView(2, address->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(3, constBuffTransform1->GetGPUVirtualAddress());
	//描画コマンド
	commandList->DrawIndexedInstanced((UINT)vi->GetSize(), 1, 0, 0, 0);
}

unsigned short IF::Model::GetTexNum()
{
	return material.texNum;
}

void IF::Model::SetTexNum(unsigned short texNum)
{
	material.texNum = texNum;
}

Model::~Model()
{
	delete vi;
}

