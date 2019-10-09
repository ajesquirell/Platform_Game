#pragma once
#include "Animator.h"
#include "Dynamics.h"

class cItem
{
public:
	cItem(std::string name, std::string desc); //Later like with characters, add a "sprite/display pack" in form of an animator object

	virtual bool OnInteract(cDynamic* object);
	virtual bool OnUse(cDynamic* object);

public:
	cAnimator animItem;
	std::string sName;
	std::string sDescription;
	bool bKeyItem = false;
	bool bEquipable = false;
};

//================================================================================================
//											Health - Give player 10 hp
//================================================================================================
class cItem_Health : public cItem
{
public:
	cItem_Health();
	bool OnInteract(cDynamic* object) override;
	bool OnUse(cDynamic* object) override;
};

//================================================================================================
//											Health - Raise player max hp by 10
//================================================================================================
class cItem_HealthBoost : public cItem
{
public:
	cItem_HealthBoost();
	bool OnInteract(cDynamic* object) override;
	bool OnUse(cDynamic* object) override;
};

//================================================================================================
//											Flames Cash - Increase score by 10
//================================================================================================
class cItem_FlamesCash : public cItem
{
public:
	cItem_FlamesCash();
	bool OnInteract(cDynamic* object) override;
	bool OnUse(cDynamic* object) override;
};