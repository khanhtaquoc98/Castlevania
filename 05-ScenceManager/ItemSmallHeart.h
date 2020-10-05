
#include "GameObject.h"

#define ITEM_SMALL_HEART_BBOX_WIDTH		8
#define ITEM_SMALL_HEART_BBOX_HEIGHT	14

#define TIME_VISIBLE_ITEM_BIG_HEART 2000

class CItemSmallHeart :public CGameObject
{
	float vx_variability;
public:
	CItemSmallHeart();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};