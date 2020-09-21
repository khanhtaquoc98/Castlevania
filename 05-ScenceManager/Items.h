#pragma once
#include"GameObject.h"

#define ITEM_GRAVITY 0.001f

class CItems
{
	unordered_map<int, vector<LPGAMEOBJECT>> items;
	static CItems* __instance;
public:
	void AddItem(int itemType, LPGAMEOBJECT item);
	void CheckAndDrop(LPGAMEOBJECT object);
	void Drop(int itemType, float x, float y);
	CItems() {};
	static CItems* GetInstance();

};

