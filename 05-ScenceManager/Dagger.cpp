#include "Dagger.h"

#include "Game.h"
#include "Torch.h"
#include "Items.h"
#include "Candle.h"
#include "Leopard.h"
#include "Boss.h"
#include "Zombie.h"
#include "Bat.h"

CDagger::CDagger() :CGameObject()
{
}

void CDagger::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (nx > 0) vx = DAGGER_SPEED;
	else vx = -DAGGER_SPEED;
	vy = 0;

	/*for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT coliObject = coObjects->at(i);

		if (this->CheckCollision(coliObject)) {
			if (dynamic_cast<CTorch*>(coliObject)) {
				this->SetVisible(false);
				coObjects->at(i)->SetState(TORCH_STATE_DESTROYED);
				coObjects->at(i)->animation_set->at(TORCH_ANI_DESTROYED)->SetAniStartTime(GetTickCount());
			}

		}
	}*/

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
			else if (dynamic_cast<CCandle*>(e->obj)) {
				this->SetVisible(false);
				e->obj->SetState(CANDLE_STATE_DESTROYED);
				e->obj->animation_set->at(CANDLE_ANI_DESTROYED)->SetAniStartTime(GetTickCount());
			}
			else if (dynamic_cast<CLeopard*>(e->obj)) {
				CSimon::GetInstance()->SetScore(100);
				e->obj->SetVisible(false);
			}
			else if (dynamic_cast<CZombie*>(e->obj)) {
				CSimon::GetInstance()->SetScore(100);
				e->obj->SetState(ZOMBIE_STATE_DESTROYED);
				e->obj->animation_set->at(ZOMBIE_ANI_DESTROYED)->SetAniStartTime(GetTickCount());
			}
			else if (dynamic_cast<CBat*>(e->obj)) {
				CSimon::GetInstance()->SetScore(100);
				e->obj->SetState(BAT_STATE_DESTROYED);
				e->obj->animation_set->at(BAT_ANI_DESTROYED)->SetAniStartTime(GetTickCount());
			}
			else if (dynamic_cast<CBoss*>(e->obj)) {
				CBoss::GetInstance()->SetHealth(CBoss::GetInstance()->GetHealth() - 2);
			}
			else {
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
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
