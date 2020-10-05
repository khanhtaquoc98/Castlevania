
#include "GameObject.h"

#define CANDLE_ANI_SET	41

#define CANDLE_STATE			 403
#define CANDLE_STATE_DESTROYED 402

#define CANDLE_ANI	0
#define CANDLE_ANI_DESTROYED	1

#define TIME_TORCH_DESTROYED 360

#define HEIGHT_TORCH	14
#define WIDTH_TORCH	8

class CCandle : public CGameObject {
	static CCandle* __instance;

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject = NULL);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

public:
	CCandle();
	static CCandle* GetInstance();
};