#include "ModelLoader.h"
#include "FBXModel.h"
#include "Texture.h"

using namespace IF;
using namespace std;

void IF::ModelLoader::ParseNodeRecursive(const aiScene* scene, aiNode* node, Node* targetParent)
{
	unique_ptr<Node> node_ = make_unique<Node>();
	string name = node->mName.C_Str();
	node_->name = name;
	node_->transform.SetX(node->mTransformation.a1, node->mTransformation.a2, node->mTransformation.a3, node->mTransformation.a4);
	node_->transform.SetY(node->mTransformation.b1, node->mTransformation.b2, node->mTransformation.b3, node->mTransformation.b4);
	node_->transform.SetZ(node->mTransformation.c1, node->mTransformation.c2, node->mTransformation.c3, node->mTransformation.c4);
	node_->transform.SetW(node->mTransformation.d1, node->mTransformation.d2, node->mTransformation.d3, node->mTransformation.d4);
	node_->globalTransform = node_->transform = MatrixTranspose(node_->transform);
	for (UINT i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		node_->meshes.push_back(this->ProcessMesh(scene, mesh));
	}
	nodes.push_back(std::move(node_));
	Node* parent;
	parent = node_.get();

	if (targetParent != nullptr)
	{
		targetParent->parent = parent;
		targetParent->globalTransform *= parent->globalTransform;
	}

	for (UINT i = 0; i < node->mNumChildren; i++) {
		this->ParseNodeRecursive(scene, node->mChildren[i], parent);
	}
}

