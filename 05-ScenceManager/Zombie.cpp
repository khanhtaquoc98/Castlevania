#include "Zombie.h"
#include "Brick.h"
#include "Candle.h"
#include "Torch.h"
#include "Simon.h"


void CZombie::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + ZOMBIE_BBOX_WIDTH;
	bottom = y + ZOMBIE_BBOX_HEIGHT;
}

void CZombie::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	vy += ZOMBIE_GRAVITY * dt;

	//this->SetOrientation(-CSimon::GetInstance()->GetOrientation());

	if (nx > 0) {
		vx = -ZOMBIE_SPEED;
	}

	if (this->state == ZOMBIE_STATE_DESTROYED) {
		vx = vy = 0;
	}

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT coliObject = coObjects->at(i);

		if (this->CheckCollision(coliObject)) {
			if (this->state == ZOMBIE_STATE_DESTROYED && animation_set->at(ZOMBIE_ANI_DESTROYED)->IsOver(TIME_TORCH_DESTROYED) &&
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
			/*if (dynamic_cast<CTorch*>(e->obj) || dynamic_cast<CCandle*>(e->obj))
			{
				
			}*/
			/*else*/ 
			if (dynamic_cast<CBrick*>(e->obj))
			{
				vy = 0;
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


void CZombie::Render()
{
	//animation_set->at(0)->Render(x, y, -nx);

	if (state != ZOMBIE_STATE_DESTROYED) {
		if (CSimon::GetInstance()->GetUseStopWatch() == true) {
			int currentFrame = animation_set->at(ZOMBIE_ANI)->GetCurrentFrame();
			animation_set->at(ZOMBIE_ANI)->SetCurrentFrame(currentFrame >= 0 ? currentFrame : 0);
			animation_set->at(ZOMBIE_ANI)->RenderByFrame(currentFrame >= 0 ? currentFrame : 0, nx, x, y, 255);
			RenderBoundingBox();
		}
		else {
			animation_set->at(ZOMBIE_ANI)->Render(x, y, -nx);
			RenderBoundingBox();
		}
	}
	else {
		animation_set->at(ZOMBIE_ANI_DESTROYED)->Render(x, y, -nx);
	}
	
	
}

CZombie* CZombie::__instance = NULL;

CZombie::CZombie() :CGameObject()
{
	SetState(ZOMBIE_STATE);
}
CZombie* CZombie::GetInstance()
{
	if (__instance == NULL) __instance = new CZombie();
	return __instance;
}