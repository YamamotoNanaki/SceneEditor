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

//���ɍ�����N���X�Ɠ������O������
const std::string Normal::objName = "Normal";

//? ����������
void IF::Normal::ClassInitialize()
{
}

//? �A�b�v�f�[�g
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
		// �����������x
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		// ����
		gravity = max(gravity + fallAcc, fallVYMin);
		// �ړ�
		obj.position.y += gravity;
	}
	// �W�����v����
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

	// ���̏�[���狅�̉��[�܂ł̃��C�L���X�g
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.y += sphereCollider->GetRadius();
	ray.dir = { 0,-1,0 };
	RaycastHit raycastHit;

	// �ڒn���
	if (onGround) {
		// �X���[�Y�ɍ������ׂ̋z������
		const float adsDistance = 0.2f;
		// �ڒn���ێ�
		if (CollisionManager::Instance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance))
		{
			onGround = true;
			obj.position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		}
		// �n�ʂ��Ȃ��̂ŗ���
		else
		{
			onGround = false;
			gravity = 0;
		}
	}
	// �������
	else if (gravity <= 0.0f) {
		if (CollisionManager::Instance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f))
		{
			// ���n
			onGround = true;
			obj.position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		}
	}

	UpdateWorldMatrix();
	collider->Update();

	// �N�G���[�R�[���o�b�N�N���X
	class PlayerQueryCallback : public QueryCallback
	{
	public:
		PlayerQueryCallback(Sphere* sphere) : sphere(sphere) {};

		// �Փˎ��R�[���o�b�N�֐�
		bool OnQueryHit(const QueryHit& info) {

			const Vector3 up = { 0,1,0 };

			Vector3 rejectDir = Vector3Normalize(info.reject);
			float cos = Vector3Dot(rejectDir, up);

			// �n�ʔ��肵�����l
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

	// ���ƒn�`�̌�����S����
	CollisionManager::Instance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	// �����ɂ��r�˕�������
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

