#pragma once
#pragma once
#include "GameObject.h"

#define ITEM_BIG_HEART_BBOX_WIDTH		10
#define ITEM_BIG_HEART_BBOX_HEIGHT	10

#define TIME_VISIBLE_ITEM_BIG_HEART 2000

class CItemBigHeart :public CGameObject
{
public:
	CItemBigHeart();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};