#pragma once
#include "GameObject.h"
#include "Whip.h"


#define MARIO_WALKING_SPEED		0.058f 
//0.1f
#define MARIO_JUMP_SPEED_Y		0.2f
#define MARIO_JUMP_DEFLECT_SPEED 0.188f
#define MARIO_GRAVITY			0.0006f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f

#define SIMON_STATE_IDLE			0	
#define SIMON_STATE_WALKING			100
#define SIMON_STATE_JUMP			200
#define SIMON_STATE_JUMP_ATTACK		201
#define SIMON_STATE_SIT			300
#define SIMON_STATE_ATTACK		500
#define SIMON_STATE_ATTACK_SUBWEAPON	501
#define	SIMON_STATE_ATTACK_SIT	600
#define SIMON_STATE_DIE				400
#define SIMON_STATE_CHANGE_COLOR	700
#define SIMON_STATE_IDLE_UPSTAIR	800
#define SIMON_STATE_IDLE_DOWNSTAIR	900
#define SIMON_STATE_GO_UPSTAIR		1000
#define SIMON_STATE_GO_DOWNSTAIR	1100
#define	SIMON_STATE_ATTACK_UPSTAIR	1200
#define SIMON_STATE_ATTACK_DOWNSTAIR	1300
#define SIMON_STATE_HURT_DEATH	1400

#define SIMON_ANI_IDLE				0
#define SIMON_ANI_WALKING			1
#define SIMON_ANI_JUMP				2
#define SIMON_ANI_DUCK				3
#define SIMON_ANI_ATTACK			4
#define SIMON_ANI_ATTACK_SIT		5
#define SIMON_ANI_DIE				6
#define SIMON_ANI_CHANGE_COLOR		7
#define SIMON_ANI_IDLE_UPSTAIR		10
#define SIMON_ANI_IDLE_DOWNSTAIR	11
#define SIMON_ANI_GO_UPSTAIR		8
#define SIMON_ANI_GO_DOWNSTAIR		9
#define SIMON_ANI_ATTACK_UPSTAIR	12
#define SIMON_ANI_ATTACK_DOWNSTAIR	13
#define SIMON_ANI_HURT_DEATH	14

#define SIMON_TIME_ATTACK	350
#define SIMON_TIME_CHANGE_COLOR	1000

#define SIMON_BBOX_WIDTH  16
#define SIMON_BBOX_HEIGHT 30

#define SUBWEAPON_DAGGER 41
#define SUBWEAPON_HOLYWATER	72
#define SUBWEAPON_AXE	73

#define MARIO_UNTOUCHABLE_TIME 800


class CSimon : public CGameObject
{
	int untouchable;
	DWORD untouchable_start;
	static CSimon* __instance;

	float start_x;			// initial position of Mario at scene
	float start_y; 
	int currentWeapon = 0;
	CWhip* whip;
	bool OnGroud;

	int directionStair;
	bool isOnStair;
	
	int numLife = 4;
	int numHeart = 20;
	int health = 2;
	int score;
public: 
	bool canGoUpStair;
	bool canGoDownStair;
	float simonGoStair;
	
	

	bool IsOnGroud() { return OnGroud; }

	bool IsOnStair() { return isOnStair; }
	void SetOnStair(bool isOnStair) {  this->isOnStair = isOnStair; }
	int nxCanGoStair;

	static CSimon* GetInstance();
	CSimon(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	int GetWeapon() { return currentWeapon; }
	int GetHealth() { return health; }
	int GetNumLife() { return numLife; }
	int GetNumHeart() { return numHeart; }
	int GetScore() { return score; }

	void SetHealth(int health) { this->health = health; }
	void SetNumLife(int numLife) { this->numLife = numLife; }
	void SetNumHeart(int numHeart) { this->numHeart = numHeart; }
	void SetWeapon(int CurrentWeapon) { this->currentWeapon = CurrentWeapon; }
	void SetScore(int score) { this->score += score; }

		
};