#include "GameObject.h"

#define ITEM_DAGGER_BBOX_WIDTH	16
#define ITEM_DAGGER_BBOX_HEIGHT	9

#define TIME_VISIBLE_ITEM_DAGGER 2000

class CItemDagger :public CGameObject
{
public:
	CItemDagger();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};