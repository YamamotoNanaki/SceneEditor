#include "Model.h"
#include "View.h"
#include "Texture.h"
#include <fstream>
#include <sstream>
#include <string>

using namespace IF;
using namespace std;

Microsoft::WRL::ComPtr<ID3D12Device> Model::device = nullptr;

bool Model::LoadModel(string name, bool smoothing)
{
	vi = new MVI;
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

				//�l�p�`�|���S��
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
	resdesc.Width = (sizeof(ConstBufferMaterial) + 0xff) & ~0xff;

	HRESULT result = device->CreateCommittedResource(
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
	return true;
}

void IF::Model::CreateCube(bool smoothing)
{
	vi = new MVI;

	Vertex vertices[] = {
		// x   y   z        u    v
		//�O
		{{-5, -5, -5},{},{0.0f, 1.0f}},	//����
		{{-5, +5, -5},{},{0.0f, 0.0f}},	//����
		{{+5, -5, -5},{},{1.0f, 1.0f}},	//�E��
		{{+5, +5, -5},{},{1.0f, 0.0f}},	//�E��
		//��			
		{{+5, -5, +5},{},{1.0f, 1.0f}},	//�E��
		{{+5, +5, +5},{},{1.0f, 0.0f}},	//�E��
		{{-5, -5, +5},{},{0.0f, 1.0f}},	//����
		{{-5, +5, +5},{},{0.0f, 0.0f}},	//����
		//��			
		{{-5, -5, -5},{},{0.0f, 1.0f}},	//����
		{{-5, -5, +5},{},{0.0f, 0.0f}},	//����
		{{-5, +5, -5},{},{1.0f, 1.0f}},	//�E��
		{{-5, +5, +5},{},{1.0f, 0.0f}},	//�E��
		//�E			
		{{+5, +5, -5},{},{1.0f, 1.0f}},	//�E��
		{{+5, +5, +5},{},{1.0f, 0.0f}},	//�E��
		{{+5, -5, -5},{},{0.0f, 1.0f}},	//����
		{{+5, -5, +5},{},{0.0f, 0.0f}},	//����
		//��			
		{{-5, +5, +5},{},{1.0f, 1.0f}},	//�E��
		{{+5, +5, +5},{},{1.0f, 0.0f}},	//�E��
		{{-5, +5, -5},{},{0.0f, 1.0f}},	//����
		{{+5, +5, -5},{},{0.0f, 0.0f}},	//����
		//��			
		{{-5, -5, -5},{},{0.0f, 1.0f}},	//����
		{{+5, -5, -5},{},{0.0f, 0.0f}},	//����
		{{-5, -5, +5},{},{1.0f, 1.0f}},	//�E��
		{{+5, -5, +5},{},{1.0f, 0.0f}},	//�E��
	};

	//�C���f�b�N�X�f�[�^
	unsigned short indices[] = {
		//�O
		0,1,2,
		2,1,3,
		//��
		4,5,6,
		6,5,7,
		//��
		8,9,10,
		10,9,11,
		//�E
		12,13,14,
		14,13,15,
		//��
		16,17,18,
		18,17,19,
		//��
		20,21,22,
		22,21,23
	};

	vi->SetVerticleIndex(vertices, _countof(vertices), indices, _countof(indices));

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
	resdesc.Width = (sizeof(ConstBufferMaterial) + 0xff) & ~0xff;

	HRESULT result = device->CreateCommittedResource(
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

	VIInitialize(smoothing, true);
}

void Model::VIInitialize(bool smoothing, bool normal)
{
	vi->Initialize(device.Get(), smoothing, normal);
}

void IF::Model::SetDevice(ID3D12Device* device)
{
	Model::device = device;
}

void IF::Model::Draw(ID3D12GraphicsCommandList* commandList, vector<D3D12_VIEWPORT> viewport, ID3D12Resource* address)
{
	if (material.tex == true)Texture::Instance()->setTexture(commandList, material.texNum);
	//���_�o�b�t�@�̐ݒ�
	commandList->IASetVertexBuffers(0, 1, &vi->GetVertexView());
	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	commandList->IASetIndexBuffer(&vi->GetIndexView());
	//�萔�o�b�t�@�r���[�̐ݒ�
	commandList->SetGraphicsRootConstantBufferView(2, address->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(3, constBuffTransform1->GetGPUVirtualAddress());
	for (int i = 0; i < viewport.size(); i++)
	{
		commandList->RSSetViewports(1, &viewport[i]);
		//�`��R�}���h
		commandList->DrawIndexedInstanced((UINT)vi->GetSize(), 1, 0, 0, 0);
	}
}

void IF::Model::Draw(ID3D12GraphicsCommandList* commandList, vector<D3D12_VIEWPORT> viewport, ID3D12Resource* address, unsigned short texNum)
{
	Texture::Instance()->setTexture(commandList, texNum);
	//���_�o�b�t�@�̐ݒ�
	commandList->IASetVertexBuffers(0, 1, &vi->GetVertexView());
	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	commandList->IASetIndexBuffer(&vi->GetIndexView());
	//�萔�o�b�t�@�r���[�̐ݒ�
	commandList->SetGraphicsRootConstantBufferView(2, address->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(3, constBuffTransform1->GetGPUVirtualAddress());
	for (int i = 0; i < viewport.size(); i++)
	{
		commandList->RSSetViewports(1, &viewport[i]);
		//�`��R�}���h
		commandList->DrawIndexedInstanced((UINT)vi->GetSize(), 1, 0, 0, 0);
	}
}

Model::~Model()
{
	constBuffTransform1->Unmap(0, nullptr);
	delete vi;
}
