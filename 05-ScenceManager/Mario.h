#pragma once
#include "GameObject.h"

#define MARIO_WALKING_SPEED		0.15f 
//0.1f
#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.002f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f

#define SIMON_STATE_IDLE			0	
#define SIMON_STATE_WALKING			100
#define SIMON_STATE_JUMP			200
#define SIMON_STATE_DUCK			300
#define SIMON_STATE_STANDING		500
#define SIMON_STATE_DIE				400


#define SIMON_ANI_IDLE				0
#define SIMON_ANI_WALKING			1
#define SIMON_ANI_JUMP				2
#define SIMON_ANI_DUCK				3
#define SIMON_ANI_STANDING			4
#define SIMON_ANI_DIE				8


#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define SIMON_TIME_STANDING	300

#define SIMON_BBOX_WIDTH  15
#define SIMON_BBOX_HEIGHT 30

#define MARIO_UNTOUCHABLE_TIME 5000


class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;

	float start_x;			// initial position of Mario at scene
	float start_y; 
public: 
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	void Reset();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};