#include "HolyWater.h"

#include "Game.h"
#include "Torch.h"
#include "Brick.h"
#include "Candle.h"
#include "Items.h"
#include "Utils.h"

CHolyWater::CHolyWater() :CGameObject()
{
	this->SetState(STATE_HOLYWATER_NORMAL);
	Burning = false;
}

void CHolyWater::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (this->xFirst == -1) {
		this->xFirst = x;
	}

	if (this->GetState() == STATE_HOLYWATER_BURN) {
		vx = vy = 0;
	}
	else {
		if (nx > 0) vx = HOLY_SPEED;
		else vx = -HOLY_SPEED;
		if (abs(xFirst - x) > 10) {
			vy = 0.06f;
		}
		else vy = -0.05f;
	}
	

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (this->GetState() == STATE_HOLYWATER_BURN && timeStartBurn != 0) {
		int time = GetTickCount() - this->timeStartBurn > 500;
		DebugOut(L"Time: %d\n", time);
		if (GetTickCount() - this->timeStartBurn > 500)
		{
			Burning = false;
			xFirst = -1;
			timeStartBurn = 0;
			this->SetVisible(false);
			SetState(STATE_HOLYWATER_NORMAL);
		}
	}


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
				 if (dynamic_cast<CCandle*>(e->obj)) 
				 {
					e->obj->SetState(CANDLE_STATE_DESTROYED);
					e->obj->animation_set->at(CANDLE_ANI_DESTROYED)->SetAniStartTime(GetTickCount());
				 }
				else if (dynamic_cast<CBrick*>(e->obj))
				{
					x += -HOLY_WATER_NORMAL_BBOX_WIDTH/2;
					y += -HOLY_WATER_NORMAL_BBOX_HEIGHT/2 - 1;
					this->SetState(STATE_HOLYWATER_BURN);
					Burning = true;
					this->timeStartBurn = GetTickCount();
					//this->animation_set->at(ANI_HOLYWATER_BURN)->SetAniStartTime(GetTickCount());
				} 
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}


void CHolyWater::SetState(int state)
{
	this->state = state;
}

void CHolyWater::Render()
{
	if (this->GetState() == STATE_HOLYWATER_NORMAL) {
		animation_set->at(ANI_HOLYWATER_NORMAL)->Render(x, y, nx);
	} else
		animation_set->at(ANI_HOLYWATER_BURN)->Render(x, y, nx);
	RenderBoundingBox();
}


void CHolyWater::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	switch (state)
	{
	case STATE_HOLYWATER_NORMAL:
	{
		right = x + HOLY_WATER_NORMAL_BBOX_WIDTH;
		bottom = y + HOLY_WATER_NORMAL_BBOX_HEIGHT;
		break;
	}

	case STATE_HOLYWATER_BURN:
	{
		right = x + HOLY_WATER_BURN_BBOX_WIDTH;
		bottom = y + HOLY_WATER_BURN_BBOX_HEIGHT;
		break;
	}
	}
}


CHolyWater* CHolyWater::__instance = NULL;

CHolyWater* CHolyWater::GetInstance()
{
	if (__instance == NULL) __instance = new CHolyWater();
	return __instance;
}
