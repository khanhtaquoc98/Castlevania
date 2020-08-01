#pragma once
#include "GameObject.h"

class CWhip : public CGameObject {
	static CWhip* __instance;

public:
	CWhip();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render(int currentFrame);
	virtual void SetState(int state);
	//virtual void GetBdingBox(float& left, float& top, float& right, float& bottom);
};