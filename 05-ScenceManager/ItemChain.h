#include "GameObject.h"

#define ITEM_CHAIN_BBOX_WIDTH	16
#define ITEM_CHAIN_BBOX_HEIGHT	16

#define TIME_VISIBLE_ITEM_CHAIN 20000

class CItemChain :public CGameObject
{
public:
	CItemChain();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};