#include "Whip.h"
#include "Simon.h"
#include "Torch.h"
#include "Items.h"


CWhip::CWhip():CGameObject()
{
	SetState(NORMAL_WHIP);
}

void CWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	for (UINT i = 0; i < coObjects->size(); i++) 
	{	
		LPGAMEOBJECT coliObject = coObjects->at(i);

		if (this->CheckCollision(coliObject)) {
			if (dynamic_cast<CTorch*>(coliObject)) {
				coObjects->at(i)->SetState(TORCH_STATE_DESTROYED);
				coObjects->at(i)->animation_set->at(TORCH_ANI_DESTROYED)->SetAniStartTime(GetTickCount());
			}
			
		}
	}
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
	switch (CWhip::GetInstance()->GetState()){
	case NORMAL_WHIP:
	{
		top = ySimon + 6;
		if (nx > 0) left = xSimon + 28;
		else left = xSimon - 20;
		right = left + 24; //width normal whip
		bottom = top + 8;  // height normal whip
		break;
	}
	case SHORT_CHAIN: {
		top = ySimon + 7;
		if (nx > 0) left = xSimon + 29;
		else left = xSimon - 19;
		right = left + 23; 
		bottom = top + 6;
		break;
	} 
	case LONG_CHAIN: {
		top = ySimon + 7;
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
		if (isStanding) simonPosition.y -= 4.0f;
		simonPosition.y += 9.0f;
	}
	else
	{
		simonPosition.x -= 46.0f;
		if (isStanding) simonPosition.y += 4.0f;
		else simonPosition.y += 9.0f;
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
