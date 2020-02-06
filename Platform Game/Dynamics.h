#pragma once

#include "olcPixelGameEngine.h"
#include "Animator.h"
#include "Game_Utilities.h"

class Platformer;
class cScriptProcessor;
class cItem;
class cWeapon;
class cDynamic_Projectile;

/*=======================================Knockback Mode Enum Declaration==============================================*/
// x_only: Uses fFaceDir for KB direction, not normalized projectile-to-victim vector for x and y components.
// y_only: Uses just y component of normalized vector
// both: Uses fFaceDir for KB direction, and y component of normalized vector
// both_omnidirectional: Uses x and y components of normalized vector for KB direction.
//		-Most useful for stationary enemies/objects that provide knockback. Not as good for moving projectiles, as an 
//		 enemy that touches the back half of the projectile will be KB'd back the opposite direction of projectile movement
enum KnockBackMode { x_only, y_only, both, both_omnidirectional };
/*====================================================================================================================*/

//================================================================================================
//											Base Dynamic
//================================================================================================
class cDynamic //Pure Virtual
{
public:
	cDynamic(std::string n);
	~cDynamic();

public:
	float px, py;
	float vx, vy;
	bool bSolidVsMap;
	bool bSolidVsDynamic;
	bool bFriendly;
	bool bRedundant;
	bool bIsProjectile; // Don't want to do any runtime type checking - just use bool
	bool bIsAttackable; // Capable of receiving damage
	bool bGravityApplies = true;
	std::string sName;

	bool bObjectOnGround; //Probably only for Dynamic Creatures and Items

public:
	virtual void DrawSelf(olc::PixelGameEngine* pge, float ox, float oy) = 0; //Screen space offset, since we already have position as data member
	virtual void Update(float fElapsedTime, cDynamic* player = nullptr) {}
	virtual bool OnInteract(cDynamic* player = nullptr) { return false; }

	static Platformer* g_engine;
	static cScriptProcessor* g_script;
};

//================================================================================================
//											Dynamic Creature
//================================================================================================
class cDynamic_Creature : public cDynamic
{
public:
	cDynamic_Creature(std::string n);

	enum FacingDirection{LEFT = -1, RIGHT = 1}; //Make a GetFacingDirection just to be easier?

public:
	int nHealth;
	int nHealthMax;
	float fFaceDir; //For player, handled in input, but for other creatures should be handled in their behavior based on their calculated destination position relative to their current pos
	bool bSquat;
	bool bControllable = true;

public:
	//void ChangeState(std::string newState);

	void DrawSelf(olc::PixelGameEngine* pge, float ox, float oy) override; //Screen space offset, since we already have position as data member
	void Update(float fElapsedTime, cDynamic* player = nullptr) override;
	virtual void Behavior(float fElapsedTime, cDynamic* player = nullptr);
	virtual void PerformAttack() {};
	void KnockBack(float dx, float dy, cDynamic_Projectile* proj);

	cWeapon* pEquipedWeapon = nullptr;

protected:
	cAnimator animations; ///Can private members be accessed by the eventual player and enemy classes by using methods of this class like ChagneState??
	float fStateTick;
	float fKnockBackTimer = 0.0f;
	float fKnockBackDX = 0.0f;
	float fKnockBackDY = 0.0f;
	float fKnockBackVel = 0.0f; //Set by each weapon when spawning projectile
	float fKnockBackXDir = 0.0f;
	KnockBackMode KnockBackMode = both;
};

//================================================================================================
//											Dynamic Creature - Jerry (Player)
//================================================================================================
class cDynamic_Creature_Jerry : public cDynamic_Creature
{
public:
	cDynamic_Creature_Jerry();

public:
	void PerformAttack() override;
	int nScore;
};

//================================================================================================
//											Dynamic Creature - Fake Jerry
//================================================================================================
class cDynamic_Creature_FakeJerry : public cDynamic_Creature
{
public:
	cDynamic_Creature_FakeJerry();

	void Behavior(float fElapsedTime, cDynamic* player = nullptr) override;
	void PerformAttack() override;
private:
	//float fDirectionX;
	//float fDirectionY;
};

//================================================================================================
//											Dynamic Teleport
//================================================================================================
class cDynamic_Teleport : public cDynamic
{
public:
	cDynamic_Teleport(float x, float y, std::string sMapName, float tx, float ty);
	void DrawSelf(olc::PixelGameEngine* pge, float ox, float oy) override;
	void Update(float fElapsedTime, cDynamic* player = nullptr) override;
	//Should have OnInteract here instead of in map?

public:
	std::string sMapName;
	float fMapPosX;
	float fMapPosY;
};

//================================================================================================
//											Dynamic Item
//================================================================================================
class cDynamic_Item : public cDynamic
{
public:
	cDynamic_Item(float x, float y, cItem* i);
	void Update(float fElapsedTime, cDynamic* player = nullptr) override;
	void DrawSelf(olc::PixelGameEngine* pge, float ox, float oy) override;
	bool OnInteract(cDynamic* player = nullptr) override;

public:
	cItem* item;
	bool bCollected = false;
};

//================================================================================================
//											Dynamic Projectile
//================================================================================================
class cDynamic_Projectile : public cDynamic
{
public:
	cDynamic_Projectile(float ox, float oy, bool bFriend, float velx, float vely, float duration, cAnimator animProj, float fFaceDir);
	void DrawSelf(olc::PixelGameEngine* pge, float ox, float oy) override;
	void Update(float fElapsedTime, cDynamic* player = nullptr) override;

public:
	cAnimator animProjectile;
	float fFaceDir;
	float fDuration;
	bool bOneHit = true;
	int nDamage = 0;
	float fKnockBackVel = 0.0f;
	float fKnockBackDuration = 0.2f;
	enum KnockBackMode KnockBackMode = both;
};