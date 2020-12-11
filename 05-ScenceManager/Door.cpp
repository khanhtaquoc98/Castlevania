#include "Door.h"
#include "Items.h"

CDoor::CDoor() :CGameObject()
{
	SetState(DOOR_STATE_CLOSE);
}

void CDoor::Render() {
	if (this->GetState() == DOOR_STATE_OPEN) {
		animation_set->at(0)->Render(x, y, -1);
	}  
	RenderBoundingBox();
}

void CDoor::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	if (this->state == DOOR_STATE_OPEN && animation_set->at(0)->IsOver(800) &&
		this->visible == true) {
		this->visible = false;
		return;
	}
}

void CDoor::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x - 10;
	top = y;
	bottom = top + HEIGHT_DOOR;
	right = left + WIDTH_DOOR;
}


CDoor* CDoor::__instance = NULL;
CDoor* CDoor::GetInstance()
{
	if (__instance == NULL) __instance = new CDoor();
	return __instance;
}


