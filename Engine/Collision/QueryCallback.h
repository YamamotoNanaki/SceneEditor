#pragma once
#include "IFMath.h"

namespace IF
{
	class CObject;
	class BaseCollider;
	/// <summary>
	/// クエリーによる情報を得る為の構造体
	/// </summary>
	struct QueryHit
	{
		// 衝突相手のオブジェクト
		CObject* object = nullptr;
		// 衝突相手のコライダー
		BaseCollider* collider = nullptr;
		// 衝突点
		Vector3 inter;
		// 排斥ベクトル
		Vector3 reject;
	};

	/// <summary>
	/// クエリーで交差を検出した時の動作を規定するクラス
	/// </summary>
	class QueryCallback
	{
	public:
		QueryCallback() = default;
		virtual ~QueryCallback() = default;

		/// <summary>
		/// 交差時コールバック
		/// </summary>
		/// <param name="info">交差情報</param>
		/// <returns>クエリーを続けるならtrue、打ち切るならfalseを返す</returns>
		virtual bool OnQueryHit(const QueryHit& info) = 0;
	};
}
