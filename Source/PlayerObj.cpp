#include "PlayerObj.h"
#include "imgui.h"

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

#ifdef _DEBUG
void IF::PlayerObj::GUI()
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
