#include "Boss.h"
#include "Utils.h"
#include "Game.h"


CBoss::CBoss()
{
	SetState(STATE_SLEEP);
}

CBoss::CBoss(CSimon* Simon)
{
	this->state = STATE_SLEEP;
	mSimon = Simon;
}

CBoss::~CBoss()
{
}



void CBoss::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (this->GetState() == STATE_SLEEP)
	{
		left = x - 16;
		top = y;
		right = x + 32;
		bottom = y + 23;
	}
	else
	{
		left = x;
		top = y;
		right = x + 48;
		bottom = y + 23;
	}
}


void CBoss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	
	if (this->health <= 0) {
		this->health = 0;
		this->state = DEAD;
		/*SetState(DEAD);*/
	}

	
	


	x += dx;
	y += dy;

	if (mSimon->GetInstance()->x > 640 && this->state == STATE_SLEEP)
	{
		this->state = FLY_AFTER_SLEEP;
	}

	if (this->state != STATE_SLEEP)
	{
		if (x <= 512)
			vx = -vx;
		if (x >= 721)
			vx = -vx;
		if (y <= 30)
			vy = 0.03f;
	}

	switch (this->state)
	{
	case FLY_AFTER_SLEEP:
		if (y <= 90)
		{
			vy = 0.03f;
		}
		else
		{
			vx = 0;
			vy = 0;
			this->state = ATTACK;
		}
		break;
	case ATTACK:
		if (timerAttack < 3000)
		{
			timerAttack += dt;
		}
		else
		{
			timerAttack = 0;
			isAttack = true;
		}
		if ((abs(x - mSimon->GetInstance()->x) >= 70 && isHitSimon == false) || isAttack == true)
		{
			if (mSimon->GetInstance()->x < x)
			{
				vx = -0.08f;
				vy = 0.078f;
				isHitSimon = true;
			}
			else
			{
				vx = 0.08f;
				vy = 0.078f;
				isHitSimon = true;
			}
			isAttack = false;
		}
		if (abs(y - mSimon->GetInstance()->y) <= 5)
		{
			vy = 0;
			if (abs(mSimon->GetInstance()->x - x) >= 5)
			{
				vy = -0.038f;
				this->state = FLY_AFTER_ATTACK;
			}
		}
		if (y - mSimon->GetInstance()->y >= 60)
		{
			vy = -0.038f;
			this->state = FLY_AFTER_ATTACK;
		}
		break;
	case FLY_AFTER_ATTACK:
		if (timerFlyUp < 1200)
			timerFlyUp += dt;
		else
		{
			timerFlyUp = 0;
			this->state = FLY_TO_THE_MIDDLE;
		}
		break;

	case FLY_TO_THE_MIDDLE:
		if (x != 640)
		{
			if (x < 635)
			{
				vx = 0.07f;
				vy = -0.01f;
			}
			else
			{
				vx = -0.07f;
				vy = -0.01f;
			}
		}
		if (x >= 620 && x < 650)
		{
			vx = 0;
			vy = 0;
			this->state = AIM;
		}
		break;

	case AIM:
		if (this->y <= 100)
			vy = 0.02f;
		else
			vy = 0;

		if (abs(x - mSimon->GetInstance()->x) > 60)
		{
			if (mSimon->GetInstance()->x < x)
				vx = -0.055f;
		}
		else
		{
			vx = 0;
		}

		isHitSimon = false;
		if (timerAim < 2000)
		{
			timerAim += dt;
		}
		else
		{
			this->state = ATTACK;
			timerAim = 0;
		}
		break;
	case DEAD:
		vx = vy = 0;
		animation_set->at(BOSS_DEAD_ANI)->SetAniStartTime(GetTickCount());
		break;
	}

	if (this->state == DEAD && animation_set->at(BOSS_DEAD_ANI)->IsOver(300) && this->visible == true) {
		SetVisible(false);
	}

	DebugOut(L"State Bosss %d", this->state);
	if (this->state != STATE_SLEEP) {
		mSimon->isStageBoss = true;
	}

}
void CBoss::Render()
{
	int ani = -1;
	if (this->state == STATE_SLEEP) ani = BOSS_SLEEP_ANI;
	else if (this->state == DEAD) ani = BOSS_DEAD_ANI;
	else {
		ani = BOSS_FLY_ANI;
	}
	animation_set->at(ani)->Render(x, y, nx);
	RenderBoundingBox();
}

CBoss* CBoss::__instance = NULL;


CBoss* CBoss::GetInstance()
{
	if (__instance == NULL)
		__instance = new CBoss();
	return __instance;
}




