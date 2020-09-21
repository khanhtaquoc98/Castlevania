#include "SubWeapons.h"
#include "Simon.h"

void CSubWeapons::AddSubWeapon(int TypeWeapon, LPGAMEOBJECT objWeapon)
{
	subWeapons[TypeWeapon].push_back(objWeapon);
}

void CSubWeapons::UseSubWeapon(int TypeWeapon)
{
	switch (TypeWeapon)
	{
	case 41: // OBJECT_SUBWEAPON_DAGGER
	{
		LPGAMEOBJECT dagger = GetSubWeapon(TypeWeapon);
		if (dagger == NULL) return;
		dagger->SetVisible(true);
		dagger->SetOrientation(CSimon::GetInstance()->GetOrientation());
		float xSimon, ySimon;
		CSimon::GetInstance()->GetPosition(xSimon, ySimon);
		if (CSimon::GetInstance()->GetOrientation() > 0) {
			dagger->SetPosition(xSimon + 16, ySimon + 7);
		}
		else {
			dagger->SetPosition(xSimon - 6, ySimon + 7);
		}

		break;
	}
	default:
		break;
	}
	
}



LPGAMEOBJECT CSubWeapons::GetSubWeapon(int TypeWeapon)
{
	for (auto i = subWeapons[TypeWeapon].begin(); i != subWeapons[TypeWeapon].end(); ++i)
	{
		if ((*i)->isVisible() == false) {
			return *i;
			break;
		}
	}
}

CSubWeapons* CSubWeapons::__instance = NULL;

CSubWeapons* CSubWeapons::GetInstance()
{
	if (__instance == NULL)
		__instance = new CSubWeapons();

	return __instance;
}
