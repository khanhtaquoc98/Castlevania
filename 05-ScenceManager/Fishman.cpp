#include "Fishman.h"
#include "Torch.h"
#include "Candle.h"
#include "Brick.h"
#include "Utils.h"

void CFishman::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FISHMAN_STATE_WALKING:
		if (this->nx == 1)
			vx = -FISHMAN_SPEED;
		else
			vx = FISHMAN_SPEED;
		break;
	case FISHMAN_STATE_HIT:
		vx = 0;
		break;
	case FISHMAN_STATE_DESTROYED:
		vx = 0;
		break;
	}
}

CFishman::CFishman() :CGameObject()
{
	this->SetState(FISHMAN_STATE_WALKING);
}

void CFishman::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + FISHMAN_BBOX_WIDTH;
	bottom = y + FISHMAN_BBOX_HEIGHT;
}

void CFishman::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	

	if (abs((CSimon::GetInstance()->x) - (this->x)) <= 50)
	{
		vy += FISHMAN_GRAVITY * dt;
	}

	//this->SetOrientation(-CSimon::GetInstance()->GetOrientation());



	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT coliObject = coObjects->at(i);

		if (this->CheckCollision(coliObject))
		{
			if (this->state == FISHMAN_STATE_DESTROYED && animation_set->at(FISHMAN_ANI_DESTROYED)->IsOver(TIME_TORCH_DESTROYED) &&
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
			if (dynamic_cast<CTorch*>(e->obj) || dynamic_cast<CCandle*>(e->obj))
			{
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}
			else if (dynamic_cast<CBrick*>(e->obj) && (this->vy < 0))
			{
				if (e->nx != 0) x += dx;
				if (e->ny != 0 ) y += dy;
				if (this->y >= yCheck)
				{
					vy += -FISHMAN_GRAVITY * dt;
				}
			}
			else if (dynamic_cast<CBrick*>(e->obj))
			{
				vy = 0;
			}

		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CFishman::Render()
{
	int ani = -1;
	if (state == FISHMAN_STATE_WALKING) ani = FISHMAN_ANI_WALKING;
	else if (state == FISHMAN_STATE_HIT) ani = FISHMAN_ANI_HIT;
	else {
		ani = FISHMAN_ANI_DESTROYED;
	}

	animation_set->at(ani)->Render(x, y, -nx);
	RenderBoundingBox();
}

CFishman* CFishman::__instance = NULL;

CFishman* CFishman::GetInstance()
{
	if (__instance == NULL) __instance = new CFishman();
	return __instance;
}
