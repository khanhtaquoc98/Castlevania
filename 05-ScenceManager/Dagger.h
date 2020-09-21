#pragma once
#include "GameObject.h"

#define WIDTH_DAGGER	16
#define HEIGHT_DAGGER	9

#define DAGGER_SPEED				0.1f

class CDagger : public CGameObject {
	static CDagger* __instance;
	virtual void SetState(int state);

public:
	CDagger();
	//virtual void RenderbyFrame(int currentFrame);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	
	static CDagger* GetInstance();
};