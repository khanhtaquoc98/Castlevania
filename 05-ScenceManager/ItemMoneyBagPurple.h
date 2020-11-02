#include "GameObject.h"

#define ITEM_BAG_WIDTH	15
#define ITEM_BAG_HEIGHT	15

#define TIME_VISIBLE_ITEM_MONEY_PURPLE 3000

class CItemMoneyBagPurple :public CGameObject
{
public:
	CItemMoneyBagPurple();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};