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
	}
}

CLeopard::CLeopard() :CGameObject()
{
	this->SetState(LEOPARD_STATE_SIT);

}

void CLeopard::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + WIDTH_LEOPARD;
	bottom = top + HEIGHT_LEOPARD;
}

void CLeopard::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	 vy += LEOPARD_GRAVITY * dt;// Simple fall down

	if (state == LEOPARD_STATE_SIT && (abs(CSimon::GetInstance()->x - (this->x + 32)) >= 300)) {
		this->SetOrientation(-CSimon::GetInstance()->GetOrientation());
	}

	//Check simon gần tới leopard thì setstate chạy
	if (state == LEOPARD_STATE_SIT &&  abs(CSimon::GetInstance()->x - (this->x)) <= LEOPARD_DEFAULT_SPACE) {
		this->SetState(LEOPARD_STATE_RUN);
	}

	// No collision occured, proceed normally
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT coliObject = coObjects->at(i);

		if (this->CheckCollision(coliObject)) {
			if (dynamic_cast<CBrick4Leopard*>(coliObject)) {
				this->SetState(LEOPARD_STATE_JUMP);
				isJump = true;
			}
		}
	}

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
			if (dynamic_cast<CBrick4Leopard*>(e->obj))
			{
				this->SetState(LEOPARD_STATE_JUMP);
				isJump = true;
			}
			else if (dynamic_cast<CBrick*>(e->obj)) {
				if (ny != 0) vy = 0;
				if (isJump) {
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
			else {
				y = y - 2;
				if (e->ny <= 0) y += dy;
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
	else {
		ani = LEOPARD_ANI_RUN;
	}

	animation_set->at(ani)->Render(x, y, nx);
	RenderBoundingBox();
}

CLeopard* CLeopard::__instance = NULL;

CLeopard* CLeopard::GetInstance()
{
	if (__instance == NULL) __instance = new CLeopard();
	return __instance;
}
