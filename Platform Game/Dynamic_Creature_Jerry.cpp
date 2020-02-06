#include "Dynamics.h"
#include "Assets.h"

//================================================================================================
//											Dynamic Creature - Jerry (Player)
//================================================================================================

cDynamic_Creature_Jerry::cDynamic_Creature_Jerry() : cDynamic_Creature("Jerry")
{
	bFriendly = true;
	nHealth = 5;
	nHealthMax = 10;
	nScore = 0;
	pEquipedWeapon = (cWeapon*)Assets::get().GetItem("Basic Sword");
}

void cDynamic_Creature_Jerry::PerformAttack()
{
	if (pEquipedWeapon == nullptr)
		return;

	pEquipedWeapon->OnUse(this);
}
