#include "NormalObj.h"
#include "imgui.h"
#include "ObjectManager.h"
#include "CameraManager.h"
#include "Collision.h"
#include "Sound.h"
#include "Input.h"
#include "SphereCollider.h"
#include "RaycastHit.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

using namespace IF;
using namespace std;

//中に作ったクラスと同じ名前を書く
const std::string Normal::objName = "Normal";

//? 初期化処理
void IF::Normal::ClassInitialize()
{
}

//? アップデート
void IF::Normal::ClassUpdate()
{
}

void IF::Normal::NormalUpdate()
{
	Float2 analog = Input::Instance()->GetLAnalog(6000);
	obj.position.x += analog.x * 0.25f;
	obj.position.z += analog.y * 0.25f;

	static bool onGround = true;
	static float gravity = 0;
	if (!onGround)
	{
		// 下向き加速度
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		// 加速
		gravity = max(gravity + fallAcc, fallVYMin);
		// 移動
		obj.position.y += gravity;
	}
	// ジャンプ操作
	else if (Input::Instance()->PadTriggere(PAD::A | PAD::B | PAD::X | PAD::Y))
	{
		onGround = false;
		const float jumpVYFist = 0.3f;
		gravity = jumpVYFist;
	}
	UpdateWorldMatrix();
	collider->Update();

	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(collider);
	assert(sphereCollider);

	// 球の上端から球の下端までのレイキャスト
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.y += sphereCollider->GetRadius();
	ray.dir = { 0,-1,0 };
	RaycastHit raycastHit;

	// 接地状態
	if (onGround) {
		// スムーズに坂を下る為の吸着距離
		const float adsDistance = 0.2f;
		// 接地を維持
		if (CollisionManager::Instance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance))
		{
			onGround = true;
			obj.position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		}
		// 地面がないので落下
		else
		{
			onGround = false;
			gravity = 0;
		}
	}
	// 落下状態
	else if (gravity <= 0.0f) {
		if (CollisionManager::Instance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f))
		{
			// 着地
			onGround = true;
			obj.position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		}
	}

	UpdateWorldMatrix();
	collider->Update();

	// クエリーコールバッククラス
	class PlayerQueryCallback : public QueryCallback
	{
	public:
		PlayerQueryCallback(Sphere* sphere) : sphere(sphere) {};

		// 衝突時コールバック関数
		bool OnQueryHit(const QueryHit& info) {

			const Vector3 up = { 0,1,0 };

			Vector3 rejectDir = Vector3Normalize(info.reject);
			float cos = Vector3Dot(rejectDir, up);

			// 地面判定しきい値
			const float threshold = cosf(ConvertToRadians(30.0f));

			if (-threshold < cos && cos < threshold) {
				sphere->center += info.reject;
				move += info.reject;
			}

			return true;
		}

		Sphere* sphere = nullptr;
		Vector3 move = {};
	};

	PlayerQueryCallback callback(sphereCollider);

	// 球と地形の交差を全検索
	CollisionManager::Instance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	// 交差による排斥分動かす
	obj.position.x += callback.move.x;
	obj.position.y += callback.move.y;
	obj.position.z += callback.move.z;

	UpdateWorldMatrix();

}

void IF::Normal::NormalInitialize()
{
	float radius = obj.scale.x;
	SetCollider(new SphereCollider({ 0,0,0 }, radius));
	collider->SetAttribute(COLLISION_ATTR_ALLIES);
}

