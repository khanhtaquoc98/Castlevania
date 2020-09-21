#pragma once
#include "GameObject.h"

#define TORCH_ANI_SET	41

#define TORCH_STATE			 401
#define TORCH_STATE_DESTROYED 402

#define TORCH_ANI	0
#define TORCH_ANI_DESTROYED	1

#define TIME_TORCH_DESTROYED 360

#define HEIGHT_TORCH	32
#define WIDTH_TORCH	16

class CTorch : public CGameObject {
	static CTorch* __instance;
	
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

public:
	CTorch();
	static CTorch* GetInstance();
};