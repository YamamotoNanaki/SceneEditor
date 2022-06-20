#include "PlayerObj.h"

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
	obj.Update(*matView, *matProjection, *cameraPos, mode);
}

void IF::PlayerObj::Draw()
{
	if (texNum == 0)obj.Draw(viewport);
	else obj.Draw(viewport, texNum);
}
