#pragma once

#include "olcPixelGameEngine.h"
#include "Animator.h"

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
	std::string sName;

	bool bObjectOnGround; //Probably only for Dynamic Creatures and Items

public:
	virtual void DrawSelf(olc::PixelGameEngine* pge, float ox, float oy) = 0; //Screen space offset, since we already have position as data member
	virtual void Update(float fElapsedTime, cDynamic* player = nullptr) {}

};

//================================================================================================
//											Dynamic Creature
//================================================================================================

class cDynamic_Creature : public cDynamic
{
public:
	cDynamic_Creature(std::string n);

	enum FacingDirection{LEFT, RIGHT};

public:
	int nHealth;
	int nMaxHealth;
	float fFaceDir; //For player, handled in input, but for other creatures should be handled in their behavior based on their calculated destination position relative to their current pos
	bool bSquat;

public:
	//void ChangeState(std::string newState);

	void DrawSelf(olc::PixelGameEngine* pge, float ox, float oy) override; //Screen space offset, since we already have position as data member
	void Update(float fElapsedTime, cDynamic* player = nullptr) override;
	virtual void Behavior(float fElapsedTime, cDynamic* player = nullptr);

protected:
	cAnimator animations; ///Can private members be accessed by the eventual player and enemy classes by using methods of this class like ChagneState??
	float fStateTick;
};

//================================================================================================
//											Dynamic Creature - Fake Jerry
//================================================================================================

class cDynamic_Creature_FakeJerry : public cDynamic_Creature
{
public:
	cDynamic_Creature_FakeJerry();
	void Behavior(float fElapsedTime, cDynamic* player = nullptr) override;
private:
	float fDirectionX;
	float fDirectionY;
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

public:
	std::string sMapName;
	float fMapPosX;
	float fMapPosY;
};