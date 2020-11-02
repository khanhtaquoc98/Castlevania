#include "GameObject.h"
#define ITEM_BAG_WIDTH	15
#define ITEM_BAG_HEIGHT	15

#define TIME_VISIBLE_ITEM_BIG_HEART 2000

class CItemMoneyBagYellow :public CGameObject
{
public:
	CItemMoneyBagYellow();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};