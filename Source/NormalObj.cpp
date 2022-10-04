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

//void IF::UsuallyObj::Initialize(Model* model)
//{
//	obj.Initialize(model);
//}

void IF::UsuallyObj::Update()
{
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
			colision->SetCenter(SetVector3({ obj.position.x,obj.position.y + 1,obj.position.z }));
			colision->SetRadius((obj.scale.x + obj.scale.y + obj.scale.z) / 3.0f * 2.0f);
		}
	}
	obj.Update(*matView, *matProjection, *cameraPos, mode);
}

void IF::UsuallyObj::Draw()
{
	if (texNum == 0)obj.Draw(viewport);
	else obj.Draw(viewport, texNum);
}