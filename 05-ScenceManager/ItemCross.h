#pragma once
#pragma once
#include "GameObject.h"

#define ITEM_CROSS_BBOX_WIDTH	16
#define ITEM_CROSS_BBOX_HEIGHT	16

#define TIME_VISIBLE_ITEM_BIG_HEART 2000

class CItemCross :public CGameObject
{
public:
	CItemCross();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};