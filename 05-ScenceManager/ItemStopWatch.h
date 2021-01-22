#pragma once
#pragma once
#include "GameObject.h"

#define ITEM_AXE_WIDTH	15
#define ITEM_AXE_HEIGHT	16

#define TIME_VISIBLE_ITEM_BIG_HEART 2000

class CItemStopWatch :public CGameObject
{
public:
	CItemStopWatch();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};