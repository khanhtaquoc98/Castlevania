
#include "GameObject.h"


class CBrickHide : public CGameObject {
	static CBrickHide* __instance;

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

public:
	CBrickHide();
	static CBrickHide* GetInstance();
};