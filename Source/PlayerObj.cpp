#include "PlayerObj.h"
#include "imgui.h"
#include "Input.h"

using namespace std;
using namespace IF;

vector<D3D12_VIEWPORT>PlayerObj::viewport;

void IF::PlayerObj::MatInitialize(Matrix* matView, Matrix* matProjection, Float3* cameraPos, BillBoard::BillBoardMode mode)
{
	SetView(matView);
	SetProjection(matProjection);
	SetCamera(cameraPos);
	SetBillBoard(mode);
}
//
//void IF::PlayerObj::Initialize(Model* model)
//{
//	obj.Initialize(model);
//}

void IF::PlayerObj::Update()
{
	//==========================//
	Input* i = Input::Instance();

	//if (i->KDown(KEY::W))obj.position.y += 0.1f;
	//if (i->KDown(KEY::S))obj.position.y -= 0.1f;
	//if (i->KDown(KEY::D))obj.position.x += 0.1f;
	//if (i->KDown(KEY::A))obj.position.x -= 0.1f;

	//==========================//
	if (colision != nullptr)
	{
		if (ptype == NotPri);
		else if (ptype == RayPri)
		{
			colision->SetCenter(SetVector3(obj.position));
			colision->SetDir(SetVector3({ obj.position.x + obj.scale.x, obj.position.y + obj.scale.y, obj.position.z + obj.scale.z }));
		}
		else
		{
			colision->SetCenter(SetVector3(obj.position));
			colision->SetRadius((obj.scale.x + obj.scale.y + obj.scale.z) / 3.0f);
		}
	}

	obj.Update(*matView, *matProjection, *cameraPos, mode);
}

void IF::PlayerObj::Draw()
{
	if (texNum == 0)obj.Draw(viewport);
	else obj.Draw(viewport, texNum);
}