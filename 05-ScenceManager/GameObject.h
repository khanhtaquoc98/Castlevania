#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Sprites.h"
#include "Animations.h"


using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

class CGameObject; 
typedef CGameObject * LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent * LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;
	
	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL) 
	{ 
		this->t = t; 
		this->nx = nx; 
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj; 
	}

	static bool compare(const LPCOLLISIONEVENT &a, LPCOLLISIONEVENT &b)
	{
		return a->t < b->t;
	}
};


class CGameObject
{
public:
	float x; 
	float y;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	int width;
	int height;

	float vx;
	float vy;

	float yCheck; //ycheckMoneyRed
	float simonXStair;

	int itemID;

	int nx;	 //right = 1; left = -1

	bool visible; //true == visible
	DWORD tStartVisible;

	int state;

	DWORD dt; 

	LPANIMATION_SET animation_set;

public: 
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void GetPosition(float& x, float& y) { x = this->x; y = this->y; }

	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }

	void SetWidth(int w) { this->width = w; }
	void GetWidth(int& w) { w = this->width; }

	void SetHeight(int h) { this->height = h; }
	void GetHeight(int& h) { h = this->height; }

	void SetyCheck(int ycheck) { this->yCheck = ycheck; }
	void GetyCheck(int& ycheck) { ycheck = this->yCheck; }

	void SetOrientation(int nx) { this->nx = nx; }
	int GetOrientation() { return nx; }
	void ReDirection() { this->nx = -nx; this->vx = -vx; }

	void SetVisible(bool visible) { this->visible = visible; }
	bool isVisible() { return visible; }

	void SetItem(int itemID) { this->itemID = itemID; }
	int GetItem() { return itemID; }

	int GetState() { return this->state; }

	void RenderBoundingBox();

	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }

	bool CheckCollision(LPGAMEOBJECT obj);

	bool CheckStair(LPGAMEOBJECT obj);

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents, 
		vector<LPCOLLISIONEVENT> &coEventsResult, 
		float &min_tx, 
		float &min_ty, 
		float &nx, 
		float &ny, 
		float &rdx, 
		float &rdy);

	CGameObject();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }


	~CGameObject();
};

