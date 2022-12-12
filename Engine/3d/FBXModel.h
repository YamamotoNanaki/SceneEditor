#pragma once
#include <d3d12.h>
#include <vector>
#include <memory>
#include <string>
#include "ModelLoader.h"
#include "ModelVI.h"
#include "ConstStruct.h"
#include "IFMath.h"

namespace IF
{
	class FBXModel
	{
		template<class T> using vector = std::vector<T>;
	private:
		std::string tag;

	public:
		std::vector<std::unique_ptr<Node>> nodes;

	public:
		void Draw(ID3D12Resource* address);
		void Draw(ID3D12Resource* address, unsigned short texNum);
		inline void SetTag(std::string tag)
		{
			this->tag = tag;
		}
		inline std::string GetTag()
		{
			return tag;
		}
	};
};
