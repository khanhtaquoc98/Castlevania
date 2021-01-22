#pragma once
#include "GameObject.h"
#include "Simon.h"

#define FISHMAN_ANI_WALKING	0
#define FISHMAN_ANI_HIT		1
#define FISHMAN_ANI_DESTROYED 2

#define FISHMAN_STATE_WALKING	491
#define FISHMAN_STATE_DESTROYED	402
#define FISHMAN_STATE_HIT		492

#define FISHMAN_BBOX_WIDTH	16
#define FISHMAN_BBOX_HEIGHT	32

#define FISHMAN_SPEED		0.04f
#define FISHMAN_GRAVITY		-0.0006f
#define TIME_TORCH_DESTROYED 360


class CFishman : public CGameObject
{
	static CFishman* __instance;
	virtual void SetState(int state);

	bool isRun = false;
	bool isJump = false;

public:
	CFishman();
	//virtual void RenderbyFrame(int currentFrame);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

	static CFishman* GetInstance();
};

