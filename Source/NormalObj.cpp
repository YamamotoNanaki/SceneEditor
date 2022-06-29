#include "NormalObj.h"
#include "imgui.h"

using namespace IF;
using namespace std;

vector<D3D12_VIEWPORT>UsuallyObj::viewport;

void IF::UsuallyObj::MatInitialize(Matrix* matView, Matrix* matProjection, Float3* cameraPos, BillBoard::BillBoardMode mode)
{
	SetView(matView);
	SetProjection(matProjection);
	SetCamera(cameraPos);
	SetBillBoard(mode);
}

void IF::UsuallyObj::Initialize(Model* model)
{
	obj.Initialize(model);
}

void IF::UsuallyObj::Update()
{
	obj.Update(*matView, *matProjection, *cameraPos, mode);
}

void IF::UsuallyObj::Draw()
{
	if (texNum == 0)obj.Draw(viewport);
	else obj.Draw(viewport, texNum);
}