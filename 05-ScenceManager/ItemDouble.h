#pragma once
#pragma once
#include "GameObject.h"

#define ITEM_DOUBLE_WIDTH	14
#define ITEM_DOUBLE_HEIGHT	14

#define TIME_VISIBLE_ITEM_BIG_HEART 2000

class CItemDouble :public CGameObject
{
public:
	CItemDouble();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};