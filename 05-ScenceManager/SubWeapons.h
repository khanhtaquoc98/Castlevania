#pragma once
#include"GameObject.h"

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