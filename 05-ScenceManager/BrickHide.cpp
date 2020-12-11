#include "BrickHide.h"
#include "Items.h"

CBrickHide::CBrickHide() :CGameObject()
{
}

void CBrickHide::Render() {

	animation_set->at(0)->Render(x, y, 1);
	RenderBoundingBox();
}

void CBrickHide::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
}

void CBrickHide::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	bottom = top + 14;
	right = left + 16;
}


CBrickHide* CBrickHide::__instance = NULL;
CBrickHide* CBrickHide::GetInstance()
{
	if (__instance == NULL) __instance = new CBrickHide();
	return __instance;
}
