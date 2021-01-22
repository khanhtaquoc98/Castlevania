#include "Whip.h"
#include "Simon.h"
#include "Torch.h"
#include "Candle.h"
#include "Items.h"
#include "Leopard.h"
#include "BrickHide.h"
#include "WallPieces.h"
#include "Zombie.h"
#include "Bat.h"
#include "Fishman.h"


CWhip::CWhip():CGameObject()
{
	SetState(NORMAL_WHIP);
}

void CWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT coliObject = coObjects->at(i);

		if (this->CheckCollision(coliObject)) {
			if (dynamic_cast<CTorch*>(coliObject)) {
				coliObject->SetState(TORCH_STATE_DESTROYED);
				coliObject->animation_set->at(TORCH_ANI_DESTROYED)->SetAniStartTime(GetTickCount());
			}
			else if (dynamic_cast<CCandle*>(coliObject)) {
				coliObject->SetState(CANDLE_STATE_DESTROYED);
				coliObject->animation_set->at(CANDLE_ANI_DESTROYED)->SetAniStartTime(GetTickCount());
			}
			else if (dynamic_cast<CLeopard*>(coliObject)) {
				coliObject->SetState(CANDLE_STATE_DESTROYED);
				coliObject->animation_set->at(CANDLE_ANI_DESTROYED)->SetAniStartTime(GetTickCount());
			}
			else if (dynamic_cast<CBrickHide*>(coliObject)) {
				coliObject->SetVisible(false);
				CWallPieces::GetInstance()->DropPiece(coliObject->x, coliObject->y);
				CItems::GetInstance()->CheckAndDrop(coliObject);
			}
			else if (dynamic_cast<CZombie*>(coliObject)) {
				coObjects->at(i)->SetState(ZOMBIE_STATE_DESTROYED);
				coObjects->at(i)->animation_set->at(ZOMBIE_ANI_DESTROYED)->SetAniStartTime(GetTickCount());
			}
			else if (dynamic_cast<CBat*>(coliObject)) {
				coObjects->at(i)->SetState(BAT_STATE_DESTROYED);
				coObjects->at(i)->animation_set->at(BAT_ANI_DESTROYED)->SetAniStartTime(GetTickCount());
			}
			else if (dynamic_cast<CFishman*>(coliObject)) {
				coObjects->at(i)->SetState(FISHMAN_STATE_DESTROYED);
				coObjects->at(i)->animation_set->at(FISHMAN_ANI_DESTROYED)->SetAniStartTime(GetTickCount());
			}
		}
	}

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);


	//if (coEvents.size() == 0)
	//{
	//	x += dx;
	//	y += dy;
	//}
	//else
	//{
	//	float min_tx, min_ty, nx = 0, ny;
	//	float rdx = 0;
	//	float rdy = 0;

	//	// TODO: This is a very ugly designed function!!!!
	//	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

	//	// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
	//	//if (rdx != 0 && rdx!=dx)
	//	//	x += nx*abs(rdx); 

	//	// block every object first!
	//	x += min_tx * dx + nx * 0.2f;
	//	y += min_ty * dy + ny * 0.2f;

	//	//
	//	// Collision logic with other objects
	//	//


	//	for (UINT i = 0; i < coEventsResult.size(); i++)
	//	{
	//		LPCOLLISIONEVENT e = coEventsResult[i];

	//		if (dynamic_cast<CTorch*>(e->obj))
	//		{
	//			e->obj->SetState(TORCH_STATE_DESTROYED);
	//			e->obj->animation_set->at(TORCH_ANI_DESTROYED)->SetAniStartTime(GetTickCount());
	//		}
	//		else if (dynamic_cast<CCandle*>(e->obj)) {
	//			e->obj->SetState(CANDLE_STATE_DESTROYED);
	//			e->obj->animation_set->at(CANDLE_ANI_DESTROYED)->SetAniStartTime(GetTickCount());
	//		}
	//		else if (dynamic_cast<CLeopard*>(e->obj)) {
	//			e->obj->SetState(CANDLE_STATE_DESTROYED);
	//			e->obj->animation_set->at(CANDLE_ANI_DESTROYED)->SetAniStartTime(GetTickCount());
	//		}
	//		/*else if (dynamic_cast<CBrickHide*>(e->obj)) {
	//			e->obj->SetVisible(false);
	//			CWallPieces::GetInstance()->DropPiece(e->obj->x, e->obj->y);
	//			CItems::GetInstance()->CheckAndDrop(e->obj);
	//		}*/

	//	}
	//}
}

void CWhip::RenderbyFrame(int currentFrame)
{
	int StateWhip = CWhip::GetInstance()->GetState();
	CAnimationSets::GetInstance()->Get(WHIP_ANI_SET)->at(StateWhip)->RenderByFrame(currentFrame, nx, x, y,255);

	if (currentFrame == 2) {
		RenderBoundingBox();
	}
	
}

void CWhip::SetState(int state)
{
	this->state = state;
}



void CWhip::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	float xSimon, ySimon;
	CSimon::GetInstance()->GetPosition(xSimon, ySimon);
	int state = CSimon::GetInstance()->GetState();
	switch (CWhip::GetInstance()->GetState()){
	case NORMAL_WHIP:
	{
		if (state == SIMON_STATE_SIT || state == SIMON_STATE_ATTACK_SIT || state == SIMON_STATE_JUMP) top = ySimon + 16;
		else top = ySimon + 6;
		if (nx > 0) left = xSimon + 28;
		else left = xSimon - 20;
		right = left + 24; //width normal whip
		bottom = top + 8;  // height normal whip
		break;
	}
	case SHORT_CHAIN: {
		if (state == SIMON_STATE_SIT || state == SIMON_STATE_ATTACK_SIT || state == SIMON_STATE_JUMP) top = ySimon + 16;
		else top = ySimon + 7;
		if (nx > 0) left = xSimon + 29;
		else left = xSimon - 19;
		right = left + 23; 
		bottom = top + 6;
		break;
	} 
	case LONG_CHAIN: {
		if (state == SIMON_STATE_SIT || state == SIMON_STATE_ATTACK_SIT || state == SIMON_STATE_JUMP) top = ySimon + 16;
		else top = ySimon + 7;
		if (nx > 0) left = xSimon + 27;
		else left = xSimon - 33;
		right = left + 38;
		bottom = top + 6;
		break;
	}
	}
}

void CWhip::SetPositionWhip(D3DXVECTOR2 simonPosition, bool isStanding)
{
	if (nx > 0)
	{
		simonPosition.x -= 42.0f;
		if (isStanding) simonPosition.y += 3.6f;
		else simonPosition.y += 12.0f;
	}
	else
	{
		simonPosition.x -= 48.0f;
		if (isStanding) simonPosition.y += 3.6f;
		else simonPosition.y += 12.0f;
	}

	this->SetPosition(simonPosition.x, simonPosition.y);
}

void CWhip::UpItemWhip()
{
	switch (CWhip::GetInstance()->GetState())
	{
	case NORMAL_WHIP:
		CWhip::GetInstance()->SetState(SHORT_CHAIN);
		break;
	case SHORT_CHAIN:
		CWhip::GetInstance()->SetState(LONG_CHAIN);
		break;
	default:
		CWhip::GetInstance()->SetState(NORMAL_WHIP);
		break;
	}
}


CWhip* CWhip::__instance = NULL;
CWhip* CWhip::GetInstance()
{
	if (__instance == NULL) __instance = new CWhip();
	return __instance;
}
