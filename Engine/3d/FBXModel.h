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
		std::vector<Bone> bones;
		std::vector<Matrix>& Transforms;
		std::vector<Animation> animations;

	public:
		void Draw(ID3D12Resource* address);
		void Draw(ID3D12Resource* address, unsigned short texNum);
		Matrix BoneTransform(float TimeInSeconds, vector<Matrix>& Transforms);
		inline void SetTag(std::string tag)
		{
			this->tag = tag;
		}
		inline std::string GetTag()
		{
			return tag;
		}
	private:
		void ReadNodeHeirarchy(float AnimationTime, const Node* pNode, const Matrix& ParentTransform);
	};
};
