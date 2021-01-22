#pragma once
#include "GameObject.h"


#define BAT_ANI_SET	48

#define BAT_ANI	0
#define BAT_ANI_DESTROYED	1

#define BAT_BBOX_WIDTH	16
#define BAT_BBOX_HEIGHT	16

#define BAT_STATE	481
#define BAT_STATE_DESTROYED	402


#define BAT_SPEED	0.04f


class CBat : public CGameObject
{
	static CBat* __instance;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

public:
	CBat();
	static CBat* GetInstance();
};

