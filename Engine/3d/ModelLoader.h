#pragma once
#include "DirectX12.h"
#include "Mesh.h"
#include "Matrix.h"
#include "Quaternion.h"
#include <string>
#include <vector>
#include <memory>
#include <assimp/IOStream.hpp>
#include <assimp/IOSystem.hpp>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#define FBX ".fbx"
#define OBJ ".obj"

namespace IF
{
	class FBXModel;
	struct Node
	{
		std::string name;
		Matrix transform;
		Matrix globalTransform;
		std::vector<Mesh*> meshes;
		Node* parent = nullptr;
	};
	struct Bone
	{
		std::string name;
		Matrix invInitPose;
		Bone(const std::string& name)
		{
			this->name = name;
		}
	};
	struct NodeAnima
	{
		std::string name;
		std::vector<Vector3>position;
		std::vector<double>positionTime;
		std::vector<Vector3>scale;
		std::vector<double>scaleTime;
		std::vector<Quaternion>rotation;
		std::vector<double>rotationTime;
	};
	struct Animation
	{
		std::string name;
		double duration;
		double ticksPerSecond;
		std::vector<NodeAnima>channels;
	};
	class ModelLoader
	{
	private:
		std::vector<std::unique_ptr<Node>> nodes;
		std::vector<Bone> bones;
		std::vector<Animation> animations;
		std::string filename;
	private:
		void ParseNodeRecursive(const aiScene* scene, aiNode* node, Node* targetParent = nullptr);
		Mesh* ProcessMesh(const aiScene* scene, aiMesh* mesh);
	public:
		FBXModel* FBXLoad(std::string fileName, std::string fileType = ".fbx", bool smooth = false);
	};
};
