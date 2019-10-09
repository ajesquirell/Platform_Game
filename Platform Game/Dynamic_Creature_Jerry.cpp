#include "Dynamics.h"

//================================================================================================
//											Dynamic Creature - Jerry (Player)
//================================================================================================

cDynamic_Creature_Jerry::cDynamic_Creature_Jerry() : cDynamic_Creature("Jerry")
{
	bFriendly = true;
	nHealth = 5;
	nHealthMax = 10;
	nScore = 0;
}
