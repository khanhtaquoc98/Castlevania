#pragma once
#include "GameObject.h"

#define ZOMBIE_ANI_SET	47

#define ZOMBIE_ANI	0
#define ZOMBIE_ANI_DESTROYED	1

#define ZOMBIE_STATE	471
#define ZOMBIE_STATE_DESTROYED	402

#define TIME_TORCH_DESTROYED 360

#define ZOMBIE_BBOX_WIDTH	16
#define ZOMBIE_BBOX_HEIGHT	32

#define ZOMBIE_GRAVITY	0.0006f

#define ZOMBIE_SPEED	0.04f

class CZombie : public CGameObject
{
	static CZombie* __instance;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

public:
	CZombie();
	static CZombie* GetInstance();
};

