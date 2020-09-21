#pragma once
#include "GameObject.h"

#define WHIP_ANI_SET	2

#define NORMAL_WHIP		0
#define SHORT_CHAIN		1
#define LONG_CHAIN		2

class CWhip : public CGameObject {
	static CWhip* __instance;
	virtual void Render() {}
	virtual void SetState(int state);
	
public:
	CWhip();	
	virtual void RenderbyFrame(int currentFrame);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void SetPositionWhip(D3DXVECTOR2 simonPosition, bool isStanding);
	void UpItemWhip();
	static CWhip* GetInstance();
};