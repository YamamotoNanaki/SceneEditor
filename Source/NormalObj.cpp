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

#ifdef _DEBUG
void IF::NormalObj::GUI()
{
	if (ImGui::TreeNode(tag.c_str())) {
		float f[3] = { obj.position.x,obj.position.y,obj.position.z };
		ImGui::Text("Position");
		ImGui::InputFloat3("", f);
		obj.position = { f[0], f[1], f[2] };

		ImGui::TreePop();
	}
}
#endif
