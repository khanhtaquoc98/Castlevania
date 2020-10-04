#include <algorithm>
#include <assert.h>
#include "Utils.h"
#include "ItemBigHeart.h"
#include "ItemChain.h"
#include "Simon.h"
#include "Game.h"
#include "Torch.h"
#include "Brick.h"
#include "Portal.h"
#include "Dagger.h"
#include "SubWeapons.h"
#include "ItemDagger.h"

CSimon* CSimon::__instance = NULL;


CSimon* CSimon::GetInstance()
{
	if (__instance == NULL)
		__instance = new CSimon();
	return __instance;
}

CSimon::CSimon(float x, float y) : CGameObject()
{
	//level = MARIO_LEVEL_BIG;

	untouchable = 0;
	SetState(SIMON_STATE_IDLE);

	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 

	whip = new CWhip();
}

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	if (x <= -6) x = -6;

	// Simple fall down
	vy += MARIO_GRAVITY*dt;
	DebugOut(L"OnGroudn: %d\n", OnGroud);
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT coliObject = coObjects->at(i);

		if (this->CheckCollision(coliObject)) {
			if (dynamic_cast<CItemBigHeart*>(coliObject)) {
				coliObject->SetVisible(false);
			}
			else if(dynamic_cast<CItemChain*>(coliObject)) {
				coliObject->SetVisible(false);
				this->SetState(SIMON_STATE_CHANGE_COLOR);
				this->animation_set->at(SIMON_ANI_CHANGE_COLOR)->SetAniStartTime(GetTickCount());
				whip->UpItemWhip();
			}
			else if (dynamic_cast<CItemDagger*>(coliObject)) {
				coliObject->SetVisible(false);
			}
		}
	}

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
		x += min_tx*dx + nx*0.2f;
		y += min_ty*dy + ny*0.2f;

		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CTorch*>(e->obj))
			{
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}
			else if (dynamic_cast<CItemBigHeart*>(e->obj)) {
				e->obj->SetVisible(false);
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}
			else if (dynamic_cast<CItemChain*>(e->obj)) {
				e->obj->SetVisible(false);
				this->SetState(SIMON_STATE_CHANGE_COLOR);
				whip->UpItemWhip();
			}
			else if (dynamic_cast<CItemDagger*>(e->obj)) {
				e->obj->SetVisible(false);
			}
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
			else {
				if (nx != 0) vx = 0;
				if (ny != 0) vy = 0;
				OnGroud = true;
			}


		}
	}


	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	if (state == SIMON_STATE_ATTACK || state == SIMON_STATE_ATTACK_SIT || state == SIMON_STATE_JUMP_ATTACK)
	{
		whip->SetOrientation(nx);
		whip->SetPositionWhip(D3DXVECTOR2(x,y), state != SIMON_STATE_ATTACK_SIT ? true : false);
		if (animation_set->at(SIMON_ANI_ATTACK)->GetCurrentFrame() == 2 ||
			animation_set->at(SIMON_ANI_ATTACK_SIT)->GetCurrentFrame() == 2) {
			whip->Update(dt, coObjects);
		}
	}
	if (state == SIMON_STATE_ATTACK_SUBWEAPON) {
		if (animation_set->at(SIMON_ANI_ATTACK)->GetCurrentFrame() == 2) {
			CSubWeapons::GetInstance()->UseSubWeapon(41);
		}
	}

}

void CSimon::Render()
{
	int ani = -1; 
	if (state == SIMON_STATE_DIE) ani = SIMON_ANI_DIE;
	else if (state == SIMON_STATE_SIT) ani = SIMON_ANI_DUCK;
	else if (state == SIMON_STATE_JUMP) ani = SIMON_ANI_JUMP;
	else if (state == SIMON_STATE_JUMP_ATTACK) ani = SIMON_ANI_ATTACK;
	else if (state == SIMON_STATE_ATTACK || state == SIMON_STATE_ATTACK_SUBWEAPON) ani = SIMON_ANI_ATTACK;
	else if (state == SIMON_STATE_ATTACK_SIT) ani = SIMON_ANI_ATTACK_SIT;
	else if (state == SIMON_STATE_CHANGE_COLOR) ani = SIMON_ANI_CHANGE_COLOR;
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

	if (state == SIMON_STATE_ATTACK || state == SIMON_STATE_ATTACK_SIT || state == SIMON_STATE_JUMP_ATTACK) {
		whip->RenderbyFrame(animation_set->at(ani)->GetCurrentFrame());
	}
	
}

void CSimon::SetState(int state)
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
		case SIMON_STATE_JUMP_ATTACK:
			OnGroud = false;
			animation_set->at(SIMON_ANI_ATTACK)->Reset();
			animation_set->at(SIMON_ANI_ATTACK)->SetAniStartTime(GetTickCount());
			break;
		case SIMON_STATE_SIT:
			vx = 0;
			break;
		case SIMON_STATE_ATTACK: {
			animation_set->at(SIMON_ANI_ATTACK)->Reset();
			animation_set->at(SIMON_ANI_ATTACK)->SetAniStartTime(GetTickCount());
			break;
		}
		case SIMON_STATE_ATTACK_SUBWEAPON: {
			animation_set->at(SIMON_ANI_ATTACK)->Reset();
			animation_set->at(SIMON_ANI_ATTACK)->SetAniStartTime(GetTickCount());
			break;
		}	
		case SIMON_STATE_ATTACK_SIT: {
			animation_set->at(SIMON_ANI_ATTACK_SIT)->Reset();
			animation_set->at(SIMON_ANI_ATTACK_SIT)->SetAniStartTime(GetTickCount());
			break;
		}
		case SIMON_STATE_IDLE:
			vx = 0;
			break;
		case SIMON_STATE_CHANGE_COLOR:
			vx = 0;
			animation_set->at(SIMON_ANI_CHANGE_COLOR)->Reset();
			animation_set->at(SIMON_ANI_CHANGE_COLOR)->SetAniStartTime(GetTickCount());
			break;
		case SIMON_STATE_DIE:
			vy = -MARIO_DIE_DEFLECT_SPEED;
			break;
		
	}
}

void CSimon::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == SIMON_STATE_SIT || state == SIMON_STATE_ATTACK_SIT || state == SIMON_STATE_JUMP) {
		top = y + 7;
		bottom = top + 23; // 23 == Chiều cao của simon khi ngồi
	}
	else {
		top = y;
		bottom = top + SIMON_BBOX_HEIGHT;
	}
		

	if (nx > 0) {
		left = x + 8;
	}
	else left = x + 6;
	
	right = left + SIMON_BBOX_WIDTH;
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CSimon::Reset()
{
	SetState(SIMON_STATE_IDLE);
	//SetLevel(MARIO_LEVEL_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

