#include "Candle.h"
#include "Items.h"

CCandle::CCandle() :CGameObject()
{
	SetState(CANDLE_STATE);
}

void CCandle::Render() {
	if (state != CANDLE_STATE_DESTROYED) {
		animation_set->at(CANDLE_ANI)->Render(x, y, -1);
	}
	else {
		animation_set->at(CANDLE_ANI_DESTROYED)->Render(x, y, -1);
	}

	RenderBoundingBox();
}

void CCandle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	if (this->state == CANDLE_STATE_DESTROYED && animation_set->at(CANDLE_ANI_DESTROYED)->IsOver(TIME_TORCH_DESTROYED) && this->visible == true) {
		this->SetVisible(false);
		CItems::GetInstance()->CheckAndDrop(this);
		return;
	}
}

void CCandle::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	bottom = top + HEIGHT_TORCH;
	right = left + WIDTH_TORCH;
}


CCandle* CCandle::__instance = NULL;
CCandle* CCandle::GetInstance()
{
	if (__instance == NULL) __instance = new CCandle();
	return __instance;
}
