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

	//if (i->KDown(KEY::UP))obj.position.y += 0.5f;
	//if (i->KDown(KEY::DOWN))obj.position.y -= 0.5f;
	//if (i->KDown(KEY::RIGHT))obj.position.x += 0.5f;
	//if (i->KDown(KEY::LEFT))obj.position.x -= 0.5f;

	static int r = 0, g = 128, b = 255, a = 255;
	static char flag = 0b011;
	r++;
	if (r > 255)r -= 255;
	if (flag & 0b100)g--;
	else g++;
	if (g >= 255 || g <= 0)flag ^= 0b100;
	if (flag & 0b010)b--;
	else b++;
	if (b >= 255 || b <= 0)flag ^= 0b010;
	if (flag & 0b001)a -= 2;
	else a += 2;
	if (a >= 255 || a <= 0)flag ^= 0b001;


	obj.SetColor(r, g, b, a);

	//==========================//

	obj.Update(*matView, *matProjection, *cameraPos, mode);
}

void IF::PlayerObj::Draw()
{
	if (texNum == 0)obj.Draw(viewport);
	else obj.Draw(viewport, texNum);
}