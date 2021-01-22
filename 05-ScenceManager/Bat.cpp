#include "Bat.h"
#include "Candle.h"
#include "Brick.h"
#include "Torch.h"
#include "CMath"
#include "Simon.h"

void CBat::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BAT_BBOX_WIDTH;
	bottom = y + BAT_BBOX_HEIGHT;
}

void CBat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (nx > 0) vx = -0.04f; else vx = 0.04f;

	if (vy < 0 && y < yCheck)
	{
		y = yCheck; vy = -vy;
	}

	if (vy > 0 && y > yCheck + 20)
	{
		y = yCheck + 20; vy = -vy;
	}

	if (this->state == BAT_STATE_DESTROYED) {
		vx = vy = 0;
	}

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT coliObject = coObjects->at(i);

		if (this->CheckCollision(coliObject)) {
			if (this->state == BAT_STATE_DESTROYED && animation_set->at(BAT_ANI_DESTROYED)->IsOver(TIME_TORCH_DESTROYED) &&
				this->visible == true) {
				this->SetVisible(false);
				return;
			}
		}
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
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
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CBat::Render()
{
	//animation_set->at(0)->Render(x, y, -nx);
	if (state != BAT_STATE_DESTROYED) {

		if (CSimon::GetInstance()->GetUseStopWatch() == true) {
			int currentFrame = animation_set->at(BAT_ANI)->GetCurrentFrame();
			animation_set->at(BAT_ANI)->SetCurrentFrame(currentFrame >= 0 ? currentFrame : 0);
			animation_set->at(BAT_ANI)->RenderByFrame(currentFrame >= 0 ? currentFrame : 0, nx, x, y, 255);
			RenderBoundingBox();
		}
		else {
			animation_set->at(BAT_ANI)->Render(x, y, -nx);
			RenderBoundingBox();
		}
		
	}
	else {
		animation_set->at(BAT_ANI_DESTROYED)->Render(x, y, -nx);
	}

	
}

CBat* CBat::__instance = NULL;

CBat::CBat() :CGameObject()
{
	SetState(BAT_STATE);
	vy = 0.03f;
	
}
CBat* CBat::GetInstance()
{
	if (__instance == NULL) __instance = new CBat();
	return __instance;
}
