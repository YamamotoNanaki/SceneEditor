#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <vector>
#include <string>
#include "ModelVI.h"
#include "ConstStruct.h"
#include "IFMath.h"

#pragma comment(lib,"d3d12.lib") 

namespace IF
{
	enum ModelCreateNums
	{
		LOAD_MODEL,
		CREATE_CUBE,
		CREATE_TRIANGLE,
		CREATE_CIRCLE,
		CREATE_SPHERE
	};
	class Model
	{
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		template<class T> using vector = std::vector<T>;
	private:
		//定数バッファ
		ComPtr<ID3D12Resource> constBuffTransform1;
		ConstBufferMaterial* constMapMaterial = nullptr;
		static ComPtr<ID3D12Device> device;
		std::string tag;

	public:
		struct Material
		{
			std::string name;
			Float3 ambient;
			Float3 diffuse;
			Float3 specular;
			float alpha;
			std::string textureFilename;
			Material()
			{
				ambient = { 0.3f,0.3f,0.3f };
				diffuse = { 0.0f,0.0f,0.0f };
				specular = { 0.0f,0.0f,0.0f };
				alpha = 1.0f;
			}
			bool tex = false;
			unsigned short texNum = 0;
		};

	public:
		MVI* vi = nullptr;
		Material material{};
		std::string name;
		bool smooth = false;
		unsigned short type = 0;

	public:
		static void SetDevice(ID3D12Device* device);
		bool LoadModel(std::string name, bool smoothing = false);
		void CreateCube(unsigned short texNum, bool smoothing = false);
		void CreateTriangle(unsigned short texNum, bool smoothing = false);
		void CreateCircle(unsigned short texNum, bool smoothing = false);
		void CreateSphere(unsigned short texNum, bool smoothing = false);
		void VIInitialize(bool smoothing, bool normal = false);
		void Draw(ID3D12GraphicsCommandList* commandList, vector<D3D12_VIEWPORT> viewport, ID3D12Resource* address);
		void Draw(ID3D12GraphicsCommandList* commandList, vector<D3D12_VIEWPORT> viewport, ID3D12Resource* address, unsigned short texNum);
		unsigned short GetTexNum();
		void SetTexNum(unsigned short texNum);
		~Model();
		inline void SetTag(std::string tag)
		{
			this->tag = tag;
		}
		inline std::string GetTag()
		{
			return tag;
		}
	};
}