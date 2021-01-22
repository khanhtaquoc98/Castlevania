#include "Leopard.h"
#include "Simon.h"
#include "Brick.h"
#include "Brick4Leopard.h"
#include "Game.h"
#include <math.h>
#include "Utils.h"
#include "StairBottom.h"

void CLeopard::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case LEOPARD_STATE_RUN:
		if (this->nx == 1) {
			vx = LEOPARD_SPEED_RUN;
		}
		else vx = -LEOPARD_SPEED_RUN;
		break;
	case LEOPARD_STATE_SIT:
		vx = 0;
		break;
	case LEOPARD_STATE_JUMP:
		if (this->nx == 1) {
			vx = LEOPARD_SPEED_RUN;
		}
		else vx = -LEOPARD_SPEED_RUN;
		vy = -LEOPARD_GRAVITY_JUMPING;
		break;
	case LEOPARD_STATE_DEAD: {
		vx = vy = 0;
		//animation_set->at(LEOPARD_ANI_DEAD)->SetAniStartTime(GetTickCount());
		break;
	}
	}
	
}

CLeopard::CLeopard() :CGameObject()
{
	this->SetState(LEOPARD_STATE_SIT);

}

void CLeopard::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state != LEOPARD_STATE_DEAD) {
		left = x;
		top = y;
		right = left + WIDTH_LEOPARD;
		bottom = top + HEIGHT_LEOPARD;
	}
	else {
		left = x;
		top = y;
		right = left + 10;
		bottom = top + 10;
	}
}

void CLeopard::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	if (CSimon::GetInstance()->GetUseStopWatch() == true) {
		vx = vy = 0;
	}
	else { 
		vy += LEOPARD_GRAVITY * dt;  // Simple fall down}

		if (state == LEOPARD_STATE_SIT && (abs(CSimon::GetInstance()->x - (this->x + 32)) >= 300)) {
			this->SetOrientation(-CSimon::GetInstance()->GetOrientation());
		}

		//Check simon gần tới leopard thì setstate chạy
		if (abs(CSimon::GetInstance()->x - (this->x)) <= LEOPARD_DEFAULT_SPACE) {
			this->SetState(LEOPARD_STATE_RUN);
		}

		if (state == LEOPARD_STATE_RUN && abs(CSimon::GetInstance()->x - (this->x)) >= 200) {
			this->SetVisible(false);
		}

		if (CSimon::GetInstance()->EatCross() == true) {
			this->SetState(LEOPARD_STATE_DEAD);
		}
	}


	// No collision occured, proceed normally
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);


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


		// block every object first!
		x += min_tx * dx + nx * 0.2f;
		y += min_ty * dy + ny * 0.2f;

		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
		
			if (dynamic_cast<CBrick4Leopard*>(e->obj) && state != LEOPARD_STATE_DEAD)
			{
				this->SetState(LEOPARD_STATE_JUMP);
				isJump = true;
			}
			else if (dynamic_cast<CBrick*>(e->obj)) {
				if (e->ny != 0) vy = 0;
				if (e->nx != 0) x += dx;
				if (isJump && state != LEOPARD_STATE_DEAD) {
					this->SetState(LEOPARD_SPEED_RUN);
					isJump = false;
					if (this->x <= CSimon::GetInstance()->x) {
						this->SetOrientation(1);
					} else this->SetOrientation(-1);
					if (this->nx == 1) {
						vx = LEOPARD_SPEED_RUN;
					}
					else vx = -LEOPARD_SPEED_RUN;
				}
			} 
			else if (dynamic_cast<CStairBottom*>(e->obj) || dynamic_cast<CStairBottom*>(e->obj)) {
				if (e->ny != 0) vy = 0;
				if (e->nx != 0) x += dx;
			}
			else {
				if (e->ny != 0) y += dy;
				if (e->nx != 0) x += dx;
			}
		}
	}

	
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	

}

void CLeopard::Render()
{
	int ani = -1;
	if (state == LEOPARD_STATE_SIT) ani = LEOPARD_ANI_SIT;
	else if (state == LEOPARD_STATE_JUMP) ani = LEOPARD_ANI_JUMP;
	else if (state == LEOPARD_STATE_DEAD) ani = LEOPARD_ANI_DEAD;
	else {
		ani = LEOPARD_ANI_RUN;
	}

	if (CSimon::GetInstance()->GetUseStopWatch() == true) {
		int currentFrame = animation_set->at(ani)->GetCurrentFrame();
		animation_set->at(ani)->SetCurrentFrame(currentFrame >= 0 ? currentFrame : 0);
		animation_set->at(ani)->RenderByFrame(currentFrame >= 0 ? currentFrame : 0, nx, x, y, 255);
		RenderBoundingBox();
	}
	else {
		animation_set->at(ani)->Render(x, y, nx);
		RenderBoundingBox();
	}
	
}

CLeopard* CLeopard::__instance = NULL;

CLeopard* CLeopard::GetInstance()
{
	if (__instance == NULL) __instance = new CLeopard();
	return __instance;
}
