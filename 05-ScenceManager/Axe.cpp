#include "Axe.h"

#include "Game.h"
#include "Torch.h"
#include "Brick.h"
#include "Candle.h"
#include "Items.h"
#include "Utils.h"

CAxe::CAxe() :CGameObject()
{
	this->SetState(730);
}

void CAxe::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);


	if (nx > 0) vx = 0.009f*dt;
	else vx = -0.009*dt;
	if (abs(CSimon::GetInstance()->x - x) >= 70) {
		vy = 0.2f;
	}
	else vy = -0.2f;

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
			if (e->nx != 0) x += dx;
			if (e->ny != 0) y += dy;
			 if (dynamic_cast<CCandle*>(e->obj))
			{
				e->obj->SetState(CANDLE_STATE_DESTROYED);
				e->obj->animation_set->at(CANDLE_ANI_DESTROYED)->SetAniStartTime(GetTickCount());
			 }
			
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}


void CAxe::SetState(int state)
{
	this->state = state;
}

void CAxe::Render()
{
	animation_set->at(0)->Render(x, y, nx);
	RenderBoundingBox();
}


void CAxe::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = x + AXE_NORMAL_BBOX_WIDTH;
	bottom = y + AXE_NORMAL_BBOX_HEIGHT;
	
}


CAxe* CAxe::__instance = NULL;

CAxe* CAxe::GetInstance()
{
	if (__instance == NULL) __instance = new CAxe();
	return __instance;
}