Mesh* IF::ModelLoader::ProcessMesh(const aiScene* scene, aiMesh* mesh)
{
	std::vector<VertexBone> vertices;
	std::vector<UINT> indices;

	struct Weight
	{
		UINT vertexID;
		float weight;
	};

	for (UINT i = 0; i < mesh->mNumVertices; i++) {
		VertexBone vertex;

		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;
		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0]) {
			vertex.uv.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.uv.y = (float)mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	Mesh* mesh_ = new Mesh;
	aiTextureType type = aiTextureType_DIFFUSE;
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	for (UINT i = 0; i < material->GetTextureCount(type); i++) {
		aiString str;
		material->GetTexture(type, i, &str);
		string f = /*filename + "/" +*/ str.C_Str();
		while (f.find("\\") != std::string::npos)
		{
			f = f.substr(f.find("\\") + 1);
		}
		f = filename + "/" + f;
		mesh_->material.textureFilename = f;
		mesh_->material.texNum = Texture::Instance()->LoadTexture(f);
		if (mesh_->material.texNum != 65535)mesh_->material.tex = true;
	}

	//É{Å[Éì
	std::vector<std::list<Weight>>lists(vertices.size());
	for (int i = 0; i < mesh->mNumBones; i++)
	{
		Bone b(mesh->mBones[i]->mName.C_Str());
		b.invInitPose.SetX(mesh->mBones[i]->mOffsetMatrix[0][0], mesh->mBones[i]->mOffsetMatrix[0][1], mesh->mBones[i]->mOffsetMatrix[0][2], mesh->mBones[i]->mOffsetMatrix[0][3]);
		b.invInitPose.SetY(mesh->mBones[i]->mOffsetMatrix[1][0], mesh->mBones[i]->mOffsetMatrix[1][1], mesh->mBones[i]->mOffsetMatrix[1][2], mesh->mBones[i]->mOffsetMatrix[1][3]);
		b.invInitPose.SetZ(mesh->mBones[i]->mOffsetMatrix[2][0], mesh->mBones[i]->mOffsetMatrix[2][1], mesh->mBones[i]->mOffsetMatrix[2][2], mesh->mBones[i]->mOffsetMatrix[2][3]);
		b.invInitPose.SetW(mesh->mBones[i]->mOffsetMatrix[3][0], mesh->mBones[i]->mOffsetMatrix[3][1], mesh->mBones[i]->mOffsetMatrix[3][2], mesh->mBones[i]->mOffsetMatrix[3][3]);
		b.invInitPose = MatrixTranspose(b.invInitPose);
		for (int j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{
			Weight w;
			w.vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
			w.weight = mesh->mBones[i]->mWeights[j].mWeight;
			lists[i].push_back(w);
		}
		bones.push_back(b);
	}

	for (int i = 0; i < vertices.size(); i++)
	{
		auto& l = lists[i];

		l.sort([](auto const& lhs, auto const& rhs) {
			return lhs.weight > rhs.weight;
			});

		int w = 0;
		for (auto& s : l)
		{
			vertices[i].boneIndex[w] = s.vertexID;
			vertices[i].boneWeight[w] = s.weight;
			if (++w >= MAX_BONE_INDICES)
			{
				float we = 0.0f;
				for (int j = 1; j < MAX_BONE_INDICES; j++)
				{
					we += vertices[i].boneWeight[j];
				}
				vertices[i].boneWeight[0] = 1.0f - we;
				break;
			}
		}
	}

	aiColor3D col;
	material->Get(AI_MATKEY_COLOR_AMBIENT, col);
	mesh_->material.ambient.x = col.r;
	mesh_->material.ambient.y = col.g;
	mesh_->material.ambient.z = col.b;
	material->Get(AI_MATKEY_COLOR_DIFFUSE, col);
	mesh_->material.diffuse.x = col.r;
	mesh_->material.diffuse.y = col.g;
	mesh_->material.diffuse.z = col.b;
	material->Get(AI_MATKEY_COLOR_SPECULAR, col);
	mesh_->material.specular.x = col.r;
	mesh_->material.specular.y = col.g;
	mesh_->material.specular.z = col.b;
	material->Get(AI_MATKEY_OPACITY, mesh_->material.alpha);

	mesh_->SetVerticleIndex(vertices, vertices.size(), indices, indices.size());
	mesh_->Initialize();

	return mesh_;
}

FBXModel* IF::ModelLoader::FBXLoad(std::string fileName, std::string fileType, bool smooth)
{
	Assimp::Importer importer;
	string f = "Data/Resources/";
	f += fileName + "/" + fileName + fileType;
	filename = fileName;
	unsigned int sf = aiProcess_GenNormals;
	if (smooth == true)
	{
		sf = aiProcess_GenSmoothNormals;
	}
	const aiScene* scene = importer.ReadFile(f,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType | aiProcess_ConvertToLeftHanded |
		sf | aiProcess_FixInfacingNormals
	);

	if (scene == nullptr)
	{
		OutputDebugStringA(importer.GetErrorString());
		return nullptr;
	}

	ParseNodeRecursive(scene, scene->mRootNode);

	for (int i = 0; i < scene->mNumAnimations; i++)
	{
		Animation a;
		a.name = scene->mAnimations[i]->mName.C_Str();
		a.duration = scene->mAnimations[i]->mDuration;
		a.ticksPerSecond = scene->mAnimations[i]->mTicksPerSecond;
		for (int j = 0; j < scene->mAnimations[i]->mNumChannels; j++)
		{
			NodeAnima n;
			n.name = scene->mAnimations[i]->mChannels[j]->mNodeName.C_Str();
			for (int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumPositionKeys; k++)
			{
				Vector3 p;
				double t;
				p.x = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.x;
				p.y = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.y;
				p.z = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.z;
				t = scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mTime;
				n.position.push_back(p);
				n.positionTime.push_back(t);
			}
			for (int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumScalingKeys; k++)
			{
				Vector3 s;
				double t;
				s.x = scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue.x;
				s.y = scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue.y;
				s.z = scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mValue.z;
				t = scene->mAnimations[i]->mChannels[j]->mScalingKeys[k].mTime;
				n.scale.push_back(s);
				n.scaleTime.push_back(t);
			}
			for (int k = 0; k < scene->mAnimations[i]->mChannels[j]->mNumRotationKeys; k++)
			{
				Quaternion q;
				double t;
				q.x = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.x;
				q.y = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.y;
				q.z = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.z;
				q.w = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.w;
				t = scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mTime;
				n.rotation.push_back(q);
				n.rotationTime.push_back(t);
			}
		}
	}

	FBXModel* fbx = new FBXModel;
	for (std::unique_ptr<Node>& node : nodes)
		fbx->nodes.push_back(std::move(node));
	for (auto& b : bones)
		fbx->bones.push_back(b);
	for (auto& a : animations)
		fbx->animations.push_back(a);


	nodes.clear();

	return fbx;
}
