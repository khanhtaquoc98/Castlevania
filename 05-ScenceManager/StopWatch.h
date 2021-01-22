#pragma once
#include "GameObject.h"

#define WIDTH_DAGGER	16
#define HEIGHT_DAGGER	9

#define DAGGER_SPEED				0.1f

class CStopWatch : public CGameObject {
	static CStopWatch* __instance;
	virtual void SetState(int state);

public:
	CStopWatch();
	//virtual void RenderbyFrame(int currentFrame);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();

	static CStopWatch* GetInstance();
};