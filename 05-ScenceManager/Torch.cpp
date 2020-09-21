#include "Torch.h"
#include "Items.h"

CTorch::CTorch() :CGameObject()
{
	SetState(TORCH_STATE);
}

void CTorch::Render() {
	if (state != TORCH_STATE_DESTROYED) {
		animation_set->at(TORCH_ANI)->Render(x, y, -1);
	}
	else {
		animation_set->at(TORCH_ANI_DESTROYED)->Render(x, y, -1);
	}
	
	RenderBoundingBox();
}

void CTorch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	if (this->state == TORCH_STATE_DESTROYED && animation_set->at(TORCH_ANI_DESTROYED)->IsOver(TIME_TORCH_DESTROYED) && 
		this->visible == true) {
		this->SetVisible(false);
		CItems::GetInstance()->CheckAndDrop(this);
		return;
	}
}

void CTorch::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	bottom = top + HEIGHT_TORCH;
	right = left + WIDTH_TORCH;
}


CTorch* CTorch::__instance = NULL;
CTorch* CTorch::GetInstance()
{
	if (__instance == NULL) __instance = new CTorch();
	return __instance;
}
