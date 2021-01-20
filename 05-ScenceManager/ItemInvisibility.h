#include "GameObject.h"

#define ITEM_INVISIBILITY_BBOX_WIDTH	13
#define ITEM_INVISIBILITY_BBOX_HEIGHT	16

#define TIME_VISIBLE_ITEM_DAGGER 2000

class CItemInvisibility :public CGameObject
{
public:
	CItemInvisibility();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};