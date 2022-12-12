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
	std::vector<Vertex> vertices;
	std::vector<UINT> indices;

	for (UINT i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;

		vertex.pos.x = mesh->mVertices[i].x;
		vertex.pos.y = mesh->mVertices[i].y;
		vertex.pos.z = mesh->mVertices[i].z;

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
		string f = filename + "/" + str.C_Str();
		mesh_->material.textureFilename = f;
		mesh_->material.texNum = Texture::Instance()->LoadTexture(f);
		if (mesh_->material.texNum != 65535)mesh_->material.tex = true;
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

FBXModel* IF::ModelLoader::FBXLoad(std::string fileName)
{
	Assimp::Importer importer;
	string f = "Data/Resources/";
	f += fileName + "/" + fileName + ".fbx";
	filename = fileName;
	const aiScene* scene = importer.ReadFile(f,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType | aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals | aiProcess_FixInfacingNormals
	);

	if (nullptr == scene) {
		return nullptr;
	}

	ParseNodeRecursive(scene, scene->mRootNode);

	FBXModel* fbx = new FBXModel;
	for (std::unique_ptr<Node>& node : nodes)
		fbx->nodes.push_back(std::move(node));

	nodes.clear();

	return fbx;
}
