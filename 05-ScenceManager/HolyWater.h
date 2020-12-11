#pragma once
#include "GameObject.h"

#define HOLY_WATER_NORMAL_BBOX_WIDTH	 8
#define HOLY_WATER_NORMAL_BBOX_HEIGHT	 8

#define HOLY_WATER_BURN_BBOX_WIDTH	16
#define HOLY_WATER_BURN_BBOX_HEIGHT	16

#define STATE_HOLYWATER_NORMAL 720
#define STATE_HOLYWATER_BURN 721

#define ANI_HOLYWATER_NORMAL 0
#define ANI_HOLYWATER_BURN 1

#define HOLY_SPEED	0.05f

class CHolyWater : public CGameObject {
	static CHolyWater* __instance;
	virtual void SetState(int state);

	int xFirst = -1;
	DWORD timeStartBurn;
	bool Burning = false;
public:
	CHolyWater();
	//virtual void RenderbyFrame(int currentFrame);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

	static CHolyWater* GetInstance();
};