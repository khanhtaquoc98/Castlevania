#include "ItemBigHeart.h"
#include "Brick.h"
#include "Items.h"


CItemBigHeart::CItemBigHeart()
{
	this->visible = false;
}

void CItemBigHeart::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt);
	vy += ITEM_GRAVITY * dt;// simple fall down
	if (this->isVisible() == true && this->tStartVisible != 0) {
		if(GetTickCount() - this->tStartVisible > TIME_VISIBLE_ITEM_BIG_HEART)
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
			else {
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CItemBigHeart::Render()
{
	animation_set->at(0)->Render(x, y, 1);
}

void CItemBigHeart::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + ITEM_BIG_HEART_BBOX_WIDTH;
	bottom = y + ITEM_BIG_HEART_BBOX_HEIGHT;
}
