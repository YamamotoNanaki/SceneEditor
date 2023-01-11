#include "FBXModel.h"
#include "DirectX12.h"
#include "Util.h"
#include <cassert>

using namespace IF;
using namespace std;

void IF::FBXModel::Draw(ID3D12Resource* address)
{
	for (unique_ptr<Node>& node : nodes)
	{
		for (auto mesh : node->meshes)
		{
			mesh->Draw(address);
		}
	}
}

void IF::FBXModel::Draw(ID3D12Resource* address, unsigned short texNum)
{
	for (unique_ptr<Node>& node : nodes)
	{
		for (auto mesh : node->meshes)
		{
			mesh->Draw(address, texNum);
		}
	}
}


//アニメーション
static const NodeAnim* FindNodeAnim(const Animation* pAnimation, const string& NodeName)
{
	for (UINT i = 0; i < pAnimation->channels.size(); i++)
	{
		const NodeAnim* pNodeAnim = &pAnimation->channels[i];

		if (pNodeAnim->name == NodeName)
		{
			return pNodeAnim;
		}
	}

	return NULL;
}

static UINT findScaling(float AnimationTime, const NodeAnim* pNodeAnim)
{
	assert(pNodeAnim->scale.size() > 0);

	for (UINT i = 0; i < pNodeAnim->scale.size() - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->scaleTime[i + 1]) {
			return i;
		}
	}

	assert(0);

	return 0;
}

static void CalcInterpolatedScaling(Vector3& Out, float AnimationTime, const NodeAnim* pNodeAnim)
{
	if (pNodeAnim->scale.size() == 1) {
		Out = pNodeAnim->scale[0];
		return;
	}

	UINT ScalingIndex = findScaling(AnimationTime, pNodeAnim);
	UINT NextScalingIndex = (ScalingIndex + 1);
	assert(NextScalingIndex < pNodeAnim->scale.size());
	float DeltaTime = (float)(pNodeAnim->scaleTime[NextScalingIndex] - pNodeAnim->scaleTime[ScalingIndex]);
	float Factor = clamp((AnimationTime - (float)pNodeAnim->scaleTime[ScalingIndex]) / DeltaTime);
	//assert(Factor >= 0.0f && Factor <= 1.0f);
	const Vector3& Start = pNodeAnim->scale[ScalingIndex];
	const Vector3& End = pNodeAnim->scale[NextScalingIndex];
	Vector3 Delta = End - Start;
	Out = Start + Factor * Delta;
}

static UINT findRotation(float AnimationTime, const NodeAnim* pNodeAnim)
{
	assert(pNodeAnim->rotation.size() > 0);

	for (UINT i = 0; i < pNodeAnim->rotation.size() - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->rotationTime[i + 1]) {
			return i;
		}
	}

	assert(0);

	return 0;
}

static void CalcInterpolatedRotation(Quaternion& Out, float AnimationTime, const NodeAnim* pNodeAnim)
{
	// we need at least two values to interpolate...
	if (pNodeAnim->rotation.size() == 1) {
		Out = pNodeAnim->rotation[0];
		return;
	}

	UINT RotationIndex = findRotation(AnimationTime, pNodeAnim);
	UINT NextRotationIndex = (RotationIndex + 1);
	assert(NextRotationIndex < pNodeAnim->rotation.size());
	float DeltaTime = (float)(pNodeAnim->rotationTime[NextRotationIndex] - pNodeAnim->rotationTime[RotationIndex]);
	float Factor = clamp((AnimationTime - (float)pNodeAnim->rotationTime[RotationIndex]) / DeltaTime);

	const Quaternion& StartRotationQ = pNodeAnim->rotation[RotationIndex];
	const Quaternion& EndRotationQ = pNodeAnim->rotation[NextRotationIndex];
	Out = SLerp(StartRotationQ, EndRotationQ, Factor);
	Out = normalize(Out);
}

static UINT findPosition(float AnimationTime, const NodeAnim* pNodeAnim)
{
	for (UINT i = 0; i < pNodeAnim->position.size() - 1; i++) {
		if (AnimationTime < (float)pNodeAnim->positionTime[i + 1]) {
			return i;
		}
	}

	assert(0);

	return 0;
}

static void CalcInterpolatedPosition(Vector3& Out, float AnimationTime, const NodeAnim* pNodeAnim)
{
	if (pNodeAnim->position.size() == 1)
	{
		Out = pNodeAnim->position[0];
		return;
	}

	UINT PositionIndex = findPosition(AnimationTime, pNodeAnim);
	UINT NextPositionIndex = (PositionIndex + 1);
	assert(NextPositionIndex < pNodeAnim->position.size());
	float DeltaTime = (float)(pNodeAnim->positionTime[NextPositionIndex] - pNodeAnim->positionTime[PositionIndex]);
	float Factor = clamp((AnimationTime - (float)pNodeAnim->positionTime[PositionIndex]) / DeltaTime);
	const Vector3& Start = pNodeAnim->position[PositionIndex];
	const Vector3& End = pNodeAnim->position[NextPositionIndex];
	Vector3 Delta = End - Start;
	Out = Start + Factor * Delta;
}

void FBXModel::ReadNodeHeirarchy(float AnimationTime, Node* pNode, const Matrix& ParentTransform, UINT num)
{
	string NodeName = pNode->name;

	const Animation* pAnimation = &animations[0];

	Matrix NodeTransformation = pNode->transform;

	const NodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

	if (pNodeAnim) {
		// スケーリングを補間し、スケーリング変換行列を生成する
		Vector3 Scaling;
		CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
		Matrix ScalingM;
		ScalingM = MatrixScaling(Scaling.x, Scaling.y, Scaling.z);

		// 回転を補間し、回転変換行列を生成する
		Quaternion RotationQ;
		CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
		Matrix RotationM = Rotate(RotationQ);

		// 移動を補間し、移動変換行列を生成する
		Vector3 Translation;
		CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
		Matrix TranslationM;
		TranslationM = MatrixTranslation(Translation.x, Translation.y, Translation.z);

		// これら上記の変換を合成する
		NodeTransformation = ScalingM * RotationM * TranslationM;
	}

	if (pNode->parent)
	{
		pNode->animMat = NodeTransformation * pNode->parent->animMat;
	}
	else
	{
		pNode->animMat = NodeTransformation;
	}

	Matrix mat = pNode->animMat;

	Bone* bone = nullptr;
	for (auto& itr : bones)
	{
		if (itr.name == NodeName)
		{
			bone = &itr;
			break;
		}
	}
	if (bone)
	{
		Matrix* boneoffset = &bone->invInitPose;
		Matrix tmpfinal = *boneoffset * mat;
		bone->finalMatrix = tmpfinal;
	}
}

void FBXModel::BoneTransform(float TimeInSeconds)
{
	Matrix Identity;

	float TicksPerSecond = animations[0].ticksPerSecond != 0 ?
		animations[0].ticksPerSecond : 25.0f;
	float TimeInTicks = TimeInSeconds * TicksPerSecond;
	float AnimationTime = fmod(TimeInTicks, animations[0].duration);

	for (UINT i = 0; i < nodes.size(); i++) {
		ReadNodeHeirarchy(AnimationTime, nodes[i].get(), Identity, i);
	}
}
