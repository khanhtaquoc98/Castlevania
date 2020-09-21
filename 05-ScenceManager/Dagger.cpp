#include "Dagger.h"

#include "Game.h"
#include "Torch.h"
#include "Items.h"

CDagger::CDagger() :CGameObject()
{
}

void CDagger::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (nx > 0) vx = DAGGER_SPEED;
	else vx = -DAGGER_SPEED;
	vy = 0;

	CGameObject::Update(dt);

	if (!CGame::GetInstance()->InCamera(this)) {
		this->SetVisible(false);
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else {
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

	}
}


void CDagger::SetState(int state)
{
	this->state = state;
}

void CDagger::Render()
{
	animation_set->at(0)->Render(x, y, nx);
	RenderBoundingBox();
}


void CDagger::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = left + WIDTH_DAGGER;
	bottom = top + HEIGHT_DAGGER;
}


CDagger* CDagger::__instance = NULL;

CDagger* CDagger::GetInstance()
{
	if (__instance == NULL) __instance = new CDagger();
	return __instance;
}
