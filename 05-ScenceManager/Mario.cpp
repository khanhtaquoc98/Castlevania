#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Whip.h"
#include "Portal.h"

CMario::CMario(float x, float y) : CGameObject()
{
<<<<<<< HEAD:05-ScenceManager/Simon.cpp
	//level = MARIO_LEVEL_BIG;

=======
	level = MARIO_LEVEL_BIG;
>>>>>>> parent of 759d0a9... Update State Simon:05-ScenceManager/Mario.cpp
	untouchable = 0;
	SetState(SIMON_STATE_IDLE);

	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 

	whip = new CWhip();
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	if (x <= -6) x = -6;

	// Simple fall down
	vy += MARIO_GRAVITY*dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state!=SIMON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size()==0)
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
		x += min_tx*dx + nx*0.4f;
		y += min_ty*dy + ny*0.4f;

		if (nx!=0) vx = 0;
		if (ny!=0) vy = 0;


		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState()!= GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable==0)
					{
						if (goomba->GetState()!=GOOMBA_STATE_DIE)
						{
								SetState(SIMON_STATE_DIE);
						}
					}
				}
			} // if Goomba
			else if (dynamic_cast<CPortal *>(e->obj))
			{
				CPortal *p = dynamic_cast<CPortal *>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	if (state == SIMON_STATE_STANDING || state == SIMON_STATE_STANDING_SIT)
	{
		whip->SetOrientation(nx);
		whip->SetPositionWhip(D3DXVECTOR2(x,y), state == SIMON_STATE_STANDING ? true : false);
	}
}

void CMario::Render()
{
	int ani = -1;
	if (state == SIMON_STATE_DIE)
		ani = SIMON_ANI_DIE;
	else if (state == SIMON_STATE_DUCK) ani = SIMON_ANI_DUCK;
	else if (state == SIMON_STATE_JUMP) ani = SIMON_ANI_JUMP;
	else if (state == SIMON_STATE_STANDING) ani = SIMON_ANI_STANDING;
	else
	{
		if (vx == 0)
		{
			ani = SIMON_ANI_IDLE;
		}
		else {
			ani = SIMON_ANI_WALKING;
		}
	}
	
	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y, nx, alpha);
	RenderBoundingBox();

	if (state == SIMON_STATE_STANDING || state == SIMON_STATE_STANDING_SIT) {
		whip->RenderbyFrame(animation_set->at(ani)->GetCurrentFrame());
	}
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_WALKING:
		if (nx > 0) {

			vx = MARIO_WALKING_SPEED;
		}
		else {
			vx = -MARIO_WALKING_SPEED;
		}
		break;
	case SIMON_STATE_JUMP:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
			vy = -MARIO_JUMP_SPEED_Y;
		break; 
	case SIMON_STATE_DUCK:
		vx = 0;
		break;
	case SIMON_STATE_STANDING: {
		animation_set->at(SIMON_ANI_STANDING)->Reset();
		animation_set->at(SIMON_ANI_STANDING)->SetAniStartTime(GetTickCount());
		break;
	}
	case SIMON_STATE_IDLE:
		vx = 0;
		break;
	case SIMON_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	}
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (nx > 0) {
		left = x + 8;
	}
	else left = x + 6;
	
	top = y; 
	right = left + SIMON_BBOX_WIDTH;
	bottom = top + SIMON_BBOX_HEIGHT;
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(SIMON_STATE_IDLE);
	//SetLevel(MARIO_LEVEL_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

