#include "Dagger.h"

#include "Game.h"
#include "Torch.h"
#include "Items.h"
#include "Zombie.h"
#include "Bat.h"
#include "Fishman.h"

CDagger::CDagger() :CGameObject()
{
}

void CDagger::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (nx > 0) vx = DAGGER_SPEED;
	else vx = -DAGGER_SPEED;
	vy = 0;

	
	if (!CGame::GetInstance()->InCamera(this)) {
		this->SetVisible(false);
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

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

		x += min_tx * dx + nx * 0.2f;
		y += min_ty * dy + ny * 0.2f;

		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CTorch*>(e->obj))
			{
				this->SetVisible(false);
				e->obj->SetState(TORCH_STATE_DESTROYED);
				e->obj->animation_set->at(TORCH_ANI_DESTROYED)->SetAniStartTime(GetTickCount());
			}
			else if (dynamic_cast<CZombie*>(e->obj)) {
				e->obj->SetState(ZOMBIE_STATE_DESTROYED);
				e->obj->animation_set->at(ZOMBIE_ANI_DESTROYED)->SetAniStartTime(GetTickCount());
			}
			else if (dynamic_cast<CBat*>(e->obj)) {
				e->obj->SetState(BAT_STATE_DESTROYED);
				e->obj->animation_set->at(BAT_ANI_DESTROYED)->SetAniStartTime(GetTickCount());
			}
			else if (dynamic_cast<CFishman*>(e->obj)) {
				e->obj->SetState(FISHMAN_STATE_DESTROYED);
				e->obj->animation_set->at(FISHMAN_ANI_DESTROYED)->SetAniStartTime(GetTickCount());
			}

		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

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
