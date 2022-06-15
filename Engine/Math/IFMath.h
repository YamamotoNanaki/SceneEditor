#pragma once
#include "Float2.h"
#include "Float3.h"
#include "Float4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Matrix.h"

namespace IF
{
	Float4 SetFloat4ToFloat3(Float3 f, float z = 0);
	Float4 SetFloat4ToFoat2(Float2 f, float z = 0, float w = 0);
	Float3 SetFloat3ToFloat2(Float2 f, float z = 0);
	Float4 SetFloat4ToVector3(Vector3 v, float w = 0);
	Float3 SetFloat3(Vector3 v);
	Float2 SetFloat3ToVector2(Vector2 v);
	Vector3 SetVector3(Float3 f);
	Vector3 SetVector3ToFloat2(Float2 f, float z = 0);
	Vector2 SetVector2(Float2 f);


	// ��x�N�g����Ԃ�
	const Vector3 Vector3Zero();
	// 2�x�N�g������v���Ă��邩���ׂ�
	bool Vector3Equal(const Vector3& v1, const Vector3& v2);
	// ���������߂�
	float Vector3Length(const Vector3& v);
	// ���K������
	Vector3& Vector3Normalize(Vector3& v);
	// ���ς����߂�
	float Vector3Dot(const Vector3& v1, const Vector3& v2);
	// �O�ς����߂�
	Vector3 Vector3Cross(const Vector3& v1, const Vector3& v2);

	// �r���[�s��̍쐬
	Matrix MatrixLookAtLH(const Vector3& eye, const Vector3& target, const Vector3& up);
	// ���s���e�s��̍쐬
	Matrix MatrixOrthographic(
		float viewLeft, float viewRight, float viewBottom, float viewTop, float nearZ, float farZ);
	// �������e�s��̍쐬
	Matrix MatrixPerspective(float fovAngleY, float aspectRatio, float nearZ, float farZ);

	// ���W�ϊ��iw���Z�Ȃ��j
	Vector3 Vector3Transform(const Vector3& v, const Matrix& m);
	// ���W�ϊ��iw���Z����j
	Vector3 Vector3TransformCoord(const Vector3& v, const Matrix& m);
	// �x�N�g���ϊ�
	Vector3 Vector3TransformNormal(const Vector3& v, const Matrix& m);

	Vector3 VectorNegate(Vector3 m);

	Matrix MatrixPerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ, float FarZ);
	bool NearEqual(float S1, float S2, float Epsilon);
	void ScalarSinCos(float* pSin, float* pCos, float  Value);
	float ConvertToRadians(float fDegrees);
};