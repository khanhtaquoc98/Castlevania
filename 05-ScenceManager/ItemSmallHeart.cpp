#include "ItemSmallHeart.h"
#include "Brick.h"
#include "Simon.h"

CItemSmallHeart::CItemSmallHeart()
{
	this->visible = false;
	this->vx_variability = 0.0008f;
}

void CItemSmallHeart::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	//vy += ITEM_GRAVITY * dt;				// simple fall down	

	vy = 0.04;

	// Horizontal movement
	vx += vx_variability * dt;
	if (abs(vx) > 0.08f && vx * vx_variability > 0) vx_variability = -vx_variability;

	if (this->isVisible() == true && this->tStartVisible != 0) {
		if (GetTickCount() - this->tStartVisible > 60000)
			this->SetVisible(false);
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx, rdy;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		for (UINT i = 0; i < coEventsResult.size(); ++i)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
				vy = vx = 0;
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}


void CItemSmallHeart::Render()
{
	animation_set->at(0)->Render(x, y, 1);
}


void CItemSmallHeart::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + ITEM_SMALL_HEART_BBOX_WIDTH;
	bottom = y + ITEM_SMALL_HEART_BBOX_HEIGHT;
}
