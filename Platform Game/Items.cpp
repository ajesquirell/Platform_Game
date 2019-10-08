#include "Items.h"
#include "Assets.h"
#include <algorithm>

using namespace std;

cItem::cItem(std::string name, std::string desc)
{
	sName = name;
	sDescription = desc;
}

bool cItem::OnInteract(cDynamic* object)
{
	return false;
}

bool cItem::OnUse(cDynamic* object)
{
	return false;
}

//================================================================================================
//											Health - Give player 10 hp
//================================================================================================
cItem_Health::cItem_Health() : 
	cItem("Small Health", "Restores 10 Health")
{
	animItem.mapStates["default"].push_back(Assets::get().GetSprite("Money_09"));
	animItem.ChangeState("default");
}

bool cItem_Health::OnInteract(cDynamic* object)
{
	OnUse(object);
	return false; //Just absorb - not add to inventory
}

bool cItem_Health::OnUse(cDynamic* object)
{
	if (object != nullptr)
	{
		cDynamic_Creature* dyn = (cDynamic_Creature*)object;
		dyn->nHealth = min(dyn->nHealth + 10, dyn->nHealthMax);
	}
	return true; //Item disappears - Used once
}


//================================================================================================
//											Health Boost - Raise player max hp by 10
//================================================================================================
cItem_HealthBoost::cItem_HealthBoost() : 
	cItem("Health Boost", "Increases Max Health by 10")
{
	animItem.mapStates["default"].push_back(Assets::get().GetSprite("Money_00"));
	animItem.ChangeState("default");
}

bool cItem_HealthBoost::OnInteract(cDynamic* object)
{
	return true; // Just add to inventory
}

bool cItem_HealthBoost::OnUse(cDynamic* object)
{
	if (object != nullptr)
	{
		cDynamic_Creature* dyn = (cDynamic_Creature*)object;
		dyn->nHealthMax += 10;
		dyn->nHealth = dyn->nHealthMax;
	}

	return true; // Remove from inventory
}
