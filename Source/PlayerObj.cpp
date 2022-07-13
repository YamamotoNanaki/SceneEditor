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

void IF::PlayerObj::Initialize(Model* model)
{
	obj.Initialize(model);
}

void IF::PlayerObj::Update()
{
	//==========================//
	Input* i = Input::Instance();

	if (i->KDown(KEY::UP))obj.position.y += 0.5f;
	if (i->KDown(KEY::DOWN))obj.position.y -= 0.5f;
	if (i->KDown(KEY::RIGHT))obj.position.x += 0.5f;
	if (i->KDown(KEY::LEFT))obj.position.x -= 0.5f;

	//==========================//

	obj.Update(*matView, *matProjection, *cameraPos, mode);
}

void IF::PlayerObj::Draw()
{
	if (texNum == 0)obj.Draw(viewport);
	else obj.Draw(viewport, texNum);
}