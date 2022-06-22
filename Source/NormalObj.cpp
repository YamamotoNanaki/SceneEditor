#include "NormalObj.h"
#include "imgui.h"

using namespace IF;
using namespace std;

vector<D3D12_VIEWPORT>NormalObj::viewport;

void IF::NormalObj::MatInitialize(Matrix* matView, Matrix* matProjection, Float3* cameraPos, BillBoard::BillBoardMode mode)
{
	SetView(matView);
	SetProjection(matProjection);
	SetCamera(cameraPos);
	SetBillBoard(mode);
}

void IF::NormalObj::Initialize(Model* model)
{
	obj.Initialize(model);
}

void IF::NormalObj::Update()
{
	obj.Update(*matView, *matProjection, *cameraPos, mode);
}

void IF::NormalObj::Draw()
{
	if (texNum == 0)obj.Draw(viewport);
	else obj.Draw(viewport, texNum);
}