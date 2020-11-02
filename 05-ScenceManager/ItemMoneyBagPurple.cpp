#include "ItemMoneyBagPurple.h"
#include "Brick.h"
#include "Items.h"


CItemMoneyBagPurple::CItemMoneyBagPurple()
{
	this->visible = false;
}

void CItemMoneyBagPurple::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt);
	vy += ITEM_GRAVITY * dt;// simple fall down

	if (this->isVisible() == true && this->tStartVisible != 0) {
		if (GetTickCount() - this->tStartVisible > TIME_VISIBLE_ITEM_MONEY_PURPLE)
			this->SetVisible(false);
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		y += dy;
		x += dx;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx, rdy;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx;
		y += min_ty * dy;

		for (UINT i = 0; i < coEventsResult.size(); ++i)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
				// Block brick
				if (e->ny != 0)
				{
					y += 0.4f * e->ny;
					vy = 0;
				}
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CItemMoneyBagPurple::Render()
{
	animation_set->at(0)->Render(x, y, 1);
}

void CItemMoneyBagPurple::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + ITEM_BAG_WIDTH;
	bottom = y + ITEM_BAG_HEIGHT;
}

