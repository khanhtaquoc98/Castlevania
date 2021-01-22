#include <algorithm>
#include <assert.h>
#include "Utils.h"
#include "ItemBigHeart.h"
#include "ItemSmallHeart.h"
#include "ItemChain.h"
#include "Simon.h"
#include "Game.h"
#include "Torch.h"
#include "Candle.h"
#include "Brick.h"
#include "Portal.h"
#include "Dagger.h"
#include "ItemHolyWater.h"
#include "SubWeapons.h"
#include "ItemDagger.h"
#include "StairBottom.h"
#include "StairTop.h"
#include "Brick4Leopard.h"
#include "Brick4MoneyRed.h"
#include "BrickHide.h"
#include "ItemMoneyBagRed.h"
#include "ItemMoneyBagPurple.h"
#include "Items.h"
#include "PointEffect.h"
#include "ItemMoneyBagYellow.h"
#include "WallPieces.h"
#include "ItemAxe.h"
#include "ItemCross.h"
#include "Leopard.h"
#include "ItemStopWatch.h"
#include "Boss.h"

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
	DebugOut(L"isOnStair First: %d\n", this->isOnStair);
	untouchable = 0;
	SetState(SIMON_STATE_IDLE);

	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;


	whip = new CWhip();
}

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	if (x <= -6) x = -6;

	if (this->health == 0) {
		this->SetNumLife(this->numLife - 1);
		this->SetHealth(16);
		this->SetPosition(start_x, start_y);
		StartUntouchable();
	}

	DebugOut(L" bool: %d\n", this->useStopWatch);
	DebugOut(L" time: %d\n", this->timeStopWatch);
	//Stopwatch
	if (GetTickCount() - timeStopWatch > 6000) {
		this->useStopWatch = false;
		this->timeStopWatch = 0;
	}
	else {
		//
	}

	// Simple fall down
	if (isOnStair == false) {
		vy += MARIO_GRAVITY * dt;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != SIMON_STATE_DIE )
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT coliObject = coObjects->at(i);

		if (this->CheckCollision(coliObject)) {
			if (isOnStair == true) {
				if (dynamic_cast<CStairBottom*>(coliObject)  && this->OnGroud == false) {
					this->isOnStair = false;
					this->canGoDownStair = false;
					this->canGoUpStair = true;
					this->simonXStair = coliObject->simonXStair;
					this->nxCanGoStair = coliObject->nx;
					this->OnGroud = true;
					this->SetState(SIMON_STATE_IDLE);
				}
				else if (dynamic_cast<CStairTop*>(coliObject) && this->OnGroud == false) {
					this->isOnStair = false;
					this->OnGroud = true;
					this->canGoUpStair = false;
					this->canGoDownStair = true;
					this->SetState(SIMON_STATE_IDLE);
					this->simonGoStair = coliObject->simonXStair;
					this->nxCanGoStair = coliObject->nx;
				}
				else if (dynamic_cast<CPortal*>(coliObject))
				{
					CPortal* p = dynamic_cast<CPortal*>(coliObject);
					CGame::GetInstance()->SwitchScene(p->GetSceneId());
				}
				/*else if (dynamic_cast<CLeopard*>(coliObject)) {
					this->SetHealth(this->health - 2);
					StartUntouchable();
				}*/
			}
			else {
				if (dynamic_cast<CItemBigHeart*>(coliObject)) {
					coliObject->SetVisible(false);
				}
				else if (dynamic_cast<CItemSmallHeart*>(coliObject)) {
					coliObject->SetVisible(false);
				}
				else if (dynamic_cast<CItemChain*>(coliObject)) {
					coliObject->SetVisible(false);
					this->SetState(SIMON_STATE_CHANGE_COLOR);
					this->animation_set->at(SIMON_ANI_CHANGE_COLOR)->SetAniStartTime(GetTickCount());
					whip->UpItemWhip();
				}
				else if (dynamic_cast<CItemDagger*>(coliObject)) {
					coliObject->SetVisible(false);
					this->SetWeapon(SUBWEAPON_DAGGER);
				}
				else if (dynamic_cast<CItemStopWatch*>(coliObject)) {
					coliObject->SetVisible(false);
					this->SetWeapon(SUBWEAPON_STOPWATCH);
				}
				else if (dynamic_cast<CStairBottom*>(coliObject)) {
					this->canGoUpStair = true;
					this->nxCanGoStair = coliObject->nx;
					this->simonGoStair = coliObject->simonXStair;
					this->canGoDownStair = false;
				}
				else if (dynamic_cast<CStairTop*>(coliObject)) {
					this->nxCanGoStair = coliObject->nx;
					this->simonGoStair = coliObject->simonXStair;
					this->canGoDownStair = true;
					this->canGoUpStair = false;
				}
				else if (dynamic_cast<CCandle*>(coliObject)) {

				}
				else if (dynamic_cast<CItemMoneyBagPurple*>(coliObject)) {
					coliObject->SetVisible(false);
					PointEffects::GetInstance()->SetPointEffect(POINT_EFFECT_1000);
				}
				else if (dynamic_cast<CItemMoneyBagYellow*>(coliObject)) {
					 y = y - 0.2f;
					coliObject->SetVisible(false);
					PointEffects::GetInstance()->SetPointEffect(POINT_EFFECT_700);
				}
				else if (dynamic_cast<CLeopard*>(coliObject)) {
					this->SetState(SIMON_STATE_HURT_DEATH);
					StartUntouchable();
				}
				else {
					this->canGoUpStair = false;
					this->canGoDownStair = false;
				}
			}
			
			/*else if (!dynamic_cast<CStairBottom*>(coliObject) && this->IsOnStair() == false) {

			}
			else if (!dynamic_cast<CStairTop*>(coliObject) && this->IsOnStair() == false) {
				this->canGoDownStair = false;
			}*/
		}
	}

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
			if (isOnStair == true) {
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
				if (dynamic_cast<CStairBottom*>(e->obj) ) {
					this->isOnStair = false;
					this->canGoDownStair = false;
					this->canGoUpStair = true;
					this->simonXStair = e->obj->simonXStair;
					this->nxCanGoStair = e->obj->nx;
					this->OnGroud = true;
					this->SetState(SIMON_STATE_IDLE);
				}
				else if (dynamic_cast<CStairTop*>(e->obj)) {
					this->isOnStair = false;
					this->OnGroud = true;
					this->canGoUpStair = false;
					this->canGoDownStair = true;
					this->SetState(SIMON_STATE_IDLE);
					this->simonGoStair = e->obj->simonXStair;
					this->nxCanGoStair = e->obj->nx;
				}
				else if (dynamic_cast<CPortal*>(e->obj))
				{
					CPortal* p = dynamic_cast<CPortal*>(e->obj);
					CGame::GetInstance()->SwitchScene(p->GetSceneId());
				}
				else if (dynamic_cast<CLeopard*>(e->obj)) {
					if (e->nx != 0) x += dx;
					if (e->ny != 0) y += dy;
					if (e->nx != 0 || e->ny != 0) y = y - 0.2f;
					if (e->nx == this->nx) this->nx = this->nx * -1;
					this->SetHealth(this->health - 2);
					StartUntouchable();
				}
			}
			else {
				if (dynamic_cast<CTorch*>(e->obj) || dynamic_cast<CCandle*>(e->obj) || dynamic_cast<CBrick4Leopard*>(e->obj)
					|| dynamic_cast<CWallPiece*>(e->obj) || dynamic_cast<PointEffect*>(e->obj))
				{
					if (e->nx != 0) x += dx;
					if (e->ny != 0) y += dy;
					if (e->nx != 0 || e->ny != 0) y = y - 0.2f;	
				}
				else if (dynamic_cast<CBoss*>(e->obj)) {
					if (e->nx != 0) x += dx;
					if (e->ny != 0) y += dy;
					health = health - 2;
					this->SetState(SIMON_STATE_HURT_DEATH);
					StartUntouchable();
				}
				else if (dynamic_cast<CBrick4MoneyRed*>(e->obj)) {
					if (e->nx != 0) x += dx;
					if (e->ny != 0) y += dy;
					CItems::GetInstance()->CheckAndDropMoney(e->obj);
				}
				else if (dynamic_cast<CItemMoneyBagRed*>(e->obj)) {
					e->obj->SetVisible(false);
					PointEffects::GetInstance()->SetPointEffect(POINT_EFFECT_1000);
					this->SetScore(1000);
				}
				else if (dynamic_cast<CItemMoneyBagPurple*>(e->obj)) {
					e->obj->SetVisible(false);
					PointEffects::GetInstance()->SetPointEffect(POINT_EFFECT_100);
					this->SetScore(100);
				}
				else if (dynamic_cast<CItemMoneyBagYellow*>(e->obj)) {
					e->obj->SetVisible(false);
					PointEffects::GetInstance()->SetPointEffect(POINT_EFFECT_700);
					this->SetScore(700);
				}
				else if (dynamic_cast<CItemBigHeart*>(e->obj)) {
					this->SetNumHeart(this->numHeart + 5);
					if (e->nx != 0 || e->ny != 0)
					{
						y = y - 0.2f;
						e->obj->SetVisible(false);
					}
				}
				else if (dynamic_cast<CItemSmallHeart*>(e->obj)) {
					this->SetNumHeart(this->numHeart + 1);
					if (e->nx != 0 || e->ny != 0)
					{
						y = y - 0.2f;
						e->obj->SetVisible(false);
					}
				}
				else if (dynamic_cast<CItemChain*>(e->obj)) {
					e->obj->SetVisible(false);
					this->SetState(SIMON_STATE_CHANGE_COLOR);
					whip->UpItemWhip();
				}
				else if (dynamic_cast<CItemDagger*>(e->obj)) {
					e->obj->SetVisible(false);
					this->SetWeapon(SUBWEAPON_DAGGER);
				}
				else if (dynamic_cast<CItemHolyWater*>(e->obj)) {
					e->obj->SetVisible(false);
					this->SetWeapon(SUBWEAPON_HOLYWATER);
				}
				else if (dynamic_cast<CItemCross*>(e->obj)) {
					e->obj->SetVisible(false);
					eatCross = true;
				}
				else if (dynamic_cast<CItemAxe*>(e->obj)) {
					e->obj->SetVisible(false);
					this->SetWeapon(SUBWEAPON_AXE);
				}
				else if (dynamic_cast<CPortal*>(e->obj))
				{
					CPortal* p = dynamic_cast<CPortal*>(e->obj);
					CGame::GetInstance()->SwitchScene(p->GetSceneId());
				}
				else if (dynamic_cast<CStairBottom*>(e->obj)) {
					this->canGoUpStair = true;
					this->nxCanGoStair = e->obj->nx;
					this->simonGoStair = e->obj->simonXStair;
					this->canGoDownStair = false;
					if (e->nx != 0) x += dx;
					if (e->ny != 0) y += dy;
					if (e->nx != 0 || e->ny != 0) y = y - 0.2f;
				}
				else if (dynamic_cast<CStairTop*>(e->obj)) {
					this->nxCanGoStair = e->obj->nx;
					this->simonGoStair = e->obj->simonXStair;
					this->canGoDownStair = true;
					this->canGoUpStair = false; 
					if (e->nx != 0) x += dx;
					if (e->ny != 0) y += dy;
					if (e->nx != 0 || e->ny != 0) y = y - 0.2f;
				}
				else if (dynamic_cast<CLeopard*>(e->obj)) {
					if (e->nx != 0) x += dx;
					if (e->ny != 0) y += dy;
					if (e->nx == this->nx) this->nx = this->nx * -1;
					//this->SetHealth(this->health - 2);
					this->SetState(SIMON_STATE_HURT_DEATH);
					StartUntouchable();
				}
				else if (dynamic_cast<CBrickHide*>(e->obj)) {
					if (e->nx != 0) x += dx;
					if (e->nx != 0 || e->ny != 0) y = y - 0.2f;
				}
				else {
					if (nx != 0) vx = 0;
					if (ny != 0) vy = 0;
					OnGroud = true;
				}
			}
			
		}
	}


	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

	if (state == SIMON_STATE_ATTACK || state == SIMON_STATE_ATTACK_SIT || state == SIMON_STATE_JUMP_ATTACK)
	{
		whip->SetOrientation(nx);
		whip->SetPositionWhip(D3DXVECTOR2(x, y), state != SIMON_STATE_ATTACK_SIT ? true : false);
		if (animation_set->at(SIMON_ANI_ATTACK)->GetCurrentFrame() == 2 ||
			animation_set->at(SIMON_ANI_ATTACK_SIT)->GetCurrentFrame() == 2 ) {
			whip->Update(dt, coObjects);
		}
	}
	if (state == SIMON_STATE_ATTACK_SUBWEAPON) {
		if (animation_set->at(SIMON_ANI_ATTACK)->GetCurrentFrame() == 2) {
			if (this->numHeart > 0) {
				CSubWeapons::GetInstance()->UseSubWeapon(currentWeapon);
			}
			
		}
	}

	
	//DebugOut(L" vx: %d\n", this->vx);
	/*DebugOut(L"canGoUpStair: %d\n", this->canGoUpStair);
	DebugOut(L"state: %d\n", this->GetState());
	DebugOut(L"canGoDownStair: %d\n", this->canGoDownStair);
	DebugOut(L"nxGoStair: %d\n", this->nxCanGoStair);*/
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
	else if (state == SIMON_STATE_GO_UPSTAIR) ani = SIMON_ANI_GO_UPSTAIR;
	else if (state == SIMON_STATE_IDLE_UPSTAIR) ani = SIMON_ANI_IDLE_UPSTAIR;
	else if (state == SIMON_STATE_GO_DOWNSTAIR) ani = SIMON_ANI_GO_DOWNSTAIR;
	else if (state == SIMON_STATE_IDLE_DOWNSTAIR) ani = SIMON_ANI_IDLE_DOWNSTAIR;
	else if (state == SIMON_STATE_HURT_DEATH) ani = SIMON_ANI_HURT_DEATH;
	else if (state == SIMON_STATE_ATTACK_DOWNSTAIR) ani = SIMON_ANI_ATTACK_DOWNSTAIR;
	else if (state == SIMON_STATE_ATTACK_UPSTAIR) ani = SIMON_ANI_ATTACK_UPSTAIR;
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
	{
		if (nx > 0) {
			vx = MARIO_WALKING_SPEED;
		}
		else {
			vx = -MARIO_WALKING_SPEED;
		}
		//isOnStair = false;
		break;
	}

	case SIMON_STATE_JUMP:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		OnGroud = false;
		//isOnStair = false;
		vy = -MARIO_JUMP_SPEED_Y;
		break;
	case SIMON_STATE_JUMP_ATTACK:
		OnGroud = false;
		//isOnStair = false;
		animation_set->at(SIMON_ANI_ATTACK)->Reset();
		animation_set->at(SIMON_ANI_ATTACK)->SetAniStartTime(GetTickCount());
		break;
	case SIMON_STATE_SIT:
		//isOnStair = false;
		vx = 0;
		break;
	case SIMON_STATE_ATTACK: {
		//isOnStair = false;
		animation_set->at(SIMON_ANI_ATTACK)->Reset();
		animation_set->at(SIMON_ANI_ATTACK)->SetAniStartTime(GetTickCount());
		break;
	}
	case SIMON_STATE_ATTACK_SUBWEAPON: {
		//isOnStair = false;
		animation_set->at(SIMON_ANI_ATTACK)->Reset();
		animation_set->at(SIMON_ANI_ATTACK)->SetAniStartTime(GetTickCount());
		break;
	}
	case SIMON_STATE_ATTACK_SIT: {
		//isOnStair = false;
		animation_set->at(SIMON_ANI_ATTACK_SIT)->Reset();
		animation_set->at(SIMON_ANI_ATTACK_SIT)->SetAniStartTime(GetTickCount());
		break;
	}
	case SIMON_STATE_IDLE:
		//isOnStair = false;
		vx = 0;
		break;
	case SIMON_STATE_CHANGE_COLOR:
		//isOnStair = false;
		vx = 0;
		animation_set->at(SIMON_ANI_CHANGE_COLOR)->Reset();
		animation_set->at(SIMON_ANI_CHANGE_COLOR)->SetAniStartTime(GetTickCount());
		break;
	case SIMON_STATE_IDLE_UPSTAIR:
	{
		nx = nxCanGoStair;
		isOnStair = true;
		vx = 0;
		vy = 0;
		break;
	}
	case SIMON_STATE_IDLE_DOWNSTAIR:
	{
		nx = nxCanGoStair;
		isOnStair = true;
		vx = 0;
		vy = 0;
		break;
	}
	case SIMON_STATE_GO_UPSTAIR:
	{
		nx = nxCanGoStair;
		this->isOnStair = true;
		if (nx > 0) {
			vx = 0.038f;
			vy = -0.038f;
		}
		else
		{
			//simon đi lên hướng trái
			vx = -0.038f;
			vy = -0.038f;
		}
		//vx = this->nx * 0.0058f;
		//vy = -0.006;
		break;
	}
	case SIMON_STATE_GO_DOWNSTAIR:
	{
		nx = nxCanGoStair;
		this->isOnStair = true;
		if (nx > 0) {
			vx = 0.038f;
			vy = +0.038f;
		}
		else
		{
			//simon đi lên hướng trái
			vx = -0.038f;
			vy = 0.038f;
		}
		break;
	}
	case SIMON_STATE_ATTACK_UPSTAIR:
	{
		animation_set->at(SIMON_ANI_ATTACK_UPSTAIR)->Reset();
		animation_set->at(SIMON_ANI_ATTACK_UPSTAIR)->SetAniStartTime(GetTickCount());
		vx = 0;
		vy = 0;
		this->isOnStair = true;

		break;
	}
	case SIMON_STATE_ATTACK_DOWNSTAIR:
	{
		animation_set->at(SIMON_ANI_ATTACK_DOWNSTAIR)->Reset();
		animation_set->at(SIMON_ANI_ATTACK_DOWNSTAIR)->SetAniStartTime(GetTickCount());
		vx = 0;
		vy = 0;
		this->isOnStair = true;

		break;
	}
	case SIMON_STATE_DIE:
		vy = -0.06f;
		vx = 0;
		break;
	
	case SIMON_STATE_HURT_DEATH: 
		animation_set->at(SIMON_ANI_HURT_DEATH)->Reset();
		animation_set->at(SIMON_ANI_HURT_DEATH)->SetAniStartTime(GetTickCount());
		if (untouchable == 1) {
			vy = -0.04f;
			vx = -this->nx * 0.06f;
		}
		
		break;
	}

}

void CSimon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
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

