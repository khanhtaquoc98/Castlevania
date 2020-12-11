
#include "GameObject.h"


#define DOOR_STATE_OPEN		433
#define DOOR_STATE_CLOSE	 432

#define DOOR_ANI_OPEN			0

#define TIME_DOOR_OPEN 200

#define HEIGHT_DOOR	48
#define WIDTH_DOOR	16

class CDoor : public CGameObject {
	static CDoor* __instance;
	
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

public:
	CDoor();
	static CDoor* GetInstance();
};

