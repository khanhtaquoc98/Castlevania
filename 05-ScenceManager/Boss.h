#pragma once
#pragma once
#pragma once
#include "GameObject.h"
#include "Simon.h"

#define WIDTH_BOSS	48
#define HEIGHT_BOSS	23
#define DISTANCE_CAN_ATTACK 70
#define BOSS_SLEEP_ANI	0
#define BOSS_FLY_ANI	1
#define BOSS_DEAD_ANI	2
#define STATE_SLEEP	510
#define FLY_AFTER_SLEEP 511
#define ATTACK 512
#define FLY_AFTER_ATTACK 513
#define FLY_TO_THE_MIDDLE 514
#define AIM 515
#define DEAD	516



class CBoss : public CGameObject {

	static CBoss* __instance;
	//virtual void SetState(int state);

	int health = 2;

protected:
	CSimon* mSimon;
	int ani;
	DWORD timerFlyUp = 0;
	DWORD timerAim = 0;
	DWORD timerAttack = 0;
	bool isAttack;
	bool isHitSimon;
	bool isAtBottom;

public:
	CBoss();
	CBoss(CSimon* simon);
	~CBoss();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();

	RECT GetBound();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	bool isDead;
	bool isSleep;

	static CBoss* GetInstance();

	int GetHealth() { return health; }
	void SetHealth(int health) { this->health = health; }
	//virtual void SetState(int state);

};