#include "Items.h"
#include "Assets.h"
#include "olcPGEX_Sound.h"
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

void cItem::DrawSelf(olc::PixelGameEngine* pge, olc::GFX2D::Transform2D t)
{
	//In case we want to add any functionality here in sub classes
	animItem.DrawSelf(pge, t);
}

void cItem::Update(float fElapsedTime, cDynamic* player)
{
	//In case we want to add any functionality here in sub classes
	animItem.Update(fElapsedTime);
}

//================================================================================================
//											Health - Give player 10 hp
//================================================================================================
cItem_Health::cItem_Health() : 
	cItem("Health", "Restores 10 Health")
{
	animItem.mapStates["default"].push_back(Assets::get().GetSprite("Health"));
	animItem.ChangeState("default");
	bGravityApplies = true;
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
	animItem.mapStates["default"].push_back(Assets::get().GetSprite("Health Boost"));
	animItem.ChangeState("default");
	bGravityApplies = true;
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

//================================================================================================
//											Flames Cash - Increase score by 10
//================================================================================================
cItem_FlamesCash::cItem_FlamesCash() :
	cItem("Flames Cash", "Increases Score by 10")
{
	animItem.mapStates["default"].push_back(Assets::get().GetSprite("Money_00"));
	animItem.mapStates["default"].push_back(Assets::get().GetSprite("Money_01"));
	animItem.mapStates["default"].push_back(Assets::get().GetSprite("Money_02"));
	animItem.mapStates["default"].push_back(Assets::get().GetSprite("Money_03"));
	animItem.mapStates["default"].push_back(Assets::get().GetSprite("Money_04"));
	animItem.mapStates["default"].push_back(Assets::get().GetSprite("Money_05"));
	animItem.mapStates["default"].push_back(Assets::get().GetSprite("Money_06"));
	animItem.mapStates["default"].push_back(Assets::get().GetSprite("Money_07"));
	animItem.mapStates["default"].push_back(Assets::get().GetSprite("Money_08"));
	animItem.mapStates["default"].push_back(Assets::get().GetSprite("Money_09"));
	animItem.mapStates["default"].push_back(Assets::get().GetSprite("Money_10"));
	animItem.mapStates["default"].push_back(Assets::get().GetSprite("Money_11"));
	animItem.mapStates["default"].push_back(Assets::get().GetSprite("Money_12"));
	animItem.mapStates["default"].push_back(Assets::get().GetSprite("Money_13"));
	animItem.ChangeState("default");

	bGravityApplies = false;
}

bool cItem_FlamesCash::OnInteract(cDynamic* object)
{
	OnUse(object);
	return false;
}

bool cItem_FlamesCash::OnUse(cDynamic* object)
{
	((cDynamic_Creature_Jerry*)object)->nScore += 10;
	olc::SOUND::PlaySample(Assets::get().GetSound("sndSampleB")); // Plays Sample B
	return true;
}
