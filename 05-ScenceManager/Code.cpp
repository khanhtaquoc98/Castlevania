#include "Code.h"
#include "Sprites.h"


#define OBJECT_SUBWEAPON_DAGGER 41
#define OBJECT_SUBWEAPON_HOYLYWATER 72
#define OBJECT_SUBWEAPON_AXE	73
#define SUBWEAPON_STOPWATCH 74

#define SPRITES_DAGGER	20024
#define SPRITES_HOLYWATER	90009
#define SPRITES_AXE	73001
#define SPRITES_STOPWATCH 90013


void Code::DrawNumber(int number, D3DXVECTOR2 position, int length)
{
	string str = to_string(number);

	//Dùng khi tăng điểm thì sẽ trừ bớt số 0 thay thế là điểm ở hàm dưới(Nếu không có thì sẽ thêm điểm ở phía sau vầ giữ nguyên length
	for (int i = 0; i < length - str.size(); i++)
	{
		CSprites::GetInstance()->Get(77000)->Draw(position.x, position.y, -1);
		position.x += 7;
	}

	//Đếm có bao nhiêu chữ số và ghi điểm
	for (int i = 0; i < str.size(); i++)
	{
		CSprites::GetInstance()->Get(77000 + Convert(str[i]))->Draw(position.x, position.y, -1);
		position.x += 7;
	}
}
void Code::DrawSubWeapon(D3DXVECTOR2 position, int info)
{
	switch (info)
	{
	case OBJECT_SUBWEAPON_DAGGER:
		CSprites::GetInstance()->Get(SPRITES_DAGGER)->Draw(position.x, position.y, 1);
		break;
	case OBJECT_SUBWEAPON_HOYLYWATER:
		CSprites::GetInstance()->Get(SPRITES_HOLYWATER)->Draw(position.x - 2, position.y - 2, 1);
		break;
	case OBJECT_SUBWEAPON_AXE:
		CSprites::GetInstance()->Get(SPRITES_AXE)->Draw(position.x, position.y, 1);
		break;
	case SUBWEAPON_STOPWATCH:
		CSprites::GetInstance()->Get(SPRITES_STOPWATCH)->Draw(position.x, position.y - 3, 1);
		break;
	default:
		break;
	}
}
void Code::DrawHP(D3DXVECTOR2 position, int health, int kind)
{
	CSprites* sprites = CSprites::GetInstance();
	CSprite* spriteHealthMinus = sprites->Get(77102);
	CSprite* spriteHealth = NULL;
	int healthGeneral = NUM_HEALTH;

	if (kind == NUM_ID_SIMON)
		spriteHealth = sprites->Get(77101); //HEALTH SIMON
	else
		spriteHealth = sprites->Get(77103); //HEALTH BOSS

	for (int i = 0; i < health; i++)
	{
		spriteHealth->Draw(position.x, position.y, -1);
		position.x += 5;
	}


	for (int i = 0; i < healthGeneral - health; i++)
	{
		spriteHealthMinus->Draw(position.x, position.y, -1);
		position.x += 5;
	}

	sprites = NULL;
	spriteHealthMinus = NULL;
	spriteHealth = NULL;
}

int Code::Convert(char c)
{
	return c - '0';
}