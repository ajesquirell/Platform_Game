#pragma once
#include "Animator.h"
#include "Dynamics.h"

class Platformer;

class cItem
{
public:
	cItem(std::string name, std::string desc); //Later like with characters, add a "sprite/display pack" in form of an animator object

	virtual bool OnInteract(cDynamic* object);
	virtual bool OnUse(cDynamic* object);
	virtual void DrawSelf(olc::PixelGameEngine* pge, olc::GFX2D::Transform2D t);
	virtual void Update(float fElapsedTime, cDynamic* player = nullptr);

public:
	std::string sName;
	std::string sDescription;
	bool bKeyItem = false;
	bool bEquipable = false;
	bool bGravityApplies = true;

	static Platformer* g_engine;

protected:
	cAnimator animItem;
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

//================================================================================================
//											Weapon - Base weapon class (Like reg item, just stores damage)
//================================================================================================
class cWeapon : public cItem
{
public:
	cWeapon(std::string name, std::string desc, int dmg);
	bool OnInteract(cDynamic* object) override;
	bool OnUse(cDynamic* object) override;

public:
	int nDamage = 0;

protected:
	cAnimator animProjectile; // What will actually appear when weapon is used. The bullet, arrow, etc. Not weapon model
};

//================================================================================================
//											Weapon - Sword
//================================================================================================

class cWeapon_Sword : public cWeapon
{
public:
	cWeapon_Sword();

public:
	bool OnUse(cDynamic* object) override;
};

//================================================================================================
//											Weapon - Pistol
//================================================================================================

class cWeapon_Pistol : public cWeapon
{
public:
	cWeapon_Pistol();

public:
	bool OnUse(cDynamic* object) override;
};