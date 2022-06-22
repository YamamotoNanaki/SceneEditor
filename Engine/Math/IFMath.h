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


	// 零ベクトルを返す
	const Vector3 Vector3Zero();
	// 2ベクトルが一致しているか調べる
	bool Vector3Equal(const Vector3& v1, const Vector3& v2);
	// 長さを求める
	float Vector3Length(const Vector3& v);
	// 正規化する
	Vector3& Vector3Normalize(Vector3& v);
	// 内積を求める
	float Vector3Dot(const Vector3& v1, const Vector3& v2);
	// 外積を求める
	Vector3 Vector3Cross(const Vector3& v1, const Vector3& v2);

	// ビュー行列の作成
	Matrix MatrixLookAtLH(const Vector3& eye, const Vector3& target, const Vector3& up);
	// 並行投影行列の作成
	Matrix MatrixOrthographic(
		float viewLeft, float viewRight, float viewBottom, float viewTop, float nearZ, float farZ);
	// 透視投影行列の作成
	Matrix MatrixPerspective(float fovAngleY, float aspectRatio, float nearZ, float farZ);

	// 座標変換（w除算なし）
	Vector3 Vector3Transform(const Vector3& v, const Matrix& m);
	// 座標変換（w除算あり）
	Vector3 Vector3TransformCoord(const Vector3& v, const Matrix& m);
	// ベクトル変換
	Vector3 Vector3TransformNormal(const Vector3& v, const Matrix& m);

	Vector3 VectorNegate(Vector3 m);

	Matrix MatrixPerspectiveFovLH(float FovAngleY, float AspectRatio, float NearZ, float FarZ);
	bool NearEqual(float S1, float S2, float Epsilon);
	void ScalarSinCos(float* pSin, float* pCos, float  Value);
	float ConvertToRadians(float fDegrees);
	float ConvertToDegrees(float fRadians);
};