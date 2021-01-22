#include "StopWatch.h"

#include "Game.h"
#include "Torch.h"
#include "Items.h"
#include "Candle.h"
#include "Leopard.h"
#include "Boss.h"

CStopWatch::CStopWatch() :CGameObject()
{
}

void CStopWatch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
}


void CStopWatch::SetState(int state)
{
	this->state = state;
}

void CStopWatch::Render()
{
	animation_set->at(0)->Render(x, y, nx);
	RenderBoundingBox();
}


void CStopWatch::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = x;
	top = y;
	right = left + WIDTH_DAGGER;
	bottom = top + HEIGHT_DAGGER;
}


CStopWatch* CStopWatch::__instance = NULL;

CStopWatch* CStopWatch::GetInstance()
{
	if (__instance == NULL) __instance = new CStopWatch();
	return __instance;
}
