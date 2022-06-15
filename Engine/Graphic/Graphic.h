#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <vector>
#include "GPipeline.h"
#include "RootParam.h"

#pragma comment(lib,"d3dcompiler.lib")

namespace IF
{
	namespace Blend
	{
		enum Blend
		{
			NORMAL,			//0
			ADD,			//1
			SUB,			//2
			INVSRC,			//3
			NORMAL2D,		//4
			ADD2D,			//5
			SUB2D,			//6
			INVSRC2D,		//7
			PNORMAL,		//8
			PADD,			//9
			PSUB,			//10
			PINVSRC,		//11
		};
	}
	namespace ShaderCode
	{
		enum ShaderCode
		{
			error,		//0
			vs,			//1
			ps,			//2
			gs,			//3
			vs2d,		//4
			ps2d,		//5
			vsP,		//1
			psP,		//2
			gsP,		//3
		};
	}
	namespace ShaderCompile
	{
		enum ShaderCompile
		{
			vs = 0b001,
			ps = 0b010,
			gs = 0b100,
			vsps = 0b011,
			all = 0b111,
		};
	}

	class Graphic
	{
		template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	private:
		std::vector<ComPtr<ID3DBlob>>Blobs;
		static ComPtr < ID3D12Device> device;

	public:
		ComPtr<ID3D12RootSignature> rootsignature;
		ComPtr<ID3D12PipelineState> pipelinestate[12]{ nullptr };

	public:
		static void SetDevice(ID3D12Device* device);
		void CompillerArray(LPCWSTR fillname, int num);
		void Compiller(LPCWSTR vs, LPCWSTR ps, LPCWSTR gs, char Compile = ShaderCompile::all);
		void Initialize(D3D12_DESCRIPTOR_RANGE& descRangeSRV, LPCWSTR vs, LPCWSTR ps, LPCWSTR gs);
		void Initialize2D(D3D12_DESCRIPTOR_RANGE& descRangeSRV, LPCWSTR vs, LPCWSTR ps);
		void InitializeParticle(D3D12_DESCRIPTOR_RANGE& descRangeSRV);
		void DrawBlendMode(ID3D12GraphicsCommandList* commandList, Blend::Blend blend = Blend::NORMAL);
		static Graphic* Instance();
		static void DeleteInstance();

	private:
		Graphic() {};
		Graphic(const Graphic&) {};
		Graphic& operator=(const Graphic&) {}
		~Graphic() {}
	};
}