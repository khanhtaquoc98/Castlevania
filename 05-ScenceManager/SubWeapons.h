#pragma once
#include"GameObject.h"

#define OBJECT_SUBWEAPON_DAGGER 41
#define OBJECT_SUBWEAPON_HOYLYWATER 72


class CSubWeapons {
	unordered_map<int, vector<LPGAMEOBJECT>> subWeapons;
	static CSubWeapons* __instance;

public:

	CSubWeapons() {};
	void AddSubWeapon(int TypeWeapon, LPGAMEOBJECT objWeapon);
	void UseSubWeapon(int TypeWeapon);
	LPGAMEOBJECT GetSubWeapon(int TypeWeapon);

	static CSubWeapons* GetInstance();
};