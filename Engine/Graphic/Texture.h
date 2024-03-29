#pragma once
#include <DirectXTex.h>
#include <d3d12.h>
#include <wrl.h>
#include <string>
#include <array>
#include "nlohmann/json.hpp"

#pragma comment(lib,"d3d12.lib")
namespace IF
{
	class Texture
	{
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		using TexMetadata = DirectX::TexMetadata;
		using ScratchImage = DirectX::ScratchImage;

	public:
		struct Tex
		{
			ComPtr<ID3D12Resource> texbuff = nullptr;
			D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle{};
			D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle{};
			std::string texName{};
			bool free = false;
		};

	private:
		static const short textureMax = 1024;
		unsigned short textureSize = 0;

	public:
		D3D12_DESCRIPTOR_RANGE descRangeSRV{};
		ComPtr < ID3D12DescriptorHeap> srvHeap = nullptr;
		std::array<Tex, textureMax> tex;

	private:
		Texture();
		Texture(const Texture&);
		Texture& operator=(const Texture&) {}
		~Texture() {};

	public:
		static Texture* Instance();
		void Initialize();
		void SetTexture(unsigned short texHandle);
		unsigned short GetTexture(const std::string filename);
		unsigned short LoadTexture(const std::string filename, int number = -1);
		void GUIInit();
		//unsigned short folder = 0;
		void OutputJson(nlohmann::json& j);
		inline void TexReset()
		{
			textureSize = 0;
			for (int i = 0; i < 1000; i++) {
				tex[i].texbuff.Reset();
				tex[i].CPUHandle.ptr = 0;
				tex[i].texName.clear();
				tex[i].free = false;
			}
		}

#ifdef _DEBUG
		bool flag = false;
		void GUI();
		void TexNum(int* texNum);
#endif
	};
}