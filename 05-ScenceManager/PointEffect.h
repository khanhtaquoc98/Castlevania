#pragma once
#include "GameObject.h"

#define POINT_EFFECT_100	0
#define POINT_EFFECT_400	1
#define POINT_EFFECT_700	2
#define POINT_EFFECT_1000	3
#define POINT_EFFECT_2000	4

#define POINT_EFFECT_LIFE_SPAN 300

class PointEffect : public CGameObject
{
public:
	DWORD start_visible = 0;
	PointEffect();
	virtual void SetState(int state);
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

class PointEffects
{
public:
	unordered_map<int, vector<LPGAMEOBJECT>> points;
	static PointEffects* __instance;
	static PointEffects* GetInstance();
	void AddPointEffect(int TypePoint, LPGAMEOBJECT objPoint);
	void SetPointEffect(int TypePoint);
	PointEffects() { };
};