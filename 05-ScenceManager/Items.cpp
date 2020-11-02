#include "Items.h"
CItems* CItems::__instance = NULL;

void CItems::AddItem(int itemType, LPGAMEOBJECT item)
{
	items[itemType].push_back(item);
}

void CItems::CheckAndDrop(LPGAMEOBJECT object)
{
	int itemType = object->GetItem();
	float x, y;
	object->GetPosition(x, y);
	Drop(itemType, x, y);
}

void CItems::CheckAndDropMoney(LPGAMEOBJECT object)
{
	int itemType = object->GetItem();
	DropMoney(itemType);
}

void CItems::DropMoney(int itemType) {
	for (auto i = items[itemType].begin(); i != items[itemType].end(); i++)
	{
		if ((*i)->isVisible() == false) {
			(*i)->SetVisible(true);
			(*i)->tStartVisible = GetTickCount();
			break;
		}
	}
}

void CItems::Drop(int itemType, float x, float y)
{
	for (auto i = items[itemType].begin(); i != items[itemType].end(); i++)
	{
		if ((*i)->isVisible() == false) {
			(*i)->SetPosition(x, y);
			(*i)->SetVisible(true);
			(*i)->tStartVisible = GetTickCount();
			break;
		}
	}
}

CItems* CItems::GetInstance()
{
	if (__instance == NULL)
		__instance = new CItems();
	return __instance;
}