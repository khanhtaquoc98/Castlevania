#include "Whip.h"
#include "Simon.h"

CWhip::CWhip():CGameObject()
{
	SetState(NORMAL_WHIP);
}

void CWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CWhip::RenderbyFrame(int currentFrame)
{
	CAnimationSets::GetInstance()->Get(WHIP_ANI_SET)->at(state)->RenderByFrame(currentFrame, nx, x, y,255);
	RenderBoundingBox();
}

void CWhip::SetState(int state)
{
	this->state = state;
}

void CWhip::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	top = y + 4;
	bottom = top + 8;
	if (nx > 0) left = x + 70;
	else
	{
		left = x + 26;
	}
	right = left + 24;
}

void CWhip::SetPositionWhip(D3DXVECTOR2 simonPosition, bool isStanding)
{
	if (nx > 0)
	{
		simonPosition.x -= 42.0f;
		if (isStanding) simonPosition.y -= 4.0f;
		simonPosition.y += 8.0f;
	}
	else
	{
		simonPosition.x -= 46.0f;
		if (isStanding) simonPosition.y += 4.0f;
		else simonPosition.y += 8.0f;
	}

	SetPosition(simonPosition.x, simonPosition.y);
}


CWhip* CWhip::__instance = NULL;
CWhip* CWhip::GetInstance()
{
	if (__instance == NULL) __instance = new CWhip();
	return __instance;
}
