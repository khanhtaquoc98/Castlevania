#include "PointEffect.h"
#include "Simon.h"

PointEffect::PointEffect()
{
	SetVisible(false);
}

void PointEffect::SetState(int state)
{
	this->state = state;
}

void PointEffect::Render()
{
	animation_set->at(state)->Render(x, y, -1);
}

void PointEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	CGameObject::Update(dt);
	if (visible)
	{
		if ((GetTickCount() - tStartVisible) > 500)
		{
			SetVisible(false);
			tStartVisible = 0;
		}
	}
}

void PointEffect::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + 14;
	b = y + 8;
}


//================ POINTS ===================================

PointEffects* PointEffects::__instance = NULL;

PointEffects* PointEffects::GetInstance()
{
	if (__instance == NULL)
		__instance = new PointEffects();

	return __instance;
}

void PointEffects::AddPointEffect(int TypePoint, LPGAMEOBJECT objPoint)
{
	points[TypePoint].push_back(objPoint);
}

void PointEffects::SetPointEffect(int TypePoint)
{
	for (auto i = points[TypePoint].begin(); i != points[TypePoint].end(); ++i)
	{
		if ((*i)->isVisible() == false) {
			float x, y;
			CSimon::GetInstance()->GetPosition(x, y);
			if (CSimon::GetInstance()->nx == 1) {
				(*i)->SetPosition(x + 32, y + 15);
			} else (*i)->SetPosition(x + 16, y + 15);
			(*i)->SetState(TypePoint);
			(*i)->SetVisible(true);
			(*i)->tStartVisible = GetTickCount();
			break;
		}
	}
}
