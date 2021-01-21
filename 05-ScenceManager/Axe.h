#pragma once
#include "GameObject.h"

#define AXE_NORMAL_BBOX_WIDTH	 15
#define AXE_NORMAL_BBOX_HEIGHT	 14

#define STATE_AXE_NORMAL 730

#define ANI_AXE_NORMAL 0

#define HOLY_SPEED	0.05f

class CAxe : public CGameObject {
	static CAxe* __instance;
	virtual void SetState(int state);

	int xFirst = -1;
public:
	CAxe();
	//virtual void RenderbyFrame(int currentFrame);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

	static CAxe* GetInstance();
};