#pragma once
#pragma once
#include "GameObject.h"

#define ITEM_HOLYWATER_WIDTH	16
#define ITEM_HOLYWATER_HEIGHT	16

#define TIME_VISIBLE_ITEM_BIG_HEART 2000

class CItemHolyWater :public CGameObject
{
public:
	CItemHolyWater();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};