#pragma once
#include "GameObject.h"

#define WIDTH_LEOPARD	30
#define HEIGHT_LEOPARD	16

#define LEOPARD_STATE_SIT	461
#define LEOPARD_STATE_RUN	462
#define LEOPARD_STATE_JUMP	465
#define LEOPARD_STATE_DEAD	466

#define LEOPARD_ANI_SIT		0
#define LEOPARD_ANI_RUN		1
#define LEOPARD_ANI_JUMP	2
#define LEOPARD_ANI_DEAD	3

#define LEOPARD_DEFAULT_SPACE 80.0f // khoảng cách Leopard start chạy bên trái

#define LEOPARD_SPEED_RUN 0.16f
#define LEOPARD_GRAVITY 0.0006f
#define LEOPARD_GRAVITY_JUMPING 0.12f

class CLeopard : public CGameObject {
	static CLeopard* __instance;
	

	bool isRun = false;
	bool isJump = false;

public:
	CLeopard();
	//virtual void RenderbyFrame(int currentFrame);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void SetState(int state);
	static CLeopard* GetInstance();
};