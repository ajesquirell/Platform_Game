#pragma once
#include "Dynamic_Base.h"]
#include "Animator.h"
#include "Assets.h"

class cDynamic_Creature : public cDynamic
{
public:
	cDynamic_Creature(std::string n);

protected:
	cAnimator animations; ///Can private members be accessed by the eventual player and enemy classes by using methods of this class like ChagneState??

public:
	int nHealth;
	int nMaxHealth;
	float fFaceDir;
	bool bSquat;

public:
	//void ChangeState(std::string newState);

	void DrawSelf(olc::PixelGameEngine* pge, float ox, float oy) override; //Screen space offset, since we already have position as data member
	void Update(float fElapsedTime) override;
};
